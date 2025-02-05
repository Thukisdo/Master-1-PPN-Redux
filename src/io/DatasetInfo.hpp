#pragma once
#include "Image.hpp"
#include "ImageInfo.hpp"
#include <filesystem>
#include <optional>
#include <vector>

/**
 * @brief Contains information about a Dataset, including its size, the label and
 * ImageInfo of each Image,
 */
class DatasetInfo {
public:
  /**
   * @brief Load a DatasetInfo from a path on disk, automatically detecting if the path is a
   * directory or an archive
   * @param dataset_path The path to the dataset
   * @return A DatasetInfo object if the dataset was successfully loaded, std::nullopt otherwise
   */
  static std::optional<DatasetInfo> loadFromPath(const std::filesystem::path& dataset_path);

  /**
   * @brief Load a DatasetInfo from a directory on disk
   * The directory must contain a subdirectory for each label, and each subdirectory must contain a
   * set of images with no subdirectories
   * @param dataset_path The path to the dataset
   * @return A DatasetInfo object if the dataset was successfully loaded, std::nullopt otherwise
   */
  static std::optional<DatasetInfo> loadFromArchive(const std::filesystem::path& dataset_path);

  /**
   * @brief Load a DatasetInfo from an archive on disk
   * The archive must contain a top level directory, which must contain a subdirectory for each
   * label, and each subdirectory must contain a set of images with no subdirectories
   * @param dataset_path  The path to the dataset
   * @return A DatasetInfo object if the dataset was successfully loaded, std::nullopt otherwise
   */
  static std::optional<DatasetInfo> loadFromDirectory(const std::filesystem::path& dataset_path);

  /**
   * @return The root path of the dataset
   */
  std::filesystem::path& getRootPath();

  /**
   * @return The root path of the dataset
   */
  [[nodiscard]] const std::filesystem::path& getRootPath() const;

  /**
   * @return A vector of strings containing the labels of the dataset
   */
  std::vector<std::string>& getLabels();

  /**
   * @return A vector of strings containing the labels of the dataset
   */
  [[nodiscard]] const std::vector<std::string>& getLabels() const;

  /**
   * @return The ImageInfo associated with each image in the dataset
   */
  std::vector<ImageInfo>& getImagesInfo();

  /**
   * @return The ImageInfo associated with each image in the dataset
   */
  [[nodiscard]] const std::vector<ImageInfo>& getImagesInfo() const;

  /**
   * @return True if the dataset is an archive, false otherwise
   */
  [[nodiscard]] bool isArchive() const { return is_archive; }

private:
  /**
   * @brief True if the dataset is an archive, false otherwise
   */
  bool is_archive = false;

  /**
   * @brief The root path of the dataset
   */
  std::filesystem::path dataset_path;

  /**
   * @brief The name of labels of the dataset
   */
  std::vector<std::string> labels;

  /**
   * @brief The ImageInfo associated with each image in the dataset
   */
  std::vector<ImageInfo> images_info;
};
