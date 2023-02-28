

#pragma once
#include "DatasetInfo.hpp"
#include <utility>
#include <unordered_map>

class Dataset {
public:
  Dataset() = default;
  Dataset(const DatasetInfo& dataset_info, int begin = 0, int end = -1);
  static std::pair<Dataset, Dataset> load_and_split(const DatasetInfo& info, float split_ratio);

private:
  std::vector<float*> data;
  std::vector<float> labels;
  std::vector<int> image_id;
  std::unordered_map<int, const ImageInfo*> image_info;
};
