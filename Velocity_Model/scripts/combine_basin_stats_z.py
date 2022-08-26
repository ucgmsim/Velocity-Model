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
    z_df=pd.read_csv(args.basin_stats_z,names=['name','Z1.0','Z2.5','sigma'],index_col=0)


    assert len(csv_df)==len(z_df)

    #make a new column "name" buy combining two existing columns
    csv_df["name"]=(csv_df["NZGD_lon"].map(str)+"_"+csv_df["NZGD_lat"].map(str))
    csv_df=csv_df.set_index("name")


    # We assume both df have the same set of names
    combined_df = csv_df.merge(z_df,on="name")
    combined_df.to_csv(out_file,columns=["NZGD_lon","NZGD_lat","in_basin","Z1.0","Z2.5","basin_name"],index=False)

