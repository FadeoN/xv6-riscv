#include "kernel/types.h"
#include "user/user.h"

// Implement the UNIX program sleep for xv6; your sleep should pause for a user-specified number of ticks. A tick is a notion of time defined by the xv6 kernel,
// namely the time between two interrupts from the timer chip. Your solution should be in the file user/sleep.c.

int main(int argc, char *argv[])
{

    if (argc <= 1){
        fprintf(2, "sleep [tick]");
        exit(1);
    }

    if (argc == 2)
    {
        int nrTicks = atoi(argv[1]);

        int ticksLeft = sleep(nrTicks);
        if(ticksLeft != 0 ){
            fprintf(2, "Ticks left %d", ticksLeft);
            exit(1);
        }
    
    }

    exit(0);
}

git@github.com:FadeoN/xv6-riscv.git