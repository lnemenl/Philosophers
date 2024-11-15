/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 17:42:21 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/15 18:47:26 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	check_active_group(t_philosopher *philosopher)
{
	pthread_mutex_lock(&philosopher->control->control_mutex);
	if (philosopher->control->active_group != -1 &&
		philosopher->control->active_group != (philosopher->id % 2))
	{
		pthread_mutex_unlock(&philosopher->control->control_mutex);
		sleep_ms(100);
		return (0);
	}
	pthread_mutex_unlock(&philosopher->control->control_mutex);
	return (1);
}

void	perform_eating(t_philosopher *philosopher)
{
	pthread_mutex_lock(philosopher->left_fork);
	pthread_mutex_lock(philosopher->right_fork);
	philosopher->last_meal_time = get_current_time_ms();
	printf("%ld Philosopher %d is eating\n", get_current_time_ms(), philosopher->id);
	sleep_ms(philosopher->control->time_to_eat);
	pthread_mutex_unlock(philosopher->left_fork);
	pthread_mutex_unlock(philosopher->right_fork);
}

void	increment_group_and_toggle(t_philosopher *philosopher)
{
	pthread_mutex_lock(&philosopher->control->control_mutex);
	philosopher->control->group_eating_count++;
	if (philosopher->control->group_eating_count >= philosopher->control->number_of_philosophers / 2)
	{
		philosopher->control->active_group = 1 - philosopher->control->active_group;
		philosopher->control->group_eating_count = 0;
		printf("Group toggled. Active group: %d\n", philosopher->control->active_group);
	}
	pthread_mutex_unlock(&philosopher->control->control_mutex);
}

void	*philosopher_routine(void *philosopher_data)
{
	t_philosopher	*philosopher;

	philosopher = (t_philosopher *)philosopher_data;
	while (1)
	{
		pthread_mutex_lock(&philosopher->control->control_mutex);
		if (philosopher->control->stop_simulation)
		{
			pthread_mutex_unlock(&philosopher->control->control_mutex);
			return (NULL);
		}
		pthread_mutex_unlock(&philosopher->control->control_mutex);
		if (!check_active_group(philosopher))
			continue;
		perform_eating(philosopher);
		increment_group_and_toggle(philosopher);
		printf("%ld Philosopher %d is sleeping\n", get_current_time_ms(), philosopher->id);
        sleep_ms(philosopher->control->time_to_sleep);
        printf("%ld Philosopher %d is thinking\n", get_current_time_ms(), philosopher->id);
        sleep_ms(100); // Simulate thinking
	}
	return (NULL);
}

int	start_philosophers(t_simulation *simulation)
{
	pthread_t	*threads;
	int			i;

	threads = malloc(sizeof(pthread_t) * simulation->control->number_of_philosophers);
	if (!threads)
		return (-1);
	i = 0;
	while (i < simulation->control->number_of_philosophers)
	{
		if (pthread_create(&threads[i], NULL, philosopher_routine, &simulation->philosophers[i]) != 0)
		{
			free(threads);
			return (-1);
		}
		i++;
	}
	i = 0;
	while (i < simulation->control->number_of_philosophers)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	free(threads);
	return (0);
}
