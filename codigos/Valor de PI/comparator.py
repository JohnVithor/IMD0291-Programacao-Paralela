""" 1a.py """
import sys
from subprocess import Popen, PIPE
import numpy as np
import io

if len(sys.argv) != 2:
  print("É necessario informar os parametros para os programas: ")
  sys.exit(1)

programa_serial = "./bin/pi_serial.bin"
programa_paralelo = "./bin/pi_parallel.bin"

f = io.BytesIO()
p = Popen(['ls', '-alt'], stdin=PIPE, stdout=PIPE, stderr=PIPE)
output, err = p.communicate(b"input data that is passed to subprocess' stdin")
rc = p.returncode

print(output.decode("utf-8"), sep='\n')

