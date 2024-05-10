CXX = g++
TARGET = test

SRC = $(wildcard,*.cpp)
OBJ = $(patsubst %.cpp, %.o,$(SRC))

CXXFLAGS = -c -Wall -std=c++11 -g

$(TARGET): $(OBJ)
	$(CXX) test_config.cpp -o $@ -std=c++11 -lyaml-cpp

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

.PHONY: clean
clean: 
	rm -rf *.o $(TARGET)
