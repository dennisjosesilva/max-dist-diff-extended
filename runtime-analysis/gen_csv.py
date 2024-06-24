# Dataset https://www.lrde.epita.fr/wiki/Publications/carlinet.14.itip

import subprocess
import re 

import time 
import pandas as pd

from argparse import ArgumentParser, ArgumentDefaultsHelpFormatter

def main(OUTPUT_FILE_NAME = "csv/runtime_0.csv",
        IMAGE_BASE_PATH= "../runtime-dataset/carlinet-14-itip",
        EXECUTION_ORDER = "nd"):
  
  # =============================================================
  # Program call 
  # =============================================================
  class ProgramCalls:
    def __init__(self, image):
      self.image_ = image

    @property
    def image(self):
      return self.image_

    @property
    def perf_diff_computation(self):
      return f"../build/diff_max_dist_runtime"
    
    @property
    def perf_nondiff_computation(self):
      return f"../build/non_diff_max_dist_runtime"
    
    def _run_program(self, program):
      full_Program_call = f"{program} {IMAGE_BASE_PATH}/{self.image_}"
      print(full_Program_call)

      p = subprocess.Popen(full_Program_call, stdout=subprocess.PIPE, shell=True)

      (output, err) = p.communicate()

      p_status = p.wait()
      p_output = output.decode()

      my_regex = re.compile(
        r"nnodes:\s+(?P<nnodes>\d+)\n"
        r"width:\s+(?P<width>\d+)\n"
        r"height:\s+(?P<height>\d+)\n"        
        r"npixels:\s+(?P<npixels>\d+)\n"
        r"time elapsed:\s+(?P<runtime>\d+)\n")
      
      mo = my_regex.search(p_output)

      nnodes_str = mo.group("nnodes")
      width_str = mo.group("width")
      height_str = mo.group("height")
      npixels_str = mo.group("npixels")
      runtime_str = mo.group("runtime")
      
      return {
        "nnodes":  int(nnodes_str),
        "width" :  int(width_str),
        "height":  int(height_str),
        "npixels": int(npixels_str),
        "runtime": int(runtime_str) }

    def run_diff_computation(self):
      return self._run_program(self.perf_diff_computation)
    
    def run_nondiff_computation(self):
      return self._run_program(self.perf_nondiff_computation)

  RUNTIME_PROGRAM_CALLS = [
    ProgramCalls("barb.pgm"),
    ProgramCalls("bird.pgm"),
    ProgramCalls("boat.pgm"),
    ProgramCalls("bridge.pgm"),
    ProgramCalls("camera.pgm"),
    ProgramCalls("frog.pgm"),
    ProgramCalls("goldhill.pgm"),
    ProgramCalls("house.pgm"),
    ProgramCalls("lena.pgm"),
    ProgramCalls("mandrill.pgm"),
    ProgramCalls("mountain.pgm"),
    ProgramCalls("naturel.pgm"),
    ProgramCalls("peppers.pgm"),
    ProgramCalls("piscine.pgm"),
    ProgramCalls("sat.pgm"),
    ProgramCalls("washingtonDC.pgm"),
    ProgramCalls("washsat.pgm"),
    ProgramCalls("zelda.pgm")]
  
  images = []
  nnodes = []
  widths = []
  heights = []
  npixels = []
  runtime_diff = []
  runtime_nondiff = []

  for pcall in RUNTIME_PROGRAM_CALLS:

    if EXECUTION_ORDER == "nd":
      time.sleep(5)
      run_data = pcall.run_nondiff_computation()
      images.append(pcall.image)
      nnodes.append(run_data["nnodes"])
      widths.append(run_data["width"])
      heights.append(run_data["height"])
      npixels.append(run_data["npixels"])
      runtime_nondiff.append(run_data["runtime"])

      time.sleep(5)
      run_data = pcall.run_diff_computation()
      runtime_diff.append(run_data["runtime"])

    elif EXECUTION_ORDER == "dn":
      time.sleep(5)
      run_data = pcall.run_diff_computation()
      images.append(pcall.image)
      nnodes.append(run_data["nnodes"])
      widths.append(run_data["width"])
      heights.append(run_data["height"])
      npixels.append(run_data["npixels"])
      runtime_diff.append(run_data["runtime"])

      time.sleep(5)
      run_data = pcall.run_nondiff_computation()
      runtime_nondiff.append(run_data["runtime"])

  df = pd.DataFrame(data={
    "image" : images,
    "nnodes": nnodes,
    "width": widths,
    "height": heights,
    "npixels": npixels,
    "runtime_nondiff": runtime_nondiff,
    "runtime_diff": runtime_diff
  })

  df.to_csv(OUTPUT_FILE_NAME, sep=";")
  print(f"runtime dataset has been written into '{OUTPUT_FILE_NAME}'")
    

parser = ArgumentParser(description="Generate runtime csv",
                        formatter_class=ArgumentDefaultsHelpFormatter)

parser.add_argument("-o", "--output-filename",
                    help="output filename",
                    default="csv/runtime_0.csv")

parser.add_argument("-i", "--image-base-path",
                    help="Base path where the input image are stored.",
                    default="../runtime-dataset/carlinet-14-itip")

parser.add_argument("-e", "--execution-order",
                    help="Order which the program run the different algorithms",
                    default="nd")

args = parser.parse_args()
print(args)

main(OUTPUT_FILE_NAME=args.output_filename,
     IMAGE_BASE_PATH=args.image_base_path,
     EXECUTION_ORDER=args.execution_order)