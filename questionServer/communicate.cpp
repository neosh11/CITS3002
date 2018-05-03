#include "server.h"
#include <sstream>
#include <array>

using namespace std;

void printStr(char *str, int n)
{
    for (int i = 0; i < n; i++)
        cout << str[i];
}
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
            result += buffer.data();
    }
    return result;
}

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
        if ((n = read(sock, buffer, BUFFER_STD - 1)) < 0)
            error("ERROR reading from socket");

        cout << buffer;

        //Write to client
        if ((n = write(sock, buffer, n)) < 0)
            error("ERROR writing to socket");

        if (n != BUFFER_STD - 1)
            break;
    }
    cout << endl;
}

bool sendContent(int sock, string content, int type)
{
    int n;
    int size = (content.length() + BUFFER_STD - 1) / BUFFER_STD;
    char temp[BUFFER_STD];

    if ((n = write(sock, &size, 4)) < 0)
        error("ERROR writing to socket");

    if ((n = write(sock, &type, 4)) < 0)
        error("ERROR writing to socket");

    for (int i = 0; i < size - 1; i++)
    {
        for (int j = 0; j < BUFFER_STD; j++)
            temp[j] = content.at(i * BUFFER_STD + j);
        if ((n = write(sock, temp, 4)) < 0)
            error("ERROR writing to socket");
    }
    int start = (size - 1) * BUFFER_STD;
    int remains = content.length() - (size - 1) * BUFFER_STD;
    for (int j = 0; j < remains; j++)
        temp[j] = content.at(j + start);
    if ((n = write(sock, temp, remains)) < 0)
        error("ERROR writing to socket");
    return true;
}

void action2(int sock)
{
    int n;
    int size_buff;
    int type_buff;
    char content_buff[BUFFER_STD + 1];
    string content = "";
    /********GET SIZE FROM HEADER********/
    if ((n = read(sock, &size_buff, 4)) < 0)
        error("ERROR reading from socket");

    if (n == 0)
    {
        return;
    }

    /********GET TYPE FROM HEADER********/
    if ((n = read(sock, &type_buff, 4)) < 0)
        error("ERROR reading from socket");
    // if(n==0){}

    for (int i = 0; i < size_buff; i++)
    {
        bzero(content_buff, BUFFER_STD);
        if ((n = read(sock, content_buff, BUFFER_STD)) < 0)
            error("ERROR reading from socket"); //remove
        content_buff[BUFFER_STD] = '\000';
        content += content_buff;

        // if(n==0){}
    }

    bool sent = false;
    // Ask for resource

    if (type_buff == 1)
    {
        const regex r("(.*)#(.*)");
        smatch sm;

        if (regex_search(content, sm, r))
        {
            if (sm[1] == "question" && sm[2] != "")
            {
                int num = stoi(sm[2]);
                if (0 < num && num < questionBank.getSize())
                    sent = sendContent(sock, questionBank.getQuestion(num).getQString(), 1);
            }
            if (sm[1] == "neosh")
            {
                sent = sendContent(sock, exec("./olives 1"), 1);
            }
        }
    }

    if (!sent)
        sendContent(sock, "BAD REQUEST", 1);
}
