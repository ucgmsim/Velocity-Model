import argparse
import os
import tempfile


def write_config_files():
    with tempfile.TemporaryDirectory as td:
        os.path.join(td, "GENERATE_THRESHOLD_USER_INPUT.txt")


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("-ll", help="Station ll file")
    parser.add_argument("-z_type", choices=["Z1p0", "Z2p5", "Vs30", "vs500"], help="Selects what Z type to extract: [%(choices)s]", nargs="+")
    parser.add_argument("-v", "--version", help="VM version to use")
    parser.add_argument("-o", "--output", help="path to output file")
    args = parser.parse_args()

