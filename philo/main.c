/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 15:08:49 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/15 18:30:58 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	main(int ac, char **av)
{
	t_control		control;
	t_simulation	simulation;
	pthread_t		monitor_thread;
	int				result;
	
	if (parse_arguments(ac, av, &control) != 0)
		return (1);
	simulation.control = initialize_control(control.number_of_philosophers);
	if (!simulation.control)
		return (1);
	simulation.philosophers = malloc(sizeof(t_philosopher) * control.number_of_philosophers);
	if (!simulation.philosophers)
	{
		free_control(simulation.control);
		return(1);
	}
	if (initialize_philosophers(simulation.philosophers, control.number_of_philosophers, simulation.control) != 0)
	{
		free_simulation(&simulation);
		return (1);
	}
	if (pthread_create(&monitor_thread, NULL, monitor_routine, &simulation) != 0)
	{
		free_simulation(&simulation);
		return (1);
	}
	result = start_philosophers(&simulation);
	pthread_join(monitor_thread, NULL);
	free_simulation(&simulation);
	return (result);
}
