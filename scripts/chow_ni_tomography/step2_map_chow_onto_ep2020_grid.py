import utm
import numpy as np
import pandas as pd
from pathlib import Path
from scipy.interpolate import griddata
import yaml

from shared import depth_categories, ep2020_yaml, NI_tomo_dir, step2_outdir, write_file

step2_outdir.mkdir(parents=True, exist_ok=True)

V_types = ["Vp", "Vs", "rho"]

"""
This code fits Chow NI tomography onto the (lat lon) grid of EP2020 via interpolation
"""

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

    z_values = list(set(tomo_df.Z))  # 90
    z_values.sort()

    for elev in z_values:
        #    print(f"    {elev}")
        crude_lats = tomo_df[tomo_df.Z == elev].lat
        crude_lons = tomo_df[tomo_df.Z == elev].lon

        # X has 118, Y 156, Z 90 unique values
        # but lat and lon are not on the same grid, and there can be different number of lon (not 118), and lat (not 156) values.

        for V_type in V_types:
            #       print(f"        {V_type}")
            T = tomo_df[tomo_df.Z == elev][V_type]
            mgrid_lon, mgrid_lat = np.meshgrid(lons, lats)
            Ti2 = griddata(
                np.array([crude_lons, crude_lats]).T,
                np.array(T),
                (mgrid_lon, mgrid_lat),
                method="linear",
                fill_value=0,
            )  # outside the domain is 0
            write_file(
                step2_outdir, V_type.lower(), Ti2 / 1000, elev / 1000, lats, lons
            )
