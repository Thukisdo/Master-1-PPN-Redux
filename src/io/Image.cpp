

#include "Image.hpp"
#include <stb_image.h>
#include <stb_image_resize.h>


ImageInfo::ImageInfo(const std::filesystem::path& image_path, int label_id)
    : image_path(image_path), label_id(label_id) {}

int ImageInfo::getId() const { return label_id; }

const std::filesystem::path& ImageInfo::getPath() const { return image_path; }


std::optional<Image> Image::load(const std::filesystem::path& filename, int nchannels) {
  int width, height, channels;
  float* pixels = stbi_loadf(filename.string().c_str(), &width, &height, &channels, nchannels);

  if (pixels == nullptr) { return std::nullopt; }

  if (channels != nchannels and nchannels != 0) {
    stbi_image_free(pixels);
    return std::nullopt;
  }

  Image res;
  res.pixels = std::unique_ptr<float[]>(pixels);
  res.width = width;
  res.height = height;
  res.channels = nchannels;

  return res;
}

void Image::resize(int new_width, int new_height) {

  if (new_width == this->width and new_height == this->height) { return; }

  std::unique_ptr<float[]> new_pixels =
          std::make_unique<float[]>(new_width * new_height * channels);
  int err = stbir_resize_float(this->pixels.get(), this->width, this->height, 0, new_pixels.get(),
                               new_width, new_height, 0, channels);

  if (err == 0) { throw std::runtime_error("Error in stb while resizing image"); }

  this->pixels = std::move(new_pixels);
  this->width = new_width;
  this->height = new_height;
}
