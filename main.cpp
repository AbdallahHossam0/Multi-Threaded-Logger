// Multi-Threaded Logger Class test

#include "logger.hpp"

using namespace std;


void logSomeMessages(int id, Logger& logger)
{
	for (int i{ 0 }; i < 10; ++i) {
        logger.log("Log entry " + to_string(i) + " from thread " + to_string(id));
	}
}

int main(void){
	Logger logger;

    vector<thread> threads;

    // Create 10 tasks to log messeges in the file
    for(int i{}; i < 10; ++i){
        threads.emplace_back(logSomeMessages, i, ref(logger));
        // note the use of emplace_back rather than push_back to construct the object in place
    }

    for(auto &t : threads){
        t.join();
    }

    
    return 0;

}