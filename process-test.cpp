#include <cstdlib>
#include <libgen.h> // for basename
#include <iostream>
#include <string>

#include "Process.h"

int main(int argc, char *argv[])
{
    { //begin scope
	if (argc < 2) {
	    std::cerr << "Usage: " << basename(argv[0]) << " path [ARG1,...]\n"
		"Example: " << basename(argv[0]) << " ./consumer\n"
		"Example: " << basename(argv[0]) << " ./consumer -v\n";
	    return 1;
	}
	std::vector<std::string> pargs;
	pargs.assign(&argv[1], &argv[1]+argc-1);
	for ( auto arg : pargs ) {
	    std::cerr << arg << ", ";
	}
	std::cerr << std::endl;
	Process p(pargs);
    
	std::string line;
	while(getline(std::cin, line)) {
        p.writeline(line+"\n");
	    std::string pread = p.readline();
	    std::cout << "Line from process[" << p.pid() << "]: " << pread;
	}
    } //end scope
    std::cout << "Process object has gone out of scope.\n"
"Check that the child process has been cleaned up and "
"all unused file descriptors for this process [" << getpid() << "] are closed.\n"
"Press Ctrl+C to exit" << std::endl;
    while(1)
    {
	sleep(1);
    }
    return EXIT_SUCCESS;
}
