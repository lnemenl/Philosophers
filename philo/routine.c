/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 17:42:21 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/26 19:58:02 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void think(t_philosopher *philo)
{
    log_action(philo, "is thinking");
    usleep(1000);
}

int take_forks(t_philosopher *philo)
{
    int left_fork = philo->id - 1;
    int right_fork = philo->id % philo->shared->num_philosophers;

    pthread_mutex_lock(&philo->shared->write_mutex);
    if (!philo->shared->is_simulation_running)
    {
        pthread_mutex_unlock(&philo->shared->write_mutex);
        return 0; 
    }
    pthread_mutex_unlock(&philo->shared->write_mutex);
    if (left_fork < right_fork)
    {
        pthread_mutex_lock(&philo->shared->forks_mutex[left_fork]);
        pthread_mutex_lock(&philo->shared->forks_mutex[right_fork]);
    }
    else
    {
        pthread_mutex_lock(&philo->shared->forks_mutex[right_fork]);
        pthread_mutex_lock(&philo->shared->forks_mutex[left_fork]);
    }
    log_action(philo, "has taken forks");
    return (1);
}

void eat(t_philosopher *philo)
{
    if (!take_forks(philo))
        return;

    pthread_mutex_lock(&philo->shared->write_mutex);
    if (!philo->shared->is_simulation_running)
    {
        pthread_mutex_unlock(&philo->shared->write_mutex);
        release_forks(philo); 
        return;
    }
    philo->last_meal_time = get_current_time();
    philo->meals_eaten++;
    log_action(philo, "is eating");
    pthread_mutex_unlock(&philo->shared->write_mutex);

    usleep(philo->shared->time_to_eat * 1000);
    release_forks(philo);
}


void release_forks(t_philosopher *philo)
{
    int left_fork;
    int right_fork;

    left_fork = philo->id - 1;
    right_fork = philo->id % philo->shared->num_philosophers;
    pthread_mutex_unlock(&philo->shared->forks_mutex[left_fork]);
    pthread_mutex_unlock(&philo->shared->forks_mutex[right_fork]);
}

void sleep_and_rest(t_philosopher *philo)
{
    log_action(philo, "is sleeping");
    usleep(philo->shared->time_to_sleep * 1000);
}

void single_philosopher_routine(t_philosopher *philo)
{
    log_action(philo, "is thinking");
    usleep(philo->shared->time_to_die * 1000);
    pthread_mutex_lock(&philo->shared->write_mutex);
    if (philo->shared->is_simulation_running)
    {
        log_action(philo, "died");
        philo->shared->is_simulation_running = 0;
    }
    pthread_mutex_unlock(&philo->shared->write_mutex);
}

void *general_philosopher_routine(t_philosopher *philo)
{
    while (1)
    {
        pthread_mutex_lock(&philo->shared->write_mutex);
        if (!philo->shared->is_simulation_running)
        {
            pthread_mutex_unlock(&philo->shared->write_mutex);
            break;
        }
        pthread_mutex_unlock(&philo->shared->write_mutex);
        think(philo);
        pthread_mutex_lock(&philo->shared->write_mutex);
        if (!philo->shared->is_simulation_running)
        {
            pthread_mutex_unlock(&philo->shared->write_mutex);
            break;
        }
        pthread_mutex_unlock(&philo->shared->write_mutex);
        eat(philo);
        sleep_and_rest(philo);
    }
    return NULL;
}

void *philosopher_routine(void *arg)
{
    t_philosopher *philo = (t_philosopher *)arg;

    if (philo->shared->num_philosophers == 1)
    {
        single_philosopher_routine(philo);
        return NULL;
    }
    return general_philosopher_routine(philo);
}
