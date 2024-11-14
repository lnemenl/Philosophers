/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_args.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 15:09:17 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/14 15:16:55 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	parse_arguments(int ac, char **av, t_control *control)
{
	if (ac < 5)
	{
		printf("Error: Not enought arguments\n");
		return (1);
	}
	control->number_of_philosophers = ft_atoi(av[1]);
	control->time_to_die = ft_atol(av[2]);
	control->time_to_eat = ft_atol(av[3]);
	control->time_to_sleep = ft_atol(av[4]);
	if (control->number_of_philosophers < 1 ||
		control->time_to_die <= 0 ||
		control->time_to_eat <= 0 ||
		control->time_to_sleep <= 0)
		{
			printf("Error: Invalid arguments\n");
			return (1);
		}
	return (0);
}
