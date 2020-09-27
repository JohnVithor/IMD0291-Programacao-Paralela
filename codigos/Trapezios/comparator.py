""" 1a.py """
import sys
from subprocess import Popen, PIPE
import io
import json

import matplotlib.pyplot as plt

if len(sys.argv) != 4:
  print("É necessario informar o caminho para os argumentos dos programas e o numero de execuções por configuração, por fim indique o nível de detalhe do log")
  print("0: Sem informação alguma\n1: Apenas a média de cada configuração usada é mostrada\n2: Todos os tempos obtidos são mostrados")
  sys.exit(1)

programa_serial = "./bin/trap_serial.bin"
programa_paralelo = "./bin/trap_parallel.bin"

with open(sys.argv[1], "r") as f:
  args = json.load(f)

repeats = int(sys.argv[2])
log_level = int(sys.argv[3])

if(log_level < 0 or log_level > 3):
  print("Nível de detalhe do log informado é inválido, use um dos seguintes")
  print("0: Sem informação alguma\n1: Apenas a média de cada configuração usada é mostrada\n2: Todos os tempos obtidos são mostrados")
  sys.exit(1)

seed = args["seed"]
serial_results = {}
parallel_results = {}
for counter in args["casos"]:
  serial_results[counter] = {"mean_time":0.0, "times":[]}
  mean_time = 0.0
  if(log_level > 0):
    print("Serial:\t\t", counter)
  for i in range(repeats):
    serial_proc = Popen([programa_serial, seed, counter], stdin=PIPE, stdout=PIPE, stderr=PIPE)
    output, err = serial_proc.communicate()
    serial_results[counter]["times"].append(json.loads(output.decode("utf-8")))
    if(log_level > 1):
      print(i, "\t","%.10f" % serial_results[counter]["times"][-1]["time"], "seconds")
    mean_time += serial_results[counter]["times"][-1]["time"]
  serial_results[counter]["mean_time"] = mean_time / repeats
  if(log_level > 0):
    print("mean:\t", "%.10f" % serial_results[counter]["mean_time"], "seconds")
  parallel_results[counter] = {}
  for procs_threads in args["procs_threads"]:
    parallel_results[counter][procs_threads] = {"mean_time":0.0, "times":[]}
    mean_time = 0.0
    if(log_level > 0):
      print("Parallel:\t", counter, procs_threads)
    for i in range(repeats):
      parallel_proc = Popen(["mpiexec", "-n", procs_threads, programa_paralelo, seed, counter], stdin=PIPE, stdout=PIPE, stderr=PIPE)
      output, err = parallel_proc.communicate()
      parallel_results[counter][procs_threads]["times"].append(json.loads(output.decode("utf-8")))
      if(log_level > 1):
        print(i, "\t", "%.10f" % parallel_results[counter][procs_threads]["times"][-1]["time"], "seconds")
      mean_time += parallel_results[counter][procs_threads]["times"][-1]["time"]
    parallel_results[counter][procs_threads]["mean_time"] = mean_time / repeats
    if(log_level > 0):
      print("mean:\t", "%.10f" % parallel_results[counter][procs_threads]["mean_time"], "seconds")

def get_metrics(counter, procs_threads):
  speedup = serial_results[counter]["mean_time"] / parallel_results[counter][procs_threads]["mean_time"]
  return {"speedup": speedup, "eficiencia" : speedup / int(procs_threads)}

problem_proc = {}
for counter in args["casos"]:
  problem_proc[counter] = {}
  for procs_threads in args["procs_threads"]:
    problem_proc[counter][procs_threads] = get_metrics(counter, procs_threads)

proc_problem = {}
for procs_threads in args["procs_threads"]:
  proc_problem[procs_threads] = {}
  for counter in args["casos"]:
    proc_problem[procs_threads][counter] = get_metrics(counter, procs_threads)


with open("serial_results.json", "w") as f:
  f.write(json.dumps(serial_results))
with open("parallel_results.json", "w") as f:
  f.write(json.dumps(parallel_results))
with open("metrics.json", "w") as f:
  f.write(json.dumps({"v1": problem_proc, "v2": proc_problem}))

print("Resultados salvos!")