/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 13:54:57 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/21 12:53:59 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

const char	*skip_whitespace(const char *str);
int	get_sign(const char **str);

int	convert_to_int(const char *str, int sign)
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
			return (0);
		}
		result = result * 10;
		if (result > max - (*str - '0'))
		{
			if (sign > 0)
				return (-1);
			return (0);
		}
		result = result + (*str - '0');
		str++;
	}
	return ((int)(sign * result));
}

int	ft_atoi(const char *str)
{
	int	sign;
	
	str = skip_whitespace(str);
	sign = get_sign(&str);
	return (convert_to_int(str, sign));
}
