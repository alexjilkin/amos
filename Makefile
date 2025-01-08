# Makefile

CXX      = g++
CXXFLAGS = -std=c++17 -O2 -Wall
TARGET   = main

all: $(TARGET)

$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o $(TARGET)

clean:
	rm -f $(TARGET)
