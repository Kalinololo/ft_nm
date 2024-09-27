#include "nm.h"

char to_lower(char c) 
{
    if (c >= 'A' && c <= 'Z')
        return c + 32;
    return c;
}

int ft_strlen(const char *s) 
{
    int i = 0;
    while (s[i]) i++;
    return i;
}

int ft_strcmp(const char *s1, const char *s2) 
{
    while (*s1 == '_') s1++;
    while (*s2 == '_') s2++;
    while (*s1 && *s2 && to_lower(*s1) == to_lower(*s2)) {
        s1++;
        s2++;
        while (*s1 == '_') s1++;
        while (*s2 == '_') s2++;
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


int	print_recursive(unsigned int nbr, char *base)
{
	char	x;
	int		i;

	i = 1;
	if (nbr == 0)
		return (0);
	x = base[nbr % 16];
	i += print_recursive(nbr / 16, base);
	write(1, &x, 1);
	return (i);
}

int	ft_putnbr_base(int nbr, char *base)
{
	int	i;

	if (nbr == 0)
	{
		write(1, &base[0], 1);
		return (1);
	}
	i = print_recursive((unsigned int)nbr, base);
	return (i);
}


void print_result_format(char to_print, int nb_char, unsigned long value, char type, char *name) 
{
    if (to_print == '0')
    {
        int value_len = 0;
        unsigned long tmp = value;
        while (tmp > 0)
        {
            tmp /= 16;
            value_len++;
        }
        nb_char -= value_len;
    }
    while (nb_char--) write(1, &to_print, 1);
    if (value) ft_putnbr_base(value, "0123456789abcdef");
    write(1, " ", 1);
    write(1, &type, 1);
    write(1, " ", 1);
    write(1, name, ft_strlen(name));
    write(1, "\n", 1);
}

void print_result(t_sym_list **list, int nb)
{
    t_sym_list *tmp = *list;

    while (tmp->next != NULL) 
    {
        if (tmp->name == 0)
            continue;
        if (tmp->value == 0 && tmp->type != 'T' && tmp->type != 't' && tmp->type != 'a' && tmp->type != 'A')
            print_result_format(' ', nb, 0, tmp->type, tmp->name);
        else
            print_result_format('0', nb, tmp->value, tmp->type, tmp->name);
        tmp = tmp->next;
    }
}

void print(char *s1, char *s2, char *s3, char *s4)
{
    if (s1) write(1, s1, ft_strlen(s1));
    if (s2) write(1, s2, ft_strlen(s2));
    if (s3) write(1, s3, ft_strlen(s3));
    if (s4) write(1, s4, ft_strlen(s4));
}