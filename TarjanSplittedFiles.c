#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <unistd.h>

#define MAX_DEGREE 5
#define MAX_NUM_VERTICES 20

struct Vertices {
    int index;
    int low_link;
    int in_stack;
    int deg;
    int adj[MAX_DEGREE]; /* < 0 if incoming edge */
};

int curr_index = 0;

struct stack_s {
    int top;
    int items[MAX_NUM_VERTICES];
} stack = {-1, {}};

void stack_push(int v) {
    stack.top++;
    if (stack.top < MAX_NUM_VERTICES)
	stack.items[stack.top] = v;
    else {
	printf("Stack is full!\n");
	exit(1);
    }
}

int stack_pop() {
    return stack.top < 0 ? -1 : stack.items[stack.top--];
}

int min(int a, int b) {
    return a < b ? a : b;
}

void scc(int v, struct Vertices file[], char res[]) {
    int i, c, n;
    file[v].index = curr_index;
    file[v].low_link = curr_index;
    ++curr_index;

    stack_push(v);
    file[v].in_stack = 1;

    for (i = 0, c = file[v].deg; i < c; i++)
	if (file[v].adj[i] > 0){
	    n = file[v].adj[i] - 1;
	    if (file[n].index == -1) {
            scc(n,file,res);
            file[v].low_link = min(file[v].low_link, file[n].low_link);
	    } else if (file[n].in_stack) {
		    file[v].low_link = min(file[v].low_link, file[n].index);
	    }
	}
    
    if (file[v].index == file[v].low_link) {
        char sccv[5];
        sprintf(sccv, "scc %d: ", v);
        strcat(res, sccv);
        char nodo[2];
        while ((n = stack_pop()) != -1) {
            file[v].in_stack = 0;
            sprintf(nodo, " %d", n+1);
            strcat(res, nodo);
            if (n == v) {
            printf("\n");
            break;
            }
        }
    }
}


int main(int argc, char *argv[]) {

    /* MPI Variables */
    int rank;
    int n_ranks;
    char *result;
    MPI_Init(&argc,&argv);

    

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_rank(MPI_COMM_WORLD, &n_ranks);

    struct Vertices file1[] = {{-1, -1, 0, 3, {-4, 2}}, 
        {-1, -1, 0, 2, {-1, 3}},
        {-1, -1, 0, 3, {-2, 4}},
        {-1, -1, 0, 4, {-3, 1}}};
    
    struct Vertices file2[] = {{-1, -1, 0, 3, {-3, 2}},
        {-1, -1, 0, 2, {-1, 3}}, 
        {-1, -1, 0, 3, {-2, 1}}};
    
    struct Vertices file3[] = {{-1, -1, 0, 3, {-5, 2}},    
        {-1, -1, 0, 2, {-1, 3}},   
        {-1, -1, 0, 3, {-2, 4}},  
        {-1, -1, 0, 3, {-3, 5}},   
        {-1, -1, 0, 3, {-4, 1}}};

    MPI_Request request;
    if (rank == 0){
        printf("Ciao, per ora sono inutile\n");
        char message1[50];
        char message2[50];
        char message3[50];
        MPI_Status  status;
        MPI_Recv(message1, 50, MPI_CHAR, 1, 0, MPI_COMM_WORLD, &status);
        printf("%s\n", message1);
        MPI_Recv(message2, 50, MPI_CHAR, 2, 0, MPI_COMM_WORLD, &status);
        printf("%s\n", message2);
        MPI_Recv(message3, 50, MPI_CHAR, 3, 0, MPI_COMM_WORLD, &status);
        printf("%s\n", message3);
        
    }
    else if (rank == 1){
        
        for (int i = 0; i < 4; i++)
            if (file1[i].index == -1)
                scc(i,file1,result);
        
        MPI_Send(result,50, MPI_CHAR, 0, 0, MPI_COMM_WORLD);        
    }
    else if (rank == 2){
        for (int i = 0; i < 3; i++)
	        if (file2[i].index == -1)
	            scc(i,file2,result);
        MPI_Send(result,50, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }
    else if (rank == 3){
        for (int i = 0; i < 5; ++i)
            if (file3[i].index == -1)
                scc(i,file3,result);
        MPI_Send(result,50, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }
    
    MPI_Finalize();
    return 0;

}


