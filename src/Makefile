CFLAGS = -std=c++17 -I. -I/Users/mv-mac/VulkanSDK/1.3.243.0/macOS/include -I/Users/mv-mac/Documents/Projects/Objects
LDFLAGS = -L/Users/mv-mac/VulkanSDK/1.3.243.0/macOS/lib `pkg-config --static --libs glfw3` -lvulkan

# create list of all spv files and set as dependency
#vertSources := $(shell find ./shaders -type f -name "*.vert")
vertObjFiles = $(patsubst %.vert, %.vert.spv, $(vertSources))
#fragSources := $(shell find ./shaders -type f -name "*.frag")
fragObjFiles = $(patsubst %.frag, %.frag.spv, $(fragSources))

TARGET = a.out
$(TARGET): $(vertObjFiles) $(fragObjFiles)
$(TARGET): *.cpp *.hpp
	g++ $(CFLAGS) -o $(TARGET) *.cpp $(LDFLAGS)

# make shader targets
%.spv: %
	${GLSLC} $< -o $@

.PHONY: test clean

test: a.out
	./a.out

clean:
	rm -f a.out
	rm -f *.spv