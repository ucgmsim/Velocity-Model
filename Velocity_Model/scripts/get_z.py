import argparse
from pathlib import Path

from qcore import formats
from Velocity_Model.z import extract_z, basin_outlines_dict

z_types = ["Z1.0", "Z2.5"]

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-ll", help="Station ll file", required=True, type=Path)
    vm_versions = basin_outlines_dict.keys()
    parser.add_argument(
        "-v",
        "--version",
        help="VM version to use",
        default="2.07",
        choices=vm_versions,
    )
    parser.add_argument(
        "--no_header",
        dest="keep_header",
        help="Save output with no header",
        default=True,
        action="store_false",
    )
    # by default, the output file will be current_dir / {ll_file}.z
    # this is to avoid overwriting existing z-file in StationInfo directory by mistake
    parser.add_argument("-o", "--outdir", help="output directory", default=Path.cwd(), type=Path)
    parser.add_argument("--nzvm-path", default="NZVM", type=Path)
    args = parser.parse_args()

    stat_df = formats.load_station_file(args.ll)

    print(args.outdir)
    output_file = args.outdir / args.ll.with_suffix(".z").name

    print(f"Output to be saved as {output_file}")

    z_df = extract_z(z_types, stat_df, args.nzvm_path, args.version)
    print(z_df)
    z_df.to_csv(output_file,header=args.keep_header)
