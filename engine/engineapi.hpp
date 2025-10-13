#pragma once

#include <memory>

// #include <GL/glx.h>

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GL/gl.h>
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
