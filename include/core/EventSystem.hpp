#pragma once

struct EventSystem
{
	struct KeyEvent
	{
		int key;
		int scancode;
		int action;
		int mods;
		float time;
	};

	struct MouseButtonEvent
	{
		int button;
		int action;
		int mods;
		float time;
	};

	struct MousePos
	{
		double x;
		double y;
	};

	struct ScrollEvent
	{
		double x;
		double y;
		float time;
	};

	struct CharInput
	{
		unsigned int character;
		float time;
	};

	struct WindowSize
	{
		int w;
		int h;
	};

	KeyEvent m_LastKeyEvent;
	MouseButtonEvent m_LastMouseButtonEvent;
	MousePos m_LastMousePos;
	float m_LastMouseMoveTime = 0.0f;
	ScrollEvent m_LastScrollEvent;
	CharInput m_LastCharInput;
	WindowSize m_LastWindowSize;
	float m_LastWindowResizeTime = 0.0f;
};
