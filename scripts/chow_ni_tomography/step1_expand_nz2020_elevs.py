import numpy as np
import pandas as pd
from qcore.geo import wgs_nztm2000x
from pathlib import Path
from scipy.interpolate import griddata
from scipy import interpolate
from shared import chow_elevs, lats, lons, elevs, NZ_tomo_dir, step1_outdir,v_file_template


v_types = ['vp','vs','rho']

tomo_index=pd.MultiIndex.from_product([lats,lons,elevs], names=['lat','lon','elev'])

ep2020_df=pd.DataFrame(index=tomo_index,columns=v_types,dtype=float)

ep2020_dic={}

# load 2020_NZ tomograpy data
for elev in elevs:
    print(elev)
    ep2020_dic[elev]={}
    for v_type in v_types:
        v_file = NZ_tomo_dir/f"surf_tomography_{v_type}_elev{elev}.in"
        v_df = pd.read_csv(v_file, delimiter=' ', header=2, na_filter=False) #use lon as column name
        v_df = v_df.truncate(axis='columns', after=v_df.columns[-2])  # remove the last NaN column and rename the index to use lat
        v_df.rename(dict(zip(v_df.index, lats)), axis='index', inplace=True)

        ep2020_dic[elev][v_type] = v_df

#ep2020_dic is the whole 2020_NZ tomography. Its elevs are only 25 long -750 <=.. <=15 , while chow_elevs are 140 long. -400<=..<= 3

new_chow_elevs = [-750, -620] + chow_elevs + [15]  # add missing elevs to chow_elevs


#We are going to expand 2020_NZ such that it covers the elevation levels of Chow NI via interpolation
for v_type in v_types:

    v_data=[ep2020_dic[elev][v_type].to_numpy() for elev in elevs] #data from each .in file (each elev value)
    v_array=np.stack(v_data,axis=2) #stack them all together along z-axis
    new_v_data=np.zeros((len(lats),len(lons),len(new_chow_elevs)))

    for lat_id in range(len(lats)):
        for lon_id in range(len(lons)):
            f=interpolate.interp1d(elevs,v_array[lat_id][lon_id])
            new_v=f(new_chow_elevs)
            new_v_data[lat_id][lon_id]=new_v
    for elev_id, elev in enumerate(new_chow_elevs):
        outfile=Path(step1_outdir)/v_file_template.format(v_type, int(elev))
        with open(outfile,"w") as f:
            f.write(f"{len(lats)} {len(lons)}\n")
            f.write(" ".join(["{:.6f}".format(x) for x in lats]) + "\n") #trim the last " "
            f.write(" ".join(["{:.6f}".format(x) for x in lons]) + "\n")
            for lat_id in range(len(lats)):
                row_array=new_v_data[lat_id,:,elev_id]
                row_str=" ".join(["{:.6f}".format(x) for x in row_array])+"\n"
                f.write(row_str)
        print(f"{outfile} written")