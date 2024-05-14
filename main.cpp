#include <iostream>
#include <unistd.h>
#include <sched.h>
#include <fcntl.h>
#include <cstdlib>
#include <cstring>
#include <errno.h>

bool fileExists(const char *filename) {
    return access(filename, F_OK) != -1;
}

int main(int argc, char *argv[]) {
    if (argc != 3 || std::strcmp(argv[1], "--affinity") != 0) {
        std::cerr << "Usage: " << argv[0] << " --affinity <cpu_number>" << std::endl;
        return 1;
    }

    int cpu_core = std::atoi(argv[2]);

    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(cpu_core, &cpuset);
    if (sched_setaffinity(0, sizeof(cpuset), &cpuset) == -1) {
        perror("sched_setaffinity");
        return 1;
    }

    int trace_fd = open("/sys/kernel/debug/tracing/tracing_on", O_WRONLY);
    if (trace_fd == -1) {
        perror("Opening tracing_on");
        return 1;
    }
    if (write(trace_fd, "1", 1) == -1) {
        perror("Writing to tracing_on");
        close(trace_fd);
        return 1;
    }
    close(trace_fd);

    if (fileExists("/sys/kernel/debug/tracing/trace_clock")) {
        int trace_clock_fd = open("/sys/kernel/debug/tracing/trace_clock", O_WRONLY);
        if (trace_clock_fd == -1) {
            perror("Opening trace_clock");
            return 1;
        }
        if (write(trace_clock_fd, "local_clock_us", 14) == -1) {
            if (errno == EINVAL) {
                std::cerr << "local_clock_us not supported, using default trace clock" << std::endl;
            } else {
                perror("Writing to trace_clock");
                close(trace_clock_fd);
                return 1;
            }
        }
        close(trace_clock_fd);
    } else {
        std::cerr << "Trace clock file not found, using default trace clock" << std::endl;
    }

    int tracer_fd = open("/sys/kernel/debug/tracing/current_tracer", O_WRONLY);
    if (tracer_fd == -1) {
        perror("Opening current_tracer");
        return 1;
    }
    if (write(tracer_fd, "function", 8) == -1) { 
        perror("Writing to current_tracer");
        close(tracer_fd);
        return 1;
    }
    close(tracer_fd);

    int trace_pipe_fd = open("/sys/kernel/debug/tracing/trace_pipe", O_RDONLY);
    if (trace_pipe_fd == -1) {
        perror("Opening trace_pipe");
        return 1;
    }

    char buffer[4096];
    ssize_t bytes_read;
    while ((bytes_read = read(trace_pipe_fd, buffer, sizeof(buffer))) > 0) {
        write(STDOUT_FILENO, buffer, bytes_read);
    }
    close(trace_pipe_fd);

    trace_fd = open("/sys/kernel/debug/tracing/tracing_on", O_WRONLY);
    if (trace_fd == -1) {
        perror("Opening tracing_on");
        return 1;
    }
    if (write(trace_fd, "0", 1) == -1) {
        perror("Writing to tracing_on");
        close(trace_fd);
        return 1;
    }
    close(trace_fd);

    return 0;
}