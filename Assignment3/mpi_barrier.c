//Binary tree model
#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>

void mpi_iittp_barrier(){
		int rank, world_size;
		MPI_Comm_rank(MPI_COMM_WORLD, &rank);
		MPI_Comm_size(MPI_COMM_WORLD, &world_size);
		if(world_size == 1){
			return;
		}
		if(rank == 0){
			int flag = 1;
			if(world_size > 1)
				MPI_Recv(&flag, 1, MPI_INT, 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			if(world_size > 2)
				MPI_Recv(&flag, 1, MPI_INT, 2, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			if(world_size > 1)
				MPI_Send(&flag, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
			if(world_size > 2)
				MPI_Send(&flag, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
		}
		else if((2*rank)+1 < world_size){
			int flag = 0;
			int parent = (rank-1)/2;
			
			MPI_Recv(&flag, 1, MPI_INT, (2*rank)+1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			if((2*rank)+2 < world_size)
				MPI_Recv(&flag, 1, MPI_INT, (2*rank)+2, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Send(&flag, 1, MPI_INT, parent, 1, MPI_COMM_WORLD);

			MPI_Recv(&flag, 1, MPI_INT, parent, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Send(&flag, 1, MPI_INT, (2*rank)+1, 0, MPI_COMM_WORLD);
			if((2*rank)+2 < world_size)
				MPI_Send(&flag, 1, MPI_INT, (2*rank)+2, 0, MPI_COMM_WORLD);
		}
		else{
			int flag = 0;
			int parent = (rank-1)/2;

			MPI_Send(&flag, 1, MPI_INT, parent, 1, MPI_COMM_WORLD);
			MPI_Recv(&flag, 1, MPI_INT, parent, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
		return;
	}


int main(int argc, char* const argv[]){
		MPI_Init(NULL, NULL);
		mpi_iittp_barrier();
		MPI_Finalize();
		return 0;
	}
