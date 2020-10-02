""" 1a.py """
import sys
from subprocess import Popen, PIPE
import io
import json
import itertools

import matplotlib.pyplot as plt

if len(sys.argv) != 5:
  print("É necessario informar o caminho para os argumentos dos programas e o numero de execuções por configuração, indique em seguida o nível de detalhe do log e por fim se hyperthreading será habilitado")
  print("0: Sem informação alguma\n1: Apenas a média de cada configuração usada é mostrada\n2: Todos os tempos obtidos são mostrados")
  sys.exit(1)

programa_serial = "./bin/pi_serial.bin"
programa_paralelo = "./bin/pi_parallel.bin"

with open(sys.argv[1], "r") as f:
  args = json.load(f)

repeats = int(sys.argv[2])
log_level = int(sys.argv[3])
hyperthread = int(sys.argv[4])

if(log_level < 0 or log_level > 3):
  print("Nível de detalhe do log informado é inválido, use um dos seguintes")
  print("0: Sem informação alguma\n1: Apenas a média de cada configuração usada é mostrada\n2: Todos os tempos obtidos são mostrados")
  sys.exit(1)

for arg_type in args["args"].keys():
  print(arg_type)

serial_results = {}
parallel_results = {}
for args_tuple in itertools.product(*args["args"].values()):
  serial_results[str(args_tuple)] = {"mean_time":0.0, "times":[]}
  mean_time = 0.0
  if(log_level > 0):
    print("Serial:\t\t", 'args:', args_tuple)
  for i in range(repeats):
    serial_proc = Popen([programa_serial, *args_tuple], stdin=PIPE, stdout=PIPE, stderr=PIPE)
    output, err = serial_proc.communicate()
    serial_results[str(args_tuple)]["times"].append(json.loads(output.decode("utf-8")))
    if(log_level > 1):
      print(i, "\t","%.10f" % serial_results[str(args_tuple)]["times"][-1]["time"], "seconds")
    mean_time += serial_results[str(args_tuple)]["times"][-1]["time"]
  serial_results[str(args_tuple)]["mean_time"] = mean_time / repeats
  if(log_level > 0):
    print("mean:\t", "%.10f" % serial_results[str(args_tuple)]["mean_time"], "seconds")
  parallel_results[str(args_tuple)] = {}
  for procs_threads in args["procs_threads"]:
    parallel_results[str(args_tuple)][procs_threads] = {"mean_time":0.0, "times":[]}
    mean_time = 0.0
    if(log_level > 0):
      print("Parallel:\t", 'args:', args_tuple, "\tprocs", procs_threads)
    for i in range(repeats):
      if(hyperthread == 1):
        parallel_proc = Popen(["mpiexec", "-use-hwthread-cpus", "-n", procs_threads, programa_paralelo, *args_tuple], stdin=PIPE, stdout=PIPE, stderr=PIPE)
      else:  
        parallel_proc = Popen(["mpiexec", "-n", procs_threads, programa_paralelo, *args_tuple], stdin=PIPE, stdout=PIPE, stderr=PIPE)
      output, err = parallel_proc.communicate()
      parallel_results[str(args_tuple)][procs_threads]["times"].append(json.loads(output.decode("utf-8")))
      if(log_level > 1):
        print(i, "\t", "%.10f" % parallel_results[str(args_tuple)][procs_threads]["times"][-1]["time"], "seconds")
      mean_time += parallel_results[str(args_tuple)][procs_threads]["times"][-1]["time"]
    parallel_results[str(args_tuple)][procs_threads]["mean_time"] = mean_time / repeats
    if(log_level > 0):
      print("mean:\t", "%.10f" % parallel_results[str(args_tuple)][procs_threads]["mean_time"], "seconds")

def get_metrics(args_tuple, procs_threads):
  speedup = serial_results[str(args_tuple)]["mean_time"] / parallel_results[str(args_tuple)][procs_threads]["mean_time"]
  return {"speedup": speedup, "eficiencia" : speedup / int(procs_threads)}

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


with open("serial_results.json", "w") as f:
  f.write(json.dumps(serial_results))
with open("parallel_results.json", "w") as f:
  f.write(json.dumps(parallel_results))
with open("metrics.json", "w") as f:
  f.write(json.dumps({"v1": problem_proc, "v2": proc_problem}))

print("Resultados salvos!")
