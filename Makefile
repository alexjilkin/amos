CXX      = g++
CXXFLAGS = -std=c++17 -O2 -Wall -I src
TARGET   = bin/main
SRC      = src/main.cpp src/amo.h src/AMOCache.h src/Graph.h

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) src/main.cpp -o $(TARGET)

clean:
	rm -f $(TARGET)
