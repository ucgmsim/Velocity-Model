import utm
import numpy as np
import pandas as pd
from pathlib import Path
from scipy.interpolate import griddata
import yaml
from  multiprocessing import Pool
from functools import partial

from shared import depth_categories, ep2020_yaml, NI_tomo_dir, step2_outdir, write_file, output_exists, smart_float

step2_outdir.mkdir(parents=True, exist_ok=True)

V_types = ["Vp", "Vs", "rho"]

"""
This code fits Chow NI tomography onto the (lat lon) grid of EP2020 via interpolation
"""
def myfunc(tomo_df, mgrid_lon, mgrid_lat, elev):
    for V_type in V_types:
        #    print(f"    {elev}")

        if output_exists(step2_outdir,V_type.lower(), elev/1000.):
            continue

        print(f"elev={elev} is good")
        crude_lats = tomo_df[tomo_df.Z == elev].lat
        crude_lons = tomo_df[tomo_df.Z == elev].lon

        # X has 118, Y 156, Z 90 unique values
        # but lat and lon are not on the same grid, and there can be different number of lon (not 118), and lat (not 156) values.

        #       print(f"        {V_type}")
        T = tomo_df[tomo_df.Z == elev][V_type]
        Ti2 = griddata(
            np.array([crude_lons, crude_lats]).T,
            np.array(T),
            (mgrid_lon, mgrid_lat),
            method="linear",
            fill_value=0,
        )  # outside the domain is 0
        write_file(
            step2_outdir, V_type.lower(), Ti2 / 1000., elev / 1000., lats, lons
        )


with open(ep2020_yaml) as file:
    ep2020_data=yaml.safe_load(file)

lats = ep2020_data['lats']
lons = ep2020_data['lons']

for dc in depth_categories:
    print(f"{dc}")
    tomo_df = pd.read_csv(
        Path(NI_tomo_dir) / "tomography_model_{}.xyz".format(dc),
        sep="\s",
        skiprows=4,
        header=None,
        names=["X", "Y", "Z", "Vp", "Vs", "rho", "Qp", "Qs"],
    )

    tomo_df["lat"], tomo_df["lon"] = utm.to_latlon(
        np.array(tomo_df["X"]), np.array(tomo_df["Y"]), 60, "G"
    )  # returns lat and lon

    z_values = sorted(list(set(tomo_df.Z))) # 90

    print(z_values)
    
    mgrid_lon, mgrid_lat = np.meshgrid(lons, lats)
    with Pool(40) as pool:
        res=pool.map_async(partial(myfunc, tomo_df, mgrid_lon, mgrid_lat), z_values)
        res=res.get()

