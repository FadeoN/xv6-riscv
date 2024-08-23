#include "kernel/types.h"
#include "user/user.h"

static const int STIN = 0;
static const int STOUT = 1;
static const int STERR = 2;


void generateNumbers(int startingFrom)
{
    static const int END_LOOKUP = 35;

    for (int i = startingFrom; i < END_LOOKUP; ++i)
    {
        if (write(STOUT, &i, sizeof i) != sizeof i)
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
        close(STIN);
        dup(p[0]);
        close(p[1]);

        aRightCmd();

        close(p[0]);
    }
    else
    {
        close(STOUT);
        dup(p[1]);
        close(p[0]);

        aLeftCmd(prime);

        close(p[1]);
    }
}

void filterPrimes(int prime)
{
    int n;

    while (read(STIN, &n, sizeof n) == sizeof n)
    {

        if ((n % prime) != 0)
        {
            write(STOUT, &n, sizeof n);
        }
    }
}

void readPrimeAndFilter()
{
    int prime;
    if(read(STIN, &prime, sizeof prime) < sizeof prime){
        exit(0);
    }
    fprintf(STERR, "executing %d\n", prime);

    runWithPrime(filterPrimes, readPrimeAndFilter, prime);
}

int main(int argc, char *argv[])
{

    runWithPrime(generateNumbers, readPrimeAndFilter, 2);

    return 0;
}
