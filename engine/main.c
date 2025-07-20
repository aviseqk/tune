#include "hdr.h"

int main(int argc, char *argv[])
{
    printf("[%d]: TUNE Engine starting up!\n", getpid());
    printf("[%d]: #args %d\n", getpid(), argc);
    exit(EXIT_SUCCESS);
}