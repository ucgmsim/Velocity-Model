# Preparation for Vs30map update

This is an instruction on how to generate a file containing points *inside* known basins, and their Z-values.
The points are from 100m x 100m grid (hard-coded in gen_basin_stats.py), and we will be using v2.07 basins.

Tihs manual is based on the assumption that you are using NeSI Maui.
It takes about 3 days to complete the steps given in this manual.



## Generation of grid points within basins

(Duration: 2 days 7 hours)

If there are more basins added, update basins.py

```
sbatch --export=ALL gen_basin_stats.sl
```
This will take 2 days and about 7 hours using 1340 cores on Maui. As Maui has 24 hours wall clock limit, you will need to resubmit twice after the 24 hours of run.
Simply enter the same command, and it will resume from the checkpoint.

To confirm if everything is completed, go into `outdir` directory and enter this command

```
$ cat checkpoint* |sort -n -r |tail
121591 121591
121591 121591
121591 121591
121591 121591
121591 121591
121591 121591
121591 121591
121591 121591
118999 121591
117999 121591
```
The first number is the last checkpoint index and the second is the last index that each core has to process. So the output above means the last 2 processes are still running.

When everything is completed, go into `outdir` and merge the result.
```
cd outdir
cat basin_stats_????.ll > ../basin_stats.ll
cat basin_stats_????.csv > ../basin_stats.csv
cd ..
```

The generated files look like

```
#(!524) $ head basin_stats.ll
#168.2632171899503 -44.71216293699213 1224800_5038100
#168.26329053597905 -44.71126577833483 1224800_5038200
#168.26336387858157 -44.710368619441034 1224800_5038300
...
```
```
#(!525) $ head basin_stats.csv
#168.2632171899503, -44.71216293699213, 1224800, 5038100, True, WakatipuBasinOutlineWGS84.txt
#168.26329053597905, -44.71126577833483, 1224800, 5038200, True, WakatipuBasinOutlineWGS84.txt
#168.26336387858157, -44.710368619441034, 1224800, 5038300, True, WakatipuBasinOutlineWGS84.txt
...
```

To keep the job size manageable, we split them into 80k lines long files

```
mkdir -p splits
cd splits
split -l80000 -d -a4 ../basin_stats.ll basin_stats.ll_
cd ../
```

This creates 80 splits named `basin_stats.ll_0001` etc.

## Coputing Z values for given locations

(Duration: 6 hours)

We will be computing Z values using NZVM. This requires quite a large memory (96Gb peak), and it is recommended to run on Maui using the supplied SLURM script.
From above, we have 80 split files to process - if you chose to go with a different number, update the SLURM script.

```
mkdir -p z_values
sbatch --export=ALL get_z_values.sl # update the command therein to use the correct version eg. -v 2.07
```

This will keep 4 array jobs running at a time, each job takes less than 30 mins.

When everything is completed, merge all the output files into a single file
```
cd z_values
cat basin_stats.z_???? > ../basin_stats.z
cd ..
```

Finally, run the command below.

```
python combine_basin_stats_z.py basin_stats.csv basin_stats.z
```

This must be quick. It will create `basin_stats_z.csv` file.

```
NZGD_lon,NZGD_lat,in_basin,Z1.0,Z2.5,basin_name
1224800,5038100,True,0.045,0.275,WakatipuBasinOutlineWGS84.txt
1224800,5038200,True,0.045,0.275,WakatipuBasinOutlineWGS84.txt
1224800,5038300,True,0.045,0.275,WakatipuBasinOutlineWGS84.txt
...
```

You can use thie file as an input to run `basin2tif.py` to generate GeoTIFF for updating Vs30map. 
Follow the instructions given in https://github.com/ucgmsim/mapping/tree/master/mapbox/vs30 and run `basin2tif.py`
