

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
   * @param end The starting index of the images to load. If 0, load all the images from begin to
   * @param enforce_equal_distribution If true, the dataset is resized to have the same number of
   * images per label
   * the end of the dataset
   */
  explicit Dataset(const DatasetInfo& dataset_info, unsigned int begin = 0, unsigned int end = 0,
                   bool enforce_equal_distribution = false);

  /**
   * @brief Load a dataset from a DatasetInfo, and split it into two dataset for training and
   * testing
   * @param info The DatasetInfo associated with the dataset
   * @param split_ratio The proportion of the dataset to use for training
   * @param enforce_equal_distribution If true, the dataset is resized to have the same number of
   * images per label
   * @return One dataset for training, and one dataset for testing, in this order
   */
  static std::pair<Dataset, Dataset> load_and_split(const DatasetInfo& info, float split_ratio,
                                                    bool enforce_equal_distribution = false);

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
  std::vector<int> labels;

  /**
   * @brief contains the id of each image
   */
  std::vector<int> image_id;
};
