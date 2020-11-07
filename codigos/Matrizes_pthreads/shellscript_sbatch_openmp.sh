#!/bin/bash

#SBATCH --partition=cluster
#SBATCH --job-name=MeuNome
#SBATCH --output=MeuNomeOutput.out
#SBATCH --error=MeuNomeError.err
#SBATCH --time=0-00:2
#SBATCH --hint=compute_bound 

#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=32

#No Supercomputador, 1 nó = 32 Cores (ou CPUs)
#Lembrar que: TASK = PROCESSO

#A configuração acima reserva 1 nó inteiro (32 cpus no mesmo processador), vai rodar apenas 1 processo por vez,
# mas este processo terá acesso a 32 cores

#Loop principal de execuções. São 10 tentativas

	seed_a=0 # seed usada para gerar valores da primeira matriz
	seed_b=42 # seed usada para gerar valores da segunda matriz
	tentativas=10 # Quantas vezes cada programa será executado
	bin_path="./bin/" # Caminho para os executáveis


	# Quais os tamanhos de problemas usados 
	declare -a problem_size_list=("100" "200") 
	# Quais quantidades de cores serão usados 
	declare -a cores_list=("2" "4") 
	# Quais os programas seriais
	declare -a programas_seriais=("serial" "serial_transpose" "serial_random_line" )
	serial_square="serial_square_transpose"
	# Quais os programas paralelos
	declare -a programas_paralelos=("parallel" "parallel_transpose" )
	parallel_square="parallel_square_transpose"

	# cabeçalho do csv gerado
	echo program, cores, problem_size, trial, execution_time

	for size in ${problem_size_list[@]}
	do   
		for tentativa in $(seq $tentativas) #Cria uma vetor de 1 a "tentativas"
		do
			for serial in ${programas_seriais[@]}
			do
				r=$($bin_path$serial.bin 0 $seed_a $seed_b $size $size $size $size)
				echo $serial, 1, $size, $tentativa, $r
			done
			r=$($bin_path$serial_square.bin 0 $seed_a $seed_b $size)
			echo $serial_square, 1, $size, $tentativa, $r

			for cores in ${cores_list[@]} #números de cores utilizados
			do
				for parallel in ${programas_paralelos[@]}
				do
					r=$($bin_path$parallel.bin $cores 0 $seed_a $seed_b $size $size $size $size)
					echo $parallel, $cores, $size, $tentativa, $r
				done
				r=$($bin_path$parallel_square.bin $cores 0 $seed_a $seed_b $size)
				echo $parallel_square, $cores, $size, $tentativa, $r
			done
		done
	done
	
	exit 

