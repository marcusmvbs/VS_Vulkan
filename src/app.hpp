#pragma once

#include "descriptors.hpp"
#include "device.hpp"
#include "game_object.hpp"
#include "renderer.hpp"
#include "window.hpp"

#include <memory>
#include <vector>

namespace lve {
class First_app {
 public:
  static const int width = 1920;
  static const int height = 1080;

  First_app();
  ~First_app();

  First_app(const First_app &) = delete;
  First_app &operator=(const First_app &) = delete;

  void run();

 private:
  void loadGameObjects();

  LveWindow lveWindow{width, height, "VS Vulkan - Project 1"};
  LveDevice lveDevice{lveWindow};
  LveRenderer lveRenderer{lveWindow, lveDevice};

  // note: order of declarations matters
  std::unique_ptr<LveDescriptorPool> globalPool{};
  LveGameObject::Map gameObjects;
};
} 