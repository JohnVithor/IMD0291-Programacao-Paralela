""" 1a.py """
import sys
from subprocess import Popen, PIPE
import io
import json

if len(sys.argv) != 2:
  print("É necessario informar o caminho para os argumentos dos programas")
  sys.exit(1)

programa_serial = "./bin/pi_serial.bin"
programa_paralelo = "./bin/pi_parallel.bin"

with open(sys.argv[1], "r") as f:
  args = json.load(f)

seed = args["seed"]
serial_results = []
parallel_results = {procs_threads: [] for procs_threads in args["procs_threads"]}
for counter in args["casos"]:
  serial_proc = Popen([programa_serial, seed, counter], stdin=PIPE, stdout=PIPE, stderr=PIPE)
  output, err = serial_proc.communicate()
  result = {"result": json.loads(output.decode("utf-8")), "counter":counter}
  serial_results.append(result)
  for procs_threads in args["procs_threads"]:
    parallel_proc = Popen(["mpiexec", "-n", procs_threads, programa_paralelo, seed, counter], stdin=PIPE, stdout=PIPE, stderr=PIPE)
    output, err = serial_proc.communicate()
    result = {"result": json.loads(output.decode("utf-8")), "counter":counter, "proc_threads": procs_threads}
    parallel_results[procs_threads].append(result)
    
print(serial_results)
print(parallel_results)

# TODO: calcular as metricas