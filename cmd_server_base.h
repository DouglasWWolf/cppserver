//==========================================================================================================
// cmd_server_base.h - Defines a base-class for a TCP command server
//==========================================================================================================
#pragma once
#include <string>
#include <vector>
#include "cthread.h"
#include "netsock.h"

//----------------------------------------------------------------------------------------------------------
// One of these must be passed to p1 during spawn()
//----------------------------------------------------------------------------------------------------------
struct cmd_server_t
{
    bool verbose;
    int  port;    
};
//----------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------
// CCmdServerBase - Serves as the generic base class for TCP command servers
//----------------------------------------------------------------------------------------------------------
class CCmdServerBase : public CThread
{
public:

    // Constructor
    CCmdServerBase() : CThread() {}

    // Call this to start the server
    void    start(cmd_server_t *params);

protected:

    // This is the entry point when the server thread spawns
    void    main(void* p1=0, void* p2=0, void* p3=0);

    // Over-ride this to handle commands
    virtual void handle_command(std::vector<std::string>&) {}

private:

    // This will be true if we want verbose output for debugging
    bool    m_verbose;
    
    // This is the TCP port we should listen on
    int     m_port;

    // This is the network socket that will be our server socket
    NetSock m_socket;
};
//----------------------------------------------------------------------------------------------------------