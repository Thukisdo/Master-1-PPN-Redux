

#include "Dataset.hpp"
#include <algorithm>
#include <execution>
#include <random>
#include <spdlog/spdlog.h>

namespace {

  bool check_range_is_valid(const DatasetInfo& info, unsigned int begin, unsigned int end) {
    if (begin >= info.getImagesInfo().size()) return false;

    if (end > info.getImagesInfo().size()) return false;

    if (begin > end) return false;

    return true;
  }

  std::pair<std::vector<Image>, std::vector<int>>
  load_image_in_range(const DatasetInfo& info, unsigned int begin, unsigned int end,
                      int max_per_label, int nlabel) {

    std::vector<int> label_count(nlabel, 0);

    std::vector<Image> image_buffer(end - begin);
    std::vector<int> image_ids(end - begin);

    size_t errors = 0;

    // Load the images in parallel, but store them in a temporary buffer
    // So we can remove the images that failed to load
    for (unsigned int i = begin; i < end; i++) {
      const ImageInfo& curr_info = info.getImagesInfo()[i];

      if (max_per_label > 0 and label_count[curr_info.getLabelId()] >= max_per_label) { continue; }

      auto loaded_image = Image::load(curr_info.getPath(), 1);

      if (not loaded_image) {
        spdlog::warn("Dataset: failed to load image {}", curr_info.getPath().string());
        errors++;
      }

      image_buffer[i - begin] = std::move(*loaded_image);
      image_ids[i - begin] = curr_info.getUniqueId();

      // Atomically increase the counter for the current label
      label_count[curr_info.getLabelId()]++;
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
  int find_smallest_label_if_different(unsigned int nlabel, const std::vector<ImageInfo>& infos) {

    std::vector<int> num_images_per_label(nlabel, 0);

    for (const auto& img: infos) { num_images_per_label[img.getLabelId()]++; }

    int min_num_images =
            *std::min_element(num_images_per_label.begin(), num_images_per_label.end());

    // No need to enforce the distribution if all the labels have the same number of images
    if (std::all_of(num_images_per_label.begin(), num_images_per_label.end(),
                    [min_num_images](int x) { return x == min_num_images; }))
      return 0;

    return min_num_images;
  }

}// namespace

Dataset::Dataset(const DatasetInfo& info, unsigned int begin, unsigned int end,
                 bool enforce_equal_distribution) {


  if (end == 0) { end = info.getImagesInfo().size(); }

  if (not check_range_is_valid(info, begin, end)) {
    spdlog::error("Dataset: invalid range [{}, {})", begin, end);
    return;
  }

  int max_per_label = 0;

  if (enforce_equal_distribution) {
    max_per_label = find_smallest_label_if_different(info.getLabels().size(), info.getImagesInfo());
  }

  auto [image_buffer, image_ids] =
          load_image_in_range(info, begin, end, max_per_label, info.getLabels().size());
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

std::pair<Dataset, Dataset> Dataset::load_and_split(const DatasetInfo& info, float split_ratio,
                                                    bool enforce_equal_distribution) {
  std::vector<int> image_ids;
  for (const auto& image_info: info.getImagesInfo()) {
    image_ids.push_back(image_info.getUniqueId());
  }

  // Shuffle the image ids
  std::shuffle(image_ids.begin(), image_ids.end(), std::mt19937{std::random_device{}()});

  // Take the first 10% of the images for the test set
  unsigned int split_index = (int) ((float) image_ids.size() * split_ratio);

  Dataset test_set(info, 0, split_index);
  Dataset train_set(info, split_index, image_ids.size(), enforce_equal_distribution);

  resize_to_max_dim(test_set, train_set);

  return {std::move(train_set), std::move(test_set)};
}

size_t Dataset::getSize() const { return images.size(); }

std::vector<Image>& Dataset::getImages() { return images; }

const std::vector<Image>& Dataset::getImages() const { return images; }