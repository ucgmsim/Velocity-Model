import numpy as np
import pandas as pd
import yaml
from multiprocessing import Pool
from functools import partial

from qcore import geo
from shared import (
    write_file,
    ep2020_yaml,
    chow_yaml,
    v_file_template,
    step1_outdir,
    step2_outdir,
    step3_outdir,
    log_dir,
    MAX_DIST,
    v_types,
    output_exists,
    p_float,
)

"""
After Step 1 and Step 2, we have EP2020 tomography data and Chow NI tomography data on the same 3d grid. 
This code combines two data sets based on a simple logic, such that for all elev levels, at a location (lat, lon),  
1. if Chow NI is 0, (outside Chow NI domain), use EP2020 value
2. else
    if (lat, lon) is within MAX_DIST away from one of 4 boundaries of the domain, apply smoothing
    else use Chow NI value
"""

step3_outdir.mkdir(parents=True, exist_ok=True)
log_dir.mkdir(parents=True, exist_ok=True)

with open(ep2020_yaml) as file:
    ep2020_data = yaml.safe_load(file)

with open(chow_yaml) as file:
    chow_data = yaml.safe_load(file)

lats = ep2020_data["lats"]
lons = ep2020_data["lons"]
elevs = ep2020_data["elevs"]
chow_elevs = chow_data["elevs"]

new_chow_elevs = sorted(
    [x for x in elevs if x < min(chow_elevs) or x > max(chow_elevs)] + chow_elevs
)  # add missing elevs to chow_elevs


def nonzero_grid(v_array):
    (
        nonzero_vert_ids,
        nonzero_hori_ids,
    ) = v_array.nonzero()  # return row col indices of nonzero values
    return nonzero_vert_ids, nonzero_hori_ids


#
def find_boundaries(nonzero_vert_ids, nonzero_hori_ids):

    right_boundary = dict(
        zip(nonzero_vert_ids, nonzero_hori_ids)
    )  # for a given row index, the maximum col index is stored
    left_boundary = dict(
        zip(nonzero_vert_ids[::-1], nonzero_hori_ids[::-1])
    )  # for a given row index, the minimum col index is stored

    top_boundary = dict(
        zip(nonzero_hori_ids, nonzero_vert_ids)
    )  # for a given col index, x, the maximum row index is stored
    bottom_boundary = dict(
        zip(nonzero_hori_ids[::-1], nonzero_vert_ids[::-1])
    )  # for a given col index, the minimum index is stored

    return top_boundary, bottom_boundary, right_boundary, left_boundary


# Find the distance to the 4 boundaries from a location, and return the minimum of 4 distances.
def distance_from_boundary(x_id, y_id, boundaries):
    top_boundary, bottom_boundary, right_boundary, left_boundary = boundaries
    res = geo.get_distances(
        np.array(
            [
                [lats[x_id], lons[left_boundary[x_id]]],
                [lats[x_id], lons[right_boundary[x_id]]],
                [lats[top_boundary[y_id]], lons[y_id]],
                [lats[bottom_boundary[y_id]], lons[y_id]],
            ]
        ),
        lats[x_id],
        lons[y_id],
    )
    return min(res)


def combine_and_smooth(
    ep2020_tomo_elevs_ext_array,
    chow_tomo_grid_mapped_array,
    nonzero_vert_ids,
    nonzero_hori_ids,
    boundaries,
    logfile=None,
):

    final_v_array = (
        ep2020_tomo_elevs_ext_array.copy()
    )  # start with ep2020_tomo_elevs_ext_array

    with open(logfile, "w") as f:
        for i in range(
            len(nonzero_vert_ids)
        ):  # go through non_zero values (ie. points in chow NI domain)
            lat_id = nonzero_vert_ids[i]
            lon_id = nonzero_hori_ids[i]

            d = distance_from_boundary(lat_id, lon_id, boundaries)

            # if beyond MAX_DIST, use values from Chow tomo, otherwise, smooth.
            if MAX_DIST > 0:
                smoothDistRatio = min(d / MAX_DIST, 1.0)
            else:
                smoothDistRatio = 1

            inverseRatio = 1 - smoothDistRatio

            # print(f"{lat_id} {lon_id} : {smoothDistRatio}*{chow_tomo_grid_mapped_array[lat_id][lon_id]}+{inverseRatio}*{ep2020_tomo_elevs_ext_array[lat_id][lon_id]}")
            final_v_array[lat_id][lon_id] = (
                smoothDistRatio * chow_tomo_grid_mapped_array[lat_id][lon_id]
                + inverseRatio * ep2020_tomo_elevs_ext_array[lat_id][lon_id]
            )
            res = f"{lat_id},{lon_id},{smoothDistRatio:.3f},{chow_tomo_grid_mapped_array[lat_id][lon_id]:.6f},{inverseRatio:.3f},{ep2020_tomo_elevs_ext_array[lat_id][lon_id]:.6f},{final_v_array[lat_id][lon_id]:.6f}"
            f.write(res + "\n")
        # print(res)
    return final_v_array


def process_per_elev(nonzero_vert_ids, nonzero_hori_ids, boundaries, elev):

    for v_type in v_types:
        v_file = v_file_template.format(v_type, p_float(elev))
        if output_exists(step3_outdir, v_type, elev):
            continue

        ep2020_tomo_elevs_ext_df = pd.read_csv(
            step1_outdir / v_file, delimiter=" ", header=2, na_filter=False
        )
        ep2020_tomo_elevs_ext_array = ep2020_tomo_elevs_ext_df.to_numpy()

        if not (step2_outdir / v_file).exists():
            # outside the chow domain. mixing not needed
            final_v_array = ep2020_tomo_elevs_ext_array
        else:
            # inside the chow domain. may need mixing
            chow_tomo_grid_mapped_df = pd.read_csv(
                step2_outdir / v_file, delimiter=" ", header=2, na_filter=False
            )
            chow_tomo_grid_mapped_array = chow_tomo_grid_mapped_df.to_numpy()
            logfile = log_dir / f"{v_file}.log"

            # depending on the distance to the boundary, we may need to do smoothing
            final_v_array = combine_and_smooth(
                ep2020_tomo_elevs_ext_array,
                chow_tomo_grid_mapped_array,
                nonzero_vert_ids,
                nonzero_hori_ids,
                boundaries,
                logfile,
            )

        write_file(step3_outdir, v_type, final_v_array, elev, lats, lons)


if __name__ == "__main__":
    # we know that for any elev and v_type, the lats and lons are all the same
    elev = -25  # pick a random elev
    v_type = v_types[0]

    v_file = v_file_template.format(v_type, p_float(elev))

    chow_tomo_grid_mapped_df = pd.read_csv(
        step2_outdir / v_file, delimiter=" ", header=2, na_filter=False
    )
    chow_tomo_grid_mapped_array = chow_tomo_grid_mapped_df.to_numpy()

    nonzero_vert_ids, nonzero_hori_ids = nonzero_grid(chow_tomo_grid_mapped_array)
    boundaries = find_boundaries(nonzero_vert_ids, nonzero_hori_ids)

    with Pool(40) as pool:
        res = pool.map_async(
            partial(process_per_elev, nonzero_vert_ids, nonzero_hori_ids, boundaries),
            new_chow_elevs,
        )
        res = res.get()

# This generates the final files in step3_outdir. Copy them to Velocity-Model/Data/Tomography/2023_Chow_NI, and edit EPtomo2010subMod.c and getSurfSubMod.c to use these files.
