#include "Logger.hpp"
#include <mutex>

namespace Logger {
    // Global Logging Object.
    std::unique_ptr<Log> g_log;

    // Initalize our logging object.
    void startLog(std::ostream& outs) {
        
        g_log = std::make_unique<Log>(outs);
        Logger::log(Level::Info, "Started logging system.");
        
    }

    // Initalize our logging object.
    void startLog(const std::string& filepath) {
        
        g_log = std::make_unique<Log>(filepath);
        Logger::log(Level::Info, "Started logging system.");
        
    }


    // Method which logs.
    void log(Level s, const std::string& msg) {
        g_log->addLog(s, msg);
    }

    // Create our global logging object.
    Log::Log(const std::string& filepath) : m_logfile{} {
        std::ofstream logfile(filepath);
        m_logfile.open(filepath);
        m_Pouts = &m_logfile;
    }

    Log::Log(std::ostream& outs) : m_Pouts(&outs){

    } 
    

    // Add a message to our log.
    void Log::addLog(Level s, const std::string& msg) {
        std::unique_lock<std::mutex> lock(m_mutexLog);
        if (m_Pouts) {
            *m_Pouts << levels[static_cast<int>(s)] << ": " << msg << std::endl;
            m_Pouts->flush();
        }
    }

    Log::~Log() {
        addLog(Level::Info, "Stopped logging system.");
        m_logfile.close();
    }
}