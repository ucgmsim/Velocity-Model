from pathlib import Path
import numpy as np
import csv

# edit these two lines depending on your environment
NI_tomo_dir = Path(
    "/home/seb56/Velocity-Model/Chow_NI"
)  # Download nz_atom_north_chow_etal_2021_vp+vs-*.r0.1-n4.nc files from http://ds.iris.edu/ds/products/emc-nz_atom_north_chow_etal_2021_vpvs/
EP2020_tomo_dir = Path(__file__).parents[2].resolve() / "Data/Tomography/2020_NZ"

chow_yaml = Path(__file__).parent / "chow_ni.yaml"
ep2020_yaml = Path(__file__).parent / "ep2020.yaml"


work_dir = NI_tomo_dir / "workdir"
step1_outdir = work_dir / "nz2020_tomo_elevs_ext"
step2_outdir = work_dir / "chow_tomo_grid_mapped"
step3_outdir = work_dir / "final"  # copy this to Data/Tomography under Velocity-Model
compare_dir = work_dir / "compare"
log_dir = work_dir / "log"

MAX_DIST = 20  # km
depth_categories = ["shallow","crust","mantle"]
v_file_template = "surf_tomography_{}_elev{}.in"  # .format(lower(v_type), int(elev))
v_types = ["vp", "vs", "rho"]

# Convert a floating point number to a string with "p" instead of "." and a given number of significant figures
def p_float(n:float, afterpoint:int=2):
    s="{:f}".format(n)
    int_part, float_part=s.split(".")
    if int(float_part)==0:
        return int_part
    else:
        return int_part+"p"+float_part[:afterpoint]

def output_exists(outdir: Path, v_type:str, elev: float):
    output_file=outdir/v_file_template.format(v_type,p_float(elev))
    if output_file.exists():
        print(f"{output_file} exists")
        return True
    else:
        return False
    
def write_file(
    outdir: Path,
    v_type: str,
    V: np.ndarray,
    elev: int,
    lats: np.ndarray,
    lons: np.ndarray,
):
    filename = outdir /  v_file_template.format(v_type, p_float(elev))
    print(filename)
    with open(filename, "w") as csvfile:
        writer = csv.writer(csvfile, delimiter=" ")
        writer.writerow([len(lats), len(lons)])
        writer.writerow(["{:.6f}".format(x) for x in lats])
        writer.writerow(["{:.6f}".format(x) for x in lons])
        for i in range(len(lats)):
            writer.writerow(["{:.6f}".format(x) for x in V[i]])



