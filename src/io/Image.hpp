

#pragma once
#include <filesystem>
#include <memory>
#include <optional>

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
