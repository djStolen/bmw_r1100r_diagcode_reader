CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -I../src

TARGET = run_tests
SRCS = test_decoder.cpp ../firmware/reader/src/MotronicDecoder.cpp

all: $(TARGET)
	./$(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)

