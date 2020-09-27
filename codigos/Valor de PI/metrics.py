import sys
from subprocess import Popen, PIPE
import io
import json
import numpy as np
import matplotlib.pyplot as plt

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
for i, counter in enumerate(args["casos"]):
  problems[counter] = {}
  problems[counter]["speedup"] = [d["speedup"] for d in list(problem_proc[counter].values())]
  problems[counter]["cores"] = [float(value) for value in list(problem_proc[counter].keys())]
  plt.plot(problems[counter]["cores"], problems[counter]["speedup"], label=counter)


plt.legend(loc='best')
plt.xlabel(xlabel='cores')
plt.ylabel(ylabel='speedup')
plt.savefig("speedups.png")
print("Gráfico dos Speedups salvo: speedups.png")


plt.close()
problems = {}
for i, counter in enumerate(args["casos"]):
  problems[counter] = {}
  problems[counter]["eficiencia"] = [d["eficiencia"] for d in list(problem_proc[counter].values())]
  problems[counter]["cores"] = [float(value) for value in list(problem_proc[counter].keys())]
  plt.plot(problems[counter]["cores"], problems[counter]["eficiencia"], label=counter)
  

plt.legend(loc='best')
plt.xlabel(xlabel='cores')
plt.ylabel(ylabel='eficiencia')
plt.savefig("eficiencias_cores.png")
print("Gráfico da Eficiência por core salvo: eficiencias_cores.png")


plt.close()
problems = {}
for i, cores in enumerate(args["procs_threads"]):
  problems[cores] = {}
  problems[cores]["eficiencia"] = [d["eficiencia"] for d in list(proc_problem[cores].values())]
  problems[cores]["tamanho_problema"] = [int(value) for value in list(proc_problem[cores].keys())]
  plt.plot(problems[cores]["tamanho_problema"], problems[cores]["eficiencia"], label=cores)
  

plt.legend(loc='best')
plt.xlabel(xlabel='Tamanho do Problema')
plt.ylabel(ylabel='eficiencia')
plt.savefig("eficiencias_problem_size.png")
print("Gráfico da Eficiência por tamanho de problema salvo: eficiencias_problem_size.png")
