/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 15:08:49 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/21 15:23:17 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	handle_one_philosopher(t_shared *shared)
{
	if (shared->num_philosophers == 1)
	{
		printf("%ld 1 has taken a fork\n", get_current_time());
		usleep(shared->time_to_die * 1000);
		printf("%ld 1 died\n", get_current_time());
		return (1);
	}
	return (0);
}

static int	setup_simulation(int argc, char **argv,
								t_shared *shared, t_philosopher **philosophers)
{
	if (initialize_simulation(argc, argv, shared))
		return (1);
	if (handle_one_philosopher(shared))
		return (1);
	*philosophers = init_philosophers(shared);
	if (!(*philosophers))
	{
		printf("Error: Philosopher initialization failed\n");
		return (1);
	}
	return (0);
}

static int	run_simulation(t_shared *shared,
							t_philosopher *philosophers, pthread_t **threads)
{
	if (start_threads(shared, philosophers, threads))
	{
		cleanup_simulation(shared, philosophers);
		return (1);
	}
	return (0);
}

int main(int argc, char **argv)
{
	t_shared		shared;
	t_philosopher	*philosophers;
	pthread_t		*threads;

	if (setup_simulation(argc, argv, &shared, &philosophers))
		return (1);
	if (run_simulation(&shared, philosophers, &threads))
		return (1);
	join_threads(&shared, threads);
	cleanup_simulation(&shared, philosophers);
	free(threads);
	return (0);
}
