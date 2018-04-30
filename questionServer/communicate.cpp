#include "server.h"

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

using namespace std;
void action(int sock)
{
    int n;
    char buffer[BUFFER_STD];
    // Write client's message to buffer
    //TODO
    // Needs an upgrade, need to get bytes rather than chars.
    std::cout << "Here is the message: ";
    while (1)
    {
        bzero(buffer, BUFFER_STD);
        // Write client's message to buffer
        //TODO
        // Needs an upgrade, need to get bytes rather than chars.
        if ((n = read(sock, buffer, BUFFER_STD-1)) < 0)
            error("ERROR reading from socket");

        cout << buffer;

        //Write to client
        if ((n = write(sock, buffer, n)) < 0)
            error("ERROR writing to socket");
        if (n == BUFFER_STD-1)
        {
            if (buffer[n + 1] != '\0')
            {
                continue;
            }
        }
        break;
    }
    cout << endl;
}
