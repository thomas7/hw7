#include "Process.h"

Process::Process(const std::vector<std::string> & args) {
    try {
        pipe(readpipe); //parents read pipe child writes to
        pipe(writepipe); //parents write pipe child reads from
        m_pid = fork();
        m_name = args[0];
        const char* name = m_name.c_str();

        m_pread = fdopen(readpipe[0], "r");

        if (m_pread == NULL)
            throw std::runtime_error(strerror(errno)); //throw error in opening the file

        std::vector<const char *> cargs;
        std::transform(args.begin(), args.end(), std::back_inserter(cargs),
                       []( const std::string s) { return s.c_str();} );
        cargs.push_back(NULL); // exec uses a NULL terminated array

        if(m_pid == 0) { // this is the child process
            int error = dup2 (readpipe[1], 1); //child writes to write end so parent can read it
            if (error < 0)
                std::cerr << strerror(errno); //dup2 failed, send error to stderr

            error = close (readpipe[0]); //close the read end of the pipe since child won't be using it
            if (error < 0)
                std::cerr << strerror(errno); //close failed, send error to stderr

            error = dup2 (writepipe[0],0); //child reads from pipe that parent writes to
            if (error < 0)
                std::cerr << strerror(errno); //dup2 failed, send error to stderr

            error = close (writepipe[1]);
            if (error < 0)
                std::cerr << strerror(errno); //close failed, send error to stderr

            error = execve(name, const_cast<char**>(&cargs[0]), NULL);
            if (error < 0)
                std::cerr << strerror(errno); //exec failed, send error to stderr
        }

        else if(m_pid < 0)
            throw std::runtime_error(strerror(errno)); //fork failed somehow throw errno

        else {
            int error = close(readpipe[1]);
            if (error < 0)
                throw std::runtime_error(strerror(errno)); //close failed, send error to stderr

            error = close(writepipe[0]);
            if (error < 0)
                throw std::runtime_error(strerror(errno)); //close failed, send error to stderr
        }
    }
    catch (const char *err){
        std::cerr << err;
    }
}

Process::~Process(){
    if (m_pid ==0) {
        close (readpipe[1]); //close file discriptors upon destruction
        close (writepipe[0]);
        kill(m_pid, SIGTERM); //make sure the child pid has been terminated
    }
}

void Process::write(const std::string& line) {
    int error = ::write(writepipe[1], line.c_str(), strlen(line.c_str()));
    if (error < 0)
        throw std::runtime_error(strerror(errno)); //there was an error in writing
}

std::string Process::readline() {
    char *line = NULL;
    size_t buffer[100];
    int error = getline(&line, buffer, m_pread);
    if (error < 0)
        throw std::runtime_error(strerror(errno)); //there was an error in getting a line
    return std::string(line);
}
