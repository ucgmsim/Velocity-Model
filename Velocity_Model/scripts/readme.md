#update basins.py

sbatch --export=ALL gen_basin_stats.sl

cd outdir

cat basin_stats_????.ll > ../basin_stats.ll
cat basin_stats_????.csv > ../basin_stats.csv

#(!524) $ head basin_stats.ll
#168.2632171899503 -44.71216293699213 1224800_5038100
#168.26329053597905 -44.71126577833483 1224800_5038200
#168.26336387858157 -44.710368619441034 1224800_5038300

#(!525) $ head basin_stats.csv
#168.2632171899503, -44.71216293699213, 1224800, 5038100, True, WakatipuBasinOutlineWGS84.txt
#168.26329053597905, -44.71126577833483, 1224800, 5038200, True, WakatipuBasinOutlineWGS84.txt
#168.26336387858157, -44.710368619441034, 1224800, 5038300, True, WakatipuBasinOutlineWGS84.txt


cd ..
mkdir -p splits
cd splits
split -l40000 -d -a4 ../basin_stats.ll basin_stats.ll_
#160 splits created

cd ../
mkdir -p z_values

sbatch --export=ALL get_z_values.sl #may need to update -v 2.07



cd z_values
cat basin_stats.z_???? > ../basin_stats.z
cd ..


python combine_basin_stats_z.py basin_stats.csv basin_stats.z

