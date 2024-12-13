/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 15:09:17 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/12/13 12:06:12 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (!str || str[0] == '\0')
		return (0);
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static int	validate_argument(const char *arg, int *result)
{
	if (!is_numeric(arg) || !safe_atoi(arg, result) || *result <= 0)
		return (0);
	return (1);
}

static int	validate_optional_argument(const char *arg, t_shared *shared)
{
	int	result;

	if (!is_numeric(arg) || !safe_atoi(arg, &result) || result < 0)
		return (0);
	shared->meals_required = result;
	if (shared->meals_required == 0)
		return (0);
	return (1);
}

int	parse_and_validate(int argc, char **argv, t_shared *shared)
{
	int	result;

	if (argc != 5 && argc != 6)
		return (0);
	if (!validate_argument(argv[1], &result))
		return (0);
	shared->num_philosophers = result;
	if (!validate_argument(argv[2], &result))
		return (0);
	shared->time_to_die = result;
	if (!validate_argument(argv[3], &result))
		return (0);
	shared->time_to_eat = result;
	if (!validate_argument(argv[4], &result))
		return (0);
	shared->time_to_sleep = result;
	if (argc == 6)
	{
		if (!validate_optional_argument(argv[5], shared))
			return (0);
	}
	else
		shared->meals_required = -1;
	return (1);
}
