#include "QuestionBank.h"

using namespace std;

/*******************QUESTION-BANK CLASS************************/
QuestionBank::QuestionBank(string file_name)
{
    fileName = string(file_name);
    setFile(file_name);
}
QuestionBank::~QuestionBank()
{
}

int QuestionBank::setFile(std::string file_name)
{
    ifstream inpFile;
    inpFile.open(file_name);
    if (inpFile.is_open())
    {
        while (!inpFile.eof())
        {
            string line;
            getline(inpFile, line);

            const regex r("(.*)#(.*)#(.*)#(.*)#(.*)#(.*)#(.*)");
            smatch sm;

            if (regex_search(line, sm, r))
            {
                if (sm[1] == 'm')
                {
                    std::vector<std::string> temp;

                    temp.push_back(sm[3]);
                    temp.push_back(sm[4]);
                    temp.push_back(sm[5]);
                    temp.push_back(sm[6]);

                    questions.push_back(Question(sm[2], temp, stoi(sm[7])));
                }
            }
            else if(regex_search(line, sm,  regex("(.*)#(.*)#(.*)#(.*)")))
            {
                if (sm[1] == 'p')
                {
                    progQuestions.push_back(ProgQuestion(sm[2], sm[3], stoi(sm[4])));
                }
            }
        }
    }
    else
    {
        cout << "faile\n";
    }
    size = questions.size();
    psize = progQuestions.size();
    inpFile.close();

    return 0;
}

string QuestionBank::getFileName()
{
    return fileName;
}

Question QuestionBank::getQuestion(int n)
{
    if (n >= 0 && (unsigned int)n < size)
        return questions[n];
    else throw out_of_range("Index out of bounds!");
}

ProgQuestion QuestionBank::getProgQuestion(int n)
{
    if (n >= 0 && (unsigned int)n < psize)
        return progQuestions[n];
    else throw out_of_range("Index out of bounds!");
}

int  QuestionBank::getSize(){
    return size;
}
int  QuestionBank::getProgSize(){
    return psize;
}

/*******************QUESTION CLASS************************/
Question::Question(std::string s, std::vector<std::string> o, int a)
{
    question = s;
    options = o;
    answer = a;
}

std::string Question::getQString()
{
    return question;
}
std::vector<std::string> Question::getOptions()
{
    return options;
}

int Question::getAns()
{
    return answer;
}


/*******************ProgQuestion CLASS************************/
ProgQuestion::ProgQuestion(std::string s, std::string f, int a)
{
    question = s;
    function = f;
    answerFile = a;
}

std::string ProgQuestion::getQString()
{
    return question;
}
std::string ProgQuestion::getFunction()
{
    return function;
}

int ProgQuestion::getAnsFile()
{
    return answerFile;
} 