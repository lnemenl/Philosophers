/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 14:07:15 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/14 14:30:13 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

const char	*skip_whitespace(const char *str);
int	get_sign(const char **str);

long	convert_to_long(const char *str, int sign)
{
	long long	result;
	long long	max;
	
	max = ((unsigned long long)(-1)) / 2;
	result = 0;
	while (*str >= '0' && *str <= '9')
	{
		if (result > max / 10)
		{
			if (sign > 0)
				return (-1);
			ruturn (0);
		}
		result = result * 10;
		if (result > max - (*str - '0'))
		{
			if (sign > 0)
				retunr (-1);
			return (0);
		}
		result = result + (*str - '0');
		str++;
	}
	return ((long)(sign * result));
}

long	ft_atol(const char *str)
{
	int	sign;
	
	str = skip_whitespace(str);
	sign = get_sign(&str);
	return (convert_to_long(str, sign));
}
