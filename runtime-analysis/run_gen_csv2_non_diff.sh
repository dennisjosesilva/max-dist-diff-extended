#!/bin/bash 

# options
options=("1920x1080", "960x540", "480x270", "240x135", "120x68", "quit")
REL=""

echo "chosse a resolution option:"
select opt in "${options[@]}"
do 
  # Save the selected option directly
  REL="$opt"

  # Check if the user select "Quit" or an invalid option
  if [ "$opt" == "quit" ]; then 
    echo "Exiting..."
    break
  elif [ -z "$opt" ]; then 
    echo "Invalid option $REL. Please choose a valid number."
  else
    echo "You selected: $REL"
    break
  fi
done

if [ "$REL" != "quit"]

  Define the directory name 
  DIR="csv2/non_diff/$REL"

  # Check if the directory exists
  if [ -d "$DIR" ]; then 
    echo "Directory $DIR exists. Removing it."
    rm -rf "$DIR"
  fi 

  # Create the directory
  mkdir "$DIR"
  echo "Directory $DIR created"

  # Run experiments
  python gen_csv2.py -i "../runtime-dataset/dataset-icdar/$REL/validation" -o "$DIR/runtime_1.csv" -p "../build/non_diff_max_dist_runtime"
  sleep 2
  python gen_csv2.py -i "../runtime-dataset/dataset-icdar/$REL/validation" -o "$DIR/runtime_2.csv" -p "../build/non_diff_max_dist_runtime"
  sleep 2
  python gen_csv2.py -i "../runtime-dataset/dataset-icdar/$REL/validation" -o "$DIR/runtime_3.csv" -p "../build/non_diff_max_dist_runtime"
if 