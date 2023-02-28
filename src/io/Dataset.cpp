

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

  std::pair<std::vector<Image>, std::vector<int>> load_image_in_range(const DatasetInfo& info,
                                                                      int begin, int end) {
    std::vector<Image> image_buffer(end - begin);
    std::vector<int> image_ids(end - begin);

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
      image_ids[i - begin] = curr_info.getUniqueId();
    }


    if (errors > 0)
      spdlog::warn(
              "Dataset: failed to load {} images, see above for the list of images that failed "
              "to load",
              errors);

    return {std::move(image_buffer), std::move(image_ids)};
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


  /**
   * @brief Find the label with the least number of images, and return its size if it is different
   * from the number of images in the other labels
   *
   * @param nlabel The total number of labels in the dataset
   * @param labels A vector containing the label of each image
   * @return 0 if all labels contain the same number of images, otherwise the number of images in
   * the label with the least number of images
   */
  int find_smallest_label_if_different(unsigned int nlabel, const std::vector<int>& labels) {

    std::vector<int> num_images_per_label(nlabel, 0);

    for (int label: labels) { num_images_per_label[static_cast<int>(label)]++; }

    int min_num_images =
            *std::min_element(num_images_per_label.begin(), num_images_per_label.end());

    // No need to enforce the distribution if all the labels have the same number of images
    if (std::all_of(num_images_per_label.begin(), num_images_per_label.end(),
                    [min_num_images](int x) { return x == min_num_images; }))
      return 0;

    return min_num_images;
  }

}// namespace

Dataset::Dataset(const DatasetInfo& info, int begin, int end) {


  if (end == -1) { end = info.getImagesInfo().size(); }

  if (not check_range_is_valid(info, begin, end)) {
    spdlog::error("Dataset: invalid range [{}, {})", begin, end);
    return;
  }

  auto [image_buffer, image_ids] = load_image_in_range(info, begin, end);
  const auto& images_info = info.getImagesInfo();

  // Remove the images that failed to load and create the final dataset
  for (unsigned int i = 0; i < image_buffer.size(); i++) {
    auto& image = image_buffer[i];

    if (image.data() == nullptr) continue;

    int curr_id = image_ids[i];

    images.emplace_back(std::move(image));
    image_id.emplace_back(curr_id);
    labels.emplace_back(images_info[curr_id].getLabelId());
  }

  labels_names = info.getLabels();
}

std::pair<Dataset, Dataset> Dataset::load_and_split(const DatasetInfo& info, float split_ratio) {
  std::vector<int> image_ids;
  for (const auto& image_info: info.getImagesInfo()) {
    image_ids.push_back(image_info.getUniqueId());
  }

  // Shuffle the image ids
  std::shuffle(image_ids.begin(), image_ids.end(), std::mt19937{std::random_device{}()});

  // Take the first 10% of the images for the test set
  int split_index = image_ids.size() * split_ratio;
  Dataset test_set(info, 0, split_index);
  Dataset train_set(info, split_index, image_ids.size());

  resize_to_max_dim(test_set, train_set);

  return {std::move(train_set), std::move(test_set)};
}

int Dataset::enforceEqualLabelDistribution() {
  int target_size = find_smallest_label_if_different(labels_names.size(), labels);

  if (target_size == 0) return 0;

  // Number of images left to pick per label to reach the target size
  std::vector<int> leftover_per_label(labels_names.size(), target_size);

  // Buffers to hold the new dataset
  std::vector<int> new_image_id;
  std::vector<int> new_labels;
  std::vector<Image> new_images;

  // Number of labels that have yet to reach the target size
  unsigned int not_done = leftover_per_label.size();

  // Rebuild the dataset with the same number of images per label
  for (unsigned int i = 0; i < image_id.size(); i++) {
    int label = labels[i];

    // If the image is part of a label that has already reached the target size, we skip it
    if (leftover_per_label[label] <= 0) continue;

    new_image_id.emplace_back(image_id[i]);
    new_labels.emplace_back(label);
    new_images.emplace_back(std::move(images[i]));

    // Check if all the labels have reached the target size
    // If true, just break the loop
    if (leftover_per_label[label] == 1) {
      not_done--;
      if (not_done == 0) break;
    }

    leftover_per_label[label]--;
  }

  // Update the dataset
  image_id = std::move(new_image_id);
  labels = std::move(new_labels);
  images = std::move(new_images);

  return target_size;
}

size_t Dataset::getSize() const { return images.size(); }

std::vector<Image>& Dataset::getImages() { return images; }

const std::vector<Image>& Dataset::getImages() const { return images; }