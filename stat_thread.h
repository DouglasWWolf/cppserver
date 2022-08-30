//==========================================================================================================
// stat_thread.h - A thread that generates ".stat" events on the server every time a specific file
//                 is either created or deleted
//==========================================================================================================
#pragma once
#include "cthread.h"


class CStatThread : public CThread
{
    protected: void main();   
};

