import yaml
from shared import EP2020_tomo_dir, ep2020_yaml

"""
This code is just a one-off to read EP2020 tomography data and find out the lats, lons and elev values it uses
"""

in_files = EP2020_tomo_dir.glob("*vp*.in")
n_lats = 0
n_lons = 0
lats = []
lons = []
elevs = []
for in_file in in_files:
    print(in_file)
    elevs.append(int(in_file.stem.strip(".in").split("_elev")[1]))
    with open(in_file, "r") as f:
        size_str = f.readline()
        lats_str = f.readline()
        lons_str = f.readline()
        new_n_lats, new_n_lons = [int(x) for x in size_str.split()]
        new_lats = [float(x) for x in lats_str.split()]
        new_lons = [float(x) for x in lons_str.split()]
        if n_lats == 0:
            n_lats = new_n_lats
        if n_lons == 0:
            n_lons = new_n_lons
        if len(lats) == 0:
            lats = new_lats
        if len(lons) == 0:
            lons = new_lons
        assert n_lats == new_n_lats
        assert n_lons == new_n_lons
        assert lats == new_lats
        assert lons == new_lons
elevs.sort()

# Save the data
ep2020={'lats': lats,'lons': lons, 'elevs': elevs}
with open(ep2020_yaml,"w") as file:
    yaml.dump(ep2020,file)