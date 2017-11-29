#include "rbm.h"
#include "utils.h"

std::string SEPARATOR = std::string(80, '-');

void header(std::string heading) {
  std::cout << std::endl << heading << std::endl << SEPARATOR << std::endl;
}

int main() {
  srand(0);
  std::string separator = std::string(80, '-') + "\n";
  int count_visible, count_hidden;
  double learning_rate = 0.1;
  int training_epochs = 10000;

  header("Initialization phase");
  // construct RBM
  RBM rbm(6, 3);
  rbm.printState();

  // training data
  int training_samples = 6;
  int *training_data[6];
  training_data[0] = new int[6]{1, 1, 1, 0, 0, 0};
  training_data[1] = new int[6]{1, 0, 1, 0, 0, 0};
  training_data[2] = new int[6]{1, 1, 1, 0, 0, 0};
  training_data[3] = new int[6]{0, 0, 1, 1, 1, 0};
  training_data[4] = new int[6]{0, 0, 1, 0, 1, 0};
  training_data[5] = new int[6]{0, 0, 1, 1, 1, 0};

  header("Training phase (" + std::to_string(training_epochs) + " epochs)");
  rbm.train(training_data, training_samples, learning_rate, training_epochs);
  rbm.printState();

  header("Test data");
  // test data
  int test[2][6] = {
    {1, 1, 0, 0, 0, 0},
    {0, 0, 0, 1, 1, 0}
  };
  double output[2][6];

  // test
  for(int i=0; i<2; i++) {
    rbm.run(test[i], output[i]);
    std::cout << "Sample " << utils::print_array(test[i], 6) << " --> ";
    std::cout << utils::print_array(output[i], 6) << std::endl;
  }

  return 0;
}
