#ifndef NM_H
# define NM_H

# include <fcntl.h>
# include <unistd.h>
# include <sys/mman.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <stdlib.h>
# include <stdio.h>
# include <elf.h>
# include <string.h>
# include <errno.h>

typedef struct s_file_info {
    char *path;
    int fd;
    struct stat stat;
    int is_64;
    char *map;
    size_t size;
} t_file_info;

typedef struct s_sym_list {
    char *name;
    char type;
    unsigned long value;
    struct s_sym_list *next;
} t_sym_list;


t_file_info *parse_arg(char *arg);
int check_format(char *c);
int ft_strcmp(const char *s1, const char *s2);
char to_lower(char c);
void process_elf64(t_file_info *file_info);
void process_elf32(t_file_info *file_info);
void name_sort(t_sym_list **list);


#endif