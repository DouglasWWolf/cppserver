//==========================================================================================================
// cppserver - A demonstration of the CCmdServerBase class
//==========================================================================================================
#include <unistd.h>
#include "cmd_line.h"
#include "globals.h"
using namespace std;

// A command line parser
CCmdLine    CmdLine;

// If the user wants to generate file-stat events, this is the default name of the file
string stat_file = "file.txt";

int main(int argc, char** argv)
{
    cmd_server_t server_params;

    // Set up some default server parameters
    server_params.verbose = false;
    server_params.port    = 12345;

    // Declare the valid command line switches
    CmdLine.declare_switch("-port",    CLP_REQUIRED);
    CmdLine.declare_switch("-verbose", CLP_NONE);
    CmdLine.declare_switch("-stat",    CLP_OPTIONAL);

    // Parse the command line
    if (!CmdLine.parse(argc, argv))
    {
        fprintf(stderr, "%s: %s\n", argv[0], CmdLine.error().c_str());
        exit(1);
    }

    // If the user specified a port, fetch it
    CmdLine.has_switch("-port", &server_params.port);

    // Find out if the user wants to be in verbose mode
    server_params.verbose = CmdLine.has_switch("-verbose");

    // Does the user want to see "stat" events?
    bool perform_stat = CmdLine.has_switch("-stat", &stat_file);

    // Start the command server
    Server.start(&server_params);

    // Wait for the server to be fully initialized
    while (!Server.is_initialized()) usleep(10000);

    // If the user wants to generate file-stat events, spawn that thread
    if (perform_stat)
    {
        if (server_params.verbose) printf("Performing periodic stat of file '%s'\n", stat_file.c_str());
        StatThread.spawn(&stat_file);
    }

    // We have nothing else to do, so we'll just wait on the server thread
    Server.join();
}

