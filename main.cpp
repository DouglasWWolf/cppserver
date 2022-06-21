//==========================================================================================================
// cppserver - A demonstration of the CCmdServerBase class
//==========================================================================================================
#include "cmd_server_base.h"

CCmdServerBase  sv;

cmd_server_t server_params;

int main()
{
    server_params.verbose = true;
    server_params.port    = 12345;

    sv.start(&server_params);
    sv.join();

}

