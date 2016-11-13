#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

void __attribute__((destructor(65535))) list_open_fds()
{
    const char *output_path = getenv("FD_OUTPUT_PATH");

    FILE *out    = output_path != NULL ? fopen(output_path, "w") : stdout;
    DIR  *fd_dir = opendir("/proc/self/fd/");

    if (out != NULL && fd_dir != NULL)
    {
        struct dirent *fd_entry;

        // skip '.' and '..'
        readdir(fd_dir);
        readdir(fd_dir);

        while ((fd_entry = readdir(fd_dir)) != NULL)
        {
            int fd = atoi(fd_entry->d_name);

            if (fd == dirfd(fd_dir))
            {
                // skip '/proc/self/fd/'
                continue;
            }

            char symlink_name[PATH_MAX];
            if (snprintf(symlink_name, PATH_MAX, "/proc/self/fd/%d", fd) > 0)
            {
                char real_name[PATH_MAX];
                if (readlink(symlink_name, real_name, PATH_MAX) == -1)
                {
                    sprintf(real_name, "<unknown>");
                }

                fprintf(out, "Open file descriptor %d: %s\n", fd, real_name);
            }
        }
    }

    if (fd_dir != NULL) { closedir(fd_dir); }
    if (out != NULL) { fclose(out); }
}

