#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <unistd.h>
#include <stdbool.h>
#include <math.h>

#define MAX_NODES 99999
#define MAX_EDGES 999999

typedef struct Graph {
  int num_nodes;
  int num_edges;
  int **adjacency_matrix;
} Graph;

void add_edge(Graph* graph, int u, int v) {
  // Check if the indices are within bounds
  if (u < 0 || u >= graph->num_nodes || v < 0 || v >= graph->num_nodes) {
    return;
  }

  graph->adjacency_matrix[u][v] = 1;
  graph->num_edges++;
}

int min(int x, int y){
    if(x<y)
        return x;
    else return y;
}


int dfn[MAX_NODES]; // Depth first search numbering
int low[MAX_NODES]; // Low link values
bool on_stack[MAX_NODES]; // Whether each node is on the stack
int stack[MAX_NODES]; // Stack of nodes being visited
int stack_top = -1; // Index of the top of the stack
int num_scc = 0; // Number of strongly connected components found
int scc[MAX_NODES][MAX_NODES]; // Strongly connected components
int scc_size[MAX_NODES]; // Size of each strongly connected component
int dfs_counter = 0;
void tarjan(Graph* graph, int u) {
  // Set the dfn and low values for the current node
  dfn[u] = low[u] = ++dfs_counter;
  stack[++stack_top] = u;
  on_stack[u] = true;

  // Check each of the nodes adjacent to the current node
  for (int v = 0; v < graph->num_nodes; v++) {
    if (graph->adjacency_matrix[u][v] == 0) {
      continue;
    }

    // If v has not been visited yet, recursively visit it
    if (dfn[v] == 0) {
      tarjan(graph, v);
      low[u] = min(low[u], low[v]);
    }
    // If v is on the stack, update the low value of u
    else if (on_stack[v]) {
      low[u] = min(low[u], dfn[v]);
    }
  }

  // If u is a root of a strongly connected component, pop the nodes from the stack
  // and add them to the current strongly connected component
  if (dfn[u] == low[u]) {
    int v;
    do {
      v = stack[stack_top--];
      on_stack[v] = false;
      scc[num_scc][scc_size[num_scc]++] = v;
    } while (u != v);
    num_scc++;
  }
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



int main() {
    Graph *graph = (Graph *)malloc(sizeof(Graph*));
    graph->num_nodes = 50;
    graph->num_edges = 0;
    graph->adjacency_matrix = (int **) malloc(graph->num_nodes * sizeof(int*));
    readGraph(graph->adjacency_matrix);
    // Initialize the dfn, low, and on_stack arrays
    memset(dfn, 0, sizeof(dfn));
    memset(low, 0, sizeof(low));
    memset(on_stack, 0, sizeof(on_stack));

    for (int u = 0; u < graph->num_nodes; u++)
        if (dfn[u] == 0) 
            tarjan(graph, u);
        
    int i = 0;
    for (i; i < num_scc; i++) {
    printf("Strongly connected component %d: ", i+1);
        for (int j = 0; j < scc_size[i]; j++) {
        printf("%d ", scc[i][j]);
    }
    printf("\n");
}

  return 0;
}
