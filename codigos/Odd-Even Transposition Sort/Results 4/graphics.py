import sys
from subprocess import Popen, PIPE
import io
import json
import numpy as np
import matplotlib.pyplot as plt
import itertools

if len(sys.argv) != 3:
  print("É necessario informar o caminho para os argumentos usados e os resultados encontrados")
  sys.exit(1)

with open(sys.argv[1], "r") as f:
  args = json.load(f)

with open(sys.argv[2], "r") as f:
  results = json.load(f)

problem_proc = results["v1"]
proc_problem = results["v2"]

problems = {}
for args_tuple in itertools.product(*args["args"].values()):
  problems[str(args_tuple)] = {}
  problems[str(args_tuple)]["speedup"] = [d["speedup"] for d in list(problem_proc[str(args_tuple)].values())]
  problems[str(args_tuple)]["cores"] = [float(value) for value in list(problem_proc[str(args_tuple)].keys())]
  plt.plot(problems[str(args_tuple)]["cores"], problems[str(args_tuple)]["speedup"], label=str(args_tuple[1]))

max_cores = int(args["procs_threads"][-1])

plt.xlim(0, max_cores+1)
plt.ylim(0, max_cores+1)
plt.plot(range(max_cores+2), range(max_cores+2), label="Identidade", color='red', linestyle='--')
plt.legend(loc='best')
plt.xlabel(xlabel='Cores')
plt.ylabel(ylabel='Speedup')
plt.savefig("speedups.png")
print("Gráfico dos Speedups salvo: speedups.png")


plt.close()
problems = {}
for cores in args["procs_threads"]:
  problems[cores] = {}
  problems[cores]["eficiencia"] = [d["eficiencia"] for d in list(proc_problem[cores].values())]
  problems[cores]["tamanho_problema"] = [int(value.replace('(', '').replace(')', '').split(',')[1].replace(" ", "").replace("'", "")) for value in list(proc_problem[cores].keys())]
  plt.plot(problems[cores]["tamanho_problema"], problems[cores]["eficiencia"], label=cores)
  
values = [int(v) for v in args["args"]["counter"]]
plt.xlim(min(values),max(values) )
plt.ylim(0.5, 1.5)
plt.legend(loc='best')
plt.xlabel(xlabel='Tamanho do Problema')
plt.ylabel(ylabel='Eficiência')
plt.savefig("eficiencias_problem_size.png")
print("Gráfico da Eficiência por tamanho de problema salvo: eficiencias_problem_size.png")

plt.close()
problems = {}
for args_tuple in itertools.product(*args["args"].values()):
  problems[str(args_tuple)] = {}
  problems[str(args_tuple)]["eficiencia"] = [d["eficiencia"] for d in list(problem_proc[str(args_tuple)].values())]
  problems[str(args_tuple)]["cores"] = [float(value) for value in list(problem_proc[str(args_tuple)].keys())]
  plt.plot(problems[str(args_tuple)]["cores"], problems[str(args_tuple)]["eficiencia"], label=str(args_tuple[1]))

plt.legend(loc='best')
plt.xlabel(xlabel='Cores')
plt.ylabel(ylabel='Eficiência')
plt.savefig("eficiencias_cores.png")
print("Gráfico da Eficiência por core salvo: eficiencias_cores.png")