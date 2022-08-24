#!/usr/bin/env python


out_file = "basin_stats_z.csv"


from pathlib import Path
import pandas as pd
import argparse



if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("basin_stats_csv", help="file containing (lon,lat,NZGD_lon,NZGD_lat,in_basin,basin_name) info. produced by gen_basin_stats.py")
    parser.add_argument("basin_stats_z", help="file containing (name,Z1.0,Z2.5,sigma) info. produced by get_z.py")

    args = parser.parse_args()


    csv_df=pd.read_csv(args.basin_stats_csv,header=None,names=['lon','lat','NZGD_lon','NZGD_lat','in_basin','basin_name'])
    z_df=pd.read_csv(args.basin_stats_z,names=['name','Z1.0','Z2.5','sigma'],skiprows=1,index_col=0)


    assert len(csv_df)==len(z_df)


    names = []
    for i in range(len(csv_df)):
        names.append(f"{csv_df.loc[i].NZGD_lon}_{csv_df.loc[i].NZGD_lat}")
    csv_df=csv_df.assign(name=names)
    csv_df=csv_df.set_index("name")


    # We assume both df have the same set of names
    csv_df["Z1.0"]=z_df["Z1.0"]
    csv_df["Z2.5"]=z_df["Z2.5"]
    csv_df.to_csv(out_file,columns=["NZGD_lon","NZGD_lat","in_basin","Z1.0","Z2.5","basin_name"],index=False)

