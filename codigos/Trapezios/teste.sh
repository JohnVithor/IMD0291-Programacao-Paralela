#!/bin/sh

python3 comparator.py args_teste.json 7 2 1

mv serial_results.json serial_results1.json
mv parallel_results.json parallel_results1.json

python3 metrics.py args_teste.json serial_results1.json parallel_results1.json

mv metrics_a.json metrics_1a.json
mv metrics_b.json metrics_1b.json

python3 graphics.py args_teste.json metrics_1a.json

mv eficiencias_problem_size.png eficiencias_problem_size_1a.png
mv speedups.png speedups_1a.png

python3 graphics.py args_teste.json metrics_1b.json

mv eficiencias_problem_size.png eficiencias_problem_size_1b.png
mv speedups.png speedups_1b.png
