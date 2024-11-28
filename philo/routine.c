/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 17:42:21 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/28 23:32:45 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"


void *philosopher_routine(void *arg)
{
    t_philosopher *philosopher = (t_philosopher *)arg;
    t_shared *shared = philosopher->shared_data;

    while (!shared->simulation_end) {
        // Thinking
        log_action(philosopher, "is thinking");
        if (shared->simulation_end) // Check if simulation ended
            break;

        // Take forks
        if (!take_forks(philosopher))
            break;

        // Eating
        if (!eat(philosopher))
            break;

        // Put forks down
        put_forks(philosopher);

        // Sleeping
        log_action(philosopher, "is sleeping");
        smart_sleep(shared->time_to_sleep, shared);
    }
    return (NULL);
}

int take_forks(t_philosopher *philosopher)
{
    t_shared *shared = philosopher->shared_data;

    pthread_mutex_lock(philosopher->left_fork);
    log_action(philosopher, "has taken a fork");

    if (shared->simulation_end)
    {
        pthread_mutex_unlock(philosopher->left_fork);
        return (0);
    }
    pthread_mutex_lock(philosopher->right_fork);
    log_action(philosopher, "has taken a fork");
    return (1);
}

void put_forks(t_philosopher *philosopher)
{
    pthread_mutex_unlock(philosopher->right_fork);
    pthread_mutex_unlock(philosopher->left_fork);
}

int eat(t_philosopher *philosopher)
{
    t_shared *shared;
    
    shared = philosopher->shared_data;
    log_action(philosopher, "is eating");
    philosopher->last_meal_time = get_current_time_ms();
    philosopher->meals_eaten++;
    smart_sleep(shared->time_to_eat, shared);
    if (shared->simulation_end)
        return (0);
    return (1);
}
