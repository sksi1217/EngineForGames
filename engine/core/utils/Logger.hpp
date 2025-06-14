#pragma once

#include <string>
#include <iostream>
#include <ctime>
#include <iomanip>
#include <filesystem>
#include <fstream>

namespace utils
{
	enum class LogLevel
	{
		DEBUG,
		INFO,
		WARNING,
		ERROR
	};

	class Logger
	{
	public:
		static void init(const std::string &logDir = "logs");
		static void shutdown();

		static void debug(const std::string &message);
		static void info(const std::string &message);
		static void warning(const std::string &message);
		static void error(const std::string &message);

	private:
		static std::ofstream logFile;
		static std::string generateLogFilename(const std::string &logDir);

		static void log(LogLevel level, const std::string &message);
	};

}
