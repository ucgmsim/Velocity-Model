import argparse
import os
import shutil
import tempfile


def write_config_files(output, version, z_type, station_ffp):
    with tempfile.TemporaryDirectory() as temp_dir:
        config_ffp = os.path.join(temp_dir, "GENERATE_THRESHOLD_USER_INPUT.txt")
        coords_ffp = os.path.join(temp_dir, "MultipleProfileParameters.txt")
        with open(config_ffp, "w") as config_fp:
            config_fp.write("CALL_TYPE=GENERATE_THRESHOLD_USER_INPUT"
                            f"MODEL_VERSION={version}"
                            f"OUTPUT_DIR={output}"
                            f"COORDINATES_TEXTFILE={coords_ffp}"
                            f"VS_TYPE={z_type}")

        with open(station_ffp) as stat_fp, open(coords_ffp, "w") as coords_fp:

            stations = stat_fp.readlines()
            coords_fp.write(f"{len(stations)}\n")
            coords_fp.write("".join(stations))




if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("-ll", help="Station ll file")
    parser.add_argument("-z", "--z-type", choices=["Z1p0", "Z2p5", "Vs30", "vs500"], help="Selects what Z type to extract: [%(choices)s]", nargs="+")
    parser.add_argument("-v", "--version", help="VM version to use")
    parser.add_argument("-o", "--output", help="path to output file")
    parser.add_argument("--NZVM-path")
    args = parser.parse_args()

    write_config_files(args.output, args.version, args.z_type, args.ll)