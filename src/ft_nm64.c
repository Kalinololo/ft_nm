#include "nm.h"

/*
Upper : Global
Lower : Local
R/r :  Read only data section
D/d :  Data section
*/

/*
0 15 1
0 6 1
*/

char get_symbol_type(Elf64_Sym symtab, Elf64_Shdr *shdr)
{
    Elf64_Word section_type = shdr[symtab.st_shndx].sh_type;
    Elf64_Word section_flags = shdr[symtab.st_shndx].sh_flags;
    unsigned char type = ELF64_ST_TYPE(symtab.st_info);
    unsigned char bind = ELF64_ST_BIND(symtab.st_info);

    if (bind == STB_WEAK) 
    {
        if (type == STT_OBJECT)
            return 'V';
        if (symtab.st_shndx == SHN_UNDEF)
            return 'w';
        return 'W';
        
    }
    if (section_type == SHT_NOBITS && section_flags == (SHF_ALLOC | SHF_WRITE))
    {
        if (bind == STB_GLOBAL) return 'B';
        if (bind == STB_LOCAL) return 'b';
    }
    if (type == STT_FUNC && section_type == SHT_PROGBITS && section_flags == (SHF_ALLOC | SHF_EXECINSTR))
    {
        if (bind == STB_GLOBAL) return 'T';
        if (bind == STB_LOCAL) return 't';
    }
    if (type == STT_OBJECT || section_type == SHT_PROGBITS)
    {
        if (((section_flags & SHF_ALLOC) && !(section_flags & SHF_WRITE)))
        {
            if (bind == STB_GLOBAL) return 'R';
            if (bind == STB_LOCAL) return 'r';
        }
        if (section_flags == (SHF_ALLOC | SHF_WRITE) || section_type == SHT_DYNAMIC)
        {
            if (bind == STB_GLOBAL) return 'D';
            if (bind == STB_LOCAL) return 'd';
        }
    }

    return 'U';
}

void process_elf64(t_file_info *file_info) 
{
    Elf64_Ehdr *ehdr = (Elf64_Ehdr *)file_info->map;
    Elf64_Shdr *shdr = (Elf64_Shdr *)(file_info->map + ehdr->e_shoff);
    int symbols_find = 0;

    for (int i = 0; i < ehdr->e_shnum; i++) 
    {
        if (shdr[i].sh_type == SHT_SYMTAB) 
        {
            symbols_find = 1;
            Elf64_Sym *symtab = (Elf64_Sym *)(file_info->map + shdr[i].sh_offset);
            int symcount = shdr[i].sh_size / sizeof(Elf64_Sym);
            char *symstrtab = (char *)(file_info->map + shdr[shdr[i].sh_link].sh_offset);

            t_sym_list **sym_list = malloc(sizeof(t_sym_list*));
            *sym_list = malloc(sizeof(t_sym_list));
            t_sym_list *tmp = *sym_list;

            for (int j = 1; j < symcount; j++) 
            {
                if (symtab[j].st_name == 0)
                    continue;
                if (ELF64_ST_TYPE(symtab[j].st_info) == STT_FILE)
                    continue;
                tmp->name = symstrtab + symtab[j].st_name;
                tmp->value = symtab[j].st_value;
                tmp->type = get_symbol_type(symtab[j], shdr);
                t_sym_list *new = malloc(sizeof(t_sym_list));
                tmp->next = new;
                tmp = tmp->next;
            }
            free(tmp);

            name_sort(sym_list);

            print_result(sym_list, 16);
        }
    }
    if (!symbols_find) print("ft_nm: ", file_info->path, ": no symbols\n", NULL);
}