import sys
from subprocess import Popen, PIPE
import io
import json
import itertools
import pandas as pd
import matplotlib.pyplot as plt

if len(sys.argv) != 4:
  print("É necessario informar o caminho para os argumentos usados e os resultados encontrados tanto serial quanto paralelo")
  sys.exit(1)

with open(sys.argv[1], "r") as f:
  args = json.load(f)

with open(sys.argv[2], "r") as f:
  serial_results = json.load(f)

with open(sys.argv[3], "r") as f:
  parallel_results = json.load(f)

def get_metrics(args_tuple, procs_threads):
  speedup = serial_results[str(args_tuple)]["mean_time"] / parallel_results[str(args_tuple)][procs_threads]["mean_time"]
  return {"speedup": speedup, "eficiencia" : speedup / int(procs_threads)}

def calculate_new_mean(times):
  ts = [t["time"] for t in times]
  ts.remove(min(ts))
  ts.remove(max(ts))
  total = sum(ts) - (min(ts) + max(ts))
  
  return total / (len(ts) - 2)

def get_metrics_2(args_tuple, procs_threads):
  speedup = calculate_new_mean(serial_results[str(args_tuple)]["times"]) / calculate_new_mean(parallel_results[str(args_tuple)][procs_threads]["times"])
  return {"speedup": speedup, "eficiencia" : speedup / int(procs_threads)}


def get_data_table(args_tuple, procs_threads):
  speedup = serial_results[str(args_tuple)]["mean_time"] / parallel_results[str(args_tuple)][procs_threads]["mean_time"]
  return {"speedup": speedup, "eficiencia" : speedup / int(procs_threads)}

table = {}
for args_tuple in itertools.product(*args["args"].values()):
  table[str(args_tuple[1])] = {}
  #table[str(args_tuple[2])]["Serial"] = calculate_new_mean(serial_results[str(args_tuple)]["times"])
  table[str(args_tuple[1])]["Serial"] = serial_results[str(args_tuple)]["mean_time"]
  for procs_threads in args["procs_threads"]:
    #table[str(args_tuple[2])][str(procs_threads)]= calculate_new_mean(parallel_results[str(args_tuple)][procs_threads]["times"])
    table[str(args_tuple[1])][str(procs_threads)]= parallel_results[str(args_tuple)][procs_threads]["mean_time"]

tempos = pd.DataFrame(table)
tempos.to_csv("tempos.csv")

problem_proc = {}
for args_tuple in itertools.product(*args["args"].values()):
  problem_proc[str(args_tuple)] = {}
  for procs_threads in args["procs_threads"]:
    problem_proc[str(args_tuple)][procs_threads] = get_metrics(args_tuple, procs_threads)

proc_problem = {}
for procs_threads in args["procs_threads"]:
  proc_problem[procs_threads] = {}
  for args_tuple in itertools.product(*args["args"].values()):
    proc_problem[procs_threads][str(args_tuple)] = get_metrics(args_tuple, procs_threads)

with open("metrics_a.json", "w") as f:
  f.write(json.dumps({"v1": problem_proc, "v2": proc_problem}))


# problem_proc = {}
# for args_tuple in itertools.product(*args["args"].values()):
#   problem_proc[str(args_tuple)] = {}
#   for procs_threads in args["procs_threads"]:
#     problem_proc[str(args_tuple)][procs_threads] = get_metrics_2(args_tuple, procs_threads)

# proc_problem = {}
# for procs_threads in args["procs_threads"]:
#   proc_problem[procs_threads] = {}
#   for args_tuple in itertools.product(*args["args"].values()):
#     proc_problem[procs_threads][str(args_tuple)] = get_metrics_2(args_tuple, procs_threads)

# with open("metrics_b.json", "w") as f:
#   f.write(json.dumps({"v1": problem_proc, "v2": proc_problem}))
