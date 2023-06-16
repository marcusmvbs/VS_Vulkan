#include "app.hpp"

#include "simple_render_system.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "/opt/homebrew/include/glm/glm.hpp"
#include "/opt/homebrew/include/glm/gtc/constants.hpp"

// std
#include <array>
#include <cassert>
#include <stdexcept>

namespace lve {

First_app::First_app() { loadGameObjects(); }

First_app::~First_app() {}

void First_app::run() {
  SimpleRenderSystem simpleRenderSystem{lveDevice, 
lveRenderer.getSwapChainRenderPass()};
  while (!lveWindow.shouldClose()) {
    glfwPollEvents();
    if (auto commandBuffer = lveRenderer.beginFrame()) {
      lveRenderer.beginSwapChainRenderPass(commandBuffer);
      simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);
      lveRenderer.endSwapChainRenderPass(commandBuffer);
      lveRenderer.endFrame();
    }
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
  triangle.color = {.99f, .4f, .0f};
  triangle.transform2d.translation.x = .2f;
  triangle.transform2d.scale = {2.f, .5f};
  triangle.transform2d.rotation = .25f * glm::two_pi<float>();

  gameObjects.push_back(std::move(triangle));
}

}

