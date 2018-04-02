//
// Starting code for the MPI coursework.
//
// Compile with:
//
// mpicc -Wall -o cwk1 cwk1.c
//
// or use the provided makefile.
//

//
// Includes
//
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

// Some extra routines for this coursework. DO NOT MODIFY OR REPLACE THESE
// ROUTINES, as this file will be replaced with a different version for
// assessment.
#include "cwk2_extras.h"

//
// Main
//
int main(int argc, char **argv) {
  int i;

  // Initialise MPI and get the rank and no. of processes.
  int rank, numProcs;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  printf("Rank - %i Processes - %i\n", rank, numProcs);

  int *localData = (int *)malloc(sizeof(int) * 2);
  int *data;
  // Read in the image file to rank 0.
  int *image = NULL, maxValue, pixelsPerProc, dataSize;
  if (rank == 0) {
    // Read in the file and extract the maximum grey scale value and the data
    // size (including padding bytes). Defined in cwk2_extras.h; do not change,
    // although feel free to inspect.
    image = readImage("image.pgm", &maxValue, &dataSize, numProcs);
    if (image == NULL) {
      MPI_Finalize();
      return EXIT_FAILURE;
    }

    // The image size has already been rounded up to a multiple of numProcs by
    // "readImage()".
    pixelsPerProc = dataSize / numProcs;

    printf(
        "Rank 0: Read in PGM image array of size %d (%d per process), with max "
        "value %d.\n",
        dataSize, pixelsPerProc, maxValue);

    // send the size of the data
    localData[0] = pixelsPerProc;
    localData[1] = maxValue;
    MPI_Bcast(localData, 2, MPI_INT, 0, MPI_COMM_WORLD);
  } else {
    // get the size of the local data that we will recieve
    MPI_Bcast(localData, 2, MPI_INT, 0, MPI_COMM_WORLD);
    printf("Data size - %d, max value - %d\n", localData[0], localData[1]);
    pixelsPerProc = localData[0];
    maxValue = localData[1];
    data = (int *)malloc(sizeof(int) * pixelsPerProc);
  }

  // Allocate memory for the final histogram on rank 0.
  int *combinedHist = NULL;
  if (rank == 0) {
    combinedHist = (int *)malloc((maxValue + 1) * sizeof(int));
    if (!combinedHist) return allocateFail("global histogram", rank);

    for (i = 0; i < maxValue + 1; i++) combinedHist[i] = 0;
  }

  //
  // Your parallel code goes here. Feel free to change anything else in this
  // file, EXCEPT calls to the routines in cwk2_extras.h which MUST be called as
  // provided.
  //

  if (rank == 0) {
    for (i = 1; i < numProcs; i++) {
      printf("Send to - %d\n", i);
      MPI_Send(&image[pixelsPerProc * (i - 1)], pixelsPerProc, MPI_INT, i, 0,
               MPI_COMM_WORLD);
    }
  } else {
    MPI_Recv(data, pixelsPerProc, MPI_INT, 0, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
  }

  // Allocate memory for the check histogram, and then initialise it to zero.
  int *checkHist = (int *)malloc((maxValue + 1) * sizeof(int));
  if (!checkHist) return allocateFail("histogram for checking", rank);
  for (i = 0; i < maxValue + 1; i++) checkHist[i] = 0;

  if (rank == 0) {
    for (i = pixelsPerProc * (numProcs - 1); i < dataSize; i++) {
      if (image[i] >= 0) {
        checkHist[image[i]]++;
      }
    }
  } else {
    for (i = 0; i < pixelsPerProc; i++) {
      if (data[i] >= 0) {
        checkHist[data[i]]++;
      }
    }
  }

  if (rank == 0) {
    int *temp = (int *)malloc((maxValue + 1) * sizeof(int));
    for (i = 0; i < maxValue + 1; i++) temp[i] = 0;

    for (i = 0; i < numProcs - 1; i++) {
      printf("Call to combine - %d\n", i);
      MPI_Recv(temp, maxValue, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD,
               MPI_STATUS_IGNORE);

      int j;
      for (j = 0; j <= maxValue; j++) {
        checkHist[j] += temp[j];
      }
    }

    free(temp);
  } else {
    MPI_Send(checkHist, maxValue, MPI_INT, 0, 0, MPI_COMM_WORLD);
  }

  if (rank == 0) {
    for (i = 0; i <= maxValue; i++) {
      combinedHist[i] = checkHist[i];
    }
  }

  free(checkHist);

  // for (i = 0; i < maxValue; i++) {
  //   printf("Greyscale value %i:\tCount %i\t\n", i, checkHist[i]);
  // }

  //
  // Constructs the histogram in serial on rank 0. Can be used as part of a
  // check that your parallel version works.
  //
  // if (rank == 0) {
  //   // Allocate memory for the check histogram, and then initialise it to
  //   zero. int *checkHist = (int *)malloc((maxValue + 1) * sizeof(int)); if
  //   (!checkHist) return allocateFail("histogram for checking", rank); for (i
  //   = 0; i < maxValue + 1; i++) checkHist[i] = 0;

  //   // Construct the histogram using the global data only.
  //   for (i = 0; i < dataSize; i++) {
  //     if (image[i] >= 0) {
  //       checkHist[image[i]]++;
  //     }
  //   }

  //   for (i = 0; i <= maxValue; i++) {
  //     combinedHist[i] = checkHist[i];
  //   }

  //   // Display the histgram.
  //   // for (i = 0; i < maxValue + 1; i++) {
  //   //   printf("Greyscale value %i:\tCount %i\t(check: %i)\n", i,
  //   //   combinedHist[i],
  //   //          checkHist[i]);
  //   // }

  //   free(checkHist);
  // }

  //
  // Clear up and quit.
  //
  if (rank == 0) {
    saveHist(combinedHist, maxValue);  // Defined in cwk2_extras.h; do not
                                       // change or replace the call.
    free(image);
    free(combinedHist);
  }

  MPI_Finalize();
  return EXIT_SUCCESS;
}