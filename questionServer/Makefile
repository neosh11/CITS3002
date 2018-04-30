 #CITS3002 Project 2018
 #Names:             Neosh Sheikh, Jeremiah Pinto, Cameron Wright, 
 #Student number(s):   21959462, xxxxxxxx, 21958958
 
PROJECT = server
HEADER = stuff.h
OBJ = server.o communicate.o

G++ = g++
FLAGS = -Wall -pedantic -Werror 

$(PROJECT) : $(OBJ)
	$(G++) $(FLAGS) -o $(PROJECT) $(OBJ)

%.o : %.c $(HEADER)
	$(G++) $(FLAGS) -c $<

clean :
	rm -f $(PROJECT) $(OBJ)