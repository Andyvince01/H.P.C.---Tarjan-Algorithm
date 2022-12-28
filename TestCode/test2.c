#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <unistd.h>
#include <stdbool.h>
#include <math.h>

int main(int argc, char* argv[]) {
    char  fileOut[10] = "output";
    sprintf(fileOut + strlen("output"), "%d", 1);
    strcat(fileOut,".txt");

    printf("%s\n", fileOut);
    return 0;
}