#CITS3002 Project 2018
#Names:             Neosh Sheikh, Jeremiah Pinto, Cameron Wright, 
#Student number(s):   21959462, xxxxxxxx, 21958958
 
PROJECT = server
HEADER = server.h globals.h QuestionBank.h
OBJ = server.o communicate.o ssl.o QuestionBank.o serverImp.o

G++ = g++ -std=c++11
FLAGS = -Wall -pedantic -Werror 
OPENSSL = -L/usr/local/lib
OPENSSLFLAS = -lssl -lcrypto

$(PROJECT) : $(OBJ)
	$(G++) $(FLAGS) -o $(PROJECT) $(OBJ) $(OPENSSL) $(OPENSSLFLAS)
%.o : %.cpp $(HEADER)
	$(G++) $(FLAGS) -c $<

clean :
	rm -f $(PROJECT) $(OBJ)