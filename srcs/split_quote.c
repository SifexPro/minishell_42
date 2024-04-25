/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pepie <pepie@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 13:50:25 by pepie             #+#    #+#             */
/*   Updated: 2024/04/18 07:31:30 by pepie            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list *create_str(char *str, bool is_double_quote, bool is_simple_quote)
{
    t_str_input *tmp;

    tmp = malloc(sizeof(t_str_input));
    if (!tmp)
        return (NULL);
    tmp->str = str;
    tmp->is_double_quote = is_double_quote;
    tmp->is_simple_quote = is_simple_quote;

    return (ft_lstnew(tmp));
}

void	create_strings_quote(char const *str, t_list **elements)
{
    int     i;
    int     str_start;
    int     quote_start;
    bool    is_simple_quote;
    bool    is_double_quote;

    i = 0;
    str_start = 0;
    quote_start = 0;
    is_simple_quote = false;
    is_double_quote = false;
    while (str[i])
    {
        if (str[i] == ' ' && str_start == i)
            str_start++;
        else if (str[i] == '\"' && !is_double_quote && !is_simple_quote)
        {
            is_double_quote = true;
            quote_start = i;
        }
        else if (str[i] == '\"' && is_double_quote)
        {
            ft_lstadd_back(elements, create_str(
                ft_strndup((char *)(&str[quote_start + 1]), i - quote_start - 1),
                true,
                false
            ));
            str_start = i + 1;
            quote_start = 0;
            is_double_quote = false;
        }
        else if (str[i] == '\'' && !is_simple_quote && !is_double_quote)
        {
            is_simple_quote = true;
            quote_start = i;
        }
        else if (str[i] == '\'' && is_simple_quote)
        {
            ft_lstadd_back(elements, create_str(
                ft_strndup((char *)(&str[quote_start + 1]), i - quote_start - 1),
                false,
                true
            ));
            str_start = i + 1;
            quote_start = 0;
            is_simple_quote = false;
        }
        else if ((str[i] == ' ' && quote_start == 0) || str[i + 1] == 0)
        {
            if (str[i + 1] == 0)
                i++;
            ft_lstadd_back(elements, create_str(
                ft_strndup((char *)(&str[str_start]), i - str_start),
                false,
                false
            ));
            str_start = i + 1;
        }
        i++;
    }
    ft_printf("", is_simple_quote, is_double_quote, elements);
}

char	**ft_split_quote(char const *str)
{
	t_list	    *elements;
    t_str_input *tmp;
    char        **ret;
    int         i;

    i = 0;
    if (!str)
        return (NULL);
    elements = NULL;
    create_strings_quote(str, &elements);
    ret = malloc(sizeof(char *) * ft_lstsize(elements));
    if (!ret)
        return (NULL);
    while (elements)
    {
        tmp = elements->content;
        ret[i] = tmp->str;
        i++;
        elements = elements->next;
    }
    return (ret);
}
