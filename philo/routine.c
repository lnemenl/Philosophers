/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 17:42:21 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/27 18:19:08 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void take_forks(t_philosopher *philo)
{
    int left_fork = philo->id - 1;
    int right_fork = philo->id % philo->shared->num_philosophers;

    pthread_mutex_lock(&philo->shared->fork_mutexes[left_fork]);
    log_action(philo, "has taken a fork");
    if (philo->shared->num_philosophers == 1)
    {
        usleep(philo->shared->time_to_die * 1000);
        pthread_mutex_unlock(&philo->shared->fork_mutexes[left_fork]);
        return;
    }
    pthread_mutex_lock(&philo->shared->fork_mutexes[right_fork]);
    log_action(philo, "has taken a fork");
}

void eat(t_philosopher *philo)
{
    pthread_mutex_lock(&philo->shared->write_mutex);
    philo->last_meal_time = get_current_time();
    pthread_mutex_unlock(&philo->shared->write_mutex);
    log_action(philo, "is eating");
    usleep(philo->shared->time_to_eat * 1000);
}

void release_forks(t_philosopher *philo)
{
    int left_fork = philo->id - 1;
    int right_fork = philo->id % philo->shared->num_philosophers;

    pthread_mutex_unlock(&philo->shared->fork_mutexes[right_fork]);
    pthread_mutex_unlock(&philo->shared->fork_mutexes[left_fork]);
}

void	sleep_and_think(t_philosopher *philo)
{
    log_action(philo, "is sleeping");
    usleep(philo->shared->time_to_sleep * 1000);
}


void *philosopher_routine(void *arg)
{
    t_philosopher *philo = (t_philosopher *)arg;
    while (philo->shared->is_simulation_running)
    {
        log_action(philo, "is thinking");
        if (!philo->shared->is_simulation_running)
            break;
        take_forks(philo);
        if (!philo->shared->is_simulation_running)
        {
            release_forks(philo);
            break;
        }
        eat(philo);
        release_forks(philo);
        if (!philo->shared->is_simulation_running)
            break;
        sleep_and_think(philo);
    }
    return (NULL);
}
