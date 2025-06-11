# Определение имени исполняемого файла
TARGET = build/myapp

# Исходные файлы вашего проекта
SRCS = sandbox/main.cpp \
	src/core/Engine.cpp \
	src/core/GameWindow.cpp \
	src/utils/Logger.cpp \
	src/utils/Time.cpp \
	src/graphics/GLContext.cpp \
	src/ui/ImGuiContext.cpp \
	libs/imgui/imgui.cpp \
	libs/imgui/imgui_demo.cpp \
	libs/imgui/imgui_draw.cpp \
	libs/imgui/imgui_tables.cpp \
	libs/imgui/imgui_widgets.cpp \
	libs/imgui/backends/imgui_impl_opengl3.cpp \
	libs/imgui/backends/imgui_impl_glfw.cpp \
	src/graphics/Renderer.cpp \
	src/utils/TextureLoader.cpp \
	src/graphics/Shader.cpp \
	src/utils/ShaderManager.cpp \
	src/core/Settings.cpp \
	src/core/EventSystem.cpp \
	src/core/SpatialPartition.cpp \
	src/utils/Systems.cpp \

# Компилятор
CXX = g++

# Флаги компиляции
CXXFLAGS = -Wall -std=c++17 -I. -Iinclude -Iimgui -g

# Проверка операционной системы
UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
	# Для Linux: добавляем библиотеки OpenGL, GLFW и GLEW
	LDFLAGS += -lGL -lglfw -lGLEW
else
	$(error Unsupported platform)
endif

# Объектные файлы будут помещены в одну папку
OBJS_DIR = build/objs
OBJS = $(patsubst %.cpp,$(OBJS_DIR)/%.o,$(SRCS))

# Цель по умолчанию
all: $(TARGET)

# Создание каталога build, если он не существует
$(TARGET): $(OBJS)
	@mkdir -p build
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Правило для компиляции .cpp файлов в .o файлы
$(OBJS): $(OBJS_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Правило для очистки временных файлов
clean:
	rm -rf build

.PHONY: all clean

# sed -i 's/^\s\+/\t/' Makefile
