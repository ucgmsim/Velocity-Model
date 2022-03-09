import pandas as pd
from pathlib import Path
from shared import NI_tomo_dir, depth_categories

"""
This code is just a one-off to read Chow NI data and find out the depth levels it uses
"""


tomo_df_dict = {}
z_values = []
for dc in depth_categories:
    print(f"{dc}")
    tomo_df = pd.read_csv(
        Path(NI_tomo_dir) / "tomography_model_{}.xyz".format(dc),
        sep="\s",
        skiprows=4,
        header=None,
        names=["X", "Y", "Z", "Vp", "Vs", "rho", "Qp", "Qs"],
    )

    tomo_df_dict[dc] = tomo_df

    z_values.extend(list(set(tomo_df.Z)))

z_values.sort()

# add this output to shared.py as chow_elevs
print(z_values)
