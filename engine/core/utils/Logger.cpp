#include <engine/core/utils/Logger.hpp>

namespace fs = std::filesystem;

namespace utils
{
	std::ofstream Logger::logFile;

	std::string Logger::generateLogFilename(const std::string &logDir)
	{
		// Создаем папку, если её нет
		if (!fs::exists(logDir))
		{
			fs::create_directory(logDir);
		}

		auto now = std::time(nullptr);
		char buffer[80];
		std::strftime(buffer, sizeof(buffer), "%Y-%m-%d_%H-%M-%S", std::localtime(&now));
		return std::string(logDir) + "/log_" + buffer + ".log";
	}

	void Logger::init(const std::string &logDir)
	{
		std::string logFilePath = generateLogFilename(logDir);
		logFile.open(logFilePath, std::ios::out | std::ios::trunc);
		if (!logFile.is_open())
		{
			std::cerr << "Failed to open log file: " << logFilePath << std::endl;
		}
		else
		{
			info("Log file created: " + logFilePath);
		}
	}

	void Logger::shutdown()
	{
		utils::Logger::info("Сlosing the Log File");
		if (logFile.is_open())
		{
			logFile.close();
		}
	}

	void Logger::log(LogLevel level, const std::string &message)
	{
		std::time_t now = std::time(nullptr);
		char timestamp[20];
		std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

		std::string levelStr;
		switch (level)
		{
		case LogLevel::DEBUG:
			levelStr = "DEBUG";
			break;
		case LogLevel::INFO:
			levelStr = "INFO";
			break;
		case LogLevel::WARNING:
			levelStr = "WARNING";
			break;
		case LogLevel::ERROR:
			levelStr = "ERROR";
			break;
		}

		std::string logMessage = std::string("[") + timestamp + "] [" + levelStr + "] " + message + "\n";

		// Цвета в консоли
		std::string color;
		if (level == LogLevel::ERROR)
			color = "\033[1;31m"; // Красный
		else if (level == LogLevel::WARNING)
			color = "\033[1;33m"; // Жёлтый
		else if (level == LogLevel::INFO)
			color = "\033[1;32m"; // Зелёный
		else
			color = "\033[1;36m"; // Голубой

		std::cout << color << logMessage << "\033[0m"; // Сброс цвета

		if (logFile.is_open())
		{
			logFile << logMessage;
			logFile.flush();
		}
	}

	void Logger::debug(const std::string &message)
	{
		log(LogLevel::DEBUG, message);
	}

	void Logger::info(const std::string &message)
	{
		log(LogLevel::INFO, message);
	}

	void Logger::warning(const std::string &message)
	{
		log(LogLevel::WARNING, message);
	}

	void Logger::error(const std::string &message)
	{
		log(LogLevel::ERROR, message);
	}
}
