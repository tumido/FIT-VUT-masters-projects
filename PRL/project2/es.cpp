/**
 * @file es.cpp
 * @author Tomas Coufal (xcoufa09)
 * @brief Paralel Enumeration Sort
 */
#include <mpi.h>
#include <iostream>
#include <fstream>

using namespace std;

#define Y_TAG 0
#define X_TAG 1
#define Z_TAG 2
#define UNSET -1
#define prev_id(id, modulo) ((id-1 < 0) ? modulo : id-1)
#define eprintf(msg) fprintf (stderr, "\x1B[31m%s\033[0m\n", msg)

#ifndef METRICS
  #define iprintf(value, last) (cout << value << ((last) ? '\n' : ' '))
  #define oprintf(value) cout << value << endl
#else
  #define iprintf(value, last) (0)
  #define oprintf(value) (0)
#endif

int main(int argc, char *argv[]) {
  int processes, id, bus_target_id, n;
  MPI_Status stat;

  // Slave variables
  int x = UNSET, y = UNSET, c = 0, z = UNSET;
  // Master
  fstream inputfile;

  //MPI initialize
  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD, &n); n--;
  MPI_Comm_rank(MPI_COMM_WORLD, &id);

  // Enable metrics
  #ifdef METRICS
  MPI_Barrier(MPI_COMM_WORLD);
  double sort_time = -(MPI_Wtime());
  #endif

  // Master opens input file
  if (id == n) {
    inputfile.open("numbers", ios::in);
    if (!inputfile.good()) eprintf("Input file ended unexpectedly.");
  }

  // Distribute values and compare them
  for (int i=0; i < n; i++) {
    if (id == n) {
      // Master reads a value and distributes it
      x = inputfile.get();
      if (!inputfile.good()) {eprintf("Input file ended unexpectedly."); break;}
      iprintf(x, i == n-1);
      MPI_Send(&x, 1, MPI_INT, i, X_TAG, MPI_COMM_WORLD);
      MPI_Send(&x, 1, MPI_INT, 0, Y_TAG, MPI_COMM_WORLD);
      continue;
    }
    // Slave receive values
    if (i == 0)
      MPI_Recv(&x, 1, MPI_INT, n, X_TAG, MPI_COMM_WORLD, &stat);
    MPI_Recv(&y, 1, MPI_INT, prev_id(id, n), Y_TAG, MPI_COMM_WORLD, &stat);
    // Compare X and Y
    if (x != UNSET && y != UNSET && (x < y || (x == y && id < i))) c++;
    // Shift Y
    if (id < n-1) MPI_Send(&y, 1, MPI_INT, id+1, Y_TAG, MPI_COMM_WORLD);
  }

  // Route Z values
  for (int i = 0; i < n; i++) {
    // Sync
    MPI_Barrier(MPI_COMM_WORLD);
    // Procesor 'i' is sending, others are listening
    if (id == i && id == c) z = x;
    bus_target_id = (id != c) ? c : UNSET;
    MPI_Bcast(&bus_target_id, 1, MPI_INT, i, MPI_COMM_WORLD);
    // If there's a value to be sent, send it
    if (id == i && id != c)
      MPI_Send(&x, 1, MPI_INT, c, Z_TAG, MPI_COMM_WORLD);
    // If the processor is not receiving, skip this round
    if (bus_target_id != id) continue;
    // Receive value
    MPI_Recv(&z, 1, MPI_INT, i, Z_TAG, MPI_COMM_WORLD, &stat);
  }

  // Collect data and let master to print them
  for (int i = 0; i < n; i++) {
    // Master collects data
    if (id == n) {
      MPI_Recv(&z, 1, MPI_INT, id-1, Z_TAG, MPI_COMM_WORLD, &stat);
      oprintf(z);
      continue;
    }

    // Slaves are sending
    MPI_Send(&z, 1, MPI_INT, id+1, Z_TAG, MPI_COMM_WORLD);
    if (id == 0) continue;
    MPI_Recv(&z, 1, MPI_INT, id-1, Z_TAG, MPI_COMM_WORLD, &stat);
  }

  // Measure time when the sort is done
  #ifdef METRICS
  sort_time += MPI_Wtime();
  #endif

  // Master closes file
  if (id == n) inputfile.close();

  MPI_Finalize();

  // Print Metrics
  #ifdef METRICS
  if (id == n) cout << sort_time << endl;
  #endif
  return 0;
}
