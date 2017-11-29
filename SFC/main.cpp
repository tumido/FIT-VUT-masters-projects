#include <string>
#include <iostream>
#include <signal.h>
#include "rbm.h"
#include "utils.h"

std::string SEPARATOR = std::string(80, '-');

void header(std::string heading) {
  std::cout << std::endl << heading << std::endl << SEPARATOR << std::endl;
}

void run_sample(RBM & rbm) {
  int sample[rbm.getSampleWidth()];
  double output[rbm.getSampleWidth()];

  std::cout << "Please enter the sample (comma separated): ";
  utils::read_array(sample, rbm.getSampleWidth());
  rbm.run(sample, output);
  std::cout << "Sample " << utils::print_array(sample, rbm.getSampleWidth()) << " --> ";
  std::cout << "Result " << utils::print_array(output, rbm.getSampleWidth()) << std::endl;
}

template<class T> void init_value(std::string label, T & data) {
  std::cout << label << " (default = "<< data << "): ";
  utils::read_value(data);
}

void exit_gracefully(int s){
  std::cout << std::endl << "Exiting RBM" << std::endl;
  exit(0);
}


int main() {
  srand(0);
  // Catch CTRL-C to exit program
  struct sigaction sig_int_handler;
  sig_int_handler.sa_handler = exit_gracefully;
  sigemptyset(&sig_int_handler.sa_mask);
  sig_int_handler.sa_flags = 0;
  sigaction(SIGINT, &sig_int_handler, NULL);

  header("Initialization phase");
  double learning_rate = 0.1;
  init_value("Specify learning rate", learning_rate);
  int training_epochs = 10000;
  init_value("Training epochs amount", training_epochs);
  int count_visible = 6, count_hidden = 3;
  init_value("Neurons in visible layer", count_visible);
  init_value("Neurons in hidden layer", count_hidden);

  // construct RBM
  RBM rbm(count_visible, count_hidden);

  header("Intialized Restricted Boltzmann Machine");
  rbm.printState();


  header("Load training data");
  std::string training_file = "samples.csv";
  do
    init_value("File with training data", training_file);
  while (!utils::check_file(training_file));
  std::vector<int *> training_data = utils::read_csv(training_file, count_visible);
  for (auto x: training_data) std::cout << utils::print_array(x, count_visible) <<std::endl;

  header("Trained network (" + std::to_string(training_epochs) + " epochs)");
  rbm.train(training_data, learning_rate, training_epochs);
  rbm.printState();

  header("Test data");

  while (true) run_sample(rbm);

  return 0;
}
