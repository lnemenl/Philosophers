/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:42:17 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/15 22:30:25 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

t_control	*initialize_control(int num_philosophers)
{
	t_control	*control;

	control = malloc(sizeof(t_control));
	if (!control)
		return (NULL);
	control->number_of_philosophers = num_philosophers;
	if (pthread_mutex_init(&control->control_mutex, NULL) != 0)
	{
		free(control);
		return (NULL);
	}
	control->forks = malloc(sizeof(pthread_mutex_t) * num_philosophers);
	if (!control->forks)
	{
		free_control(control);
		return (NULL);
	}
	if (initialize_forks(control->forks, num_philosophers) != 0)
	{
		free_control(control);
		return (NULL);
	}
	return (control);
}

int	initialize_forks(pthread_mutex_t *forks, int num_philosophers)
{
	int	i;

	i = 0;
	while (i < num_philosophers)
	{
		if (pthread_mutex_init(&forks[i], NULL) != 0)
		{
			printf("Error: Failed to initialize fork %d\n", i);
			destroy_forks(forks, i);
			return (-1);
		}
		i++;
	}
	return (0);
}

int	setup_philosopher(t_philosopher *philosopher, int id, t_control *control)
{
	philosopher->control = control;
	philosopher->id = id + 1;
	philosopher->last_meal_time = get_current_time_ms();
	philosopher->left_fork = &control->forks[id];
	philosopher->right_fork = &control->forks[(id + 1) % control->number_of_philosophers];
	return (0);
}

int	initialize_philosophers(t_philosopher *philosophers, int num_philosophers, t_control *control)
{
	int	i;
	
	i = 0;
	while (i < num_philosophers)
	{
		if (setup_philosopher(&philosophers[i], i, control) != 0)
			return (i);
		i++;
	}
	return (0);
}

int	initialize_simulation(t_simulation *simulation)
{
	simulation->philosophers = malloc(sizeof(t_philosopher) * simulation->control->number_of_philosophers);
	if (!simulation->philosophers)
		return (-1);
	if (initialize_philosophers(simulation->philosophers, simulation->control->number_of_philosophers, simulation->control) != 0)
	{
		free(simulation->philosophers);
		return (-1);
	}
	return (0);
}
