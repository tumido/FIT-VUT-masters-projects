#include "rbm.h"

const void RBM::printState() {
  std::cout << std::fixed << std::setprecision(4);

  std::cout << "Bias of visible nodes (" << count_visible << "):\t[";
  for(int x = 0; x < count_visible; x++) std::cout << bias_visible[x] << ", ";
  std::cout << "\b\b]" << std::endl;

  std::cout << "Bias of hidden nodes (" << count_hidden << "):\t[";
  for(int x = 0; x < count_hidden; x++) std::cout << bias_hidden[x] << ", ";
  std::cout << "\b\b]" << std::endl;


  std::cout << "Weights:" << std::endl << std::setw(12) << std::setfill(' ') << "|";
  for(int v=0; v<count_visible; v++) std::cout << std::setw(7) << std::setfill(' ') << "#" << v;
  std::cout << std::endl << std::string(12+count_visible*8, '-') << std::endl;
  for(int h=0; h<count_hidden; h++) {
    std::cout << std::setw(9) << std::setfill(' ') << "Hidden #" << h << " |";
    for(int v=0; v<count_visible; v++) std::cout << std::setw(8) << std::setfill(' ') << weights[h][v];
    std::cout << std::endl;
  }
}


RBM::RBM(size_t count_visible, size_t count_hidden) : count_visible(count_visible), count_hidden(count_hidden) {
  weights = new double * [count_visible];

  double around = 1.0 / count_visible;

  for(int h=0; h<count_hidden; h++) {
    weights[h] = new double[count_visible];
    for(int v=0; v<count_visible; v++) {
      weights[h][v] = utils::uniform(around);
    }
  }

  bias_hidden  = new double[count_hidden];
  bias_visible = new double[count_visible];

  for(int h=0; h<count_hidden; h++)  bias_hidden[h] = 0;
  for(int v=0; v<count_visible; v++) bias_visible[v] = 0;
}

RBM::~RBM() {
  for(int i=0; i<count_hidden; i++) delete[] weights[i];
  delete[] weights;
  delete[] bias_visible;
  delete[] bias_hidden;
}

void RBM::probability_of_h_given_v(int *sample, double *mean, int *probabilities) {
  for(int h=0; h<count_hidden; h++) {
    mean[h] = propagate_from_visible(sample, weights[h], bias_hidden[h]);
    probabilities[h] = utils::binomial(mean[h]);
  }
}

void RBM::probability_of_v_given_h(int *sample, double *means, int *activations) {
  for(int v=0; v<count_visible; v++) {
    means[v] = propagate_from_hidden(sample, v, bias_visible[v]);
    activations[v] = utils::binomial(means[v]);
  }
}

double RBM::propagate_from_visible(int *visible, double *weight, double bias) {
  double pre_sigmoid_activation = bias;
  for(int v=0; v<count_visible; v++) pre_sigmoid_activation += weight[v] * visible[v];

  return utils::sigmoid(pre_sigmoid_activation);
}

double RBM::propagate_from_hidden(int *hidden, int visible_idx, double bias) {
  double pre_sigmoid_activation = bias;
  for(int h=0; h<count_hidden; h++) pre_sigmoid_activation += weights[h][visible_idx] * hidden[h];

  return utils::sigmoid(pre_sigmoid_activation);
}

void RBM::train(int **input, int input_count, double rate, int epoch_count) {
  double negative_mean_hidden[count_hidden], positive_mean_hidden[count_hidden];
  double negative_mean_visible[count_visible];
  int negative_activation_hidden[count_hidden], positive_activation_hidden[count_hidden];
  int negative_activation_visible[count_visible];

  for (int e=0; e<epoch_count; e++) {
    for (int i=0; i<input_count; i++) {
      probability_of_h_given_v(input[i], positive_mean_hidden, positive_activation_hidden);
      probability_of_v_given_h(positive_activation_hidden, negative_mean_visible, negative_activation_visible);
      probability_of_h_given_v(negative_activation_visible, negative_mean_hidden, negative_activation_hidden);

      for(int h=0; h<count_hidden; h++) {
        for(int v=0; v<count_visible; v++) {
          weights[h][v] += rate * (positive_mean_hidden[h] * input[i][v] - negative_mean_hidden[h] * negative_activation_visible[v]) / input_count;
        }
        bias_hidden[h] += rate * (positive_activation_hidden[h] - negative_mean_hidden[h]) / input_count;
      }

      for(int v=0; v<count_visible; v++)
        bias_visible[v] += rate * (input[i][v] - negative_activation_visible[v]) / input_count;
    }
  }
}

void RBM::run(int *input, double *output) {
  double tmp_hidden[count_hidden];
  double pre_sigmoid_activation;

  for(int h=0; h<count_hidden; h++)
    tmp_hidden[h] = propagate_from_visible(input, weights[h], bias_hidden[h]);

  for(int v=0; v<count_visible; v++) {
    pre_sigmoid_activation = bias_visible[v];
    for(int h=0; h<count_hidden; h++)
      pre_sigmoid_activation += weights[h][v] * tmp_hidden[h];

    output[v] = utils::sigmoid(pre_sigmoid_activation);
  }
}
