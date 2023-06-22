#import utm
import numpy as np
import pandas as pd
from pathlib import Path
import xarray as xr
from scipy.interpolate import griddata
import yaml
from  multiprocessing import Pool
from functools import partial

from shared import depth_categories, ep2020_yaml, NI_tomo_dir, step2_outdir, write_file, output_exists, smart_float

step2_outdir.mkdir(parents=True, exist_ok=True)

V_types = ["vp", "vs", "rho"]

"""
This code fits Chow NI tomography onto the (lat lon) grid of EP2020 via interpolation
"""
def myfunc(tomo_df, mgrid_lon, mgrid_lat, elev):
    #print(tomo_df)
    for V_type in V_types:
        print(f"    {elev}")

        # this effectively skips the overlapping depth from the block with lower resolution
        if output_exists(step2_outdir,V_type.lower(), elev):
            continue

        print(f"elev={elev} is good")
        #print(tomo_df.loc[tomo_df.depth == -1*elev])

        crude_lats = tomo_df.loc[tomo_df.depth == -1*elev].latitude
        crude_lons = tomo_df.loc[tomo_df.depth == -1*elev].longitude

        # X has 118, Y 156, Z 90 unique values
        # but lat and lon are not on the same grid, and there can be different number of lon (not 118), and lat (not 156) values.

        #       print(f"        {V_type}")
        T = tomo_df[tomo_df.depth == -1*elev][V_type]
        Ti2 = griddata(
            np.array([crude_lons, crude_lats]).T,
            np.array(T),
            (mgrid_lon, mgrid_lat),
            method="linear",
            fill_value=0,
        )  # outside the domain is 0
        write_file(
            step2_outdir, V_type.lower(), Ti2, elev, lats, lons
        )


with open(ep2020_yaml) as file:
    ep2020_data=yaml.safe_load(file)

lats = ep2020_data['lats']
lons = ep2020_data['lons']

for dc in depth_categories: # starting from shallow (higher res)
    print(f"{dc}")
    tomo_df = xr.open_dataset(Path(NI_tomo_dir/f"nz_atom_north_chow_etal_2021_vp+vs-{dc}.r0.1-n4.nc"))
    tomo_df = tomo_df.to_dataframe().reset_index()
    tomo_df['Z']=-1*tomo_df.depth


    z_values = sorted(list(set(tomo_df.Z.values.flatten())))
    #print(z_values)
    
    mgrid_lon, mgrid_lat = np.meshgrid(lons, lats)
    with Pool(8) as pool:
        res=pool.map_async(partial(myfunc, tomo_df, mgrid_lon, mgrid_lat), z_values)
        res=res.get()
    # For debugging, use below instead of the above with Pool
    # for z in z_values:
    #     myfunc(tomo_df, mgrid_lon, mgrid_lat, z)

