//Soledad Gallardo
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void lider(int datos[],int *idEnviar,int *numeroEnviar)
{

	//comparar dos numeros
	if (datos[1]>*numeroEnviar){
		*idEnviar=datos[0];
		*numeroEnviar=datos[1];
	}
}

int main (int argc, char* argv[])
{
	int id, numeroProcesos, numero, idEnviar, numeroEnviar, i;
	int datos[2];
	MPI_Status stat;
	MPI_Init (&argc, &argv); /* starts MPI */
	MPI_Comm_rank (MPI_COMM_WORLD, &id);/* get current process id */
	MPI_Comm_size (MPI_COMM_WORLD, &numeroProcesos);/* get number of processes */
	
	//calculando el numero aleatorio
	srand(time(0)+id);
	numero=rand()%50;
	idEnviar=id;
	numeroEnviar=numero;
	datos[0]=id;
	datos[1]=numero;
	
	if(id==0){
		MPI_Send(&idEnviar, 1, MPI_INT,id+1,0,MPI_COMM_WORLD);
		MPI_Send(&numeroEnviar, 1, MPI_INT,id+1,1,MPI_COMM_WORLD);

		MPI_Recv(&idEnviar, 1, MPI_INT,numeroProcesos-1,2,MPI_COMM_WORLD,&stat);
		MPI_Recv(&numeroEnviar, 1, MPI_INT,numeroProcesos-1,3,MPI_COMM_WORLD,&stat);

	}else if ((id>0)&&(id<numeroProcesos-1)){
		MPI_Recv(&idEnviar, 1, MPI_INT,id-1,0,MPI_COMM_WORLD,&stat);
		MPI_Recv(&numeroEnviar, 1, MPI_INT,id-1,1,MPI_COMM_WORLD,&stat);

		lider(datos,&idEnviar,&numeroEnviar);

		MPI_Send(&idEnviar, 1, MPI_INT,id+1,0,MPI_COMM_WORLD);
		MPI_Send(&numeroEnviar, 1, MPI_INT,id+1,1,MPI_COMM_WORLD);

		MPI_Recv(&idEnviar, 1, MPI_INT,numeroProcesos-1,2,MPI_COMM_WORLD,&stat);
		MPI_Recv(&numeroEnviar, 1, MPI_INT,numeroProcesos-1,3,MPI_COMM_WORLD,&stat);
	}else{
		MPI_Recv(&idEnviar, 1, MPI_INT,id-1,0,MPI_COMM_WORLD,&stat);
		MPI_Recv(&numeroEnviar, 1, MPI_INT,id-1,1,MPI_COMM_WORLD,&stat);

		lider(datos,&idEnviar,&numeroEnviar);

		for(i=0;i<numeroProcesos-1;i++){
			MPI_Send(&idEnviar, 1, MPI_INT,i,2,MPI_COMM_WORLD);
			MPI_Send(&numeroEnviar, 1, MPI_INT,i,3,MPI_COMM_WORLD);
		}
	}
	printf("Yo soy el proceso %d y calcule %d; el lider es el proceso %d que calculo %d\n",id,numero,idEnviar,numeroEnviar);
	MPI_Finalize();
	return 0;
}
