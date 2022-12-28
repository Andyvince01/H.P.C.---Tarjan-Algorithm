#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Apri il file in modalità di lettura
    MPI_File file;
    MPI_File_open(MPI_COMM_WORLD, "arr1D.bin", MPI_MODE_RDONLY, MPI_INFO_NULL, &file);

    // Leggi il numero di elementi del file
    MPI_Offset file_size;
    MPI_File_get_size(file, &file_size);
    int num_elements = file_size / sizeof(double);

    // Alloca un array per i dati letti dal file
    double* data = malloc(num_elements * sizeof(double));

    // Leggi i dati dal file
    MPI_File_read(file, data, num_elements, MPI_DOUBLE, MPI_STATUS_IGNORE);

    // Stampa i dati letti
    printf("Process %d read the following data:\n", rank);
    for (int i = 0; i < num_elements; i++) {
        printf("%f\n", data[i]);
    }

    // Libera la memoria
    free(data);

    // Chiudi il file
    MPI_File_close(&file);

    

    MPI_Finalize();
    return 0;
}
