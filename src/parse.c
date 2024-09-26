#include "nm.h"

/* TODO : CLEAN FORBIDDEN FCT   */

t_file_info *parse_arg(char *arg)
{
    t_file_info *file_info = (t_file_info *)malloc(sizeof(t_file_info));
    file_info->fd = open(arg, O_RDONLY);
    if (file_info->fd == -1) 
    {
        if (errno == EACCES) printf("ft_nm: %s: Permission denied\n", arg);
        else if (errno == ENOENT) printf("ft_nm: '%s': No such file or directory\n", arg);
        else printf("fd error : %s %d", arg, errno);
        free(file_info);
        return NULL;
    }
    int ok = fstat(file_info->fd, &file_info->stat);
    if (ok == -1) 
    {
        close(file_info->fd);
        printf("stat error : %s %d", arg, errno);
        free(file_info);
        return NULL;
    }
    if (file_info->stat.st_size == 0) 
    {
        close(file_info->fd);
        free(file_info);
        return NULL;
    }
    if (S_ISDIR(file_info->stat.st_mode)) 
    {
        close(file_info->fd);
        printf("ft_nm: Warning: '%s' is a directory\n", arg);
        free(file_info);
        return NULL;
    }
    file_info->map = (char *) mmap(NULL, file_info->stat.st_size, PROT_READ, MAP_PRIVATE, file_info->fd, 0);
    if (file_info->map == MAP_FAILED) 
    {
        close(file_info->fd);
        printf("map error : %s %d\n", arg, errno);
        free(file_info);
        return NULL;
    }
    if (file_info->stat.st_size < 5 || check_format(file_info->map))
    {
        close(file_info->fd);
        munmap(file_info->map, file_info->size);
        printf("ft_nm: %s: file format not recognized\n", arg);
        free(file_info);
        return NULL;
    }
    file_info->path = arg;
    file_info->is_64 = file_info->map[4] == 2;
    return file_info;
}

int check_format(char *c)
{
    if (c[0] == (char)127 && c[1] == 'E' && c[2] == 'L' && c[3] == 'F' && (c[4] == 2 || c[4] == 1))
        return (0);
    return (1);
}