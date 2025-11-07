CXX = g++
CXXFLAGS = -c -Wall -std=c++17

TARGET = lab10

all: $(TARGET)

$(TARGET): 348_Lab10.o
	$(CXX) 348_Lab10.o -o $(TARGET)

348_Lab10.o: 348_Lab10.cpp
	$(CXX) $(CXXFLAGS) 348_Lab10.cpp

clean:
	rm -f *.o $(TARGET)