/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 17:42:21 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/28 05:20:10 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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

    if (!safe_mutex_lock(&philo->table->waiter))
        return (0);

    if (philo->id % 2 == 0)
    {
        if (!safe_mutex_lock(left_fork))
            return (safe_mutex_unlock(&philo->table->waiter), 0);
        if (!safe_mutex_lock(right_fork))
        {
            pthread_mutex_unlock(left_fork);
            return (safe_mutex_unlock(&philo->table->waiter), 0);
        }
    }
    else
    {
        if (!safe_mutex_lock(right_fork))
            return (safe_mutex_unlock(&philo->table->waiter), 0);
        if (!safe_mutex_lock(left_fork))
        {
            pthread_mutex_unlock(right_fork);
            return (safe_mutex_unlock(&philo->table->waiter), 0);
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
    return (!philo->table->simulation_end);
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
    return (!philo->table->simulation_end);
}

int sleep_philosopher(t_philosopher *philo)
{
    if (philo->table->simulation_end)
        return (0);
    print_status(philo, "is sleeping");
    smart_sleep(philo->table->time_to_sleep, philo->table);
    return (!philo->table->simulation_end);
}

void *philosopher_routine(void *arg)
{
    t_philosopher *philo = (t_philosopher *)arg;

    if (philo->table->num_philosophers == 1)
        return (handle_single_philosopher(philo));
    if (philo->id % 2 == 0)
        usleep(100); 
    while (!get_simulation_status(philo->table))
    {
        if (!think(philo) || !get_simulation_status(philo->table))
            break;
        if (!eat(philo) || !get_simulation_status(philo->table))
            break;
        if (!sleep_philosopher(philo) || !get_simulation_status(philo->table))
            break;
    }
    return (NULL);
}
