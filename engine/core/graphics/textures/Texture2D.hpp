#pragma once

#include <GL/glew.h>

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
