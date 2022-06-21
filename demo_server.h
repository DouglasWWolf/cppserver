//==========================================================================================================
// demo_server.h - Defines a TCP command-line server
//==========================================================================================================
#pragma once
#include "cmd_server_base.h"


class CDemoServer : public CCmdServerBase
{
public:

    // Default constructor
    CDemoServer() : CCmdServerBase() {}

    // Whenever a command arrives, this will be called to handle it
    void handle_command(std::vector<std::string>&);

};