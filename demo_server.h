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

protected:

    // Whenever a command arrives, this will be called to handle it
    void    handle_command();

    // These are the handlers for each specific command
    void    handle_add();
    void    handle_sub();
    void    handle_mul();
    void    handle_help();

};