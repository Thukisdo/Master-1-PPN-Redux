

#pragma once
#include <filesystem>
#include <memory>
#include <optional>

/**
 * @brief A simple image class, used to load images from disk and perform basic operations on them
 */
class Image {
public:
  /**
   * @brief Load an image from disk, optionally specifying the number of channelsy
   * @param path The path to the image
   * @param nchannels The desired number of channels, 0 for automatic detection
   * @return An Image object if the image was successfully loaded, std::nullopt otherwise
   */
  static std::optional<Image> load(const std::filesystem::path& path, int nchannels = 0);

  /**
   * @brief Resize the image inplace
   * @param new_width the target width
   * @param new_height the target height
   */
  void resize(int new_width, int new_height);

  // Defined here for inlining purposes

  /**
   * @return A pointer to the image data
   */
  float* data() { return pixels.get(); }

  /**
   * @return The width of the image. Multiply by getNChannels() to get the number of elements in a row
   */
  [[nodiscard]] int getWidth() const { return width; }

  /**
   * @return The height of the image
   */
  [[nodiscard]] int getHeight() const { return height; }

  /**
   * @return The number of channels in the image
   */
  [[nodiscard]] int getNChannels() const { return channels; }

private:
  std::unique_ptr<float[]> pixels;
  int width, height, channels;
};
