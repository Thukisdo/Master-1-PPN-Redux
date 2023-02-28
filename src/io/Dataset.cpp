

#include "Dataset.hpp"
#include <algorithm>
#include <random>
#include <spdlog/spdlog.h>

Dataset::Dataset(const DatasetInfo& info, int begin, int end) {

  if (begin >= info.getImagesInfo().size()) {
    throw std::runtime_error("Dataset: invalid begin index ");
  }

  if (end == -1) { end = info.getImagesInfo().size(); }
  if (end > info.getImagesInfo().size()) {
    throw std::runtime_error("Dataset: invalid end index ");
  }

  if (begin > end) {
    throw std::runtime_error("Dataset: begin index must be smaller than end index ");
  }


  for (int i = begin; i < end; i++) {
    const ImageInfo& curr_info = info.getImagesInfo()[i];
    int curr_id = curr_info.getId();
    image_id.emplace_back(curr_id);
    image_info[curr_id] = &curr_info;
  }
}

std::pair<Dataset, Dataset> Dataset::load_and_split(const DatasetInfo& info, float split_ratio) {
  std::vector<int> image_ids;
  for (const auto& image_info: info.getImagesInfo()) { image_ids.push_back(image_info.getId()); }

  // Shuffle the image ids
  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(image_ids.begin(), image_ids.end(), g);

  // Take the first 10% of the images for the test set
  int split_index = image_ids.size() * split_ratio;
  Dataset test_set(info, 0, split_index);
  Dataset train_set(info, split_index, image_ids.size());

  return {train_set, test_set};
}
