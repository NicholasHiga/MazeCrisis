#pragma once
#include <fstream>
#include <string>
#include <memory>

using std::unique_ptr;

enum LogPriority
{
    NONE = 0,
    LOG_INFO = 1,
    LOG_TRACE = 2,
    LOG_WARN = 3,
    LOG_ERROR = 4
};

class LogManager
{
public:
    /// getInstance()
    /// Singleton design pattern used to get a single instance of LogManager.
    ///
    static LogManager& getInstance();

    /// close()
    /// Closes the log file and handles clean up of the ostream myFile.
    ///
    void close();

    /// setFilename()
    /// Change the log file's name to the given filename.
    ///
    /// @param[in]      filename
    ///                 New filename.
    ///
    void setFilename(const std::string &filename);

    /// setPriority()
    /// Changes the priority of the log file, so any message lower than the
    /// priority given will not be printed to the log.
    ///
    /// @param[in]      priority
    ///                 New priority.
    ///
    void setPriority(LogPriority priority);

    /// log()
    /// Print the given message to the file, if the priority is higher than 
    /// the log's priority level.
    ///
    /// @param[in]      message
    ///                 Message to print.
    ///
    /// @paramp[in]     priority
    ///                 Priority level of the message.
    ///
    void log(const std::string &message, LogPriority priority);

    /// info()
    /// Prints the message to the log file if the priority is less than or
    /// equal to info.
    ///
    /// @param[in]      message
    ///                 Message to print.
    ///
    void info(const std::string &message);

    /// trace()
    /// Prints the message to the log file if the priority is less than or
    /// equal to trace.
    ///
    /// @param[in]      message
    ///                 Message to print.
    ///
    void trace(const std::string &message);

    /// warn()
    /// Prints the message to the log file if the priority is less than or
    /// equal to warn.
    ///
    /// @param[in]      message
    ///                 Message to print.
    ///
    void warn(const std::string &message);

    /// error()
    /// Prints the message to the log file if the priority is less than or
    /// equal to error.
    ///
    /// @param[in]      message
    ///                 Message to print.
    ///
    void error(const std::string &message);

    ~LogManager();

private:
    static unique_ptr<LogManager> myInstance;
    std::ofstream* myFile;
    std::string myFilename;
    LogPriority myPriority;

    LogManager();
    LogManager(const std::string & filename, LogPriority priority);
};