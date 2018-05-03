#CITS3002 Project 2018
#Names:             Neosh Sheikh, Jeremiah Pinto, Cameron Wright, 
#Student number(s):   21959462, xxxxxxxx, 21958958
 
PROJECT = server
HEADER = server.h globals.h QuestionBank.h
OBJ = server.o communicate.o encrypt.o QuestionBank.o

G++ = g++
FLAGS = -Wall -pedantic -Werror 
OPENSSL = -L/usr/local/lib -lssl -lcrypto

$(PROJECT) : $(OBJ)
	$(G++) $(FLAGS) -o $(PROJECT) $(OBJ) $(OPENSSL)
%.o : %.cpp $(HEADER)
	$(G++) $(FLAGS) -c $<

clean :
	rm -f $(PROJECT) $(OBJ)