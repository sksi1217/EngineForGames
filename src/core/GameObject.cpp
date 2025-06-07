#include "include/core/GameObject.hpp"
#include <src/utils/Logger.hpp>
#include <src/utils/Time.hpp>

GameObject::GameObject(std::shared_ptr<Texture2D> texture)
	: m_texture(texture)
{
	if (!texture)
	{
		utils::Logger::error("Texture is null!");
		throw std::runtime_error("Texture is null!");
	}
}

void GameObject::Draw()
{
	if (m_texture)
	{
		Renderer::Get().Render(*m_texture, m_params);
	}
}

void GameObject::Update()
{
	// ! Пример анимации - можно удалить или заменить на свою логику
	// ? 45 градусов в секунду
	m_params.rotation += 45.0f * utils::Time::DeltaTime();
}

void GameObject::OnMouseClick(float mouseX, float mouseY, bool isPressed)
{
	glm::vec2 pos = m_params.position;
	glm::vec2 size = m_params.size;

	// ! Проверяем попадание
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
