## VERSION 1
# sylar: main.cpp config.cpp log.cpp util/util.cpp 
# 	g++ -o sylar main.cpp config.cpp log.cpp util/util.cpp -std=c++11 -lyaml-cpp

##VERSION 2
# CXX = g++
# TARGET = main
# OBJ = lib/main.o lib/log.o lib/util.o lib/config.o

# $(TARGET): $(OBJ)
# 	$(CXX) -o $(TARGET) $(OBJ) -std=c++11 -lyaml-cpp

# lib/main.o: main.cpp
# 	$(CXX) -c main.cpp -o lib/main.o -std=c++11

# lib/log.o: log.cpp
# 	$(CXX) -c log.cpp -o lib/log.o -std=c++11

# lib/util.o: util/util.cpp
# 	$(CXX) -c util/util.cpp -o lib/util.o -std=c++11

# lib/config.o: config.cpp
# 	$(CXX) -c config.cpp -o lib/config.o -std=c++11



##VERSION 3
# CXX = g++
# TARGET = main-sylar
# OBJ = lib/main.o lib/log.o lib/util.o lib/config.o

# CXXFLAGS = -c -Wall -std=c++11

# $(TARGET): $(OBJ)
# 	$(CXX) -o $@ $^ -std=c++11 -lyaml-cpp

# lib/%.o: %.cpp
# 	$(CXX) $(CXXFLAGS) $< -o $@

# lib/%.o: util/%.cpp
# 	$(CXX) $(CXXFLAGS) $< -o $@

# .PHONY: clean
# clean: 
# 	rm -rf lib/*.o $(TARGET)


##VERSION 4
CXX = g++
TARGET = main
SRC = $(wildcard *.cpp)
OBJ = $(patsubst %.cpp, %.o,$(SRC))

CXXFLAGS = -c -Wall -std=c++11

$(TARGET): $(OBJ)
	$(CXX) -o ../bin/$@ ../lib/*.o -std=c++11 -lyaml-cpp

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $< -o ../lib/$@

.PHONY: clean
clean: 
	rm -rf ../lib/*.o $(TARGET)
