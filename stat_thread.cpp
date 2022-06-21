//==========================================================================================================
// stat_thread.cpp - Implements a thread that generates ".stat" events on the server every time a specific 
//                   file is either created or deleted
//==========================================================================================================
#include <unistd.h>
#include <string>
#include <filesystem>
#include <sys/stat.h>
#include "globals.h"
using namespace std;


//==========================================================================================================
// main() - Execution starts here when this thread is spawned
//
// Passed: p1 = Pointer to a std::string containing the filename 
//
// Notes: This code is aimed at C++11.
//        If it were aimed at C++17, we could use std::filesystem instead of stat()
//==========================================================================================================
void CStatThread::main(void* p1, void* p2, void* p3)
{
    struct stat sb;

    // Convert p1 into a string reference
    string& filename = *(string*)p1;

    // stat() will need a C-style character string
    const char* cfilename = filename.c_str();

    // Find out whether or not that file currently exists
    bool prior_status = stat(cfilename, &sb) == 0;

    // We're going to loop forever, generating events when the file status changes
    while (true)
    {
        // Do nothing for one second
        usleep(1000000);

        // Does our file exist?
        bool new_status = stat(cfilename, &sb) == 0;

        // If the state of that file has changed, issue a ".stat" event on the server
        if (new_status != prior_status)
        {
            prior_status = new_status;
            Server.sendf(".stat %i\r\n", new_status);
        }
    }
}
//==========================================================================================================


