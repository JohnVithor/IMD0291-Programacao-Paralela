#include <stdio.h>
#include <mpi.h>

int main(void) {
    int my_rank, comm_sz;

    MPI_Init(NULL, NULL);
    MPI_Comm_size( MPI_COMM_WORLD , &comm_sz);
    MPI_Comm_rank( MPI_COMM_WORLD , &my_rank);

    printf("Proc %d of %d > Hello World!\n", my_rank+1, com_sz);
    MPI_Finalize();
    return 0;
} /* main */