//
//  microshell2.h
//  
//
//  Created by Marian Such on 8/5/13.
//
//

#ifndef _microshell2_h
#define _microshell2_h

#include <stdbool.h>

extern SerialUSBDriver SDU2;
#define chprint(...) chprintf((BaseSequentialStream *)&SDU2, __VA_ARGS__)

void start_shell(void);

typedef void (*shellcmd_t)(int argc, char * argv[]);

typedef struct {
    const char        *sc_name;
    shellcmd_t        sc_function;
} ShellCommand;


#endif
