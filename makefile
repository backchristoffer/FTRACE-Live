CC = g++
CFLAGS = -std=c++11 -pthread
SRC = main.cpp
OBJ = $(SRC:.cpp=.o)
EXEC = ftrace_live

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
		rm -f $(OBJ) $(EXEC)
