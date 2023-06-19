#pragma once

#include "camera.hpp"
#include "device.hpp"
#include "frame_info.hpp"
#include "game_object.hpp"
#include "pipeline.hpp"

// std
#include <memory>
#include <vector>

namespace lve {
class PointLightSystem {
 public:
  PointLightSystem(
      LveDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
  ~PointLightSystem();

  PointLightSystem(const PointLightSystem &) = delete;
  PointLightSystem &operator=(const PointLightSystem &) = delete;

  void render(FrameInfo &frameInfo);

 private:
  void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
  void createPipeline(VkRenderPass renderPass);

  LveDevice &lveDevice;

  std::unique_ptr<LvePipeline> lvePipeline;
  VkPipelineLayout pipelineLayout;
};
}