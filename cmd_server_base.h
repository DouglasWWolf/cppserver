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
// Command handlers in the derived class will be passed one of these objects
//----------------------------------------------------------------------------------------------------------
class server_command_t
{
public:

    // Overloading the '=' operator so we can assign a string vector
    void  operator=(const std::vector<std::string> rhs) {m_tokens = rhs; m_next_index = 1;}

    // Call this to fetch the command token
    std::string&    get_cmd();

    // Call one of these to fetch the next command parameter
    bool            get_next_token(std::string *p_result);
    bool            get_next_float(double      *p_result);
    bool            get_next_int  (int         *p_result);

    // Returns the number of parameters
    int             param_count() {return m_tokens.size() - 1;}

protected:

    // This is the index of the next item to be retrieved via get_next_XXX()
    int m_next_index;

    // The tokens
    std::vector<std::string> m_tokens;

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