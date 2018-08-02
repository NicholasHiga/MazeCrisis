#include "LogManager.h"

using std::string;
using std::ofstream;

unique_ptr<LogManager> LogManager::myInstance = nullptr;

LogManager::LogManager() : LogManager("Log.log", LogPriority::LOG_INFO)
{
}

LogManager::LogManager(const string &filename, LogPriority priority)
{
    myFilename = filename;
    myPriority = priority;
    myFile = new ofstream(filename.c_str());
}

LogManager::~LogManager()
{
    close();

}

LogManager&
LogManager::getInstance()
{
    if (myInstance == nullptr)
        myInstance = std::unique_ptr<LogManager>(new LogManager());

    return *myInstance;
}

void 
LogManager::close()
{
    if (myFile != nullptr)
    {
        myFile->close();
        delete myFile;
        myFile = nullptr;
    }
}

void 
LogManager::setFilename(const string &filename)
{
    myFile->close();
    rename(myFilename.c_str(), filename.c_str());
    myFilename = filename;
}

void 
LogManager::setPriority(LogPriority priority)
{
    myPriority = priority;
}

void 
LogManager::log(const string &message, LogPriority priority)
{
    if (priority >= myPriority)
        *myFile << message << std::endl;
}

void 
LogManager::info(const string &message)
{
    log(message, LogPriority::LOG_INFO);
}

void 
LogManager::trace(const string &message)
{
    log(message, LogPriority::LOG_TRACE);
}

void 
LogManager::warn(const string &message)
{
    log(message, LogPriority::LOG_WARN);
}

void 
LogManager::error(const string &message)
{
    log(message, LogPriority::LOG_ERROR);
}

