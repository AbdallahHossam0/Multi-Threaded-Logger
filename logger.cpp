
#include "logger.hpp"
#include "fstream"
#include <iostream>

using namespace std;



Logger::Logger(){
    // Start the background Thread
    m_thread = thread {&Logger :: processLogs, this};
    // We run processLogs method in this thread and we passed the default parameter "this" to the method
}

void Logger::log(string entry){
    // Lock the mutex and add Entry to the queue
    // After the end of this function the lock is unlocked automatically RAII lock ^^
    unique_lock lock{m_mutex};
    // Push the log to the queue
    m_queue.push(move(entry));
    // notify other functions wait for the conditional variable to run as there is entries in the queue
    m_condVar.notify_all();


}

void Logger::processLogs(){
    // Open the log file
    ofstream logFile{"log.txt"};
    if(logFile.fail()){
        cerr << "Failed to Open the Filel" << endl;
        return; 
    }

    //make the lock but it is not acquired yet  
    unique_lock lock{m_mutex, defer_lock};

    // Start processing
    for(;;){
        // Lock is locked now to be able to read from the queue
        lock.lock();
        if(!m_exit){

            // ? why the lock is passed to wait
            m_condVar.wait(lock); // accept a notification from the processlog methods 
        }
        else{

            processLogsHelper(m_queue, logFile);
        }

        // pass the contents of the m_queue to the local queue
        // and delete the contents of the m_queue as the have been processed and no need to keep them
        queue<string> local;
        local.swap(m_queue);

        // unlocl the lock as we do not use the queue anymore
        lock.unlock();

        // pass the queue and the output file to this method to start output the contents of the queue on the log file ^^
        processLogsHelper(local, logFile);

    }

}



void Logger::processLogsHelper(queue<string>& logs, ofstream& logFile) const{

    while(!logs.empty()){
        logFile << logs.front() << endl;
        logs.pop();
    }

}

Logger::~Logger()
{
	{
		unique_lock lock{ m_mutex };
		// Gracefully shut down the thread by setting m_exit to true.
		m_exit = true;
	}
	// Notify condition variable to wake up thread.
	m_condVar.notify_all();
	// Wait until thread is shut down. This should be outside the above code
	// block because the lock on m_mutex must be released before calling join()!
	m_thread.join();
}



