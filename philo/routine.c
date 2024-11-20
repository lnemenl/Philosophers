/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 17:42:21 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/20 15:19:39 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void take_forks(t_philosopher *philo)
{
    if (philo->id % 2 == 0)
    {
        pthread_mutex_lock(philo->right_fork);
        pthread_mutex_lock(philo->left_fork);
    }
    else
    {
        pthread_mutex_lock(philo->left_fork);
        pthread_mutex_lock(philo->right_fork);
    }
    pthread_mutex_lock(&philo->shared->write_lock);
    printf("%ld %d has taken a fork\n", get_current_time(), philo->id);
    pthread_mutex_unlock(&philo->shared->write_lock);
}

void release_forks(t_philosopher *philo)
{
    pthread_mutex_unlock(philo->left_fork);
    pthread_mutex_unlock(philo->right_fork);
}

void eat(t_philosopher *philo)
{
    pthread_mutex_lock(&philo->shared->monitor_lock);
    philo->last_meal_time = get_current_time();
    philo->meals_eaten++;
    pthread_mutex_unlock(&philo->shared->monitor_lock);

    pthread_mutex_lock(&philo->shared->write_lock);
    printf("%ld %d is eating\n", get_current_time(), philo->id);
    pthread_mutex_unlock(&philo->shared->write_lock);

    usleep(philo->shared->time_to_eat * 1000);
}

void sleep_and_think(t_philosopher *philo)
{
    pthread_mutex_lock(&philo->shared->write_lock);
    printf("%ld %d is sleeping\n", get_current_time(), philo->id);
    pthread_mutex_unlock(&philo->shared->write_lock);
    usleep(philo->shared->time_to_sleep * 1000);

    pthread_mutex_lock(&philo->shared->write_lock);
    printf("%ld %d is thinking\n", get_current_time(), philo->id);
    pthread_mutex_unlock(&philo->shared->write_lock);
}

void *philosopher_routine(void *arg)
{
    t_philosopher *philo = (t_philosopher *)arg;

    while (philo->shared->is_simulation_running)
    {
        take_forks(philo);
        eat(philo);
        release_forks(philo);
        sleep_and_think(philo);
    }
    return (NULL);
}
