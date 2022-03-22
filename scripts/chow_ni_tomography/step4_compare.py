import numpy as np
import pandas as pd
import yaml

from shared import (
    ep2020_yaml,
    step1_outdir,
    step3_outdir,
    v_types,
    v_file_template,
    compare_dir,
    smart_float,
)

"""
This code is to produce plots similar to those from Chow's paper, showing difference between EP2020 and Chow NI. 
"""

compare_dir.mkdir(parents=True, exist_ok=True)
with open(ep2020_yaml) as file:
    ep2020_data=yaml.safe_load(file)

lats = ep2020_data['lats']
lons = ep2020_data['lons']

def compare(v_type, elev):
    combined_array = pd.read_csv(
        step3_outdir / v_file_template.format(v_type, smart_float(elev)),
        delimiter=" ",
        header=2,
        na_filter=False,
    ).to_numpy()
    ep2020_array = pd.read_csv(
        step1_outdir / v_file_template.format(v_type, smart_float(elev)),
        delimiter=" ",
        header=2,
        na_filter=False,
    ).to_numpy()
    bias_array = np.log10(combined_array / ep2020_array)  # 0 if equal
    return bias_array


for v_type in v_types:
    for elev in [
        -23,
        -15,
        -5,
    ]:  # to compare with the Chow paper's plots (25km is not available, so using 23km instead)
        bias_array = compare(v_type, elev)
        mesh = np.meshgrid(lons, lats)
        meshed_lats = mesh[1].reshape(mesh[1].size)
        meshed_lons = mesh[0].reshape(mesh[0].size)
        meshed_bias_array = bias_array.reshape(bias_array.size)

        assert meshed_bias_array.size == meshed_lats.size

        outfile = v_file_template.format(v_type, smart_float(elev)) + ".xyz"
        df = pd.DataFrame(
            {"lon": meshed_lons, "lat": meshed_lats, "bias": meshed_bias_array}
        )
        df.to_csv(
            compare_dir / outfile,
            header=False,
            index=False,
            float_format="%.6f",
            sep=" ",
        )

        # for plotting
        # for x in *vp*.xyz; do x1="${x/surf_tomography_/}";x2="${x1/.in.xyz/}";echo $x2;python ~/visualization/sources/plot_items.py --xyz $x -t "NZVM\&Chow\ vs\ NZVM\ $x2" --xyz-transparency 30 --xyz-cpt polar --xyz-cpt-labels "log10(new/original)" --xyz-landmask --xyz-grid --xyz-grid-type surface --xyz-grid-search 12m --xyz-size 1k --xyz-cpt-min "-0.01" --xyz-cpt-max 0.01 -n 4 -f plots/$x2;done
