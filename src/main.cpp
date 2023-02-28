#include <filesystem>
#include <iostream>
#include <spdlog/spdlog.h>
#include "DatasetInfo.hpp"
#include "Dataset.hpp"

// C headers

extern "C" {
  #include "context.h"
  // #include "dataset_manager.h"
  #include "neural_network.h"
  #include "store.h"
  // #include "training.h"
  #include "type.h"
}

#define NB_MAX_LAYER 50
#define NB_MAX_OUTPUTS 50


namespace fs = std::filesystem;


std::string get_timestamp() {
  auto now = std::chrono::system_clock::now();
  auto in_time_t = std::chrono::system_clock::to_time_t(now);
  std::stringstream ss;
  ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d_%H_%M_%S");
  return ss.str();
}


int main(int argc, char* argv[]) {

  spdlog::set_level(spdlog::level::debug);

  spdlog::info("Neural network trainer Version TBD");

  if (argc < 2) {
    spdlog::error("usage {} <config_file> (result_directory)", argv[0]);
    return 1;
  }

  fs::path config_path(argv[1]);

  if (not fs::exists(config_path)) {
    spdlog::error("config file {} does not exist", config_path.string());
    return 1;
  }

  fs::path result_path;
  if (argc > 2) {
    result_path = argv[2];
  } else {
    result_path = config_path.parent_path() / "results";
  }

  // TODO: Refactor the context into an experiment configuration
  Context context;
  load_context(&context, config_path.string().c_str());
  info_context(&context);

  // Placeholder
  auto dataset_info = DatasetInfo::loadFromPath("../../dataset");
  auto [training_set, testing_set] = Dataset::load_and_split(*dataset_info, 0.1);

  spdlog::info("Training set size: {}", training_set.getSize());
  spdlog::info("Testing set size: {}", testing_set.getSize());

  training_set.enforceEqualLabelDistributionn();

  exit(1);
  /*

  srand(time(NULL));

  // char * train_dirs[] = { "../dataset/train/NonDemented", "../dataset/train/ModerateDemented"};
  // char * test_dirs[]  = { "../dataset/test/NonDemented" , "../dataset/test/ModerateDemented"};

  FILE* fpTrain = fopen(context.train_dat_path, "w");
  FILE* fpTest = fopen(context.test_dat_path, "w");

  int num_folder = 2;
  int max_per_folder = context.max_per_folder;

  // u64 nb_layers = 5;
  // u64 neurons_per_layers[NB_MAX_LAYER] = {480,200,50,10,1,1};

  // u64 nb_layers = 7;
  // u64 neurons_per_layers[NB_MAX_LAYER] = {480,64,64,64,64,64,1,1};
  // u64 epoch_max = 100;


  // number of images
  // for( int i = 0; i < 2; i++ ){
  //     int num = count_file(trainDirs[i]);
  //     printf(" file in %s :  %d\n", trainDirs[i],num);
  // }

  Dataset train_dataset;
  Dataset test_dataset;

  // TODO la taille du dataset est mise au max theorique d'element et non au nombre reel d'element
  train_dataset.size = num_folder * max_per_folder;
  test_dataset.size = num_folder * max_per_folder;
  train_dataset.images = (mri_image*) malloc(train_dataset.size * sizeof(mri_image));
  test_dataset.images = (mri_image*) malloc(test_dataset.size * sizeof(mri_image));

  // u64 * random_pattern = malloc( num_folder * max_per_folder * sizeof(u64));

  // fill dataset
  u64 train_dataset_size =
          (u64) load_dataset(context.train_dirs, num_folder, &train_dataset, max_per_folder);
  u64 test_dataset_size =
          (u64) load_dataset(context.test_dirs, num_folder, &test_dataset, max_per_folder);

  printf("Dataset filled with %llu images\n", train_dataset_size);
  printf("Test dataset filled with %llu images\n", test_dataset_size);


  //  Initialise The NN
  Layer** neural_network = init_neural_network(context.topology, context.nn_size);


  train(&context, &train_dataset, &test_dataset, neural_network, fpTest, fpTrain);

  fclose(fpTrain);
  fclose(fpTest);
  store_neural_network(&context, neural_network);

  free_neural_network(neural_network, context.nn_size);
  free_dataset(&train_dataset);
  free_dataset(&test_dataset);
  free_context(&context);
   */
}
