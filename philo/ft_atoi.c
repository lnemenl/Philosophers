/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 13:54:57 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/21 15:19:33 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static const char	*skip_whitespace(const char *str)
{
    while ((*str >= 9 && *str <= 13) || *str == ' ')
        str++;
    return (str);
}

static int	get_sign(const char **str)
{
    int sign = 1;

    if (**str == '-' || **str == '+')
    {
        if (**str == '-')
            sign = -1;
        (*str)++;
    }
    return (sign);
}

static int	convert_to_int(const char *str, int sign)
{
    long long result = 0;
    long long max = ((unsigned long long)(-1)) / 2;

    while (*str >= '0' && *str <= '9')
    {
        if (result > max / 10 || (result == max / 10 && (*str - '0') > 7))
        {
            if (sign > 0)
                return (-1);
            else
                return (0);
        }
        result = result * 10 + (*str - '0');
        str++;
    }
    return ((int)(sign * result));
}

int	ft_atoi(const char *str)
{
    str = skip_whitespace(str);
    int sign = get_sign(&str);
    return (convert_to_int(str, sign));
}
