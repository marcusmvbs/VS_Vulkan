#include "app.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "/opt/homebrew/include/glm/glm.hpp"
#include "/opt/homebrew/include/glm/gtc/constants.hpp"

// std
#include <array>
#include <cassert>
#include <chrono>
#include <stdexcept>

namespace lve {

struct SimplePushConstantData {
  glm::mat2 transform{1.f};
  glm::vec2 offset; //Pattern 4 + 4 bytes per vector
  alignas(16) glm::vec3 color; //16 bytes of size, to avoid issues on blue vector
};

First_app::First_app() {
  loadGameObjects();
  createPipelineLayout();
  recreateSwapChain();
  createCommandBuffers();
  drawFrame();
}

First_app::~First_app() {
  vkDestroyPipelineLayout(lveDevice.device(), pipelineLayout, nullptr);
}

void First_app::run() {

  while (!lveWindow.shouldClose()) {
    glfwPollEvents();
    drawFrame();
  }
  vkDeviceWaitIdle(lveDevice.device());
}

void First_app::loadGameObjects() {
  std::vector<LveModel::Vertex> vertices{
    {{0.0f, -0.5f}, {1.0f, 0.0f,  0.0f}}, 
    {{0.5f, 0.5f}, {0.0f, 1.0f,  0.0f}},
    {{-0.5f, 0.5f}, {0.0f, 0.0f,  1.0f}}};
  auto lveModel = std::make_shared<LveModel>(lveDevice, vertices);

  auto triangle = LveGameObject::createGameObject();
  triangle.model = lveModel;
  triangle.color = {.9f, .9f, .1f};
  triangle.transform2d.translation.x = .2f;
  triangle.transform2d.scale = {2.f, .5f};
  triangle.transform2d.rotation = .25f * glm::two_pi<float>();

  gameObjects.push_back(std::move(triangle));
}

void First_app::createPipelineLayout() {
    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(SimplePushConstantData);

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
    
    if (vkCreatePipelineLayout(lveDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout.");
    }
}

void First_app::recreateSwapChain() {
  auto extent = lveWindow.getExtent();
  while (extent.width == 0 || extent.height == 0) {
    extent = lveWindow.getExtent();
    glfwWaitEvents();
  }
  vkDeviceWaitIdle(lveDevice.device());

  if (lveSwapChain == nullptr) {
    lveSwapChain = std::make_unique<LveSwapChain>(lveDevice, extent);
  } else {
    lveSwapChain = std::make_unique<LveSwapChain>(lveDevice, extent, std::move(lveSwapChain));
    if (lveSwapChain->imageCount() != commandBuffers.size()) {
      freeCommandBuffers();
      createCommandBuffers();
    }
  }

  createPipeline();
}

void First_app::createPipeline() {
    PipelineConfigInfo pipelineConfig{};
    LvePipeline::defaultPipelineConfigInfo(pipelineConfig);
    pipelineConfig.renderPass = lveSwapChain->getRenderPass();
    pipelineConfig.pipelineLayout = pipelineLayout;
    PPipeline = std::make_unique<LvePipeline>(lveDevice, 
        "/Users/mv-mac/Documents/Projects/VS_Vulkan/src/shaders/simple_shader.vert.spv",
        "/Users/mv-mac/Documents/Projects/VS_Vulkan/src/shaders/simple_shader.frag.spv",
        pipelineConfig);
}
void First_app::createCommandBuffers() {
    
    commandBuffers.resize(lveSwapChain->imageCount());
    
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = lveDevice.getCommandPool();
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());
    
    if (vkAllocateCommandBuffers(lveDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers");
    }
}

void First_app::freeCommandBuffers() {
  vkFreeCommandBuffers(
      lveDevice.device(),
      lveDevice.getCommandPool(),
      static_cast<uint32_t>(commandBuffers.size()),
      commandBuffers.data());
  commandBuffers.clear();
}
        
void First_app::recordCommandBuffer(int imageIndex) {
  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS) {
    throw std::runtime_error("failed to begin recording command buffer!");
  }    

  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = lveSwapChain->getRenderPass();
  renderPassInfo.framebuffer = lveSwapChain->getFrameBuffer(imageIndex);

  renderPassInfo.renderArea.offset = {0, 0};
  renderPassInfo.renderArea.extent = lveSwapChain->getSwapChainExtent();

  std::array<VkClearValue, 2> clearValues{};
  clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
  clearValues[1].depthStencil = {1.0f, 0};
  renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
  renderPassInfo.pClearValues = clearValues.data();

  vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = static_cast<float>(lveSwapChain->getSwapChainExtent().width);
  viewport.height = static_cast<float>(lveSwapChain->getSwapChainExtent().height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  VkRect2D scissor{{0, 0}, lveSwapChain->getSwapChainExtent()};
  vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
  vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

  renderGameObjects(commandBuffers[imageIndex]);

  vkCmdEndRenderPass(commandBuffers[imageIndex]);
  if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS) {
    throw std::runtime_error("failed to record command buffer!");
  }
}

void First_app::renderGameObjects(VkCommandBuffer commandBuffer) {
  PPipeline->bind(commandBuffer);

  for (auto& obj : gameObjects) {
    obj.transform2d.rotation = glm::mod(obj.transform2d.rotation + 0.01f, glm::two_pi<float>());

    SimplePushConstantData push{};
    push.offset = obj.transform2d.translation;
    push.color = obj.color;
    push.transform = obj.transform2d.mat2();

    vkCmdPushConstants(
        commandBuffer,
        pipelineLayout,
        VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
        0,
        sizeof(SimplePushConstantData),
        &push);
    
    obj.model->bind(commandBuffer);
    obj.model->draw(commandBuffer);
  }
}

void First_app::drawFrame() {
  uint32_t imageIndex;
  auto result = lveSwapChain->acquireNextImage(&imageIndex);

  if (result == VK_ERROR_OUT_OF_DATE_KHR) {
    recreateSwapChain();
    return;
  }
    
  if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
      throw std::runtime_error("failed to acquire swap chain image");
  }
    
  recordCommandBuffer(imageIndex);
  result = lveSwapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || 
  lveWindow.wasWindowResized()) {
      lveWindow.resetWindowResizedFlag();
      recreateSwapChain();
      return;
  } else if (result != VK_SUCCESS) {
      throw std::runtime_error("failed to present swap chain image");
  }
}

}

