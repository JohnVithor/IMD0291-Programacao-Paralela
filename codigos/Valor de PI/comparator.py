""" 1a.py """
import sys
from subprocess import Popen, PIPE
import io
import json

import matplotlib.pyplot as plt

if len(sys.argv) != 2:
  print("É necessario informar o caminho para os argumentos dos programas")
  sys.exit(1)

programa_serial = "./bin/pi_serial.bin"
programa_paralelo = "./bin/pi_parallel.bin"

with open(sys.argv[1], "r") as f:
  args = json.load(f)

seed = args["seed"]
serial_results = {}
parallel_results = {}
for counter in args["casos"]:
  serial_proc = Popen([programa_serial, seed, counter], stdin=PIPE, stdout=PIPE, stderr=PIPE)
  output, err = serial_proc.communicate()
  serial_results[counter] = json.loads(output.decode("utf-8"))
  parallel_results[counter] = {}
  for procs_threads in args["procs_threads"]:
    parallel_proc = Popen(["mpiexec", "--use-hwthread-cpus", "-n", procs_threads, programa_paralelo, seed, counter], stdin=PIPE, stdout=PIPE, stderr=PIPE)
    output, err = parallel_proc.communicate()
    parallel_results[counter][procs_threads] = json.loads(output.decode("utf-8"))

def get_metrics(counter, procs_threads):
  serial_results[counter]["time"]
  parallel_results[counter][procs_threads]["time"]
  speedup = serial_results[counter]["time"] / parallel_results[counter][procs_threads]["time"]
  return {"speedup": speedup, "eficiencia" : speedup / int(procs_threads)}

alvos = {}
for counter in args["casos"]:
  alvos[counter] = {}
  for procs_threads in args["procs_threads"]:
    alvos[counter][procs_threads] = get_metrics(counter, procs_threads)


with open("serial_results.json", "w") as f:
  f.write(json.dumps(serial_results))
with open("parallel_results.json", "w") as f:
  f.write(json.dumps(parallel_results))
with open("metrics.json", "w") as f:
  f.write(json.dumps(alvos))

problems = {}
for i, counter in enumerate(args["casos"]):
  problems[counter] = {}
  problems[counter]["speedup"] = [d["speedup"] for d in list(alvos[counter].values())]
  problems[counter]["cores"] = [float(value) for value in list(alvos[counter].keys())]
  plt.plot(problems[counter]["cores"], problems[counter]["speedup"], label=counter)

plt.legend(loc='upper left')
plt.xlabel(xlabel='cores')
plt.ylabel(ylabel='speedup')
plt.savefig("speedups.png")

plt.close()
problems = {}
for i, counter in enumerate(args["casos"]):
  problems[counter] = {}
  problems[counter]["eficiencia"] = [d["eficiencia"] for d in list(alvos[counter].values())]
  problems[counter]["cores"] = [float(value) for value in list(alvos[counter].keys())]
  plt.plot(problems[counter]["cores"], problems[counter]["eficiencia"], label=counter)

plt.legend(loc='upper left')
plt.xlabel(xlabel='cores')
plt.ylabel(ylabel='eficiencia')
plt.savefig("eficiencias.png")