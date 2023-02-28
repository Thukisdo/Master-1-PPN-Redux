

#include "Dataset.hpp"
#include <algorithm>
#include <execution>
#include <random>
#include <spdlog/spdlog.h>

namespace {

  bool check_range_is_valid(const DatasetInfo& info, int begin, int end) {
    if (begin >= info.getImagesInfo().size()) return false;

    if (end > info.getImagesInfo().size()) return false;

    if (begin > end) return false;

    return true;
  }

  std::vector<Image> load_image_in_range(const DatasetInfo& info, int begin, int end) {
    std::vector<Image> image_buffer(end - begin);

    // TODO: Add support for loading image from an archive
    size_t errors = 0;

    // Load the images in parallel, but store them in a temporary buffer
// So we can remove the images that failed to load
#pragma omp parallel for schedule(dynamic) reduction(+ : errors)
    for (int i = begin; i < end; i++) {
      const ImageInfo& curr_info = info.getImagesInfo()[i];
      auto loaded_image = Image::load(curr_info.getPath(), 1);

      if (not loaded_image) {
        spdlog::warn("Dataset: failed to load image {}", curr_info.getPath().string());
        errors++;
      }

      image_buffer[i - begin] = std::move(*loaded_image);
    }


    if (errors > 0)
      spdlog::warn(
              "Dataset: failed to load {} images, see above for the list of images that failed "
              "to load",
              errors);
    return image_buffer;
  }


  std::pair<int, int> find_max_size(const Dataset& set) {

    int max_w = 0, max_h = 0;

    for (const auto& image: set.getImages()) {
      max_w = std::max(max_w, image.getWidth());
      max_h = std::max(max_h, image.getHeight());
    }
    return {max_w, max_h};
  }

  void resize_to_max_dim(Dataset& test_set,
                         Dataset& train_set) {// Ensure all the images have the same size
    auto [max_width_test, max_height_test] = find_max_size(test_set);
    auto [max_width_train, max_height_train] = find_max_size(train_set);

    int max_width = std::max(max_width_test, max_width_train);
    int max_height = std::max(max_height_test, max_height_train);

    auto resize_lambda = [max_width, max_height](Image& image) {
      image.resize(max_width, max_height);
    };

    std::for_each(std::execution::par_unseq, test_set.getImages().begin(),
                  test_set.getImages().end(), resize_lambda);
    std::for_each(std::execution::par_unseq, train_set.getImages().begin(),
                  train_set.getImages().end(), resize_lambda);
  }

}// namespace

Dataset::Dataset(const DatasetInfo& info, int begin, int end) {

  labels_names = info.getLabels();

  if (end == -1) { end = info.getImagesInfo().size(); }

  if (not check_range_is_valid(info, begin, end)) {
    spdlog::error("Dataset: invalid range [{}, {})", begin, end);
    return;
  }

  std::vector<Image> image_buffer = load_image_in_range(info, begin, end);

  // Remove the images that failed to load and create the final dataset
  for (int i = 0; i < image_buffer.size(); i++) {
    auto& image = image_buffer[i];

    if (image.data() == nullptr) continue;

    const auto& curr_info = info.getImagesInfo()[i];
    int curr_id = info.getImagesInfo()[i].getId();

    images.emplace_back(std::move(image));
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

  resize_to_max_dim(test_set, train_set);

  return {std::move(test_set), std::move(train_set)};
}

void Dataset::enforceEqualLabelDistribution() {
  std::vector<int> num_images_per_label(labels_names.size(), 0);

  for (int i = 0; i < image_id.size(); i++) {
    int label = labels[i];
    num_images_per_label[label]++;
  }

  int min_num_images = *std::min_element(num_images_per_label.begin(), num_images_per_label.end());

  std::vector<int> new_image_id;
  std::vector<float> new_labels;
  std::vector<Image> new_images;

  num_images_per_label = std::vector<int>(labels_names.size(), min_num_images);

  if (min_num_images == 0) return;

  // No need to enforce the distribution if all the labels have the same number of images
  if (std::all_of(num_images_per_label.begin(), num_images_per_label.end(),
                  [min_num_images](int x) { return x == min_num_images; }))
    return;

  // Rebuild the dataset with the same number of images per label
  for (int i = 0; i < image_id.size(); i++) {
    int label = labels[i];

    if (num_images_per_label[label] <= 0) continue;

    new_image_id.emplace_back(image_id[i]);
    new_labels.emplace_back(label);
    new_images.emplace_back(std::move(images[i]));
    num_images_per_label[label]--;
  }

  image_id = std::move(new_image_id);
  labels = std::move(new_labels);
  images = std::move(new_images);
}

size_t Dataset::getSize() const { return images.size(); }


std::vector<Image>& Dataset::getImages() { return images; }

const std::vector<Image>& Dataset::getImages() const { return images; }