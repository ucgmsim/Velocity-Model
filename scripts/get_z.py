import argparse
import os
import shutil
import subprocess
import tempfile

import pandas as pd

from qcore import formats

file_output_format = {"Z1.0": "Z/Z_1.0.txt", "Z2.5": "Z/Z_2.5.txt"}
column_names = {"Z1.0": "Z_1.0(km)", "Z2.5": "Z_2.5(km)"}


def extract_z(version, z_types, stat_df, nzvm_path):
    with tempfile.TemporaryDirectory() as temp_dir:
        temp_dir = "/home/jam335/scratch/syncthing/velocity_model/21p3_get_z/temp"
        config_ffp = os.path.join(temp_dir, "GENERATE_THRESHOLD_USER_INPUT.txt")
        coords_ffp = os.path.join(temp_dir, "MultipleProfileParameters.txt")
        vm_working_dir = os.path.join(temp_dir, "z_output")
        with open(coords_ffp, "w") as coords_fp:
            coords_fp.write(f"{len(stat_df)}\n")
        stat_df["stat_name"] = stat_df.index
        stat_df.to_csv(coords_ffp, sep=" ", header=None, mode="a", index=None)
        merged_df = stat_df
        for z_type in z_types:

            with open(config_ffp, "w") as config_fp:
                config_fp.write("CALL_TYPE=GENERATE_THRESHOLD_USER_INPUT\n"
                                f"MODEL_VERSION={version}\n"
                                f"OUTPUT_DIR={vm_working_dir}\n"
                                f"COORDINATES_TEXTFILE={coords_ffp}\n"
                                f"VS_TYPE={z_type}\n")

            shutil.rmtree(vm_working_dir)
            subprocess.run([nzvm_path, config_ffp], cwd="/home/jam335/code/Velocity-Model")

            z_values = pd.read_csv(os.path.join(vm_working_dir, file_output_format[z_type]), delim_whitespace=True)
            stat_df["z_index"] = z_values.index
            merged_df = merged_df.merge(z_values, left_on="z_index", right_index=True)
    output_columns = [column_names[z_type] for z_type in z_types]
    return merged_df[output_columns]


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("-ll", help="Station ll file")
    parser.add_argument("-z", "--z-type", choices=["Z1.0", "Z2.5", "vs30", "vs500"], help="Selects what Z type to extract: [%(choices)s]", nargs="+")
    parser.add_argument("-v", "--version", help="VM version to use")
    parser.add_argument("-o", "--output", help="path to output file")
    parser.add_argument("--nzvm-path")
    args = parser.parse_args()

    stat_df = formats.load_station_file(args.ll)

    output_file = os.path.join(args.output, "z.csv")
    z_df = extract_z(args.version, args.z_type, stat_df, args.nzvm_path)

    z_df.to_csv(output_file)
