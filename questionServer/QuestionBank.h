#include <string>

class QuestionBank
{
    public:
    QuestionBank(std::string);
    
    // Getters
    std::string getFileName();
    std::string getQuestion(int);

    // setters
    int setFile(std::string);
    
    // Destructor
    ~QuestionBank();
    private:
    std::string fileName;

};