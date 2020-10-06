#!/bin/sh

python3 comparator.py args_teste.json 12 2 0

mv serial_results.json serial_results1.json
mv parallel_results.json parallel_results1.json

python3 metrics.py args_teste.json serial_results1.json parallel_results1.json

mv metrics_a.json metrics_a1.json
mv metrics_b.json metrics_b1.json

python3 graphics.py args_teste.json metrics_a1.json

mv eficiencias_cores.png eficiencias_cores_a1.png
mv eficiencias_problem_size.png eficiencias_problem_size_a1.png
mv speedups.png speedups_a1.png

python3 graphics.py args_teste.json metrics_b1.json

mv eficiencias_cores.png eficiencias_cores_b1.png
mv eficiencias_problem_size.png eficiencias_problem_size_b1.png
mv speedups.png speedups_b1.png



python3 comparator.py args_teste2.json 12 2 1

mv serial_results.json serial_results2.json
mv parallel_results.json parallel_results2.json

python3 metrics.py args_teste.json serial_results2.json parallel_results2.json

mv metrics_a.json metrics_a2.json
mv metrics_b.json metrics_b2.json

python3 graphics.py args_teste.json metrics_a2.json

mv eficiencias_cores.png eficiencias_cores_a2.png
mv eficiencias_problem_size.png eficiencias_problem_size_a2.png
mv speedups.png speedups_a2.png

python3 graphics.py args_teste.json metrics_b2.json

mv eficiencias_cores.png eficiencias_cores_b2.png
mv eficiencias_problem_size.png eficiencias_problem_size_b2.png
mv speedups.png speedups_b2.png