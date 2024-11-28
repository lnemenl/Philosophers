/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 17:42:21 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/28 03:50:01 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int lock_fork(pthread_mutex_t *fork, t_philosopher *philo)
{
    if (!safe_mutex_lock(fork))
        return (0);
    if (philo->table->simulation_end)
    {
        pthread_mutex_unlock(fork);
        return (0);
    }
    print_status(philo, "has taken a fork");
    return (1);
}

static void release_forks(t_philosopher *philo)
{
    pthread_mutex_unlock(&philo->table->forks[philo->left_fork_id]);
    pthread_mutex_unlock(&philo->table->forks[philo->right_fork_id]);
    printf("Philosopher %d released forks %d and %d\n", 
           philo->id, philo->left_fork_id, philo->right_fork_id);
}

static int take_forks(t_philosopher *philo)
{
    pthread_mutex_t *left_fork = &philo->table->forks[philo->left_fork_id];
    pthread_mutex_t *right_fork = &philo->table->forks[philo->right_fork_id];

    if (philo->table->simulation_end)
        return (0);
    if (!safe_mutex_lock(&philo->table->waiter))
        return (0);
    if (philo->table->simulation_end)
    {
        safe_mutex_unlock(&philo->table->waiter);
        return (0);
    }
    if (philo->id % 2 == 0)
    {
        if (!lock_fork(left_fork, philo) || !lock_fork(right_fork, philo))
        {
            release_forks(philo);
            safe_mutex_unlock(&philo->table->waiter);
            return (0);
        }
    }
    else
    {
        if (!lock_fork(right_fork, philo) || !lock_fork(left_fork, philo))
        {
            release_forks(philo);
            safe_mutex_unlock(&philo->table->waiter);
            return (0);
        }
    }
    safe_mutex_unlock(&philo->table->waiter);
    return (1);
}

int think(t_philosopher *philo)
{
    if (philo->table->simulation_end)
        return (0);
    print_status(philo, "is thinking");
    return (1);
}

int eat(t_philosopher *philo)
{
    if (philo->table->simulation_end)
        return (0);
    if (!take_forks(philo))
        return (0);
    print_status(philo, "is eating");
    safe_mutex_lock(&philo->table->write_lock);
    philo->last_meal_time = get_current_time();
    philo->meals_eaten++;
    safe_mutex_unlock(&philo->table->write_lock);
    smart_sleep(philo->table->time_to_eat, philo->table);
    release_forks(philo);
    if (philo->table->simulation_end)
        return (0);
    return (1);
}

int sleep_philosopher(t_philosopher *philo)
{
    if (philo->table->simulation_end)
        return (0);
    print_status(philo, "is sleeping");
    smart_sleep(philo->table->time_to_sleep, philo->table);
    return (1);
}

void *philosopher_routine(void *arg)
{
    t_philosopher *philo = (t_philosopher *)arg;

    if (philo->table->num_philosophers == 1)
        return (handle_single_philosopher(philo));

    if (philo->id % 2 == 0)
        usleep(100); // Stagger start

    while (1)
    {
        if (philo->table->simulation_end)
        {
            printf("Philosopher %d detected simulation end, exiting\n", philo->id);
            break;
        }
        if (!think(philo) || !eat(philo) || !sleep_philosopher(philo))
        {
            printf("Philosopher %d exiting due to state failure\n", philo->id);
            break;
        }
    }
    printf("Philosopher %d thread terminated\n", philo->id);
    return (NULL);
}
