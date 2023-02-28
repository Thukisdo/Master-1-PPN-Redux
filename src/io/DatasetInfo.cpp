

#include "DatasetInfo.hpp"
#include <spdlog/spdlog.h>
namespace fs = std::filesystem;

namespace {
  void load_from_directory(DatasetInfo& dataset_info) {
    spdlog::debug("Loading dataset from directory {}", dataset_info.getPath().string());

    std::vector<fs::path> labels_dir;
    std::vector<ImageInfo> images_info;

    // Iterate over the directories in the dataset directory
    for (const auto& entry: fs::directory_iterator(dataset_info.getPath())) {
      if (not fs::is_directory(entry)) {
        spdlog::warn("Ignoring file {} in dataset directory", entry.path().string());
        continue;
      }

      // Add the label to the list of labels
      labels_dir.emplace_back(entry.path());
    }

    spdlog::debug("Found {} labels in root dataset directory {}", labels_dir.size(),
                  dataset_info.getPath().string());

    // Load all the labelled data
    for (const auto& label_dir: labels_dir) {
      int label_id = dataset_info.getLabels().size();
      dataset_info.getLabels().emplace_back(label_dir.filename().string());

      for (const auto& entry: fs::directory_iterator(label_dir)) {
        if (not fs::is_regular_file(entry)) {
          spdlog::warn("Ignoring file {} in dataset directory", entry.path().string());
          continue;
        }

        images_info.emplace_back(entry.path(), label_id);
      }
    }

    spdlog::debug("Found {} images in root dataset directory {}", images_info.size(),
                  dataset_info.getPath().string());
  }

  void load_from_archive(DatasetInfo& dataset_info) {
    spdlog::debug("Loading dataset from archive {}", dataset_info.getPath().string());
    throw std::runtime_error("Not implemented yet");
  }
}// namespace

ImageInfo::ImageInfo(const std::filesystem::path& image_path, int label_id)
    : image_path(image_path), label_id(label_id) {}

int ImageInfo::getId() const { return label_id; }

const std::filesystem::path& ImageInfo::getPath() const { return image_path; }


std::optional<const DatasetInfo> DatasetInfo::load(const std::filesystem::path& dataset_path) {

  if (not fs::exists(dataset_path)) {
    spdlog::error("dataset path {} does not exist", dataset_path.string());
    return std::nullopt;
  }

  DatasetInfo res;
  res.dataset_path = dataset_path;

  if (fs::is_directory(dataset_path)) {
    load_from_directory(res);
  }
  // Else, if the file is an archive, load from archive
  else if (fs::is_regular_file(dataset_path)) {
    load_from_archive(res);
  } else {
    spdlog::error("dataset path {} is not a directory or a zip archive", dataset_path.string());
    return std::nullopt;
  }
  return res;
}

std::filesystem::path& DatasetInfo::getPath() { return dataset_path; }
const std::filesystem::path& DatasetInfo::getPath() const { return dataset_path; }

std::vector<std::string>& DatasetInfo::getLabels() { return labels; }

const std::vector<std::string>& DatasetInfo::getLabels() const { return labels; }

std::vector<ImageInfo>& DatasetInfo::getImagesInfo() { return images_info; }

const std::vector<ImageInfo>& DatasetInfo::getImagesInfo() const { return images_info; }
