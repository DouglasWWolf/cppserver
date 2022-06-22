//==========================================================================================================
// stat_thread.cpp - Implements a thread that generates ".stat" events on the server every time a specific 
//                   file is either created or deleted
//==========================================================================================================
#include <unistd.h>
#include <string>
#include <filesystem>
#include "globals.h"
using namespace std;


//==========================================================================================================
// main() - Execution starts here when this thread is spawned
//
// Passed: p1 = Pointer to a std::string containing the filename 
//==========================================================================================================
void CStatThread::main(void* p1, void* p2, void* p3)
{
    // Convert p1 into a string reference
    string& filename = *(string*)p1;

    // Find out whether or not that file currently exists
    bool prior_status = filesystem::exists(filename);

    // We're going to loop forever, generating events when the file status changes
    while (true)
    {
        // Do nothing for one second
        usleep(1000000);

        // Does our file exist?
        bool new_status = filesystem::exists(filename);

        // If the state of that file has changed, issue a ".stat" event on the server
        if (new_status != prior_status)
        {
            prior_status = new_status;
            Server.sendf(".stat %i\r\n", new_status);
        }
    }
}
//==========================================================================================================


