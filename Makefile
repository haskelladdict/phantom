# Declaration of variables
CC = g++-5
CFLAGS = -std=c++14 -O0 -ggdb -Wall
LDFLAGS = -lssl -lcrypto -lpthread

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
