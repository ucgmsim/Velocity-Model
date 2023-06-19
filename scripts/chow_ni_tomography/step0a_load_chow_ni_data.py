import numpy as np
import pandas as pd
from pathlib import Path
import xarray as xr
import yaml
from shared import NI_tomo_dir, depth_categories, chow_yaml

"""
This code is just a one-off to read Chow NI data and find out the depth levels it uses
"""


tomo_df_dict = {}
z_values = []
for dc in depth_categories:
    print(f"{dc}")
    tomo_df = xr.open_dataset(Path(NI_tomo_dir/f"nz_atom_north_chow_etal_2021_vp+vs-{dc}.r0.1-n4.nc"))

    tomo_df_dict[dc] = tomo_df
    print(tomo_df.depth.values)

    z_values.extend(list(tomo_df.depth.values.flatten()))
z_values=sorted(list(set(z_values)))

# add this output to shared.py as chow_elevs
print(f"{len(z_values)} {z_values}")
set_z_values=list(set(z_values))
print(f"{len(set_z_values)} {set_z_values}")

z_values= sorted(np.array(z_values, dtype=float)*(-1))
chow_ni={"elevs": [float(e) for e in z_values]}
with open(chow_yaml,"w") as file:
    yaml.dump(chow_ni,file)
