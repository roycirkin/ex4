#pragma once

/*
Use it by including this header in every file required, and in your main function start a new log.
    Logger::startLog("Log.txt");
Use the various error levels by naming them and simply passing the info and what you want to output.
    Logger::log(ERROR, "Something went wrong.");
*/

// For the unique pointers.
#include <memory>
// Filestream.
#include <fstream>
// String class for names and parameters passed around.
#include <string>
#include <mutex>

#define FATAL Logger::Level::Fatal
#define ERROR Logger::Level::Error
#define WARNING Logger::Level::Warning
#define INFO Logger::Level::Info
#define DEBUG Logger::Level::Debug

namespace Logger {

    // Severity level enum.
    enum class Level {
        Fatal,
        Error,
        Warning,
        Info,
        Debug
    };

    // Initialize the log.
    void startLog(const std::string& filepath);

    void startLog(std::ostream& outs);

    // Log a message.
    void log(Level s, const std::string& msg);

    // Logging class.
    class Log {
    public:
        Log(const std::string& filepath);
        Log(std::ostream& outs);
        void addLog(Level s, const std::string& msg);
        ~Log();
    private:
        // File for logging.
        std::mutex m_mutexLog;
        std::ofstream m_logfile;
        std::ostream* m_Pouts;
        std::string levels[5] = {"Fatal", "Error", "Warning", "Info", "Debug"};
    };
}