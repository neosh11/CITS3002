#include "server.h"

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

void action(int sock)
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
                sent = sendContent(sock, "elmo", 1);
            }
        }
    }

    if (!sent)
        sendContent(sock, "BAD REQUEST", 1);
}

bool sendContent(SSL *ssl, string content, int type)
{
    int n;
    int size = (content.length() + BUFFER_STD - 1) / BUFFER_STD;
    char temp[BUFFER_STD];

    if ((n = SSL_write(ssl, &size, 4)) < 0)
        error("ERROR writing to socket");

    if ((n = SSL_write(ssl, &type, 4)) < 0)
        error("ERROR writing to socket");

    for (int i = 0; i < size - 1; i++)
    {
        for (int j = 0; j < BUFFER_STD; j++)
            temp[j] = content.at(i * BUFFER_STD + j);
        if ((n = SSL_write(ssl, temp, 4)) < 0)
            error("ERROR writing to socket");
    }
    int start = (size - 1) * BUFFER_STD;
    int remains = content.length() - (size - 1) * BUFFER_STD;
    for (int j = 0; j < remains; j++)
        temp[j] = content.at(j + start);
    if ((n = SSL_write(ssl, temp, remains)) < 0)
        error("ERROR writing to socket");
    return true;
}

std::string routeResponse(std::string content)
{
    std::string json = "";
    smatch sm;

    if (regex_search(content, sm, regex("(.*)#(.*)#(.*)")))
    {

        if (sm[1] == "mark" && sm[2] != "" && sm[3] != "")
        {
            int qnum = stoi(sm[2]);
            int ans = stoi(sm[3]);
            if (0 <= ans && ans < 4 && 0 <= qnum && qnum < questionBank.getSize())
            {
                char v = 'F';
                if (questionBank.getQuestion(qnum).getAns() == stoi(sm[3]))
                    v = 'T';

                json = "{\"value\":\"" + std::string(1, v) + "\"}";
            }
        }
    }
    else if (regex_search(content, sm, regex("(.*)#(.*)")))
    {
        if (sm[1] == "question" && sm[2] != "")
        {
            int num = stoi(sm[2]);
            if (0 <= num && num < questionBank.getSize())
            {

                json = "{";
                json += "\"id\":\"" + string(sm[2]) + "\"";
                json += ",";
                json += "\"question\":\"" + questionBank.getQuestion(num).getQString() + "\"";
                json += ",";
                //options
                json += "\"options\":";
                json += "[";
                std::vector<std::string> op = questionBank.getQuestion(num).getOptions();
                unsigned int i;
                for (i = 0; i < op.size() - 1; i++)
                {
                    json += "\"" + op[i] + "\"" + ",";
                }
                json += "\"" + op[i] + "\"";
                json += "]";
                json += "}";
            }
        }
    }
    else if (content == "neosh")
        json = "HELLO DARKNESS MY OLD FRIEND, IVE COME TO BE WITH YOU AGAIN";
    else if (content == "size")
        json = std::to_string(questionBank.getSize());

    return json;
}

void actionSSL(SSL *ssl)
{
    int n;
    int size_buff;
    int type_buff;
    char content_buff[BUFFER_STD + 1];
    string content = "";
    std::string json = "";
    /********GET SIZE FROM HEADER********/
    if ((n = SSL_read(ssl, &size_buff, 4)) < 0)
        error("ERROR reading from socket");

    if (n == 0)
    {
        return;
    }

    /********GET TYPE FROM HEADER********/
    if ((n = SSL_read(ssl, &type_buff, 4)) < 0)
        error("ERROR reading from socket");
    // if(n==0){}

    for (int i = 0; i < size_buff; i++)
    {
        bzero(content_buff, BUFFER_STD);
        if ((n = SSL_read(ssl, content_buff, BUFFER_STD)) < 0)
            error("ERROR reading from socket"); //remove
        content_buff[BUFFER_STD] = '\000';
        content += content_buff;

        // if(n==0){}
    }

    // Ask for resource
    if (type_buff == 1)
    {
        json = routeResponse(content);
    }

    if (json == "")
        sendContent(ssl, "BAD REQUEST", 1);
    else
        sendContent(ssl, json, 1);
}
