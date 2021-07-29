import argparse
from pathlib import Path

from qcore import formats
from Velocity_Model.z import extract_z, basin_outlines as vm_versions

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-ll", help="Station ll file", required=True)
    parser.add_argument(
        "-z",
        "--z-type",
        choices=[
            "Z1.0",
            "Z2.5",
        ],  # , "vs30", "vs500"], -- couldn't get vs30 or vs500 working despite documentation saying it should work
        help="Selects what Z type to extract: [%(choices)s]",
        nargs="+",
        required=True,
    )
    parser.add_argument(
        "-v",
        "--version",
        help="VM version to use",
        default="2.06",
        choices=vm_versions.keys(),
    )
    parser.add_argument("-o", "--output", help="path to output file", default=".", type=Path)
    parser.add_argument("--nzvm-path", default="NZVM", type=Path)
    args = parser.parse_args()

    stat_df = formats.load_station_file(args.ll)

    output_file = args.output
    z_df = extract_z(args.z_type, stat_df, args.nzvm_path, args.version)
    z_df.to_csv(output_file)
