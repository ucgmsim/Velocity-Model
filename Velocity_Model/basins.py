from pathlib import Path
import yaml

vm_data_dir = (Path(__file__).parents[1] / "Data").resolve()

def get_basin_full_path(basin_outlines):
    return [vm_data_dir / basin for basin in basin_outlines]


with open(vm_data_dir/ "basins.yaml", "r") as yaml_file:
    yaml_data = yaml.safe_load(yaml_file)


# [x['files'] for x in data['version'][0]['items']]
def get_basin_files_for_a_version(ver):
    flat_files = []
    for data in yaml_data:
        if str(data["version"]) == ver:
            files = [basin["files"] for basin in data["basins"]]
            flat_files = [file for sublist in files for file in sublist]
            break
    if len(flat_files) == 0:
        print(f"Empty - check version {ver}")
    return flat_files


versions = ["2.03", "2.04", "2.05", "2.06", "2.07"]
basin_outlines_dict = {}
prev_ver_basins = []
for ver in versions:
    basin_outlines_dict[ver] = prev_ver_basins + get_basin_full_path(
        get_basin_files_for_a_version(ver)
    )
    prev_ver_basins = basin_outlines_dict[ver]
