#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]){
    int size, rank, nproc, ID, predecessor, successor;
    MPI_Status st;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   

        ID = rank;
        if(rank == 0){
            predecessor = size - 1;
        } else{
            predecessor = rank - 1;
        }

        if(rank == size - 1){
            successor = 0; 
        } else {
            successor = rank + 1;
        }


        int current_leader,
            is_leader,
            leader_elected = 0,
            message,
            st_value;

        if(rank == 0){
            message = ID;
            MPI_Send(&message, 1, MPI_INT, successor, 0, MPI_COMM_WORLD);
        }
        while(!leader_elected){
            MPI_Recv(&message, 1, MPI_INT, predecessor, MPI_ANY_TAG, MPI_COMM_WORLD, &st);

            printf("Sou o processo %d, recebi o número %d do processo %d\n", ID, message, predecessor);
            fflush(stdout);

            st_value = st.MPI_TAG;

            if(st_value == 2){
                leader_elected = 1;
                current_leader = message;
            } else {
                if(message < ID){
                    message = ID;
                } else if(message == ID){
                    is_leader = 1;
                    printf("Sou o processo %d e sou o lider!\n", rank);
                    fflush(stdout);

                    st_value = 2;
                }
            }
            MPI_Send(&message, 1, MPI_INT, successor, st_value, MPI_COMM_WORLD);
            printf("ID: %d, número do processo repassado: %d\n", ID, message);
            fflush(stdout);
        }
        MPI_Barrier(MPI_COMM_WORLD);

        if(is_leader == 0 ){
            printf("\nMPI_RANK: %d, ID: %d, LIDER: FALSE\n", rank, ID);
            fflush(stdout);
        } else if (is_leader == 1){
    
            printf("\nMPI_RANK: %d, ID: %d, LIDER: TRUE\n", rank, ID);
            fflush(stdout);
        }

   

    MPI_Finalize();
    return 0;
}