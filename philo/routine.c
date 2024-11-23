/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 17:42:21 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/23 21:21:24 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	take_forks(t_philosopher *philo)
{
    int	left_fork;
	int	right_fork;

	left_fork = philo->id - 1;
	right_fork = philo->id % philo->shared->num_philosophers;
    while (1)
	{
        pthread_mutex_lock(&philo->shared->forks_mutex);
        if (philo->shared->forks[left_fork] == 0 && philo->shared->forks[right_fork] == 0)
		{
            philo->shared->forks[left_fork] = 1;
            philo->shared->forks[right_fork] = 1;
            log_action(philo, "has taken forks");
            pthread_mutex_unlock(&philo->shared->forks_mutex);
            break;
        }
        pthread_mutex_unlock(&philo->shared->forks_mutex);
        usleep(1000);
    }
}

void release_forks(t_philosopher *philo)
{
	int	left_fork;
	int	right_fork;

	left_fork = philo->id - 1;
	right_fork = philo->id % philo->shared->num_philosophers;
    pthread_mutex_lock(&philo->shared->forks_mutex);
    philo->shared->forks[left_fork] = 0;
    philo->shared->forks[right_fork] = 0;
    pthread_mutex_unlock(&philo->shared->forks_mutex);
}

void	eat(t_philosopher *philo)
{
    pthread_mutex_lock(&philo->shared->forks_mutex);
    philo->last_meal_time = get_current_time();
    pthread_mutex_unlock(&philo->shared->forks_mutex);
    log_action(philo, "is eating");
    usleep(philo->shared->time_to_eat * 1000);
    release_forks(philo);
}


void	sleep_and_think(t_philosopher *philo)
{
    log_action(philo, "is sleeping");
    usleep(philo->shared->time_to_sleep * 1000);
}


void	*philosopher_routine(void *arg)
{
	t_philosopher *philo;

    philo = (t_philosopher *)arg;
	while (philo->shared->is_simulation_running)
	{
		log_action(philo, "is thinking");
		usleep(1000);
		take_forks(philo);
		eat(philo);
		sleep_and_think(philo);
	}
	return (NULL);
}
