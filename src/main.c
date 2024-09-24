#include "nm.h"

int ft_strcmp(const char *s1, const char *s2) {
    while (*s1 == '_') s1++;
    while (*s2 == '_') s2++;
    while (*s1 && *s2 && *s1 == *s2) {
        s1++;
        s2++;
    }
    return (*s1 - *s2);
}

void name_sort(t_sym_list *list) {
    t_sym_list *tmp = list;
    while (tmp) {
        t_sym_list *tmp2 = list;
        while (tmp2) {
            if (ft_strcmp(tmp->name, tmp2->name) == 0) {
                char *name = tmp->name;
                tmp->name = tmp2->name;
                tmp2->name = name;
                unsigned long value = tmp->value;
                tmp->value = tmp2->value;
                tmp2->value = value;
                char type = tmp->type;
                tmp->type = tmp2->type;
                tmp2->type = type;
            }
            tmp2 = tmp2->next;
        }
        tmp = tmp->next;
    }
}

void process_elf(t_file_info *file_info) {
    Elf64_Ehdr *ehdr = (Elf64_Ehdr *)file_info->map;
    Elf64_Shdr *shdr = (Elf64_Shdr *)(file_info->map + ehdr->e_shoff);

    for (int i = 0; i < ehdr->e_shnum; i++) 
    {
        if (shdr[i].sh_type == SHT_SYMTAB) 
        {
            
            Elf64_Sym *symtab = (Elf64_Sym *)(file_info->map + shdr[i].sh_offset);
            int symcount = shdr[i].sh_size / sizeof(Elf64_Sym);
            char *symstrtab = (char *)(file_info->map + shdr[shdr[i].sh_link].sh_offset);

            t_sym_list *sym_list = malloc(sizeof(t_sym_list));
            t_sym_list *tmp = sym_list;

            for (int j = 1; j < symcount; j++) 
            {
                tmp->name = symstrtab + symtab[j].st_name;
                tmp->value = symtab[j].st_value;
                tmp->type = ELF64_ST_TYPE(symtab[j].st_info);
                t_sym_list *new = malloc(sizeof(t_sym_list));
                tmp->next = new;
                tmp = tmp->next;
            }
            free(tmp);

            name_sort(sym_list);

            tmp = sym_list;
            while (tmp) 
            {
                if (tmp->name == 0)
                    continue;
                if (tmp->value == 0)
                    printf("%16c ", ' ');
                else
                    printf("%016lx ", tmp->value);
                printf("%c ", tmp->type);
                printf("%s\n", tmp->name);
            }
        }
    }
}

int main(int ac, char **av) {
    int i = 0;
    while (++i < ac) {
        t_file_info *file_info = parse_arg(av[i]);
        if (file_info == NULL) continue;
        process_elf(file_info);

        munmap(file_info->map, file_info->size);
        close(file_info->fd);
        free(file_info);
    }
}