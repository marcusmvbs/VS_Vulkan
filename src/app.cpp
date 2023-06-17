#include "app.hpp"

#include "keyboard.hpp"
#include "camera.hpp"
#include "simple_render_system.hpp"

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

First_app::First_app() { loadGameObjects(); }

First_app::~First_app() {}

void First_app::run() {
      SimpleRenderSystem simpleRenderSystem{lveDevice, lveRenderer.getSwapChainRenderPass()};
  LveCamera camera{};

  auto viewerObject = LveGameObject::createGameObject();
  KeyboardMovementController cameraController{};

  auto currentTime = std::chrono::high_resolution_clock::now();

  while (!lveWindow.shouldClose()) {
    glfwPollEvents();
    
    auto newTime = std::chrono::high_resolution_clock::now();
    float frameTime =
        std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
    currentTime = newTime;

    cameraController.moveInPlaneXZ(lveWindow.getGLFWwindow(), frameTime, viewerObject);
    camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

    float aspect = lveRenderer.getAspectRatio();
    // camera.setOrthographicProjection(-aspect, aspect, -1, 1, -1, 1);
    camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.f);
    
    if (auto commandBuffer = lveRenderer.beginFrame()) {
      
      lveRenderer.beginSwapChainRenderPass(commandBuffer);
      simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects, camera);
      lveRenderer.endSwapChainRenderPass(commandBuffer);
      lveRenderer.endFrame();
    }
  }
  vkDeviceWaitIdle(lveDevice.device());
}

void First_app::loadGameObjects() {
  std::shared_ptr<LveModel> lveModel =
      LveModel::createModelFromFile(lveDevice, "/Users/mv-mac/Documents/Projects/VS_Vulkan/src/models/colored_cube.obj");
  auto gameObj = LveGameObject::createGameObject();
  gameObj.model = lveModel;
  gameObj.transform.translation = {.0f, .0f, 2.5f};
  gameObj.transform.scale = glm::vec3(1.f);
  gameObjects.push_back(std::move(gameObj));
}

}

