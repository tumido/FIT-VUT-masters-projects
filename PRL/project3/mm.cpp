/**
 * @file mm.cpp
 * @author Tomas Coufal (xcoufa09)
 * @brief Paralel Mash Multiplication
 */
#include <mpi.h>
#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include <sstream>

using namespace std;

#define TAG_A 0
#define TAG_B 1
#define TAG_C 1
#define MAT_A "mat1"
#define MAT_B "mat2"
#define UNSET -1
#define eprintf(msg) fprintf (stderr, "\x1B[31m%s\033[0m\n", msg)

/**
 * Get dimension stated on the first line of a file
 * @param in    file stream from where the line should be retrieved
 * @param value destination, where the dimension should be stored
 */
void parse_first_line(fstream * in, int * value) {
	string line;
	getline(*in, line);
	istringstream(line) >> *value;
}

/**
 * Loop through a file vertically (by columns)
 *
 * Read file line by line and if the column number matches processor ID,
 * store the cell value in its queue
 * @param in      file stream to read and parse
 * @param id      processor ID
 * @param columns amount of items in a row
 * @param q       processor's input queue
 */
void fill_inputB(fstream * in, int id, int columns, queue<int> * q) {
  string line;
  int val;
  // Loop by lines
  while (getline(*in, line)) {
    istringstream l(line);
    // Loop the items while counting columns
    for (int i=0; i < columns; i++) {
      l >> val;
      if (id != i) continue;
      // Store the value
      q->push(val);
      break;
    }
  }
}

/**
 * Loop through a file by lines (rows)
 *
 * Read file line by line and store the cell values inside the
 * appropriate processor
 * @param in      file stream to parse
 * @param id      processor ID
 * @param columns amount of items on a row (to calculate correct ID matching)
 * @param q       processor's input queue
 */
void fill_inputA(fstream * in, int id, int columns, queue<int> * q) {
  string line;
  int val, i = 0;
  // Loop by lines
  while (getline(*in, line)) {
    // Skip line if it's not useful for this processor
    if (id != i*columns) {i++; continue;}
    // Parse values and store them
    istringstream l(line);
    while (l >> val){
      q->push(val);
    }
    return;
  }
}

/**
 * Mash Multiplication
 */
int main(int argc, char *argv[]) {
  int processes, id, bus_target_id, n;
  MPI_Status stat;

  // Input matrices
	fstream matA, matB;
	int rows, columns;
  queue<int> inputA, inputB;
	// Each processor registry
  int a = UNSET, b = UNSET, c = 0;

  //MPI initialize
  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD, &n);
  MPI_Comm_rank(MPI_COMM_WORLD, &id);

  // Enable metrics
  #ifdef METRICS
  MPI_Barrier(MPI_COMM_WORLD);
  double sort_time = -(MPI_Wtime());
  #endif

	// Open input files
	matA.open(MAT_A, ios::in);
	if (!matA.good()) eprintf("Input file ended unexpectedly.");
	matB.open(MAT_B, ios::in);
	if (!matB.good()) eprintf("Input file ended unexpectedly.");

  // Get dimensions
	parse_first_line(&matA, &rows);
	parse_first_line(&matB, &columns);

  // Determine position of current processor
  bool first_row = id < columns;
	bool first_col = id % columns == 0;
  bool last_row = id >= columns*(rows-1);
	bool last_col = id % columns == columns-1;

  // Load data on first row and column processors
  if (first_col) fill_inputA(&matA, id, columns, &inputA);
  if (first_row) fill_inputB(&matB, id, columns, &inputB);
	matA.close();
	matB.close();

  // Multiply matrices
  for (int i=0; i< columns+rows; i++) {
    // Receive input
    if (first_row) {b = inputB.front(); inputB.pop();}
    else MPI_Recv(&b, 1, MPI_INT, id-columns, TAG_B, MPI_COMM_WORLD, &stat);
    if (first_col) {a = inputA.front(); inputA.pop();}
    else MPI_Recv(&a, 1, MPI_INT, id-1, TAG_A, MPI_COMM_WORLD, &stat);

    // Compute
    c += (a*b);

    // Pass data
    if (!last_row) MPI_Send(&b, 1, MPI_INT, id+columns, TAG_B, MPI_COMM_WORLD);
    if (!last_col) MPI_Send(&a, 1, MPI_INT, id+1, TAG_A, MPI_COMM_WORLD);
  }

  // Synchronize the output by proc0
  if (id != 0)
    // All processors except proc0 sends its C value
    MPI_Send(&c, 1, MPI_INT, 0, TAG_C, MPI_COMM_WORLD);
  else {
    // Print the header "rows:columns" and its own C value
    cout << rows << ":" << columns << endl << c;
    // Receive C from others
    for (int i=1; i<n; i++) {
      MPI_Recv(&c, 1, MPI_INT, i, TAG_C, MPI_COMM_WORLD, &stat);
      cout << ((i % columns == 0) ? '\n' : ' ') << c;
    }
    // Trailing eol
    cout << endl;
  }

	// Measure time when the mm is done
  #ifdef METRICS
  sort_time += MPI_Wtime();
  #endif

  MPI_Finalize();

  // Print Metrics
  #ifdef METRICS
  if (id == n) cout << sort_time << endl;
  #endif
  return 0;
}
