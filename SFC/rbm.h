#include <iostream>
#include <iterator>
#include <iomanip>
#include <cstring>
#include <vector>
#include <stdlib.h>
#include <curses.h>
#include <signal.h>
#include "utils.h"

#ifndef RBM_H
#define RBM_H

class RBM {
private:
  size_t count_visible, count_hidden;
  double **weights;
  double *bias_visible, *bias_hidden;
  void probability_of_h_given_v(int *, double *, int *);
  void probability_of_v_given_h(int *, double *, int *);
  double propagate_from_visible(int *, double *, double);
  double propagate_from_hidden(int *, int, double);

public:
  RBM(size_t, size_t);
  ~RBM();
  const void printState();
  void train(std::vector<int *>, int, double, int);
  void run(int*, double*);
  int getSampleWidth();
};
#endif
