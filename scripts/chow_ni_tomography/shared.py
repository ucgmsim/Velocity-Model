from pathlib import Path
import numpy as np
import csv

# edit these two lines depending on your environment
NI_tomo_dir = Path(
    "/Users/sungbae/NI_tomography"
)  # place data from https://core.geo.vuw.ac.nz/d/feae69f61ea54f81bee1/
EP2020_tomo_dir = Path(__file__).parents[2].resolve() / "Data/Tomography/2020_NZ"


work_dir = NI_tomo_dir / "workdir"
step1_outdir = work_dir / "nz2020_tomo_elevs_ext"
step2_outdir = work_dir / "chow_tomo_grid_mapped"
step3_outdir = work_dir / "final"  # copy this to Data/Tomography under Velocity-Model
compare_dir = work_dir / "compare"
log_dir = work_dir / "log"

MAX_DIST = 20  # km
depth_categories = ["crust", "shallow", "mantle"]
v_file_template = "surf_tomography_{}_elev{}.in"  # .format(lower(v_type), int(elev))
v_types = ["vp", "vs", "rho"]


def write_file(
    outdir: Path,
    v_type: str,
    V: np.ndarray,
    elev: int,
    lats: np.ndarray,
    lons: np.ndarray,
):
    filename = outdir / v_file_template.format(v_type, int(elev))
    print(filename)
    with open(filename, "w") as csvfile:
        writer = csv.writer(csvfile, delimiter=" ")
        writer.writerow([len(lats), len(lons)])
        writer.writerow(["{:.6f}".format(x) for x in lats])
        writer.writerow(["{:.6f}".format(x) for x in lons])
        for i in range(len(lats)):
            writer.writerow(["{:.6f}".format(x) for x in V[i]])

chow_yaml = Path(__file__).parent / "chow_ni.yaml"
ep2020_yaml = Path(__file__).parent / "ep2020.yaml"


