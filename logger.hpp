#pragma once

#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>



class Logger{
public:
    // The constructor to start the background thread to write the logs on the file
    Logger();

    Logger(const Logger& src) = delete; // delete the copy constructor
    Logger& operator=(const Logger& rhs) = delete; // Delete the copy assignment operator

    // Method to add logs to the queue
    void log(std::string entry);

    // Destrucor of the class
    virtual ~Logger();

private:
    // This Function in the background thread 
    void processLogs();
    // Helper to log
    void processLogsHelper(std::queue<std::string>& logQueue, std::ofstream& ofs) const;
    // Mutex and conditional variable to protect the queue 
    std::mutex m_mutex; // this mutex is for the prevent data racee for the queue
    std::condition_variable m_condVar; // this conditional var to make the process logs method ablr to work after adding logs to the queue
    // The queue
    std::queue<std::string> m_queue;
    // Background thread
    std::thread m_thread;

    bool m_exit {false};

};