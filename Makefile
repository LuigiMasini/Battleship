
DIR=src/
FLAGS= -I $(DIR)headers/ -std=c99 -Wall -finput-charset=UTF-8


ifeq ($(OS),Windows_NT)
	TARGET = WIN
	FLAGS += -D WINDOWS
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		TARGET = LINUX
		FLAGS += -D LINUX
	endif
	ifeq ($(UNAME_S),Darwin)
		TARGET = OSX
		FLAGS += -D OSX
	endif
endif

# all:
# 	gcc -I.  main.c
# 	if [[ -eq $? 0 ]]
# 	do
# 		./a.out
# 	done
# -lcurses

all: $(TARGET)

WIN:
	$(CC) $(FLAGS) $(DIR)main.c -o battaglia_navale.exe -lwinmm

LINUX:
	$(CC) $(FLAGS) $(DIR)main.c -o battaglia_navale

OSX:
	$(CC) $(FLAGS) $(DIR)main.c -o battaglia_navale
