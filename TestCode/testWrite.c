/* ///////////////////////////////////////////////////////////////////// */
/*! 
  \file  
  \brief Writing of a 1D buffer in parallel.

  Write a 1D buffer in parallel using 4 different versions.
  For a contiguous data type, use:

  VERSION == 1 employs shared file pointer
  VERSION == 2 employs individual file pointer with offset computed
               by the MPI_File_seek()
  VERSION == 3 defines a file view with offset depending on the process
               rank
  VERSION == 4 similar to VERSION == 3, but using a contiguous MPI
               datatype

  A non-contiguous version is handled with VERSION == 5, which defines
  a MPI vector datatype and a file view.
   
  \author A. Mignone (mignone@to.infn.it)
  \date   March 1, 2020
*/
/* ///////////////////////////////////////////////////////////////////// */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#define NELEM    3

#define VERSION  1
int main(int argc, char **argv)
{
  int i, rank, size;
  double buf[NELEM];
  char fname[] = "arr1D.bin";
  MPI_File   fh;
  MPI_Offset disp;

/* --------------------------------------------------------
   0. Initialize the MPI execution environment
   -------------------------------------------------------- */

  MPI_Init (&argc, &argv);
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);
  MPI_Comm_size (MPI_COMM_WORLD, &size);

/* --------------------------------------------------------
   1. Initialize array
   -------------------------------------------------------- */

  for (i = 0; i < NELEM; i++) buf[i] = rank;

/* --------------------------------------------------------
   2. Delete, re-open file and write
   -------------------------------------------------------- */

  MPI_Datatype vect_type;
  MPI_Type_vector(NELEM, 1, size, MPI_DOUBLE, &vect_type);
  MPI_Type_commit(&vect_type);
  disp = rank*sizeof(double);

  MPI_File_delete(fname, MPI_INFO_NULL);

  MPI_File_open(MPI_COMM_WORLD, fname, 
                MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fh);
  MPI_File_set_view(fh, disp, MPI_DOUBLE, vect_type, "native", MPI_INFO_NULL);
  MPI_File_write_all(fh, buf, NELEM, MPI_DOUBLE, MPI_STATUS_IGNORE);
  MPI_File_close(&fh);

  MPI_Type_free(&vect_type);
  MPI_Finalize();
  return 0;
}
