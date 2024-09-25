#include "nm.h"

/*
Upper : Global
Lower : Local
R/r :  Read only data section
D/d :  Data section
*/

char get_symbol_type(Elf64_Sym symtab, Elf64_Shdr *shdr)
{
    Elf64_Word section_type = shdr[symtab.st_shndx].sh_type;
    Elf64_Word section_flags = shdr[symtab.st_shndx].sh_flags;
    unsigned char type = ELF64_ST_TYPE(symtab.st_info);
    unsigned char bind = ELF64_ST_BIND(symtab.st_info);

    if (type == STT_OBJECT)
    {
        if (bind == STB_WEAK) return 'V';
        if (section_type == SHT_NOBITS)
        {
            if (bind == STB_GLOBAL) return 'B';
            if (bind == STB_LOCAL) return 'b';
        }
        if ((section_type == SHT_PROGBITS && section_flags == (SHF_ALLOC | SHF_WRITE)) || section_type == SHT_DYNAMIC)
        {
            if (bind == STB_GLOBAL) return 'D';
            if (bind == STB_LOCAL) return 'd';
        }
        if (section_type == SHT_PROGBITS || ((section_flags & SHF_ALLOC) && !(section_flags & SHF_WRITE)))
        {
            if (bind == STB_GLOBAL) return 'R';
            if (bind == STB_LOCAL) return 'r';
        }
    }
    if (type == STT_FUNC)
    {
        if (section_type == SHT_PROGBITS && section_flags == (SHF_ALLOC | SHF_EXECINSTR))
        {
            if (bind == STB_GLOBAL) return 'T';
            if (bind == STB_LOCAL) return 't';
        }
    }
    return 'U';
}

/*char get_symbol_type(Elf64_Sym symtab, Elf64_Shdr *shdr)
{
    Elf64_Word section_type = shdr[symtab.st_shndx].sh_type;
    Elf64_Word section_flags = shdr[symtab.st_shndx].sh_flags;

    if (ELF64_ST_TYPE(symtab.st_info) == STT_SECTION)
        return 'N';
    if (ELF64_ST_TYPE(symtab.st_info) == STT_FILE)
        return 'f';

    if (section_type == SHT_NOBITS && section_flags == (SHF_ALLOC | SHF_WRITE))
        return 'B';
    if (section_type == SHT_PROGBITS && section_flags == SHF_ALLOC)
        return 'R';
    if (section_type == SHT_PROGBITS && section_flags == (SHF_ALLOC | SHF_WRITE))
        return 'D';
    if (section_type == SHT_PROGBITS && section_flags == (SHF_ALLOC | SHF_EXECINSTR))
        return 'T';
    if (section_type == SHT_DYNAMIC)
        return 'D';

    if (section_flags == (SHF_ALLOC | SHF_WRITE))
        return 'D';
    if (section_flags == SHF_ALLOC)
        return 'R';
    if (section_flags == (SHF_ALLOC | SHF_EXECINSTR))
        return 'T';

    return '?';
}*/

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

            tmp = *sym_list;
            while (tmp->next != NULL) 
            {
                if (tmp->name == 0)
                    continue;
                if (tmp->value == 0)
                    printf("%16c ", ' ');
                else
                    printf("%016lx ", tmp->value);
                printf("%c ", tmp->type);
                printf("%s\n", tmp->name);
                tmp = tmp->next;
            }
        }
    }
    if (!symbols_find) printf("ft_nm: %s: no symbols\n", file_info->path);
}