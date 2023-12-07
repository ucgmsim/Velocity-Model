"""
Library functions for extracting Z values from the NZVM in a pythonic manner
Supports NZVM versions 2.03, 2.04, 2.06
"""
from pathlib import Path
import shutil
import subprocess
import tempfile

import numpy as np
import pandas as pd
from matplotlib import path as mpltPath

from Velocity_Model.basins import basin_outlines_dict

MAX_NUM_GEN_MULTI_PROFILES = 300000
SLICE_SIZE = 50000

OUT_BASIN_SIGMA = 0.5

IN_BASIN_SIGMA = 0.3

file_output_format = {"Z1.0": "Z/Z_1.0.txt", "Z2.5": "Z/Z_2.5.txt"}
column_names = {"Z1.0": "Z_1.0(km)", "Z2.5": "Z_2.5(km)"}


def extract_z(
    z_types,
    stat_df,
    nzvm_path=Path("NZVM"),
    version=2.07,
):
    """
    Create a dataframe based on values from the NZVM

    :param z_types:
    :param stat_df: Dataframe containing stations - station should be the index and lon and lat should be the columns
    :param nzvm_path:
    :param version: NZVM version to extract values for
    :return: Dataframe containing all z_types, station names and sigma
    """
    n_stats = len(stat_df)

    merged_df = stat_df
    stat_df["stat_name"] = stat_df.index

    z_values = {}

    for i in range(0, n_stats, SLICE_SIZE):
        with tempfile.TemporaryDirectory() as temp_dir:
            temp_dir = Path(temp_dir)
            config_ffp = temp_dir / "GENERATE_THRESHOLD_USER_INPUT.txt"
            coords_ffp = temp_dir / "MultipleProfileParameters.txt"
            vm_working_dir = temp_dir / "z_output"

            stat_df_slice = stat_df[i: i + SLICE_SIZE]

            n_stats_slice = len(stat_df_slice)
            with open(coords_ffp, "w") as coords_fp:
                coords_fp.write(f"{n_stats_slice}\n")
            stat_df_slice[["lon", "lat", "stat_name"]].to_csv(
                coords_ffp, sep=" ", header=None, mode="a", index=None
            )

            for z_type in z_types:
                z_val = calculate_z(
                        config_ffp, coords_ffp, nzvm_path, version, vm_working_dir, z_type)
                if z_type in z_values:
                    z_val.index = range(z_values[z_type].index.max() + 1,
                                        z_values[z_type].index.max() + n_stats_slice + 1)
                    z_values[z_type] = pd.concat([z_values[z_type], z_val], ignore_index=True)
                else:
                    z_values[z_type] = z_val

    for z_type in z_types:
        stat_df["z_index"] = z_values[z_type].index
        merged_df = merged_df.merge(z_values[z_type], left_on="z_index", right_index=True)

    merged_df["sigma"] = calculate_z_sigma(stat_df, version)

    output_columns = [column_names[z_type] for z_type in z_types] + ["sigma"]
    merged_df.index.name = "Station_Name"
    return merged_df[output_columns]


def calculate_z(config_ffp, coords_ffp, nzvm_path, version, vm_working_dir, z_type):
    """
    For a given Z type and coords file write a config file and calculate Z by calling the NZVM binary

    :param config_ffp: Path to config file to be written
    :param coords_ffp: Path to given coords file
    :param nzvm_path: path to the NZVM binary
    :param version: version of the nzvm binary to extract the values for
    :param vm_working_dir: folder where the raw output is written
    :param z_type: Z1.0 or Z2.5
    :return: Dataframe containing Z values and lat/lon
    """

    with open(config_ffp, "w") as config_fp:
        config_fp.write(
            "CALL_TYPE=GENERATE_THRESHOLD_USER_INPUT\n"
            f"MODEL_VERSION={version}\n"
            f"OUTPUT_DIR={vm_working_dir}\n"
            f"COORDINATES_TEXTFILE={coords_ffp}\n"
            f"VS_TYPE={z_type}\n"
        )
    shutil.rmtree(vm_working_dir, ignore_errors=True)
    subprocess.run([nzvm_path, config_ffp], cwd=nzvm_path.parent)
    z_values = pd.read_csv(
        vm_working_dir / file_output_format[z_type],
        delim_whitespace=True,
    )

    return z_values


def calculate_z_sigma(stat_df, version):
    """
    Returns a np array of len(stat_df) containing 0.3 or 0.5 depending if the station is within a basin or not

    Loops over the respective basin files for each NZVM version
    """
    sigma_df = pd.DataFrame(index=stat_df.index)
    sigma_df["in_basin_flag"] = False
    for outline_fp in basin_outlines_dict[version]:
        outline = np.loadtxt(outline_fp)
        path = mpltPath.Path(outline)

        for stat in stat_df.iterrows():
            if path.contains_point((stat[1].lon, stat[1].lat)):
                sigma_df.in_basin_flag.loc[stat[0]] = True

    return np.where(sigma_df.in_basin_flag, IN_BASIN_SIGMA, OUT_BASIN_SIGMA)
