import subprocess
import re 

import time 
import pandas as pd 

from tqdm import tqdm

from argparse import ArgumentParser, ArgumentDefaultsHelpFormatter

def main(PROGRAM="../build/diff_max_dist_runtime",
    IMAGE_BASE_PATH="../runtime-database/carlinet-14-itip",
    OUTPUT_FILE_NAME= "csv/non_diff_max_dist_runtime"):
  
  # ============================================================================
  # Program call
  # ============================================================================
  class ProgramCall:
    def __init__(self, program, image):
      self.image_ = image 
      self.program_ = program

    @property
    def image(self):
      return self.image_ 
    
    @property
    def program(self):
      return self.program_
    
    def _run_program(self):
      full_program_call = f"{self.program} {IMAGE_BASE_PATH}/{self.image_}"

      p = subprocess.Popen(full_program_call, stdout=subprocess.PIPE, shell=True)

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
        "nnodes": int(nnodes_str),
        "width": int(width_str),
        "height": int(height_str),
        "npixels": int(npixels_str),
        "runtime": int(runtime_str) }
    
    def run_program(self):
      return self._run_program()

  # =================================================================
  # extract program calls 
  # =================================================================
  def extract_programs_calls(program):
    import os
    from os.path import isfile, join
    import numpy as np

    lfiles = [f for f in os.listdir(IMAGE_BASE_PATH) 
              if isfile(join(IMAGE_BASE_PATH, f))]
    
    return [ProgramCall(program, f) for f in lfiles]
  

  # ==================================================================
  # main 
  # ==================================================================
  RUNTIME_PROGRAM_CALLS = extract_programs_calls(PROGRAM)

  images = []
  nnodes = []
  width = []
  height = []
  npixels = []
  runtime = []

  for pcall in tqdm(RUNTIME_PROGRAM_CALLS):
    run_data = pcall.run_program()
    images.append(pcall.image)
    nnodes.append(run_data["nnodes"])
    width.append(run_data["width"])
    height.append(run_data["height"])
    npixels.append(run_data["npixels"])
    runtime.append(run_data["runtime"])

    time.sleep(2)


  df = pd.DataFrame(data={
    "image": images,
    "nnodes": nnodes,
    "width": width,
    "height": height,
    "npixels": npixels,
    "runtime": runtime})
  
  df.to_csv(OUTPUT_FILE_NAME, sep=";")
  print(f"runtime dataset has been written into '{OUTPUT_FILE_NAME}'")


parser = ArgumentParser(description="Generate runtime csv",
                        formatter_class=ArgumentDefaultsHelpFormatter)

parser.add_argument("-o", "--output-filename", help="output filename",
                    default="csv/runtime_0.csv")

parser.add_argument("-i", "--image-base-path", 
                    help="Base path where the input image are stored",
                    default="../runtime-dataset/carlinet-14-itip")

parser.add_argument("-p", "--program",
                    help="Program path which outputs runtime",
                    default="../build/diff_max_dist_opt2_runtime")

args = parser.parse_args()
print(args)

main(OUTPUT_FILE_NAME=args.output_filename,
      IMAGE_BASE_PATH=args.image_base_path,
      PROGRAM=args.program)

