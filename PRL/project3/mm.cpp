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
#include <algorithm>

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
 * @return      destination, where the dimension should be stored
 */
int parse_first_line(fstream * in) {
	string line;
	int value;
	getline(*in, line);
	istringstream(line) >> value;
	return value;
}

/**
 * Parse input file and check if its valid
 *
 * A valid matrix contains numbers only, has proper amount of rows and columns
 * @param  in   filedescription containing the input matrix
 * @param  rows desired amount of rows, if not checked set to UNSET
 * @param  cols desired amount of columns, if not checked set to UNSET
 * @return      true if matrix is valid
 */
bool check_mat(fstream * in, int rows, int cols) {
	string line;
  int val;
	int real_rows = 0, real_cols = 0;
	bool trailing_lines = false;

	// Count lines
  while (getline(*in, line)) {
    istringstream l(line);

		// Loop through line and count values
		real_cols = 0;
		while (l >> val) real_cols++;

		// Allow trailing blank lines
		if (real_cols == 0 && !trailing_lines) trailing_lines = true;
		if (real_cols == 0 && trailing_lines) continue;
		else if(real_cols == 0) return false;

		// Check if all values are numbers
		if (line.find_first_not_of("-0123456789 ") != string::npos) return false;

		// Match columns counter
		if (cols != UNSET && real_cols != cols) return false;
		real_rows++;
  }

	// Match rows counter
	if (rows != UNSET && real_rows != rows) return false;
	return true;
}

/**
 * Get dimensions from imput matrices if they are valid
 * @param rows location where amount of rows is saved
 * @param cols destination where to save amount of columns
 */
void get_dimensions(int * rows, int * cols) {
	// Open input files
	fstream matA, matB;
	int r, c;
	matA.open(MAT_A, ios::in); matB.open(MAT_B, ios::in);

	if (!matA.good() || !matB.good()) {
		eprintf("Input file ended unexpectedly.");
		return;
	}

	// Get dimensions
	r = parse_first_line(&matA);
	c = parse_first_line(&matB);

	// check if the value stated on first line is correct
	if (!check_mat(&matA, r, UNSET) || !check_mat(&matB, UNSET, c)) {
		eprintf("Input matrices are not matching requirements.");
		return;
	}

	matA.close(); matB.close();

	*rows = r; *cols = c;
}

/**
 * Loop through a file vertically (by columns)
 *
 * Read file line by line and if the column number matches processor ID,
 * store the cell value in its queue
 * @param id      processor ID
 * @param columns amount of items in a row
 * @param q       processor's input queue
 */
void fill_inputB(int id, int columns, queue<int> * q) {
	fstream in;
	string line;
  int val;

	// Open file and skip first line
	in.open(MAT_B, ios::in);
	if (!in.good()) eprintf("Input file ended unexpectedly.");
	getline(in, line);

  // Loop by lines
  while (getline(in, line)) {
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

	in.close();
}

/**
 * Loop through a file by lines (rows)
 *
 * Read file line by line and store the cell values inside the
 * appropriate processor
 * @param id      processor ID
 * @param columns amount of items on a row (to calculate correct ID matching)
 * @param q       processor's input queue
 */
void fill_inputA(int id, int columns, queue<int> * q) {
	fstream in;
	string line;
	int val, i = 0;

	// Open file and skip first line
	in.open(MAT_A, ios::in);
	if (!in.good()) eprintf("Input file ended unexpectedly.");
	getline(in, line);

  // Loop by lines
  while (getline(in, line)) {
    // Skip line if it's not useful for this processor
    if (id != i*columns) {i++; continue;}
    // Parse values and store them
    istringstream l(line);
    while (l >> val){
      q->push(val);
    }
    return;
  }

	in.close();
}

/**
 * Mash Multiplication
 */
int main(int argc, char *argv[]) {
  int processes, id, bus_target_id, n;
  MPI_Status stat;

  // Input matrices
	int rows = UNSET, columns = UNSET;
  queue<int> inputA, inputB;
	// Each processor registry
  int a = UNSET, b = UNSET, c = 0;

  //MPI initialize
  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD, &n);
  MPI_Comm_rank(MPI_COMM_WORLD, &id);

	// Get dimensions via one process
	if (id == 0) get_dimensions(&rows, &columns);
	// Let's broadcast the dimensions
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Bcast(&rows, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&columns, 1, MPI_INT, 0, MPI_COMM_WORLD);

	// The rows and colums don't match OR something bad happened when opening input
	if (rows == UNSET || columns == UNSET) {MPI_Finalize(); return 1;}

  // Determine position of current processor
  bool first_row = id < columns;
	bool first_col = id % columns == 0;
  bool last_row = id >= columns*(rows-1);
	bool last_col = id % columns == columns-1;

  // Load data on first row and column processors
  if (first_col) fill_inputA(id, columns, &inputA);
  if (first_row) fill_inputB(id, columns, &inputB);

  // Enable metrics
  #ifdef METRICS
  MPI_Barrier(MPI_COMM_WORLD);
  double mm_time = -(MPI_Wtime());
  #endif

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

	// Measure time when the mm is done
  #ifdef METRICS
  MPI_Barrier(MPI_COMM_WORLD);
  mm_time += MPI_Wtime();
	#else
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
  #endif

  MPI_Finalize();

  // Print Metrics
  #ifdef METRICS
  if (id == 0) cout << mm_time << endl;
  #endif
  return 0;
}
