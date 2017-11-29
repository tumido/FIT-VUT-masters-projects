#include "rbm.h"

const void RBM::printState() {
  std::cout << std::fixed << std::setprecision(4);

  std::cout << "Bias of visible nodes (" << count_visible << "):\t";
  std::cout << utils::print_array(bias_visible, count_visible) << std::endl;
  std::cout << "Bias of hidden nodes (" << count_hidden << "):\t";
  std::cout << utils::print_array(bias_hidden, count_hidden) << std::endl;


  std::cout << "Weights:" << std::endl << std::setw(12) << std::setfill(' ') << "|";
  for(size_t v=0; v<count_visible; v++) std::cout << std::setw(7) << std::setfill(' ') << "#" << v;
  std::cout << std::endl << std::string(12+count_visible*8, '-') << std::endl;
  for(size_t h=0; h<count_hidden; h++) {
    std::cout << std::setw(9) << std::setfill(' ') << "Hidden #" << h << " |";
    for(size_t v=0; v<count_visible; v++) std::cout << std::setw(8) << std::setfill(' ') << weights[h][v];
    std::cout << std::endl;
  }
}

/**
 * Restricted Boltzmann Machine constructor
 * @param count_visible Amount of neurons in visible layer
 * @param count_hidden  Amount of neurons in hidden layer
 */
RBM::RBM(size_t count_visible, size_t count_hidden) : count_visible(count_visible), count_hidden(count_hidden) {
  weights = new double * [count_visible];

  double around = 1.0 / count_visible;

  // initialize weight for each neuron connection to a random number
  for(size_t h=0; h<count_hidden; h++) {
    weights[h] = new double[count_visible];
    for(size_t v=0; v<count_visible; v++) {
      weights[h][v] = utils::uniform(around);
    }
  }

  // start with neutral response
  bias_hidden  = new double[count_hidden];
  bias_visible = new double[count_visible];

  for(size_t h=0; h<count_hidden; h++)  bias_hidden[h] = 0;
  for(size_t v=0; v<count_visible; v++) bias_visible[v] = 0;
}

/**
 * RBM destructor
 */
RBM::~RBM() {
  for(size_t i=0; i<count_hidden; i++) delete[] weights[i];
  delete[] weights;
  delete[] bias_visible;
  delete[] bias_hidden;
}

/**
 * Compute probabilities for hidden layer of network
 * @param sample        Array of activation status for visible layer/test sample
 * @param mean          Array of probabilities for hidden layer
 * @param probabilities Array describing which neurons are activated
 */
void RBM::probability_of_h_given_v(int *sample, double *mean, int *probabilities) {
  for(size_t h=0; h<count_hidden; h++) {
    mean[h] = propagate_from_visible(sample, weights[h], bias_hidden[h]);
    probabilities[h] = utils::binomial(mean[h]);
  }
}

/**
 * Compute probabilities for visible layer of network
 * @param sample      Array of activation status for hidden layer/test sample
 * @param means       Array of probabilities for visible layer
 * @param activations Array describing which neurons are activated
 */
void RBM::probability_of_v_given_h(int *sample, double *means, int *activations) {
  for(size_t v=0; v<count_visible; v++) {
    means[v] = propagate_from_hidden(sample, v, bias_visible[v]);
    activations[v] = utils::binomial(means[v]);
  }
}

/**
 * Propagate activation of visible units to a hidden neuron
 *
 * @param  visible Array of all neurons in visible layer
 * @param  weight  Array of weights for connection between hidden and visible layer
 * @param  bias    The bias value of the selected neuron
 * @return         Probability of activation for the selected neuron
 */
double RBM::propagate_from_visible(int *visible, double *weight, double bias) {
  double pre_sigmoid_activation = bias;
  for(size_t v=0; v<count_visible; v++) pre_sigmoid_activation += weight[v] * visible[v];

  return utils::sigmoid(pre_sigmoid_activation);
}

/**
 * Propagate activation from the hidden units to a visible neuron
 * @param  hidden      Array of all neurons in the hidden layer
 * @param  visible_idx Index on the selected neuron in visible layer
 * @param  bias        Current bias value of the selected neuron
 * @return             Probability of activation for the selected neuron
 */
double RBM::propagate_from_hidden(int *hidden, int visible_idx, double bias) {
  double pre_sigmoid_activation = bias;
  for(size_t h=0; h<count_hidden; h++) pre_sigmoid_activation += weights[h][visible_idx] * hidden[h];

  return utils::sigmoid(pre_sigmoid_activation);
}

/**
 * Train the network
 * @param input       Vector of training samples
 * @param input_count [description]
 * @param rate        Desired learning rate
 * @param epoch_count Amount of training epochs
 */
void RBM::train(std::vector<int *>input, double rate, int epoch_count) {
  double negative_mean_hidden[count_hidden], positive_mean_hidden[count_hidden];
  double negative_mean_visible[count_visible];
  int negative_activation_hidden[count_hidden], positive_activation_hidden[count_hidden];
  int negative_activation_visible[count_visible];

  // in each epoch train on each sample
  for (int e=0; e<epoch_count; e++) {
    for (size_t i=0; i<input.size(); i++) {
      // first populate the hidden layer from the test sample as it would be the visible layer
      probability_of_h_given_v(input[i], positive_mean_hidden, positive_activation_hidden);
      // then populate visible layer from the hidden
      probability_of_v_given_h(positive_activation_hidden, negative_mean_visible, negative_activation_visible);
      // now based on active neurons in the visible layer calculate activation for hidden layer again
      probability_of_h_given_v(negative_activation_visible, negative_mean_hidden, negative_activation_hidden);

      for(size_t h=0; h<count_hidden; h++) {
        for(size_t v=0; v<count_visible; v++) {
          // for each neuron connection adjust weight
          weights[h][v] += rate * (positive_mean_hidden[h] * input[i][v] - negative_mean_hidden[h] * negative_activation_visible[v]) / input.size();
        }
        // for each neuron in the hidden layer adjust bias
        bias_hidden[h] += rate * (positive_activation_hidden[h] - negative_mean_hidden[h]) / input.size();
      }

      // for each neuron in visible layer adjust bias as well
      for(size_t v=0; v<count_visible; v++)
        bias_visible[v] += rate * (input[i][v] - negative_activation_visible[v]) / input.size();
    }
  }
}

/**
 * Run the network for input data
 * @param input  Array of input data
 * @param output Calculated array containing the network response
 */
void RBM::run(int *input, double *output) {
  double tmp_hidden[count_hidden];
  double pre_sigmoid_activation;

  // propagate input sample to visible layer (as it would be an activation of visible layer)
  for(size_t h=0; h<count_hidden; h++)
    tmp_hidden[h] = propagate_from_visible(input, weights[h], bias_hidden[h]);

  // compute network response probabilities
  for(size_t v=0; v<count_visible; v++) {
    pre_sigmoid_activation = bias_visible[v];
    for(size_t h=0; h<count_hidden; h++)
      pre_sigmoid_activation += weights[h][v] * tmp_hidden[h];

    output[v] = utils::sigmoid(pre_sigmoid_activation);
  }
}

/**
 * Return size of input array (how may members network accepts)
 */
int RBM::getSampleWidth() {
  return count_visible;
}
