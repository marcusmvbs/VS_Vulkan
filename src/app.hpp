#pragma once

#include "game_object.hpp"
#include "swap_chain.hpp"
#include "pipeline.hpp"
#include "device.hpp"
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
  void createPipelineLayout();
  void createPipeline();
  void createCommandBuffers();
  void freeCommandBuffers();
  void drawFrame();
  void recreateSwapChain();
  void recordCommandBuffer(int imageIndex);
  void renderGameObjects(VkCommandBuffer commandBuffer);

  LveWindow lveWindow{width, height, "VS Vulkan - Project 1"};
  LveDevice lveDevice{lveWindow};
  std::unique_ptr<LveSwapChain> lveSwapChain;
  std::unique_ptr<LvePipeline> PPipeline; //Smart Pointer (manage memory by itself | new, delete)
  VkPipelineLayout pipelineLayout;
  std::vector<VkCommandBuffer> commandBuffers;
  std::vector<LveGameObject> gameObjects;
};
} 