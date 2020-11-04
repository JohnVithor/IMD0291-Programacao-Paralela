import sys
import pandas as pd
import matplotlib.pyplot as plt

if len(sys.argv) != 2:
  print("É necessario informar o caminho para o .xlsx com os tempos, speedups e eficiências")
  sys.exit(1)

data = pd.read_excel(sys.argv[1])

data.index = data.loc[:, "Tamanho do Problema"]
data = data.astype(float)
speedup = data.iloc[:,8:14]
eficiencia = data.iloc[:,14:]

speedup.columns = [2, 3, 4, 6, 8, 12]
speedup = speedup.transpose()
speedup

max_cores = 12
ax = speedup.plot()
plt.xlim(0, 100)
plt.ylim(0, 100)
plt.plot(range(100), range(100), label="Identidade", color='red', linestyle='--')
plt.legend(loc='best')
plt.xlabel(xlabel='Cores')
plt.ylabel(ylabel='Speedup')
plt.savefig("speedup_core.png")

eficiencia.columns = [2, 3, 4, 6, 8, 12]
eficiencia = eficiencia.transpose()
eficiencia

ax = eficiencia.plot()
plt.legend(loc='best')
plt.xlabel(xlabel='Cores')
plt.ylabel(ylabel='Eficiência')
plt.savefig("eficiencia_core.png")

eficiencia = eficiencia.transpose()
eficiencia.plot()
plt.legend(loc='best')
plt.xlabel(xlabel='Tamanho do Problema')
plt.ylabel(ylabel='Eficiência')
plt.savefig("eficiencia_problema.png")



