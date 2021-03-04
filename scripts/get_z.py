import argparse
import os
import shutil
import subprocess
import tempfile

from qcore import formats


def write_config_files(output, version, z_types, stat_df, nzvm_path):
    with tempfile.TemporaryDirectory() as temp_dir:
        temp_dir = "/home/jam335/scratch/velocity_model/21p3_get_z/temp"
        config_ffp = os.path.join(temp_dir, "GENERATE_THRESHOLD_USER_INPUT.txt")
        coords_ffp = os.path.join(temp_dir, "MultipleProfileParameters.txt")
        with open(coords_ffp, "w") as coords_fp:
            coords_fp.write(f"{len(stat_df)}\n")
        stat_df.to_csv(coords_ffp, sep=" ", header=None, mode="a")

        for z_type in z_types:

            with open(config_ffp, "w") as config_fp:
                config_fp.write("CALL_TYPE=GENERATE_THRESHOLD_USER_INPUT\n"
                                f"MODEL_VERSION={version}\n"
                                f"OUTPUT_DIR={output}/z_output\n"
                                f"COORDINATES_TEXTFILE={coords_ffp}\n"
                                f"VS_TYPE={z_type}\n")

            subprocess.run([nzvm_path, config_ffp], cwd="/home/jam335/code/Velocity-Model")
            pass




if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("-ll", help="Station ll file")
    parser.add_argument("-z", "--z-type", choices=["Z1p0", "Z2p5", "Vs30", "vs500"], help="Selects what Z type to extract: [%(choices)s]", nargs="+")
    parser.add_argument("-v", "--version", help="VM version to use")
    parser.add_argument("-o", "--output", help="path to output file")
    parser.add_argument("--nzvm-path")
    args = parser.parse_args()

    stat_df = formats.load_station_file(args.ll)

    write_config_files(args.output, args.version, args.z_type, stat_df, args.nzvm_path)
