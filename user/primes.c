#include "kernel/types.h"
#include "user/user.h"

void generateNumbers(int startingFrom)
{
    static const int END_LOOKUP = 35;
    int buf[1];

    for (int i = startingFrom; i < END_LOOKUP; ++i)
    {
        buf[0] = i;
        if (write(1, buf, sizeof buf) != sizeof buf)
        {
            fprintf(2, "could not write");
            exit(1);
        };
    }
}

void runWithPrime(void (*aLeftCmd)(int), void (*aRightCmd)(), int prime)
{
    int p[2];

    pipe(p);

    if (fork() == 0)
    {
        close(0);
        dup(p[0]);
        close(p[1]);

        aRightCmd();

        close(p[0]);
    }
    else
    {
        close(1);
        dup(p[1]);
        close(p[0]);

        aLeftCmd(prime);

        close(p[1]);
    }
}

void filterPrimes(int prime)
{
    int n[1];

    while (read(0, n, sizeof n) == sizeof n)
    {

        if ((n[0] % prime) != 0)
        {
            write(1, n, sizeof n);
        }
    }
}

void readPrimeAndFilter()
{
    int prime[1];
    if(read(0, prime, sizeof prime) < sizeof prime){
        exit(0);
    }
    fprintf(2, "executing %d\n", prime[0]);

    runWithPrime(filterPrimes, readPrimeAndFilter, prime[0]);
}

int main(int argc, char *argv[])
{

    runWithPrime(generateNumbers, readPrimeAndFilter, 2);

    return 0;
}
