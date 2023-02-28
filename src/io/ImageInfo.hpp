

#pragma once
#include <filesystem>

/**
 * @brief An image's metadata in the context of its dataset, including its path, unique id and label
 * id
 */
class ImageInfo {
public:
  /**
   * @param image_path The path to the image, relative to the dataset root
   * @param unique_id The unique id of the image inside the dataset
   * @param label_id The id of the label of the image
   */
  ImageInfo(const std::filesystem::path& image_path, int unique_id, int label_id);

  /**
   * @return The id of the label of this image
   */
  [[nodiscard]] int getLabelId() const;

  /**
   * @return The unique id of this image inside the dataset
   */
  [[nodiscard]] int getUniqueId() const;

  /**
   * @return The path to this image, relative to the dataset root
   */
  [[nodiscard]] const std::filesystem::path& getPath() const;

private:
  std::filesystem::path image_path;
  int unique_id = -1;
  int label_id = -1;
};
