#include "nm.h"

int main(int ac, char **av) {
    int i = 0;
    while (++i < ac) {
        t_file_info *file_info = parse_arg(av[i]);
        if (file_info == NULL) continue;
        process_elf64(file_info);

        munmap(file_info->map, file_info->size);
        close(file_info->fd);
        free(file_info);
    }
}