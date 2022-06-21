//==========================================================================================================
// demo_server.h - Defines a TCP command-line server
//==========================================================================================================
#include "demo_server.h"
using namespace std;


void CDemoServer::handle_command()
{

    string cmd = m_line.get_cmd();

    if      (cmd == "add") handle_add();
    else if (cmd == "sub") handle_sub();
    else if (cmd == "mul") handle_mul();
    else fail_syntax();

}


//==========================================================================================================
// handle_add() - Adds two numbers and reports the result
//==========================================================================================================
void CDemoServer::handle_add()
{
    int v1, v2;

    // Fetch the command parameters as integers
    m_line.get_next(&v1);
    m_line.get_next(&v2);

    // Perform the operation
    int result = v1 + v2;

    // Report the result to the caller
    pass("%i", result);
}
//==========================================================================================================



//==========================================================================================================
// handle_sub() - Subtracts two integers and reports the result
//==========================================================================================================
void CDemoServer::handle_sub()
{
    int v1, v2;

    // Fetch the command parameters as integers
    m_line.get_next(&v1);
    m_line.get_next(&v2);

    // Perform the operation
    int result = v1 - v2;

    // Report the result to the caller
    pass("%i", result);
}
//==========================================================================================================




//==========================================================================================================
// handle_mul() - multiples two floating-point numbers and reports the result
//==========================================================================================================
void CDemoServer::handle_mul()
{
    double v1, v2;

    // Fetch the command parameters as integers
    m_line.get_next(&v1);
    m_line.get_next(&v2);

    // Perform the operation
    double result = v1 * v2;

    // Report the result to the caller
    pass("%1.4lf", result);
}
//==========================================================================================================










