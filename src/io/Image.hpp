

#pragma once
#include <filesystem>
#include <memory>
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


class Image {
public:
  static std::optional<Image> load(const std::filesystem::path& filename, int nchannels = 0);

  void resize(int new_width, int new_height);

  // Defined here for inlining purposes
  float* data() { return pixels.get(); }
  int getWidth() const { return width; }
  int getHeight() const { return height; }
  int getNChannels() const { return channels; }

private:
  std::unique_ptr<float[]> pixels;
  int width, height, channels;
};
