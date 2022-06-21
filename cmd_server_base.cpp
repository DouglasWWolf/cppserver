#include <unistd.h>
#include <vector>
#include <string>
#include "cmd_server_base.h"
using namespace std;

//==========================================================================================================
// parse_tokens() - Parses an input string into a vector of tokens
//==========================================================================================================
static vector<string> parse_tokens(const char* in)
{
    vector<string> result;
    char           token[512];

    // If we weren't given an input string, return an empty result;
    if (in == nullptr) return result;

    // So long as there are input characters still to be processed...
    while (*in)
    {
        // Point to the output buffer 
        char* out = token;

        // Skip over any leading spaces on the input
        while (*in == ' ') in++;

        // If we hit end-of-line, there are no more tokens to parse
        if (*in == 0) break;

        // Assume for the moment that we're not starting a quoted string
        char in_quotes = 0;

        // If this is a single or double quote-mark, remember it and skip past it
        if (*in == '"' || *in == '\'') in_quotes = *in++;

        // Loop until we've parsed this entire token...
        while (*in)
        {
            // If we're parsing a quoted string...
            if (in_quotes)
            {
                // If we've hit the ending quote-mark, we're done parsing this token
                if (*in == in_quotes)
                {
                    ++in;
                    break;
                }
            }

            // Otherwise, we're not parsing a quoted string. A space or comma ends the token
            else if (*in == ' ' || *in == ',') break;

            // Append this character to the token buffer
            *out++ = *in++;
        }

        // nul-terminate the token string
        *out = 0;

        // Add the token to our result list
        result.push_back(token);

        // Skip over any trailing spaces in the input
        while (*in == ' ') ++in;

        // If there is a trailing comma, throw it away
        if (*in == ',') ++in;
    }

    // Hand the caller a vector of tokens
    return result;
}
//==========================================================================================================

//==========================================================================================================
// convert_tabs_to_spaces() - Changes every tab to a space
//==========================================================================================================
static void convert_tabs_to_spaces(char* in)
{
    while (*in)
    {
        if (*in == 9) *in = ' ';
        ++in;
    }
}
//==========================================================================================================




//==========================================================================================================
// main() - This is the entry point of the new thread when it spawns
//==========================================================================================================
void CCmdServerBase::main(void* p1, void* p2, void* p3)
{

wait_for_connection:

    char buffer[512];

    // If we're in verbose mode, tell the user that we're creating a server
    if (m_verbose) printf("Creating server on port %i\n", m_port);;
    
    // Create the server
    if (!m_socket.create_server(m_port))
    {
        fprintf(stderr, "Can't create server on port %i", m_port);
        exit(1);
    }

    // Wait for a client to connect to us
    m_socket.listen_and_accept();

    // If we're in verbose mode, tell the user that we have a connection
    if (m_verbose) printf("A client has connected from %s\n", m_socket.get_peer_address().c_str());

    // While the socket is open, read and handle input lines
    while (m_socket.get_line(buffer, sizeof buffer))
    {
        // Convert tabs to spaces
        convert_tabs_to_spaces(buffer);

        // Parse the buffer into a vector of tokens
        vector<string> tokens = parse_tokens(buffer);

        // If the line is empty, ignore it
        if (tokens.size() == 0) continue;

        // If we're in verbose mode, show the user the line we just received
        if (m_verbose) printf("> %s\n", buffer);

        for (int i=0; i<3; ++i)
        {
            m_socket.sendf("I see %s\n", buffer);
            printf("Counting %i\n", i);
            usleep(1000000);
        }
    }

    // The socket has closed.   If we're in verbose mode, tell the user
    if (m_verbose) printf("Socket closed by client\n");
    
    // And go back to waiting for a connection
    goto wait_for_connection;
}
//==========================================================================================================


//==========================================================================================================
// start() - Starts the server
//==========================================================================================================
void CCmdServerBase::start(cmd_server_t* p_params)
{
    // Fill in the parameter fields
    m_verbose = p_params->verbose;
    m_port    = p_params->port; 

    // And spawn the server
    spawn();   
}
//==========================================================================================================
