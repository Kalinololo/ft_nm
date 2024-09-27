#include "nm.h"

int check_format(char *c)
{
    if (c[0] == (char)127 && c[1] == 'E' && c[2] == 'L' && c[3] == 'F' && (c[4] == 2 || c[4] == 1))
        return (0);
    return (1);
}

int check_size(t_file_info *file)
{
    Elf64_Ehdr *ehdr = (Elf64_Ehdr *)file->map;
    Elf64_Shdr *shdr = (Elf64_Shdr *)(file->map + ehdr->e_shoff);
    if (shdr->sh_offset + shdr->sh_size > file->size) 
    {
        print("ft_nm: warning: ", file->path, " has a section extending past end of file\n", NULL);
        return (1);
    }
    return (0);
}

t_file_info *parse_arg(char *arg)
{
    t_file_info *file_info = (t_file_info *)malloc(sizeof(t_file_info));
    file_info->fd = open(arg, O_RDONLY);
    file_info->path = arg;
    if (file_info->fd == -1) 
    {
        if (errno == EACCES) print("ft_nm: '", arg, "': Permission denied\n", NULL);
        else if (errno == ENOENT) print("ft_nm: '", arg, "': No such file or directory\n", NULL);    
        else print("ft_nm: '", arg, "': Error while opening file\n", NULL);
        free(file_info);
        return NULL;
    }
    int ok = fstat(file_info->fd, &file_info->stat);
    if (ok == -1) 
    {
        close(file_info->fd);
        print("ft_nm: '", arg, "': Error while reading file\n", NULL);
        free(file_info);
        return NULL;
    }
    if (file_info->stat.st_size == 0) 
    {
        close(file_info->fd);
        free(file_info);
        return NULL;
    }
    file_info->size = file_info->stat.st_size;
    if (S_ISDIR(file_info->stat.st_mode)) 
    {
        close(file_info->fd);
        print("ft_nm: Warning: '", arg, "' is a directory\n", NULL);
        free(file_info);
        return NULL;
    }
    file_info->map = (char *) mmap(NULL, file_info->stat.st_size, PROT_READ, MAP_PRIVATE, file_info->fd, 0);
    if (file_info->map == MAP_FAILED) 
    {
        close(file_info->fd);
        print("ft_nm: '", arg, "': Error while mapping file\n", NULL);
        free(file_info);
        return NULL;
    }
    if (file_info->stat.st_size < 5 || check_format(file_info->map))
    {
        close(file_info->fd);
        munmap(file_info->map, file_info->size);
        print("ft_nm: ", arg, ": file format not recognized\n", NULL);
        free(file_info);
        return NULL;
    }
    file_info->is_64 = file_info->map[4] == 2;
    return file_info;
}