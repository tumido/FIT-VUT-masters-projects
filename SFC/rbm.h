#include <iostream>
#include <iomanip>
#include <vector>
#include "utils.h"

#pragma once

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
  /**
   * Restricted Boltzmann Machine constructor
   * @param count_visible Amount of neurons in visible layer
   * @param count_hidden  Amount of neurons in hidden layer
   */
  RBM(size_t, size_t);
  ~RBM();

  /**
   * Print state of the RBM
   *
   * Show biases and weights
   */
  const void printState();

  /**
   * Train the network
   * @param input       Vector of training samples
   * @param input_count [description]
   * @param rate        Desired learning rate
   * @param epoch_count Amount of training epochs
   */
  void train(std::vector<int *>, double, int);

  /**
   * Run the network for input data
   * @param input  Array of input data
   * @param output Calculated array containing the network response
   */
  void run(int*, double*);

  /**
   * Return size of input array (how may members network accepts)
   */
  int getSampleWidth();
};
