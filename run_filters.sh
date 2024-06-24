# height
echo "cleaning images"
rm ./out/height/*
rm ./out/max_dist/*
rm ./out/volume/*
rm ./out/area/*
rm ./out/inertia/*

echo "\nPerforming extinction filter for height"
./build/extinction_filter_height ./dat/cable.png ./out/height/01.png 0 1
./build/extinction_filter_height ./dat/cable.png ./out/height/02.png 0 2
./build/extinction_filter_height ./dat/cable.png ./out/height/03.png 0 3
./build/extinction_filter_height ./dat/cable.png ./out/height/04.png 0 4
./build/extinction_filter_height ./dat/cable.png ./out/height/05.png 0 5
./build/extinction_filter_height ./dat/cable.png ./out/height/06.png 0 6
./build/extinction_filter_height ./dat/cable.png ./out/height/07.png 0 7
./build/extinction_filter_height ./dat/cable.png ./out/height/08.png 0 8
./build/extinction_filter_height ./dat/cable.png ./out/height/09.png 0 9

echo "\nPerforming extinction filter for maximum distance"
./build/extinction_filter_max_dist ./dat/cable.png ./out/max_dist/01.png 0 1
./build/extinction_filter_max_dist ./dat/cable.png ./out/max_dist/02.png 0 2
./build/extinction_filter_max_dist ./dat/cable.png ./out/max_dist/03.png 0 3
./build/extinction_filter_max_dist ./dat/cable.png ./out/max_dist/04.png 0 4
./build/extinction_filter_max_dist ./dat/cable.png ./out/max_dist/05.png 0 5
./build/extinction_filter_max_dist ./dat/cable.png ./out/max_dist/06.png 0 6
./build/extinction_filter_max_dist ./dat/cable.png ./out/max_dist/07.png 0 7
./build/extinction_filter_max_dist ./dat/cable.png ./out/max_dist/08.png 0 8
./build/extinction_filter_max_dist ./dat/cable.png ./out/max_dist/09.png 0 9

echo "\nPerforming extinction filter for volume"
./build/extinction_filter_volume ./dat/cable.png ./out/volume/01.png 0 1
./build/extinction_filter_volume ./dat/cable.png ./out/volume/02.png 0 2
./build/extinction_filter_volume ./dat/cable.png ./out/volume/03.png 0 3
./build/extinction_filter_volume ./dat/cable.png ./out/volume/04.png 0 4
./build/extinction_filter_volume ./dat/cable.png ./out/volume/05.png 0 5
./build/extinction_filter_volume ./dat/cable.png ./out/volume/06.png 0 6
./build/extinction_filter_volume ./dat/cable.png ./out/volume/07.png 0 7
./build/extinction_filter_volume ./dat/cable.png ./out/volume/08.png 0 8
./build/extinction_filter_volume ./dat/cable.png ./out/volume/09.png 0 9


echo "\nPerforming extinction filter for area"
./build/extinction_filter_area ./dat/cable.png ./out/area/01.png 0 1
./build/extinction_filter_area ./dat/cable.png ./out/area/02.png 0 2
./build/extinction_filter_area ./dat/cable.png ./out/area/03.png 0 3
./build/extinction_filter_area ./dat/cable.png ./out/area/04.png 0 4
./build/extinction_filter_area ./dat/cable.png ./out/area/05.png 0 5
./build/extinction_filter_area ./dat/cable.png ./out/area/06.png 0 6
./build/extinction_filter_area ./dat/cable.png ./out/area/07.png 0 7
./build/extinction_filter_area ./dat/cable.png ./out/area/08.png 0 8
./build/extinction_filter_area ./dat/cable.png ./out/area/09.png 0 9

echo "\nPerforming extinction filter for inertia"
./build/extinction_filter_area ./dat/cable.png ./out/inertia/01.png 0 1
./build/extinction_filter_area ./dat/cable.png ./out/inertia/02.png 0 2
./build/extinction_filter_area ./dat/cable.png ./out/inertia/03.png 0 3
./build/extinction_filter_area ./dat/cable.png ./out/inertia/04.png 0 4
./build/extinction_filter_area ./dat/cable.png ./out/inertia/05.png 0 5
./build/extinction_filter_area ./dat/cable.png ./out/inertia/06.png 0 6
./build/extinction_filter_area ./dat/cable.png ./out/inertia/07.png 0 7
./build/extinction_filter_area ./dat/cable.png ./out/inertia/08.png 0 8
./build/extinction_filter_area ./dat/cable.png ./out/inertia/09.png 0 9