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

            const regex r("(.*)#(.*)#(.*)#(.*)#(.*)");
            smatch sm;

            if (regex_search(line, sm, r))
            {
                cout << sm[1];
                if (sm[1] == 'm')
                {
                    cout << "here\n";
                    std::vector<std::string> temp;

                    temp.push_back(sm[3]);
                    temp.push_back(sm[4]);
                    temp.push_back(sm[5]);

                    questions.push_back(Question('m', sm[2], temp));
                }
            }
        }
    }
    else
    {
        cout << "faile\n";
    }
    size = questions.size();
    inpFile.close();

    return 0;
}

string QuestionBank::getFileName()
{
    return fileName;
}

Question QuestionBank::getQuestion(int n)
{
    if (n >= 0 && n < questions.size())
        return questions[n];
    else throw out_of_range("Index out of bounds!");
}

int  QuestionBank::getSize(){
    return size;
}

/*******************QUESTION CLASS************************/
Question::Question(char t, std::string s, std::vector<std::string> o)
{
    type = t;
    question = s;
    options = o;
}
char Question::getType()
{
    return type;
}
std::string Question::getQString()
{
    return question;
}
std::vector<std::string> Question::getOptions()
{
    return options;
}

// int main()
// {
//     QuestionBank x("./resources/questions.data");
//     cout << x.getQuestion(0).getQString() << "\n";
//     cout << x.getQuestion(1).getQString() << "\n";
//     cout << x.getQuestion(2).getQString() << "\n";
//     cout << x.getFileName() << '\n';
// }
