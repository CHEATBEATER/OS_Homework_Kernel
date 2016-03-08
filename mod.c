#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

void *load_file(const char*fn, unsigned *_sz);
static int insmod(const char *filename);
static int rmmod(const char *modname);
void usage(void);

int main(int argc, char *argv[]) {
    const char *action = argv[1];
    const char *filename = argv[2];
    if ( (argc != 3) || (strcmp(action, "-i") && strcmp(action, "-r")) ){
        usage();
    }
    if (!strcmp(action, "-i")) {
        insmod(filename);
    } else {

        rmmod(filename);
    }
    return 0;
}

static int insmod(const char *filename) {
    void *module;
    unsigned int size;
    int ret;

    module = load_file(filename, &size);
    if (!module) return -1;

    ret = init_module(module, size, "");

    free(module);

    return ret;
}

static int rmmod(const char *modname) {
    int ret = -1;
    int maxtry = 10;

    while (maxtry--) {
        ret = delete_module(modname, O_NONBLOCK | O_EXCL);

        if (ret < 0 && errno == EAGAIN)
            usleep(50000);
        else
            break;
    }

    if (ret != 0)
        printf("Unable to uload driver module \"%s\": %s\n",
                modname, strerror(errno));

    return ret;
}

void *load_file(const char *fn, unsigned *_sz) {
    char *data;
    int sz;
    int fd;

    data = 0;
    fd = open(fn, O_RDONLY);
    if (fd < 0) return 0;

    sz = lseek(fd, 0, SEEK_END);
    if (sz < 0) goto oops;

    if (lseek(fd, 0, SEEK_SET) != 0) goto oops;

    data = (char*) malloc (sz + 1);
    if (data == 0) goto oops;

    if (read(fd, data, sz) != sz) goto oops;
    close(fd);
    data[sz] = 0;

    if(_sz) *_sz = sz;
    return data;

oops:
    close(fd);
    if (data != 0) free(data);
    return 0;
}

void usage(void) {

    fprintf(stderr,
            "usage: mod -i[r] module_name\n"
            "\n"
            "examples:\n"
            " 1.mod -i module_name # install module to kernel\n"
            " 2.mod -r module_name # remove module from kernel\n"
            "\n"
           );
    exit(1);
}

