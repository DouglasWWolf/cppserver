//==========================================================================================================
// demo_server.h - Defines a TCP command-line server
//==========================================================================================================
#include "demo_server.h"
using namespace std;

void CDemoServer::handle_command(std::vector<std::string>& tokens)
{

    string cmd = tokens[0];

    printf("Handling command \"%s\"\n", cmd.c_str());
}
