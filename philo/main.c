/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 15:08:49 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/12/13 16:28:54 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	initialize_simulation_data(t_shared *shared,
	int argc, char **argv, int *flags)
{
	if (!initialize_simulation(shared, argc, argv, flags))
	{
		clean_up_simulation(NULL, shared, *flags);
		return (0);
	}
	return (1);
}

static int	allocate_and_init_resources(t_shared *shared,
	t_thread_data *data, int *flags)
{
	if (!allocate_thread_data(data, shared, flags))
	{
		printf("Error: Failed to allocate thread data.\n");
		clean_up_simulation(NULL, shared, *flags);
		return (0);
	}
	if (!init_forks(shared, flags))
	{
		clean_up_simulation(data, shared, *flags);
		return (0);
	}
	return (1);
}

static int	launch_simulation_threads(t_shared *shared,
	t_thread_data *data, int *flags)
{
	if (!launch_threads(data, flags))
	{
		printf("Error: Failed to launch threads.\n");
		clean_up_simulation(data, shared, *flags);
		return (0);
	}
	return (1);
}

int	main(int argc, char **argv)
{
	t_shared		shared;
	t_thread_data	data;
	int				cleanup_flags;

	cleanup_flags = 0;
	if (!initialize_simulation_data(&shared, argc, argv, &cleanup_flags))
		return (1);
	if (!allocate_and_init_resources(&shared, &data, &cleanup_flags))
		return (1);
	if (!launch_simulation_threads(&shared, &data, &cleanup_flags))
		return (1);
	clean_up_simulation(&data, &shared, cleanup_flags);
	return (0);
}
