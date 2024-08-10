# Requirements

* [CMake](https://cmake.org/)
* C++ Compiler (C++ 17)
* [ZLIB](https://www.zlib.net/)
* [OpenMP](https://www.openmp.org/)
* (optional) [CUDA](https://developer.nvidia.com/cuda-toolkit)

# Compiling

1. Create a "build" directory and go to this directory:
   
   ```shell
   $ mkdir build && cd build
   ```

2. Generate files for building:
   
   ```shell
   $ cmake .. 
   ```

3. build the programs:
   
   ```shell
   $ cmake --build .
   ```

After step 3, CMake will build the source code into different applications 
inside the build directory.

# Applications

The applications are in categories as follows:

* **Checking:** Applications that compare the results of the different implementations for computing 
                the maximum EDT value.
* **Display:**  Applications that display the values computed, usually by node.
* **Filters:**  Applications that filter the input image and produce a filtered image according to 
                a filter in component trees.
* **Runtime:**  Applications that measure the runtime of the different implementations for computing 
                the maximum EDT value.

## Checking

The applications in this category compare the results of the differential maximum EDT approaches to the 
nondifferential approach and print out the nodes that have mismatched results or a 
message if there is no mismatch. The application list is:

* **check_implementations** 
   ```shell
   $ ./check_implementation <input_image_path>
   ```
   * `<input_image_path>`: path to the image where the maximum EDT value will be computed.

   The application computes the maximum EDT value using the differential approach and 
   the non-differential approach and prints the mismatch values if it happens. It also
   saves a log .csv file if the compilation symbol APPDEBUG is defined.

* **check_contour_opt**
   ```shell
   $ ./check_contour_opt [input_image_path] [-c]   
   ```   
   * `[input_image_path]`: path to the image where the contour will be computed.
   * `[-c]`: Use this option to print the contour on the console and do not write debug images into the disk.

* **check_edt_opt** 
   ```shell
   $ ./check_diff_edt_opt [input_image_path]
   ```

   * `[input_image_path]`: path to the image where the maximum EDT value will be computed.

   If the `input_image_path` is set, the program computes the maximum EDT value for each node of the 
   component tree of the image stored in `input_image_path` using the optimised differential EDT 
   (using erosion) and the reconstruction-based EDT value. If there is a mismatch, it prints the 
   node ID and the mismatched values. Otherwise, it prints a successful checking message. 
   If the `input_image_path` is not set, the program computes the maximum EDT value using 
   the optimised differential EDT (using erosion) and the reconstruction-based EDT value of 
   a simple image and prints the binary nodes and the distance value in the console.

* **check_diff_edt_parallel**
   ```shell
   $ ./check_diff_edt_parallel [input_image_path]
   ```

   * `[input_image_path]`: path to the image where the maximum EDT value will be computed.

   The same as the `check_diff_edt_opt`, but the optimised maximum differential EDT value 
   approach uses a parallel implementation (OpenMP).

* **check_diff_edt_gpu**
   ```shell
   $ ./check_diff_edt_gpu [input_image_path]
   ```

   * `[input_image_path]`: path to the image where the maximum EDT value will be computed.

   The same as the `check_diff_edt_opt`, but the optimised maximum differential EDT value 
   approach uses a GPU-based parallel implementation (CUDA).


## Display

The applications in this category display the values of attributes in the console or 
on a csv file for each node of the component tree of the input image.

* **basic_attr_disp**
   ```shell
   $ ./basic_attr_disp <input_image_path> <csv_out>
   ```
   * `<input_image_path>`: path to the image where the attributes will be computed.
   * `<csv_out>`: path to the csv file where the values of the attribute will be recorded.

   The program builds the max-tree of the input image and incrementally computes: 
   area, volume, level, mean level, variance level, width, height, rectangularity, ratio 
   width and height, moment 02, moment 20, moment 11, inertia, orientation, length of 
   major axis, length of minor axis, eccentricity, and compactness. The values of the attributes
   are stored in a csv.
   
* **max_dist_attr_disp**
   ```shell
   $ ./max_dist_attr_disp <input_image_path> <out_dir>
   ```

   * `<input_image_path>`: path to the image where maximum EDT value will be computed.
   * `<out_dir>`: path to a directory to store the binary image of the nodes.

   The program computes the max-tree of the input image and displays the value of 
   the attribute in the console. If the symbol `ONLY_ATTRIBUTE_VALUE` is not define
   the program also stores the binary image of each node of the max-tree in the 
   output directory.

## Filters

# Runtime

A Google Colab page containing the analysis of the run-time files is available in this [link](https://colab.research.google.com/drive/18Kyx5M83iL1TQoti4BIih4t4DJyD72Gg?usp=drive_link).


