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
    Question(std::string, std::vector<std::string>, int);

    std::string getQString();
    std::vector<std::string> getOptions();
    int getAns();

    private:
    std::string question;
    std::vector<std::string> options;
    int answer;
};

class ProgQuestion
{
    public:
    ProgQuestion(std::string, std::string, int);

    std::string getQString();
    std::string getFunction();
    int getAnsFile();

    private:
    std::string question;
    std::string function;
    int answerFile;
};

class QuestionBank
{
    public:
    QuestionBank(std::string);
    
    // Getters
    std::string getFileName();
    Question getQuestion(int);
    int getSize();

    ProgQuestion getProgQuestion(int);
    int getProgSize();

    // setters
    int setFile(std::string);
    
    // Destructor
    ~QuestionBank();
    private:
    std::string fileName;
    std::vector<Question> questions;
    std::vector<ProgQuestion> progQuestions;
    int size;
    int psize;

};

#endif
