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

// Function to execute command and return output
// Used to run short answer's solution and user's answer
// Taken from:
// https://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c-using-posix
string exec(const char* cmd) 
{
    array<char, BUFFER_SIZE> buffer;
    string result;
    shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);

    if (!pipe) 
        throw runtime_error("popen() failed!");

    while (!feof(pipe.get())) 
    {
        if (fgets(buffer.data(), BUFFER_SIZE, pipe.get()) != nullptr)
            result += buffer.data();
    }

    return result;
}

// A function to create a file for the user's answer, compile and execute it and
// test it with the given solution 
string createAndRunFile(string pathname, string filename, string function, string functionName)
{
    // Checks if the user has defined main() insted of the function or used another class
    if(function.find("main()") != string::npos || function.find("#include") != string::npos)
        return "E";

    // Create file for user's answer, read through the string/file and remove any dangerous header files
    string headers = "#include <iostream>\n#include <cstring>\n#include <cmath>\n#include <cstdio>\n#include <cstdlib>\n#include <cstdbool>\n\nusing namespace std;\n";
    string intmain = "int main() {\n\n\t" + functionName + ";\n\treturn EXIT_SUCCESS;\n}";
    ofstream outfile;
    outfile.open(pathname);
    outfile << headers << endl << function << endl << intmain << std::endl;
    outfile.close();

    string compile = "g++ " + pathname + " -o " + filename; // the command to compile the file as a string
    string run = "./"+filename; // the command to run the compiled string as a string

    // exec first time to compile file
    string compiled = exec(compile.c_str());

    string answer;

    //exec second time to run file if no errors generated
    if (compiled.empty())
        answer = exec(run.c_str());
    else 
        answer = "E";

    // Delete file after it runs
    remove(pathname.c_str());
    remove(filename.c_str());

    return answer;
}

/**
 * Function to mark the user's answer with the solution
 * Returns "T", "F" or "E"
 * */
string markFunction(string function, string functionName, string solutionFile)
{
    char cwd[MAX_PATH_SIZE];

    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("CURR_DIR ERROR");
        return "E";
    }

    char new_folder[MAX_PATH_SIZE];
    
    strcpy(new_folder, cwd);    
    strcat(new_folder, "/testQuestions");

    string saved_solution = cwd;
    saved_solution += ( "/resources/" + solutionFile );

    // Read from file and save to string "solFunction"

    string line, solFunction;
    ifstream solution (saved_solution);
    if (solution.is_open())
    {
        while ( getline (solution, line) )
            solFunction = solFunction + line + "\n";
        solution.close();
    }
    else
        return "E";

    // End read file *********

    // Randomize user and solution filenames and append to path
    string user_path = new_folder, sol_path = new_folder; // path to user's answer and solution answer
    user_path += '/';
    sol_path += '/';
    string user_file = "", sol_file = "Sol"; // initialise name of user's file and solution file to randomize

    // generates a new random number everytime based on the seed given - system time
    // Once for user
    srand(time(0));

    for(int i = 0; i < 10; i++)
        user_file += 'a' + (rand() % 26);
    
    // Second for solution
    srand(time(0));

    for(int i = 0; i < 10; i++)
        sol_file += 'a' + (rand() % 26);

    user_path = user_path + user_file + ".cpp";
    sol_path = sol_path + sol_file + ".cpp";

    string userAnswer = "E", solAnswer = "E";

    // Open/create a directory called "testQuestions" in the question server to test user's file and solution file
    DIR *dir = opendir(new_folder);
    if (dir)
    {
        /* Directory exists. */
        closedir(dir);
        chdir(new_folder);

        userAnswer = createAndRunFile((string)user_path, user_file, function, functionName);
        solAnswer = createAndRunFile((string)sol_path, sol_file, solFunction, functionName);
        
        cout << userAnswer << "\n\n" << solAnswer << "\n";
    }
    else if (ENOENT == errno)
    {
        /* Directory does not exist. */
        mkdir(new_folder, 0700);
        chdir(new_folder);

        userAnswer = createAndRunFile((string)user_path, user_file, function, functionName);
        solAnswer = createAndRunFile((string)sol_path, sol_file, solFunction, functionName);

        cout << userAnswer << "\n\n" << solAnswer << "\n";
    }
    else
    {
        /* opendir() failed for some other reason. */
        perror("OPEN_DIR ERROR");
    }
    
    if (userAnswer.compare("E") == 0 || solAnswer.compare("E") == 0) // Check for errors
        return "E";
    else if(userAnswer.compare(solAnswer) == 0) // If user's answer is the same as solution, return true "T"
        return "T";
    else 
        return "F";

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
        if (sm[1] == "pmark" && sm[2] != "" && sm[3] != "")
        {
            int qnum = stoi(sm[2]);
            std::string ans = sm[3];
            if (0 <= qnum && qnum < questionBank.getProgSize())
            {
                ProgQuestion x = questionBank.getProgQuestion(qnum);
                std::string val = markFunction(ans, x.getFunction(), std::to_string(x.getAnsFile())+".c");
                json = "{\"value\":\"" + val + "\"}";
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
        else if (sm[1] == "progQuestion" && sm[2] != "")
        {
            int num = stoi(sm[2]);
            if (0 <= num && num < questionBank.getProgSize())
            {
                json = "{";
                json += "\"id\":\"" + string(sm[2]) + "\"";
                json += ",";
                json += "\"question\":\"" + questionBank.getProgQuestion(num).getQString() + "\"";
                json += ",";
                json += "\"function\":";
                json += "\"" + questionBank.getProgQuestion(num).getFunction() + "\"";
                json += "}";
            }
        }
    }
    else if (content == "neosh")
        json = "HELLO DARKNESS MY OLD FRIEND, IVE COME TO BE WITH YOU AGAIN";
    else if (content == "size")
        json = std::to_string(questionBank.getSize());
    else if (content == "psize")
        json = std::to_string(questionBank.getProgSize());

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
