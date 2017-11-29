#include <string>
#include <iostream>
#include <signal.h>
#include <time.h>
#include "rbm.h"
#include "utils.h"

/**
 * Provide network response for user input
 * @param rbm [description]
 */
void run_sample(RBM & rbm) {
  int sample[rbm.getSampleWidth()];
  double output[rbm.getSampleWidth()];

  // fetch user input
  std::cout << "Please enter the sample (comma separated): ";
  utils::read_array(sample, rbm.getSampleWidth());

  // run network and show response
  rbm.run(sample, output);
  std::cout << "Sample " << utils::print_array(sample, rbm.getSampleWidth()) << " --> ";
  std::cout << "Result " << utils::print_array(output, rbm.getSampleWidth()) << std::endl;
}

/**
 * Exit program message
 */
void exit_gracefully(int s){
  std::cout << std::endl << "Exiting RBM" << std::endl;
  exit(0);
}

/**
 * Setup the CLI workflow for RBM
 */
int main() {
  // initialize random module
  srand(time(NULL));
  // catch CTRL-C to exit program
  struct sigaction sig_int_handler;
  sig_int_handler.sa_handler = exit_gracefully;
  sigemptyset(&sig_int_handler.sa_mask);
  sig_int_handler.sa_flags = 0;
  sigaction(SIGINT, &sig_int_handler, NULL);

  // init RBM settings
  utils::header("Initialization phase");
  double learning_rate = 0.1;
  utils::init_value("Specify learning rate", learning_rate);
  int training_epochs = 10000;
  utils::init_value("Training epochs amount", training_epochs);
  int count_visible = 6, count_hidden = 3;
  utils::init_value("Neurons in visible layer", count_visible);
  utils::init_value("Neurons in hidden layer", count_hidden);

  // construct RBM
  RBM rbm(count_visible, count_hidden);
  utils::header("Intialized Restricted Boltzmann Machine");
  rbm.printState();

  // fetch training data
  utils::header("Load training data");
  std::string training_file = "samples.csv";
  do
    utils::init_value("File with training data", training_file);
  while (!utils::check_file(training_file));
  std::vector<int *> training_data = utils::read_csv(training_file, count_visible);
  for (auto x: training_data) std::cout << utils::print_array(x, count_visible) <<std::endl;

  // training of the network
  utils::header("Trained network (" + std::to_string(training_epochs) + " epochs)");
  rbm.train(training_data, learning_rate, training_epochs);
  rbm.printState();

  // let user input its data
  utils::header("Test data");
  while (true) run_sample(rbm);

  return 0;
}
