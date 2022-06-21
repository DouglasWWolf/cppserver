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

    // Default constructor
    server_command_t() {m_next_index = 1;}

    // Constructor from a string vector
    server_command_t(const std::vector<std::string> rhs) {fill_tokens(rhs);}

    // Copy constructor
    server_command_t(const server_command_t& rhs) {fill_tokens(rhs.m_tokens);}

    // Overloading the '=' operator so we can assign a string vector
    void  operator=(const std::vector<std::string> rhs) {fill_tokens(rhs);}

    // Call this to fetch the command token
    std::string get_cmd(bool force_lower = true);

    // Call one of these to fetch the next command parameter
    bool        get_next(std::string  *p_result, bool force_lower = true);
    bool        get_next(double       *p_result);
    bool        get_next(int          *p_result);
    bool        get_next(unsigned int *p_result);

    // Returns the number of parameters
    int         param_count() {return m_tokens.size() - 1;}

protected:

    // Call this to fill in the tokens array
    void fill_tokens(const std::vector<std::string>& rhs) {m_tokens = rhs, m_next_index = 1;}

    // This is the index of the next item to be retrieved via get_next())
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

    // Sends data to the other side of the connection
    void    send(const char* buffer, int length = -1);

protected:

    // This is the entry point when the server thread spawns
    void    main(void* p1=0, void* p2=0, void* p3=0);

    // Over-ride this to handle commands
    virtual void handle_command() {}

    // When handle_command() gets called, the command will be in m_line
    server_command_t m_line;

    // Call this to report an "ok" result to the client
    void    pass(const char* fmt = nullptr, ...);

    // Call this to report a "fail" result to the client
    void    fail(const char* failure, const char* fmt = nullptr, ...);

    // This is a convenience method for reporting a syntax error
    void    fail_syntax() {send("fail syntax\r\n");}

private:

    // This will be true if we want verbose output for debugging
    bool    m_verbose;
    
    // This is the TCP port we should listen on
    int     m_port;

    // This is the network socket that will be our server socket
    NetSock m_socket;

    // This protects the "send()" routine to make it thread-safe
    CMutex  m_send_mutex;
};
//----------------------------------------------------------------------------------------------------------