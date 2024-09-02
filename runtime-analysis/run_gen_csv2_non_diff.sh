#!/bin/bash 

# Define the directory name 
DIR="csv2/non_diff"

# Check if the directory exists
if [ -d "$DIR" ]; then 
  echo "Directory $DIR exists. Removing it."
  rm -rf "$DIR"
fi 

# Create the directory
mkdir "$DIR"
echo "Directory $DIR created"

# Run experiments
python gen_csv2.py -i "../runtime-dataset/test" -o "$DIR/runtime_1.csv" -p "../build/non_diff_max_dist_runtime"
python gen_csv2.py -i "../runtime-dataset/test" -o "$DIR/runtime_2.csv" -p "../build/non_diff_max_dist_runtime"