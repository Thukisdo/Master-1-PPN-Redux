

#include "DatasetInfo.hpp"
#include <spdlog/spdlog.h>
namespace fs = std::filesystem;

std::optional<DatasetInfo>
DatasetInfo::loadFromDirectory(const std::filesystem::path& dataset_path) {
  spdlog::debug("Loading dataset from directory {}", dataset_path.string());

  DatasetInfo res;
  res.dataset_path = dataset_path;

  int current_id = 0;

  for (const auto& labels_dir: fs::directory_iterator(dataset_path)) {
    if (not fs::is_directory(labels_dir)) {
      spdlog::warn("Ignoring file {} in dataset directory", labels_dir.path().string());
      continue;
    }

    int label_id = res.labels.size();
    res.labels.emplace_back(labels_dir.path().filename().string());

    for (const auto& entry: fs::directory_iterator(labels_dir.path())) {
      if (not fs::is_regular_file(entry)) {
        spdlog::warn("Ignoring file {} in dataset directory", entry.path().string());
        continue;
      }
      res.images_info.emplace_back(entry.path(), current_id++, label_id);
    }
  }

  spdlog::debug("Found {} labels in root dataset directory {}", res.labels.size(),
                dataset_path.string());


  spdlog::debug("Found {} images in root dataset directory {}", res.images_info.size(),
                dataset_path.string());
  return res;
}

std::optional<DatasetInfo> DatasetInfo::loadFromArchive(const std::filesystem::path& dataset_path) {
  spdlog::debug("Loading dataset from archive {}", dataset_path.string());
  throw std::runtime_error("Not implemented yet");
}

std::optional<DatasetInfo> DatasetInfo::loadFromPath(const std::filesystem::path& dataset_path) {

  if (not fs::exists(dataset_path)) {
    spdlog::error("dataset path {} does not exist", dataset_path.string());
    return std::nullopt;
  }


  if (fs::is_directory(dataset_path)) {
    return loadFromDirectory(dataset_path);
  }
  // Else, if the file is an archive, load from archive
  else if (fs::is_regular_file(dataset_path)) {
    return loadFromArchive(dataset_path);
  } else {
    spdlog::error("dataset path {} is not a directory or a zip archive", dataset_path.string());
    return std::nullopt;
  }
}


std::filesystem::path& DatasetInfo::getRootPath() { return dataset_path; }
const std::filesystem::path& DatasetInfo::getRootPath() const { return dataset_path; }

std::vector<std::string>& DatasetInfo::getLabels() { return labels; }

const std::vector<std::string>& DatasetInfo::getLabels() const { return labels; }

std::vector<ImageInfo>& DatasetInfo::getImagesInfo() { return images_info; }

const std::vector<ImageInfo>& DatasetInfo::getImagesInfo() const { return images_info; }
