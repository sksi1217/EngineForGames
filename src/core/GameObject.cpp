#include "include/core/GameObject.hpp"
#include <src/utils/Logger.hpp>
#include <src/utils/Time.hpp>

GameObject::GameObject(std::shared_ptr<Texture2D> texture, std::shared_ptr<Shader> shader, std::shared_ptr<Renderer> renderer)
	: m_texture(texture), m_shader(shader), m_renderer(renderer)
{
	if (!texture || !shader)
	{
		utils::Logger::error("Texture or shader is null!");
		throw std::runtime_error("Texture or shader is null!");
	}
}

void GameObject::Draw()
{
	if (m_texture && m_shader)
	{
		m_renderer->Render(*m_texture, *m_shader, m_params);
	}
}

void GameObject::Update()
{
	// Пример анимации - можно удалить или заменить на свою логику
	// m_params.rotation += 45.0f * utils::Time::DeltaTime(); // 45 градусов в секунду
}

void GameObject::OnMouseClick(float mouseX, float mouseY, bool isPressed)
{
	glm::vec2 pos = m_params.position;
	glm::vec2 size = m_params.size;

	// Проверяем попадание
	bool isInside = (mouseX >= pos.x && mouseX <= pos.x + size.x && mouseY >= pos.y && mouseY <= pos.y + size.y);

	if (isPressed)
	{
		if (isInside)
		{
			m_isDragging = true;
			m_dragOffset = glm::vec2(mouseX, mouseY) - pos;
		}
	}
	else
	{
		m_isDragging = false;
	}
}

void GameObject::OnMouseMove(float mouseX, float mouseY)
{
	if (m_isDragging)
	{
		m_params.position = glm::vec2(mouseX, mouseY) - m_dragOffset;
	}
}
