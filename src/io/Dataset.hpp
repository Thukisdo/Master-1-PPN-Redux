

#pragma once
#include "DatasetInfo.hpp"
#include "Image.hpp"
#include <unordered_map>
#include <utility>

/**
 * @brief A dataset is a collection of images and their labels
 * This class is meant to be used in conjunction with the DatasetInfo class, to load a dataset from
 * disk
 */
class Dataset {
public:
  /**
   * @brief Construct an empty dataset
   */
  Dataset() = default;

  /**
   * @brief Load a dataset from a DatasetInfo, optionally loading only a subset of the images
   * @param dataset_info The DatasetInfo associated with the dataset
   * @param begin The starting index of the images to load
   * @param end The starting index of the images to load. If -1, load all the images from begin to
   * the end of the dataset
   */
  explicit Dataset(const DatasetInfo& dataset_info, int begin = 0, int end = -1);

  /**
   * @brief Load a dataset from a DatasetInfo, and split it into two dataset for training and testing
   * @param info The DatasetInfo associated with the dataset
   * @param split_ratio The proportion of the dataset to use for training
   * @return One dataset for training, and one dataset for testing
   */
  static std::pair<Dataset, Dataset> load_and_split(const DatasetInfo& info, float split_ratio);

  /**
   * @brief Count the number of images per label, and ensures all the labels have the same number of
   * images. If not, the dataset is resized to have the same number of images per label
   */
  void enforceEqualLabelDistribution();

  /**
   * @return Returns a vector containing the images in the dataset
   */
  std::vector<Image>& getImages();

  /**
   * @return Returns a vector containing the images in the dataset
   */
  const std::vector<Image>& getImages() const;

  /**
   * @return the size of the dataset
   */
  size_t getSize() const;

private:
  /**
   * @brief contains all the images in the dataset
   */
  std::vector<Image> images;

  /**
   * @brief contains the name of the labels for easy access
   */
  std::vector<std::string> labels_names;

  /**
   * @brief contains the labels for each image as a float
   */
  std::vector<float> labels;

  /**
   * @brief contains the id of each image
   */
  std::vector<int> image_id;
};
