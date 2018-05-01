#include "QuestionBank.h"
#include <iostream>

using namespace std;

QuestionBank::QuestionBank(string file_name)
{
    cout << "Creating obj for " << file_name << '\n';
    fileName = string(file_name);
}
QuestionBank::~QuestionBank()
{
    cout << "Destroying object" << fileName <<'\n';
}

int QuestionBank::setFile(std::string file_name)
{
    return 0;
}
string QuestionBank::getFileName()
{
    return fileName;
}

int main()
{
    QuestionBank x("hello");
    cout << x.getFileName() << '\n';
}