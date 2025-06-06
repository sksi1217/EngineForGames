#pragma once

namespace utils
{
	class Time
	{
	public:
		static void Update();
		static float DeltaTime();
		static float FPS();

	private:
		static float m_deltaTime;
		static float m_lastFrame;
		static float m_fps;
		static int m_frameCount;
		static float m_fpsLastTime;
	};
}
