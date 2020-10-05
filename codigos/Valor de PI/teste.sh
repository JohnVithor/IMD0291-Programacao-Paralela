#!/bin/sh

python3 comparator.py args_teste.json 10 2 0

mv metrics.json metrics1.json
mv parallel_results.json parallel_results1.json
mv serial_results.json serial_results1.json

python3 metrics.py args_teste.json metrics1.json

mv eficiencias_cores.png eficiencias_cores1.png
mv eficiencias_problem_size.png eficiencias_problem_size1.png
mv speedups.png speedups1.png

python3 comparator.py args_teste2.json 10 2 1

mv metrics.json metrics2.json
mv parallel_results.json parallel_results2.json
mv serial_results.json serial_results2.json

python3 metrics.py args_teste2.json metrics2.json

mv eficiencias_cores.png eficiencias_cores2.png
mv eficiencias_problem_size.png eficiencias_problem_size2.png
mv speedups.png speedups2.png

