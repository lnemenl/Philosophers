/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 15:08:49 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/15 23:09:47 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	main(int ac, char **av)
{
	t_simulation	simulation;
	pthread_t		monitor_thread;

	if (parse_arguments(ac, av, simulation.control) != 0)
		return (1);
	simulation.control = initialize_control(simulation.control->number_of_philosophers);
	if (!simulation.control)
		return (1);
	if (initialize_simulation(&simulation) != 0)
	{
		free_control(simulation.control);
		return (1);
	}
	if (pthread_create(&monitor_thread, NULL, monitor_routine, &simulation) != 0)
	{
		free_simulation(&simulation);
		return (1);
	}
	start_philosophers(&simulation);
	pthread_join(monitor_thread, NULL);
	free_simulation(&simulation);
	return (0);
}
