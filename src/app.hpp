#pragma once

#include "device.hpp"
#include "game_object.hpp"
#include "renderer.hpp"
#include "window.hpp"

#include <memory>
#include <vector>

namespace lve {
class First_app {
 public:
  static const int width = 800;
  static const int height = 600;

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
  std::vector<LveGameObject> gameObjects;
};
} 