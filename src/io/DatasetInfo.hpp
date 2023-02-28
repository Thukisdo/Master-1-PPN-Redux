#pragma once
#include <vector>
#include <filesystem>
#include <optional>

class ImageInfo {
public:

  ImageInfo(const std::filesystem::path& image_path, int label_id);
  int getId() const;
  const std::filesystem::path& getPath() const;

private:
  std::filesystem::path image_path;
  int label_id = -1;
};

class DatasetInfo {
public:

  static std::optional<const DatasetInfo> load(const std::filesystem::path& dataset_path);

  std::filesystem::path& getPath();
  const std::filesystem::path& getPath() const;

  std::vector<std::string>& getLabels();
  const std::vector<std::string>& getLabels() const;

  std::vector<ImageInfo>& getImagesInfo();
  const std::vector<ImageInfo>& getImagesInfo() const;

private:
  bool is_archive = false;
  std::filesystem::path dataset_path;
  std::vector<std::string> labels;
  std::vector<ImageInfo> images_info;
};
