/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 17:42:21 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/12/15 00:12:26 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	get_forks_order(t_philosopher *philosopher,
							pthread_mutex_t **first_fork,
							pthread_mutex_t **second_fork)
{
	t_shared	*shared;

	shared = philosopher->shared_data;
	if (philosopher->id % shared->num_philosophers == 0)
	{
		*first_fork = philosopher->right_fork;
		*second_fork = philosopher->left_fork;
	}
	else
	{
		*first_fork = philosopher->left_fork;
		*second_fork = philosopher->right_fork;
	}
}

int take_forks(t_philosopher *philosopher)
{
    t_shared        *shared;
    pthread_mutex_t *first_fork;
    pthread_mutex_t *second_fork;

    shared = philosopher->shared_data;
    get_forks_order(philosopher, &first_fork, &second_fork);
    pthread_mutex_lock(first_fork);
    log_action(philosopher, "has taken a fork");
    if (is_simulation_end(shared))
    {
        pthread_mutex_unlock(first_fork);
        return (0);
    }
    update_last_meal_time(philosopher);
    pthread_mutex_lock(second_fork);
    log_action(philosopher, "has taken a fork");
    if (is_simulation_end(shared))
    {
        pthread_mutex_unlock(second_fork);
        pthread_mutex_unlock(first_fork);
        return (0);
    }
    return (1);
}

void	put_forks(t_philosopher *philosopher)
{
	pthread_mutex_unlock(philosopher->right_fork);
	pthread_mutex_unlock(philosopher->left_fork);
}

int	eat(t_philosopher *philosopher)
{
	t_shared	*shared;

	shared = philosopher->shared_data;
	if (is_simulation_end(shared))
		return (0);
	log_action(philosopher, "is eating");
	pthread_mutex_lock(&philosopher->meal_lock);
	philosopher->last_meal_time = get_current_time_ms();
	philosopher->meals_eaten++;
	pthread_mutex_unlock(&philosopher->meal_lock);
	smart_sleep(shared->time_to_eat, shared);
	if (is_simulation_end(shared))
		return (0);
	return (1);
}

void	*philosopher_routine(void *arg)
{
	t_philosopher	*philosopher;
	t_shared		*shared;

	philosopher = (t_philosopher *)arg;
	shared = philosopher->shared_data;
	if (philosopher->id % 2 == 0)
        usleep(1000);
	while (!is_simulation_end(shared))
	{
		log_action(philosopher, "is thinking");
		if (is_simulation_end(shared) || !take_forks(philosopher))
			break ;
		if (is_simulation_end(shared) || !eat(philosopher))
		{
			put_forks(philosopher);
			break ;
		}
		put_forks(philosopher);
		if (!is_simulation_end(shared))
			log_action(philosopher, "is sleeping");
		smart_sleep(shared->time_to_sleep, shared);
	}
	return (NULL);
}
