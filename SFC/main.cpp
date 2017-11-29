#include "rbm.h"

int main() {
  //
  srand(0);
  //
  // double learning_rate = 0.1;
  // int training_epochs = 1000;
  // int k = 1;
  //
  // int train_N = 6;
  // int test_N = 2;
  // int n_visible = 6;
  // int n_hidden = 3;
  //
  // training data
  // int training_data[6][6] = {
  //   {1, 1, 1, 0, 0, 0},
  //   {1, 0, 1, 0, 0, 0},
  //   {1, 1, 1, 0, 0, 0},
  //   {0, 0, 1, 1, 1, 0},
  //   {0, 0, 1, 0, 1, 0},
  //   {0, 0, 1, 1, 1, 0}
  // };


  // construct RBM
  RBM rbm(6, 3);
  rbm.printState();

  // train
  // for(int epoch=0; epoch<training_epochs; epoch++) {
  //   for(int i=0; i<train_N; i++) {
  //     rbm.contrastive_divergence(train_X[i], learning_rate, k);
  //   }
  // }
  //
  // // test data
  // int test_X[2][6] = {
  //   {1, 1, 0, 0, 0, 0},
  //   {0, 0, 0, 1, 1, 0}
  // };
  // double reconstructed_X[2][6];
  //
  //
  // // test
  // for(int i=0; i<test_N; i++) {
  //   rbm.reconstruct(test_X[i], reconstructed_X[i]);
  //   for(int j=0; j<n_visible; j++) {
  //     printf("%.5f ", reconstructed_X[i][j]);
  //   }
  //   cout << endl;
  // }

  return 0;
}
