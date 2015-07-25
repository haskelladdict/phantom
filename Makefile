# Declaration of variables
CC = g++-5
CFLAGS = -std=c++14 -O0 -ggdb -Wall -fsanitize=shift -fsanitize=integer-divide-by-zero -fsanitize=unreachable -fsanitize=null -fsanitize=signed-integer-overflow -fsanitize=bounds -fsanitize=float-divide-by-zero -fsanitize=bool -fsanitize-undefined-trap-on-error -fsanitize=address -fsanitize=undefined
LDFLAGS = -lssl -lcrypto -lpthread -lasan

# File names
EXEC = phantom
SOURCES = $(wildcard *.cpp)
HEADERS = $(wildcard *.hpp)
OBJECTS = $(SOURCES:.cpp=.o)

# Main target
$(EXEC): $(OBJECTS) $(HEADERS)
	$(CC) $(OBJECTS) -o $(EXEC) $(LDFLAGS)

# To obtain object files
%.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@

# To remove generated files
.PHONY: clean

clean:
	rm -f $(EXEC) $(OBJECTS)
