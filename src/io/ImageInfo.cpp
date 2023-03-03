

#include "ImageInfo.hpp"

ImageInfo::ImageInfo(const std::filesystem::path& image_path, int unique_id, int label_id)
    : image_path(image_path), unique_id(unique_id), label_id(label_id) {}

int ImageInfo::getLabelId() const { return label_id; }

int ImageInfo::getUniqueId() const { return unique_id; }

const std::filesystem::path& ImageInfo::getPath() const { return image_path; }
