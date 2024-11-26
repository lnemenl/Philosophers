/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 17:07:05 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/26 19:54:12 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void check_starvation(t_philosopher *philosophers, t_shared *shared)
{
    int i;
    long current_time;

    i = 0;
    while (i < shared->num_philosophers)
    {
        pthread_mutex_lock(&philosophers[i].meal_mutex);
        current_time = get_current_time();
        if (current_time - philosophers[i].last_meal_time > shared->time_to_die)
        {
            pthread_mutex_unlock(&philosophers[i].meal_mutex);
            pthread_mutex_lock(&shared->write_mutex);
            log_action(&philosophers[i], "died");
            shared->is_simulation_running = 0;
            pthread_mutex_unlock(&shared->write_mutex);
            return;
        }
        pthread_mutex_unlock(&philosophers[i].meal_mutex);
        i++;
    }
}

int check_all_meals(t_philosopher *philosophers, t_shared *shared)
{
    int i;
    
    i = 0;
    while (i < shared->num_philosophers)
    {
        if (philosophers[i].meals_eaten < shared->num_meals_required)
            return (0);
        i++;
    }
    pthread_mutex_lock(&shared->write_mutex);
    if (shared->is_simulation_running)
        shared->is_simulation_running = 0;
    pthread_mutex_unlock(&shared->write_mutex);
    return (1);
}

void *monitor_health(void *arg)
{
    t_philosopher *philosophers = (t_philosopher *)arg;
    t_shared *shared = philosophers[0].shared;

    while (1)
    {
        pthread_mutex_lock(&shared->write_mutex);
        if (!shared->is_simulation_running)
        {
            pthread_mutex_unlock(&shared->write_mutex);
            break;
        }
        pthread_mutex_unlock(&shared->write_mutex);
        check_starvation(philosophers, shared);
        if (shared->num_meals_required > 0 && check_all_meals(philosophers, shared))
            break;
        usleep(100);
    }
    return NULL;
}
