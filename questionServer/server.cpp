/* A simple server in the internet domain using TCP
   The port number is passed as an argument */

#include "server.h"

using namespace std;

QuestionBank questionBank = QuestionBank("./resources/questions.data");

int main(int argc, char *argv[])
{
    //Kill Zombies!!
    /*******Signal catcher for killing zombies********/
    signal(SIGCHLD, SIG_IGN);
    init_openssl();

    Server x(PORT);
    x.init();
    return x.run();
}