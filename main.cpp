//==========================================================================================================
// cppserver - A demonstration of the CCmdServerBase class
//==========================================================================================================
#include <unistd.h>
#include <sys/stat.h>
#include "demo_server.h"
#include "cmd_line.h"
using namespace std;

CDemoServer server;
CCmdLine    CmdLine;

string stat_file = "file.txt";

int main(int argc, char** argv)
{
    string       s;
    cmd_server_t server_params;
    struct stat  sb;

    // Set up some default server parameters
    server_params.verbose = false;
    server_params.port    = 12345;

    // Declare the valid command line switches
    CmdLine.declare_switch("port",    CLP_REQUIRED);
    CmdLine.declare_switch("verbose", CLP_NONE);
    CmdLine.declare_switch("stat",    CLP_OPTIONAL);

    // Parse the command line
    CmdLine.parse(argc, argv);

    // If the user specified a port, fetch it
    CmdLine.has_switch("port", &server_params.port);

    // Find out if the user wants to be in verbose mode
    server_params.verbose = CmdLine.has_switch("verbose");

    // Does the user want to see "fstat" events?
    bool perform_stat = CmdLine.has_switch("stat", &s);

    // If so, did they over-ride the name of the fstat file?
    if (perform_stat && !s.empty()) stat_file = s;

    // Start the command line server
    server.start(&server_params);

    // Wait for the server to be fully initialized
    while (!server.is_initialized()) usleep(10000);

    // Does the stat_file exist?
    bool prior_status = (stat(stat_file.c_str(), &sb) == 0);

    // If the user wants to perform stat on a file, sit in this loop forever
    if (perform_stat)
    {
        if (server_params.verbose) printf("Performing periodic stat of file '%s'\n", stat_file.c_str());
        
        while (perform_stat)
        {
            usleep(1000000);

            // Does the stat_file exist?
            bool new_status = (stat(stat_file.c_str(), &sb) == 0);

            if (new_status != prior_status)
            {
                prior_status = new_status;
                server.sendf(".stat %i\r\n", new_status);
            }
        }
    }

    // If the user didn't want to sit in that loop above, join the server thread
    server.join();
}

