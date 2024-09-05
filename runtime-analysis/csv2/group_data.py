import pandas as pd
from argparse import ArgumentParser, ArgumentDefaultsHelpFormatter

import os
from os.path import isfile, join
import numpy as np

def main(RESOLUTION="1920x1080"):

  def extract_mean_runtime(path):        
    lfiles = [f for f in os.listdir(path) if isfile(join(path, f))]
    dfs = []
    
    for f in lfiles:
      dfs.append(pd.read_csv(join(path, f), sep=";"))

    # compute mean runtime
    runtime_col = sum(df["runtime"] for df in dfs) / len(dfs)
    
    return runtime_col 

  

  non_diff_runtime = extract_mean_runtime(join("non_diff", f"{RESOLUTION}"))
  diff_max_runtime = extract_mean_runtime(join("max_diff", f"{RESOLUTION}"))
  exact_edt_runtime = extract_mean_runtime(join("exact_edt", f"{RESOLUTION}"))
  diff_max_dist_opt2_runtime = extract_mean_runtime(join("diff_max_dist_opt2", f"{RESOLUTION}"))

  df = pd.read_csv(join("non_diff", f"{RESOLUTION}", "runtime_1.csv"), sep=";")
  df = df.drop(columns=["runtime"])

  df = df.assign(
    **{ "runtime_non_diff": non_diff_runtime ,
      "runtime_exact_edt": exact_edt_runtime ,
      "runtime_max_diff": diff_max_runtime ,      
      "runtime_max_dist_opt2": diff_max_dist_opt2_runtime,
      "speedup_non_diff": non_diff_runtime / diff_max_dist_opt2_runtime, 
      "speedup_exact": exact_edt_runtime / diff_max_dist_opt2_runtime,
      "speedup_max_diff": diff_max_runtime / diff_max_dist_opt2_runtime,
      })

  df.to_csv(f"mean_runtime_{RESOLUTION}.csv", sep=";")



parser = ArgumentParser(description="Generate mean runtime data",
                        formatter_class=ArgumentDefaultsHelpFormatter)

parser.add_argument("-r", "--resolution", 
                    choices=["1920x1080", "960x540", "480x270", "240x135", "120x68"],
                    default="1920x1080")

args = parser.parse_args()


main(RESOLUTION=args.resolution)