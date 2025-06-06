#pragma once

#include <memory>
#include "include/graphics/Renderer.hpp"
#include "src/graphics/Texture2D.hpp"
#include "src/graphics/Shader.hpp"

class GameObject
{
public:
	GameObject(std::shared_ptr<Texture2D> texture, std::shared_ptr<Shader> shader, std::shared_ptr<Renderer> renderer);

	void Draw();
	void Update();

	// Методы преобразования
	void SetPosition(const glm::vec2 &position) { m_params.position = position; }
	void SetSize(const glm::vec2 &size) { m_params.size = size; }
	void SetRotation(float rotation) { m_params.rotation = rotation; }
	void SetOrigin(const glm::vec2 &origin) { m_params.origin = origin; }
	void SetColor(const glm::vec4 &color) { m_params.color = color; }

	void SetTextureSize(const glm::vec2 &textureSize) { m_params.textureSize = textureSize; }
	//void SetTextureSize(const glm::vec2 &size) { m_params.textureSize = size; }

	void SetTextureOffset(const glm::vec2 &offset) { m_params.textureOffset = offset; }
	void SetFlipX(bool flip) { m_params.flipX = flip; }
	void SetFlipY(bool flip) { m_params.flipY = flip; }

	void OnMouseClick(float mouseX, float mouseY, bool isPressed);
	void OnMouseMove(float mouseX, float mouseY);

	// Методы обработки текстурных областей
	void SetTextureRegion(const glm::vec2 &offset, const glm::vec2 &size)
	{
		m_params.textureOffset = offset;
		m_params.textureSize = size;
	}

	// Гетерры
	const glm::vec2 &GetPosition() const { return m_params.position; }
	const glm::vec2 &GetSize() const { return m_params.size; }
	float GetRotation() const { return m_params.rotation; }
	const glm::vec2 &GetOrigin() const { return m_params.origin; }
	const glm::vec4 &GetColor() const { return m_params.color; }
	const glm::vec2 &GetTextureSize() const { return m_params.textureSize; }
	const glm::vec2 &GetTextureOffset() const { return m_params.textureOffset; }
	bool GetFlipX() const { return m_params.flipX; }
	bool GetFlipY() const { return m_params.flipY; }

private:
	Renderer::RenderParams m_params;
	std::shared_ptr<Texture2D> m_texture;
	std::shared_ptr<Shader> m_shader;
	std::shared_ptr<Renderer> m_renderer;

	bool m_isDragging = false;
	glm::vec2 m_dragOffset;
};
