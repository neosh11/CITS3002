#ifndef QUESTION_BANK_H
#define QUESTION_BANK_H

#include <string>
#include <regex>
#include <iostream>
#include <fstream>
#include <vector>
#include <exception>

class Question
{
    public:
    Question(char, std::string, std::vector<std::string>);

    char getType();
    std::string getQString();
    std::vector<std::string> getOptions();

    private:
    char type;
    std::string question;
    std::vector<std::string> options;
};

class QuestionBank
{
    public:
    QuestionBank(std::string);
    
    // Getters
    std::string getFileName();
    Question getQuestion(int);
    int getSize();

    // setters
    int setFile(std::string);
    
    // Destructor
    ~QuestionBank();
    private:
    std::string fileName;
    std::vector<Question> questions;
    int size;

};

#endif
