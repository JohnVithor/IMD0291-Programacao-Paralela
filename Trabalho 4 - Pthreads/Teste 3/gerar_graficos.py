import sys
import pandas as pd
import matplotlib.pyplot as plt

if len(sys.argv) < 2:
  print("É necessario informar o caminho para o .xlsx com os tempos, speedups e eficiências")
  print("Em seguida o numero da coluna onde os tempos do algoritmo paralelo iniciam")
  print("E por fim informar os cores usados")
  print("Exemplo: python3 teste.xlsx")
  sys.exit(1)

name = sys.argv[1].split('.')[0]
times_start = 2
cores = [4, 8, 16, 32]

data = pd.read_excel(sys.argv[1])
data.index = data.loc[:, "Tamanho do Problema"]
data = data.astype(float)
speedup = data.iloc[:,times_start+len(cores):times_start+len(cores)*2]
eficiencia = data.iloc[:,times_start+len(cores)*2:]

speedup.columns = cores
speedup = speedup.transpose()

max_cores = max(cores) + 5
ax = speedup.plot()
plt.xlim(0, max_cores)
plt.ylim(0, max_cores)
plt.plot(range(max_cores), range(max_cores), label="Identidade", color='red', linestyle='--')
plt.legend(loc='best')
plt.xlabel(xlabel='Cores')
plt.ylabel(ylabel='Speedup')
plt.savefig(name+"_speedup_core.png")

eficiencia.columns = cores
eficiencia = eficiencia.transpose()

ax = eficiencia.plot()
plt.legend(loc='best')
plt.xlabel(xlabel='Cores')
plt.ylabel(ylabel='Eficiência')
plt.savefig(name+"_eficiencia_core.png")

eficiencia = eficiencia.transpose()
eficiencia.plot()
plt.legend(loc='best')
plt.xlabel(xlabel='Tamanho do Problema')
plt.ylabel(ylabel='Eficiência')
plt.savefig(name+"_eficiencia_problema.png")



