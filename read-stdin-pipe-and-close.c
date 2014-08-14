#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <sys/wait.h>
#include <sys/types.h>

#include <uv.h>

void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t* buff)
{
    static char buffer[1 << 16];
    *buff = uv_buf_init(buffer, 1 << 16);
}

void read_stdin(uv_stream_t *stream, ssize_t nread, const uv_buf_t* buf)
{
    if (nread < 0) {
        uv_read_stop(stream);
        return;
    } else {
        printf("Read %ld bytes!\n", nread);
    }
}

int main(int argc, char *argv[])
{
    int r = -1;
    int pid;
    int fd[2];

    pipe(fd);

    if ((pid = fork()) == 0) {
        fprintf(stderr, "Child started!\n");

        // Make the read side of the pipe our stdin.
        close(fd[1]);
        close(0);
        dup(fd[0]);

        // Read from the pipe.
        uv_pipe_t stdin_pipe;

        r = uv_pipe_init(uv_default_loop(), (uv_pipe_t*)&stdin_pipe, 0);
        assert(r == 0);

        r = uv_pipe_open((uv_pipe_t*)&stdin_pipe, 0);
        assert(r == 0);

        r = uv_read_start((uv_stream_t *)&stdin_pipe, alloc_buffer, read_stdin);
        assert(r == 0);

  		uv_run(uv_default_loop(), UV_RUN_DEFAULT);

        fprintf(stderr, "Event loop exited!\n");
    } else {
        fprintf(stderr, "Parent!\n");

        // Close both ends of the pipe so that the child
        // get a POLLHUP event when it tries to read from
        // the other end.
        close(fd[1]);
        close(fd[0]);

        fprintf(stderr, "Parent done!\n");

        fprintf(stderr, "Waiting for child...\n");
        waitpid(pid, 0, 0);
    }

    return EXIT_SUCCESS;
}
