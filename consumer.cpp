#include <string.h>
#include <unistd.h>
#include <iostream>

int main(int argc, char *argv[]) {
    double num;
    double total=0;
    size_t lines=0;

    if (argc > 1 && strcmp(argv[1],"-v") == 0)
	std::cerr << "Being verbose\n";

    while(std::cin >> num) {
	std::cout << "Consumer[" << getpid() << "] received line: " << num << std::endl;
	total += num;
	++lines;
    }
    std::cout << total << "/" << lines << "=" << total/lines << std::endl;
}
