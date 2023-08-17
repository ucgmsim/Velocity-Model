from pathlib import Path

def get_basin_full_path(basin_outlines):
    vm_dir = Path(__file__).parent.parent.resolve()
    return [vm_dir / basin for basin in basin_outlines]


v203_basin_outlines = get_basin_full_path(
    [
        "Data/Basins/Wellington/v19p6/Wellington_Polygon_Wainuiomata_WGS84.txt",
        "Data/Boundaries/NewCanterburyBasinBoundary_WGS84_1m.txt",
        "Data/Boundaries/BPVBoundary.txt",
        "Data/SI_BASINS/Cheviot_Polygon_WGS84.txt",
        "Data/SI_BASINS/Hanmer_Polygon_WGS84.txt",
        "Data/SI_BASINS/Kaikoura_Polygon_WGS84.txt",
        "Data/SI_BASINS/Marlborough_Polygon_WGS84_v0p1.txt",
        "Data/SI_BASINS/Nelson_Polygon_WGS84.txt",
        "Data/SI_BASINS/NorthCanterbury_Polygon_WGS84.txt",
        "Data/Boundaries/WaikatoHaurakiBasinEdge_WGS84.txt",
    ]
)
v204_basin_outlines = v203_basin_outlines + get_basin_full_path(
    [
        "Data/USER20_BASINS/WanakaOutlineWGS84.txt",
        "Data/USER20_BASINS/WakatipuBasinOutlineWGS84.txt",
        "Data/USER20_BASINS/alexandra_outline.txt",
        "Data/USER20_BASINS/ranfurly_outline.txt",
        "Data/USER20_BASINS/NE_otago/NE_otago_A_outline.txt",
        "Data/USER20_BASINS/NE_otago/NE_otago_B_outline.txt",
        "Data/USER20_BASINS/NE_otago/NE_otago_C_outline.txt",
        "Data/USER20_BASINS/NE_otago/NE_otago_D_outline.txt",
        "Data/USER20_BASINS/NE_otago/NE_otago_E_outline.txt",
        "Data/USER20_BASINS/bal_outline_WGS84.txt",
        "Data/USER20_BASINS/dun_outline_WGS84.txt",
        "Data/USER20_BASINS/mos_outline_WGS84.txt",
        "Data/USER20_BASINS/Murchison_Basin_Outline_v1_WGS84.txt",
        "Data/USER20_BASINS/hakataramea_outline_WGS84.txt",
        "Data/USER20_BASINS/waitaki_outline_WGS84.txt",
        "Data/USER20_BASINS/mackenzie_basin_outline_nzmg.txt",
    ]
)
v205_basin_outlines = v204_basin_outlines
v206_basin_outlines = v205_basin_outlines + get_basin_full_path(
    [
        "Data/USER20_BASINS/SpringsJ_basin_outline_v1_WGS84.txt",
        "Data/USER20_BASINS/Karamea_basin_outline_v1_WGS84.txt",
        "Data/USER20_BASINS/CollingwoodBasinOutline_1_WGS84_v1.txt",
        "Data/USER20_BASINS/CollingwoodBasinOutline_2_WGS84_v1.txt",
        "Data/USER20_BASINS/CollingwoodBasinOutline_3_WGS84_v1.txt",
    ]
)

# Wellington Basin update has the same outline so have not updated it for this
v207_basin_outlines = v206_basin_outlines + get_basin_full_path(
    [
        "Data/Basins/Greater_Wellington_and_Porirua/v21p7/GreaterWellington1_Outline_WGS84.dat",
        "Data/Basins/Greater_Wellington_and_Porirua/v21p7/GreaterWellington2_Outline_WGS84.dat",
        "Data/Basins/Greater_Wellington_and_Porirua/v21p7/GreaterWellington3_Outline_WGS84.dat",
        "Data/Basins/Greater_Wellington_and_Porirua/v21p7/GreaterWellington4_Outline_WGS84.dat",
        "Data/Basins/Greater_Wellington_and_Porirua/v21p7/GreaterWellington5_Outline_WGS84.dat",
        "Data/Basins/Greater_Wellington_and_Porirua/v21p7/GreaterWellington6_Outline_WGS84.dat",
        "Data/Basins/Greater_Wellington_and_Porirua/v21p7/Porirua1_Outline_WGS84.dat",
        "Data/Basins/Greater_Wellington_and_Porirua/v21p7/Porirua2_Outline_WGS84.dat",
        "Data/Basins/Napier_Hawkes_Bay/v21p7/HawkesBay1_Outline_WGS84_delim.dat",
        "Data/Basins/Napier_Hawkes_Bay/v21p7/HawkesBay2_Outline_WGS84_delim.dat",
        "Data/Basins/Napier_Hawkes_Bay/v21p7/HawkesBay3_Outline_WGS84_delim.dat",
        "Data/Basins/Napier_Hawkes_Bay/v21p7/HawkesBay4_Outline_WGS84_delim.dat",
        "Data/Basins/Napier_Hawkes_Bay/v21p7/Napier1_Outline_WGS84_delim.dat",
        "Data/Basins/Napier_Hawkes_Bay/v21p7/Napier2_Outline_WGS84_delim.dat",
        "Data/Basins/Napier_Hawkes_Bay/v21p7/Napier3_Outline_WGS84_delim.dat",
        "Data/Basins/Napier_Hawkes_Bay/v21p7/Napier4_Outline_WGS84_delim.dat",
        "Data/Basins/Napier_Hawkes_Bay/v21p7/Napier5_Outline_WGS84_delim.dat",
        "Data/Basins/Napier_Hawkes_Bay/v21p7/Napier6_Outline_WGS84_delim.dat",
        "Data/Basins/Gisborne/v21p11/Gisborne_Outline_WGS84_delim.dat",
        "Data/Basins/Southern_Hawkes_Bay/v21p12/SHB_Outline_WGS84_delim.dat",
        "Data/Basins/Wairarapa/v21p12/Wairarapa_Outline_WGS84_delim.dat",
        "Data/Basins/East_Cape/v22p3/Motu_bay1_Outline_WGS84.txt",
        "Data/Basins/East_Cape/v22p3/Motu_bay2_Outline_WGS84.txt",
        "Data/Basins/East_Cape/v22p3/Motu_bay3_Outline_WGS84.txt",
        "Data/USER20_BASINS/Whangaparoa_surface_WG84.txt",
    ]
)
basin_outlines_dict = {
    "2.03": v203_basin_outlines,
    "2.04": v204_basin_outlines,
    "2.05": v205_basin_outlines,
    "2.06": v206_basin_outlines,
    "2.07": v207_basin_outlines,
}


