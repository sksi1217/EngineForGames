#pragma once

#include <iostream>

#include <glm/glm.hpp>
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <GL/glx.h>

#include <string>
#include <extern/entt/entt.hpp>
#include <vector>
#include <memory>
#include <functional>
#include <stdexcept>
#include <unordered_map>
#include <random>
#include <thread>

#include <engine/core/utils/Logger.hpp>
#include <engine/core/utils/Time.hpp>

#include <engine/core/graphics/renderer/GLContext.hpp>

// ** Компоненты
#include <engine/core/ecs/components/ActiveComponent.hpp>
#include <engine/core/ecs/components/LayerRender.hpp>
#include <engine/core/ecs/components/NameComponent.hpp>
#include <engine/core/ecs/components/ScriptComponent.hpp>
#include <engine/core/ecs/components/Transform.hpp>
#include <engine/core/ecs/components/TagComponent.hpp>
#include <engine/core/ecs/components/BoxCollider2D.hpp>
#include <engine/core/ecs/components/CircleCollider2D.hpp>
#include <engine/core/ecs/components/Rigidbody2D.hpp>

// **
#include <extern/stb/stb_image.h>

// ** Шейдеры
#include <engine/core/graphics/shaders/Shader.hpp>
#include <engine/core/graphics/shaders/ShaderManager.hpp>

// ** Текстуры
#include <engine/core/graphics/textures/TextureLoader.hpp>

// ** Окно
#include <engine/core/window/GameWindow.hpp>

// ** Рендер
#include <engine/core/graphics/renderer/Renderer.hpp>

// ** Объекты
#include <engine/core/scene/GameObject.hpp>

#include <engine/core/ecs/ECS.hpp>

#include <engine/core/SpatialPartitioning.hpp>

// ** События
#include <engine/core/events/EventSystem.hpp>
#include <engine/core/events/InputManager.hpp>

class GameObject;

// !
struct EngineState
{
	// ! Состояния окна/системы
	bool isRunning = true;
	bool isFocused = true;

	// ! Состояния отладки/пользовательского интерфейса
	bool showDebugUI = true;
	bool showSettingsWindow = false;

	bool isPaused = false;
	bool isFullscreen = false;
};

// !
typedef struct Texture2D
{
	unsigned int id; // ! ID текстуры OpenGL
	int width;		 // ! Ширина текстуры
	int height;		 // ! Высота текстуры

	void Bind() const // ! делает текстуру активной
	{
		glBindTexture(GL_TEXTURE_2D, id);
	}
} Texture2D;

// !
typedef struct RenderParams
{
	glm::vec2 Position{10.0f, 10.0f}; //
	glm::vec2 Scale{50.0f, 50.0f};	  //

	glm::vec2 SpriteOffset{0.0f, 0.0f};		 //
	glm::vec2 SpriteSize{0.0f, 0.0f};		 //
	glm::vec2 Origin{0, 0};					 //
	float Rotation = 0.0f;					 //
	glm::vec4 Color{1.0f, 1.0f, 1.0f, 1.0f}; //
	bool FlipX = false;						 //
	bool FlipY = false;						 //
} RenderParams;

struct Camera2D
{
	float zoom = 1.0f;
	bool isMain = false;
};

typedef struct Sprite
{
public:
	Texture2D *Sprite;
	glm::vec4 Color{1.0f, 1.0f, 1.0f, 1.0f};
	bool FlipX = false;
	bool FlipY = false;
	glm::vec2 SpriteOffset{0.0f, 0.0f};
	glm::vec2 SpriteSize{0.0f, 0.0f};
	short OrderLayer = 0;
} Sprite;

// ** Шейдеры
std::shared_ptr<Shader> const GetDefaultShader();
std::shared_ptr<Shader> LoadShader(const std::string &vertexPath, const std::string &fragmentPath);
// ** ---

// ** Рендер
// ? Основной метод отрисовки спрайта
void RenderSprite(const Texture2D &texture, const RenderParams &params);
// ?
void BeginBatch();
// ?
void EndBatch();
// ? Установка размера области отрисовки

// ! Камера
void SetCamera(const Camera2D &camera, const RenderParams &params);

// ** ---

// ** Окно для игры
// ! Основные операции
void SwapBuffers();
void Close();
void SetVSync(bool enabled);

// ! Управление окном
void SetTitle(const std::string &title);
void SetSize(int width, int height);
void SetPosition(int x, int y);
void Maximize();
void Minimize();
void Restore();
void SetIcon(const std::string &iconPath);

// ! Состояние окна
bool ShouldClose();
bool IsVSyncEnabled();
bool IsMaximized();
bool IsMinimized();
bool IsFocused();

// ! Получение параметров
std::pair<int, int> GetSize();
std::pair<int, int> GetPosition();
std::pair<float, float> GetContentScale();
std::string GetTitle();
inline GLFWwindow *GetWindowGLFW();

// TODO: ! применения настроек ИСПРАВИТЬ
/*
void ApplySettings(const Settings &settings);

void ApplyAudioSettings(const AudioSettings &audio);
void ApplyControlSettings(const ControlsSettings &controls);
void ApplyUISettings(const UISettings &ui);
void ApplyGameSettings(const GameSettings &game);
void ApplyAdvancedSettings(const AdvancedSettings &advanced);
*/
// ** ---

// ** Пустой объект
static GameObject CreateObject(entt::registry &registry);
void SetName(const std::string &name);
void SetTag(const std::string &tag);
void SetActive(bool active);
std::string GetName();
std::string GetTag();
bool IsActive();
template <typename T, typename... Args>
T &AddComponent(Args &&...args);
template <typename T>
void RemoveComponent();
template <typename T>
bool HasComponent();
template <typename T>
T &GetComponent();
template <typename T, typename... Args>
void AddScript(Args &&...args);
// ** ---

// **

// ! Добавление/удаление объекта
void AddObject(entt::entity entity);

int GetCellSize();

void DrawDebug();

void UpdateObjectPosition(entt::entity entity, const glm::vec2 &oldPosition);

// ! Geters
const int GetGridWidth();
const int GetGridHeight();
const std::vector<entt::entity> &GetCell(int x, int y);

const std::vector<std::vector<std::vector<entt::entity>>> &GetGrid();
// ? Возвращает список объектов, находящихся в ячейке по координатам позиции или в соседних ячейках
std::vector<entt::entity> GetNearbyObjects(const glm::vec2 &position);

// ** ---

// ** Текстуры
// ! Загрузка текстуры
static std::shared_ptr<Texture2D> loadTexture(const std::string &filePath);

// ! Очистка кэша текстур
static void clearCache();
// ** ---

// ** Input Manager
void Update(GLFWwindow *window);
bool IsKeyPressed(int key);
bool WasKeyPressed(int key);

// ** ---
