// vmachine.cpp -- wrapper to fake virtual machine for client

#include "vmachine.h"
#pragma warning (disable : 4514)
/*
==============================================================

VIRTUAL MACHINE

==============================================================
*/
int	VM_Call( int callnum, ... )
{
//	assert (cgvm.entryPoint);
	
	if (cgvm.entryPoint)
	{
        int args[10];
        args[0] = callnum;

        va_list argList;
        va_start(argList, callnum);
        for (int i = 1; i<10; i++)
        {
            args[i] = va_arg(argList, int);
        }

        va_end(argList);


        return cgvm.entryPoint( args[0], args[1], args[2], args[3],
            args[4], args[5], args[6], args[7],
            args[8],  args[9] );
	}
	
	return -1;
}

/*
============
VM_DllSyscall

we pass this to the cgame dll to call back into the client
============
*/
extern int CL_CgameSystemCalls( int *args );
extern int CL_UISystemCalls( int *args );

int VM_DllSyscall( int arg, ... ) {
    int args[16];
    args[0] = arg;

    va_list argList;
    va_start(argList, arg);
    for (int i = 1; i<16; i++)
    {
        args[i] = va_arg(argList, int);
    }

    va_end(argList);

    return CL_CgameSystemCalls(args);

    //return CL_CgameSystemCalls( &arg );
}
