//==========================================================================================================
// stat_thread.cpp - Implements a thread that generates ".stat" events on the server every time a specific 
//                   file is either created or deleted
//==========================================================================================================
#include <unistd.h>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>

#include "globals.h"
using namespace std;


//==========================================================================================================
// file_exists() - Returns true if the specified filename exists
//==========================================================================================================
static bool file_exists(string& filename)
{
    struct stat sb;
    return (stat(filename.c_str(), &sb) == 0);
}
//==========================================================================================================


//==========================================================================================================
// main() - Execution starts here when this thread is spawned
//
// Passed: p1 = Pointer to a std::string containing the filename 
//==========================================================================================================
void CStatThread::main()
{
    // Convert p1 into a string reference
    string& filename = *(string*)m_spawn_param[0];

    // Find out whether or not that file currently exists
    bool prior_status = file_exists(filename);

    // We're going to loop forever, generating events when the file status changes
    while (true)
    {
        // Do nothing for one second
        usleep(1000000);

        // Does our file exist?
        bool new_status = file_exists(filename);

        // If the state of that file has changed, issue a ".stat" event on the server
        if (new_status != prior_status)
        {
            prior_status = new_status;
            Server.sendf(".stat %i\r\n", new_status);
        }
    }
}
//==========================================================================================================


