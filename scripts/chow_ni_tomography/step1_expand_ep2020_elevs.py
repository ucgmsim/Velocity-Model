import numpy as np
import pandas as pd
from scipy import interpolate
import yaml
from shared import (
    chow_yaml,
    ep2020_yaml,
    EP2020_tomo_dir,
    step1_outdir,
    v_file_template,
    v_types,
    p_float,
)

"""
This code expands EP2020 tomography such that it covers the elevation levels of Chow NI via interpolation.
"""
with open(ep2020_yaml) as file:
    ep2020_data=yaml.safe_load(file)
with open(chow_yaml) as file:
    chow_data=yaml.safe_load(file)


lats = ep2020_data['lats']
lons = ep2020_data['lons']
elevs = ep2020_data['elevs']

chow_elevs= chow_data['elevs']

tomo_index = pd.MultiIndex.from_product(
    [lats, lons, elevs], names=["lat", "lon", "elev"]
)

ep2020_df = pd.DataFrame(index=tomo_index, columns=v_types, dtype=float)

ep2020_dic = {}

step1_outdir.mkdir(parents=True, exist_ok=True)

# load 2020_NZ tomograpy data
for elev in elevs:
    print(elev)
    ep2020_dic[elev] = {}
    for v_type in v_types:
        v_file = EP2020_tomo_dir / f"surf_tomography_{v_type}_elev{elev}.in"
        v_df = pd.read_csv(
            v_file, delimiter=" ", header=2, na_filter=False
        )  # use lon as column name
        v_df = v_df.truncate(
            axis="columns", after=v_df.columns[-2]
        )  # remove the last NaN column and rename the index to use lat
        v_df.rename(dict(zip(v_df.index, lats)), axis="index", inplace=True)

        ep2020_dic[elev][v_type] = v_df

# ep2020_dic is the whole 2020_NZ tomography. Its elevs are only 25 long -750 <=.. <=15 , while chow_elevs are 172 long. -400<=..<= 2.25
# take elevs from ep2020 data that are not present in Chow. 
new_chow_elevs = sorted([x for x in elevs if x < min(chow_elevs) or x>max(chow_elevs)] + chow_elevs)


for v_type in v_types:

    v_data = [
        ep2020_dic[elev][v_type].to_numpy() for elev in elevs
    ]  # data from each .in file (each elev value)
    v_array = np.stack(v_data, axis=2)  # stack them all together along z-axis
    new_v_data = np.zeros((len(lats), len(lons), len(new_chow_elevs)))

    for lat_id in range(len(lats)):
        for lon_id in range(len(lons)):
            f = interpolate.interp1d(elevs, v_array[lat_id][lon_id])
            new_v = f(new_chow_elevs)
            new_v_data[lat_id][lon_id] = new_v
    for elev_id, elev in enumerate(new_chow_elevs):
        outfile = step1_outdir / v_file_template.format(v_type, p_float(elev))
        with open(outfile, "w") as f:
            f.write(f"{len(lats)} {len(lons)}\n")
            f.write(
                " ".join(["{:.6f}".format(x) for x in lats]) + "\n"
            )  # trim the last " "
            f.write(" ".join(["{:.6f}".format(x) for x in lons]) + "\n")
            for lat_id in range(len(lats)):
                row_array = new_v_data[lat_id, :, elev_id]
                row_str = " ".join(["{:.6f}".format(x) for x in row_array]) + "\n"
                f.write(row_str)
        print(f"{outfile} written")
