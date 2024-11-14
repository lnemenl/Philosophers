/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 13:54:57 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/14 14:23:17 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

const char	*skip_whitespace(const char *str)
{
	while ((*str >= 9 && *str <= 13) || *str == ' ')
		str++;
	return (str);
}

int	get_sign(const char **str)
{
	int	sign;
	
	sign = 1;
	if (**str == '-' || **str == '+')
	{
		if (**str == '-')
			sign = -1;
		(*str)++;
	}
}

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
