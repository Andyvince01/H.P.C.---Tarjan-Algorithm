#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <unistd.h>
#include <stdbool.h>
#include <math.h>

#define MAX_NODES 9999
#define MAX_EDGES 99999


typedef struct Graph {
  int num_nodes;
  int num_edges;
  int **adjacency_matrix;
} Graph;

void add_node(Graph g,int i){

}

void readGraph(int **adj_matrix){
    FILE *fp = fopen("graph.bin", "rb");
    if (fp == NULL) {
        perror("Errore nell'apertura del file");
        exit(1);
    }

    // Legge il numero di vertici dal file
    int n_vertices;
    fread(&n_vertices, sizeof(int), 1, fp);
    // Alloca la matrice di adiacenza
    
    for (int i = 0; i < n_vertices; i++) {
        adj_matrix[i] = (int *) malloc(n_vertices * sizeof(int));
    }

    // Legge la matrice di adiacenza dal file
    for (int i = 0; i < n_vertices; i++) {
        // Legge una riga della matrice dal file
        int row[n_vertices];
        fread(row, sizeof(int), n_vertices, fp);
        // Copia la riga nella matrice di adiacenza
        memcpy(adj_matrix[i], row, n_vertices * sizeof(int));
    }

    // Chiudi il file
    fclose(fp);

}

void writeBlocks(Graph *graph, int rank, int num_procs, int num_nodes){
    
    // Determina la dimensione di ogni blocco della matrice di adiacenza
    int block_size = num_nodes/num_procs;

    // Calcola l'indice di inizio del blocco della matrice di adiacenza per il processo corrente
    int block_start = block_size * rank;

    // Calcola l'indice di fine del blocco della matrice di adiacenza per il processo corrente
    int block_end = block_start + block_size;

    // Alloca un array per il proprio blocco della matrice di adiacenza
    int block[block_size][num_nodes];

    // Copia il proprio blocco della matrice di adiacenza nell'array
    for (int i = block_start; i < block_end; i++) {
        for (int j = 0; j < num_nodes; j++)
            block[i - block_start][j] = graph->adjacency_matrix[i][j];
    }

    char  file_name[20];
    snprintf(file_name, 20, "MPIOutput/file%d.bin", rank);

    MPI_File file;
    MPI_File_open(MPI_COMM_SELF, file_name, MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &file);

    // Scrivi il proprio blocco nella file
    MPI_File_write(file, block, block_size * num_nodes, MPI_INT, MPI_STATUS_IGNORE);
    
    // Chiudi il file
    MPI_File_close(&file);

  }

void readBlocks(int rank, int num_procs, int num_nodes){
    
    // Determina la dimensione di ogni blocco della matrice di adiacenza
    int block_size = num_nodes/num_procs;

    // Alloca un array per il proprio blocco della matrice di adiacenza
    int block[block_size][num_nodes];

    char  file_name[20];
    snprintf(file_name, 20, "MPIOutput/file%d.bin", rank);

    MPI_File file;
    MPI_File_open(MPI_COMM_SELF, file_name, MPI_MODE_RDONLY, MPI_INFO_NULL, &file);

    // Leggi il proprio blocco nella file
    MPI_File_read(file, block, block_size * num_nodes, MPI_INT, MPI_STATUS_IGNORE);
    
    // Chiudi il file
    MPI_File_close(&file);

    printf("\n===RANK %d===\n", rank);
    for(int i = 0; i < block_size; i++)
        for(int j = 0; j < num_nodes; j++)
            printf("%d", block[i][j]);

  }

int main(int argc, char* argv[]) {

    // Inizializza MPI
    MPI_Init(&argc, &argv);

    // Ottieni il numero di processi e il rank del processo corrente
    int num_procs, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    //printf("AHAHAH SONO IO %d",rank);

    Graph *graph = (Graph *)malloc(sizeof(Graph*));
    graph->num_nodes = 10;
    graph->adjacency_matrix = (int **) malloc(graph->num_nodes*graph->num_nodes * sizeof(int*));

    readGraph(graph->adjacency_matrix);
   
    writeBlocks(graph, rank, num_procs, graph->num_nodes);
    readBlocks(rank, num_procs, graph->num_nodes);

    MPI_Finalize();

    return 0;

}