CXX := g++
CPPFLAGS := -Wall -g -std=c++0x

all: process-test consumer

process-test: process-test.o Process.o
	$(CXX) $(CPPFLAGS) -o $@ $^

consumer: consumer.o
	$(CXX) $(CPPFLAGS) -o $@ $^

clean:
	-rm -f *.o
