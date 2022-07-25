#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]){
    int size, rank, nproc, ID, predecessor, successor;
    MPI_Status st;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   

        ID = rank;
        predecessor = (rank == 0) ? (size - 1) : (rank - 1);

        successor = (rank == size - 1) ? 0 : (rank + 1);

        int current_leader,
            is_leader = 0,
            leader_elected = 0,
            message,
            st_value = 0;

        if(rank == 0){
            message = ID;
            MPI_Send(&message, 1, MPI_INT, successor, 0, MPI_COMM_WORLD); // Iniciando eleição enviando ID do processo 0
        }
        while(!leader_elected){
            MPI_Recv(&message, 1, MPI_INT, predecessor, MPI_ANY_TAG, MPI_COMM_WORLD, &st); //Processo recebe mensagem do processo antecessor

            printf("Sou o processo %d, recebi o ID %d do processo %d\n", ID, message, predecessor);
            fflush(stdout);

            st_value = st.MPI_TAG;

            if(st_value == 200){
                leader_elected = 1;
                current_leader = message;
            } else {
                if(message < ID){
                    message = ID;
                } else if(message == ID){
                    is_leader = 1;
                    printf("O processo %d foi eleito lider!\n", rank);
                    fflush(stdout);

                    st_value = 200;
                }
               
            }   
            MPI_Send(&message, 1, MPI_INT, successor, st_value, MPI_COMM_WORLD); //Processo repassa mensagem para o processo sucessor
            printf("ID: %d, número do processo repassado: %d\n", ID, message);
            fflush(stdout);
        }
        MPI_Barrier(MPI_COMM_WORLD);
        
    MPI_Finalize();
    return 0;
}