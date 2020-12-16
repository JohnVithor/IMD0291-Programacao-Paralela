# IMD0291 - Topicos especiais em internet das coisas B

Repositório utilizado para manter os códigos, relatórios e arquivos gerados nas atividades da disciplina IMD0291.
No semestre de 2020.6 essa disciplina de tópicos teve como tema a programação paralela e foi ministrada pelo Professor Kayo Gonçalves e Silva.

Foram abordados diversos conceitos na disciplina alguns estão listados abaixo:
- Contexto historico no qual a programação paralela surgiu assim como as motivações desse surgimento.
- Paralelismo de tarefas e paralelismo de dados.
- Coordenação na paralelização: Comunicação, Balanceamento de carga e Sincronização.
- Memória Compartilhada x Memória Distribuida
- Multitasking e Threading
- Speedup e Eficiência
- Métodologia de Foster

Após esses conceitos (e alguns outros não listados), foram abordados as seguintes formas de implementar paralelismo nos programas, assim como as particularidades delas:
 - MPI: a comunicação entre processos e a natureza distribuida da programação, utilizando mensagens para transportar dados e barreiras para manter a sincronia dos processos;
 - Pthreads: a utilização de threads e a natureza compartilhada da programação, utilizando variáveis globais para realizar a comunicação, surgimento de regiões críticas e deadlocks;
 - OpenMP: a praticidade dessa biblioteca com suas diretrizes pragma, escalonamento do for, ideia de tasks e outras diretrizes;
 
 Diversas outras particularidades de cada uma dessas 3 formas de implementar programas paralelos foram apresentadas e explanadas, apenas algumas foram citadas acima.
