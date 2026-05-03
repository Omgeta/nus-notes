/**
 * CS3210 - Hello World in MPI
 **/
#include <iostream>
#include <mpi.h>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>

int main(int argc, char **argv)
{
    int rank, size;
    char hostname[256];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    memset(hostname, 0, sizeof(hostname));
    int sc_status = gethostname(hostname, sizeof(hostname)-1);
    if (sc_status)
    {
        perror("gethostname");
        return sc_status;
    }


    /* From here on, each process is free to execute its own code */
    std::cout << "Hello world from process " << rank << " out of " << size << " on host " << hostname << std::endl;
    MPI_Finalize();
    return 0;
}
