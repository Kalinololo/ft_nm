#include "nm.h"

char to_lower(char c) 
{
    if (c >= 'A' && c <= 'Z')
        return c + 32;
    return c;
}

int ft_strcmp(const char *s1, const char *s2) 
{
    while (*s1 == '_') s1++;
    while (*s2 == '_') s2++;
    while (*s1 && *s2 && to_lower(*s1) == to_lower(*s2)) {
        s1++;
        s2++;
    }
    return (to_lower(*s1) - to_lower(*s2));
}

void name_sort(t_sym_list **list) 
{
    t_sym_list *tmp = *list;
    t_sym_list *tmp2 = *list;
    while (tmp->next != NULL) 
    {
        tmp2 = tmp->next;
        while (tmp2->next != NULL) 
        {
            if (ft_strcmp(tmp->name, tmp2->name) >= 0) 
            {
                char *name = tmp->name;
                unsigned long value = tmp->value;
                char type = tmp->type;
                tmp->name = tmp2->name;
                tmp->value = tmp2->value;
                tmp->type = tmp2->type;

                tmp2->name = name;
                tmp2->value = value;
                tmp2->type = type;
            }
            tmp2 = tmp2->next;
        }
        tmp = tmp->next;
    }
}