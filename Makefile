#Source: https://github.com/evanugarte/mongocxx-tutorial/blob/09dc4bf76d57fe40cf7154a8eb9e7530d49ab536/Makefile
CXX		  := g++
CXX_FLAGS := -Wall -Wextra -std=c++17 -ggdb -Wno-unused-parameter -Wno-deprecated-declarations

BIN		:= bin
SRC		:= src/*.cpp $(shell find $(lib) -name *.cpp)
INCLUDE	:= -Iinclude -Ilib \
 -I/usr/local/include/mongocxx/v_noabi \
 -I/usr/local/include/bsoncxx/v_noabi
LIB		:= lib

LIBRARIES	:=  -lpthread -lcrypto \
-lssl  -lbsoncxx -lcurl -lboost_system -lmongocxx	# TODO fix this
EXECUTABLE	:= main


all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)
	$(CXX) $(CXX_FLAGS) $(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES)

clean:
	-rm $(BIN)/*