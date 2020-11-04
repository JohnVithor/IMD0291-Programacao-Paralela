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

programa_serial = "./bin/odd_even_serial.bin"
programa_paralelo1 = "./bin/odd_even_parallel.bin"
programa_paralelo2 = "./bin/odd_even_parallel_otimized.bin"

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

def run_parallel(parallel_results, programa_paralelo):
  for procs_threads in args["procs_threads"]:
    parallel_results[str(args_tuple)][procs_threads] = {"mean_time":0.0, "times":[]}
    mean_time = 0.0
    if(log_level > 0):
      print("Parallel:\t", 'args:', args_tuple, "\tprocs", procs_threads)
    for i in range(repeats):
      if(hyperthread == 1 and int(procs_threads) > 6):
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


serial_results = {}
parallel_results1 = {}
parallel_results2 = {}
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

  parallel_results1[str(args_tuple)] = {}
  parallel_results2[str(args_tuple)] = {}

  run_parallel(parallel_results1, programa_paralelo1)
  run_parallel(parallel_results2, programa_paralelo2)

with open("serial_results.json", "w") as f:
  f.write(json.dumps(serial_results))

with open("parallel_results.json", "w") as f:
  f.write(json.dumps(parallel_results1))

with open("parallel_results_otimized.json", "w") as f:
  f.write(json.dumps(parallel_results2))

print("Resultados salvos!")


