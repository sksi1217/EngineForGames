# Цвета для вывода в консоль
GREEN  := $(shell tput -Txterm setaf 2)
YELLOW := $(shell tput -Txterm setaf 3)
RESET  := $(shell tput -Txterm sgr0)

# Режим сборки: debug или release
BUILD_TYPE ?= debug

ifeq ($(BUILD_TYPE), debug)
	CXXFLAGS += -g
else ifeq ($(BUILD_TYPE), release)
	CXXFLAGS += -O3 -DNDEBUG
else
	$(error Invalid BUILD_TYPE=$(BUILD_TYPE). Use 'debug' or 'release')
endif

# Имя исполняемого файла
TARGET = build/myapp

# Исходники
SRCS = tests/main.cpp \
	engine/core/Engine.cpp \
	engine/core/SpatialPartitioning.cpp \
	engine/core/events/EventSystem.cpp \
	engine/core/window/GameWindow.cpp \
	engine/core/utils/Logger.cpp \
	engine/core/Systems.cpp \
	engine/core/utils/Time.cpp \
	engine/core/ui/Settings.cpp \
	engine/core/graphics/renderer/Renderer.cpp \
	engine/core/graphics/shaders/Shader.cpp \
	engine/core/graphics/shaders/ShaderManager.cpp \
	engine/core/graphics/textures/TextureLoader.cpp \
	engine/core/ui/ImGuiContext.cpp \
	extern/imgui/imgui.cpp \
	extern/imgui/imgui_draw.cpp \
	extern/imgui/imgui_tables.cpp \
	extern/imgui/imgui_widgets.cpp \
	extern/imgui/backends/imgui_impl_glfw.cpp \
	extern/imgui/backends/imgui_impl_opengl3.cpp \


# Объектные файлы
OBJS_DIR = build/objs
OBJS = $(patsubst %.cpp,$(OBJS_DIR)/%.o,$(SRCS))

# Компилятор
CXX = g++

# Флаги компиляции
INCLUDES = -I. -Iinclude -Iimgui
COMMON_CXXFLAGS = -Wall -std=c++17 $(INCLUDES)
CXXFLAGS += $(COMMON_CXXFLAGS)

# Линковка
UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
	LDFLAGS += -lGL -lglfw -lGLEW
else
	$(error Unsupported platform)
endif

# Правила сборки
all: $(TARGET)

$(TARGET): $(OBJS)
	@echo "$(GREEN)🔗 Linking $@...$(RESET)"
	@mkdir -p build
	$(CXX) $^ -o $@ $(LDFLAGS)

$(OBJS_DIR)/%.o: %.cpp
	@echo "$(YELLOW)⚙️ Compiling $<...$(RESET)"
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo "$(YELLOW)🗑 Cleaning up...$(RESET)"
	rm -rf build/*

.PHONY: all clean

# sed -i 's/^\s\+/\t/' Makefile
