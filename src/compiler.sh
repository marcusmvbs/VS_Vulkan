/Users/mv-mac/VulkanSDK/1.3.243.0/macOS/bin/glslc /Users/mv-mac/Documents/Projects/VS_Vulkan/src/shaders/simple_shader.vert -o /Users/mv-mac/Documents/Projects/VS_Vulkan/src/shaders/simple_shader.vert.spv
/Users/mv-mac/VulkanSDK/1.3.243.0/macOS/bin/glslc /Users/mv-mac/Documents/Projects/VS_Vulkan/src/shaders/simple_shader.frag -o /Users/mv-mac/Documents/Projects/VS_Vulkan/src/shaders/simple_shader.frag.spv
/Users/mv-mac/VulkanSDK/1.3.243.0/macOS/bin/glslc /Users/mv-mac/Documents/Projects/VS_Vulkan/src/shaders/point_light.vert -o /Users/mv-mac/Documents/Projects/VS_Vulkan/src/shaders/point_light.vert.spv
/Users/mv-mac/VulkanSDK/1.3.243.0/macOS/bin/glslc /Users/mv-mac/Documents/Projects/VS_Vulkan/src/shaders/point_light.frag -o /Users/mv-mac/Documents/Projects/VS_Vulkan/src/shaders/point_light.frag.spv
#!/bin/bash
make
./a.out