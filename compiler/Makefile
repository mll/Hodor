all: parser

SRC=$(wildcard AST*.cpp)
OBJS = $(SRC:.cpp=.o)
LLVM_MODULES = 

CPPFLAGS =  -g
LDFLAGS = 
LIBS = 

clean:
	$(RM) -rf parser.cpp parser.hpp ../scripts/parser tokens.cpp $(OBJS) *.o

parser.cpp: parser.y
	bison --locations -d  -o $@ $^
	
parser.hpp: parser.cpp

tokens.cpp: tokens.l parser.hpp
	flex -o $@ $^

%.o: %.cpp tokens.cpp
	g++ -c $(CPPFLAGS) $<


parser: $(OBJS) parser.o tokens.o main.o hodor.o 
	g++ -o ../scripts/$@ $(LDFLAGS) $(OBJS) parser.o tokens.o main.o hodor.o $(LIBS)


