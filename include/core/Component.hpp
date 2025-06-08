#pragma once

#include <memory>
#include <string>

class GameObject;

class Component
{
public:
	virtual ~Component() = default;

	virtual void Update() {}
	virtual void Draw() {}
	virtual void Initialize() {}

	void SetOwner(GameObject *owner) { m_owner = owner; }
	GameObject *GetOwner() const { return m_owner; }

	const std::string &GetName() const { return m_name; }
	void SetName(const std::string &name) { m_name = name; }

protected:
	GameObject *m_owner = nullptr;
	std::string m_name = "Component";
};
