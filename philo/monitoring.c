/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 17:07:05 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/12/04 11:37:44 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int check_philosopher_death(t_philosopher *philosopher)
{
    long long current_time;
    t_shared *shared;

    shared = philosopher->shared_data;
    current_time = get_current_time_ms();
    pthread_mutex_lock(&philosopher->meal_lock);
    if (current_time - philosopher->last_meal_time > shared->time_to_die)
    {
        pthread_mutex_lock(&shared->log_lock);
        if (!is_simulation_end(shared))
            printf("%lld %d died\n", current_time, philosopher->id);
        pthread_mutex_unlock(&shared->log_lock);
        pthread_mutex_unlock(&philosopher->meal_lock);
        return (1);
    }
    pthread_mutex_unlock(&philosopher->meal_lock);
    return (0);
}

int check_all_meals(t_shared *shared, t_philosopher *philosophers)
{
    int i;

    if (shared->meals_required == 0)
        return (1);
    i = 0;
    while (i < shared->num_philosophers)
    {
        pthread_mutex_lock(&philosophers[i].meal_lock);
        if (philosophers[i].meals_eaten < shared->meals_required)
        {
            pthread_mutex_unlock(&philosophers[i].meal_lock);
            return (0);
        }
        pthread_mutex_unlock(&philosophers[i].meal_lock);
        i++;
    }
    return (1);
}


int check_termination_conditions(t_thread_data *data)
{
    int i;
    t_shared *shared;

    shared = data->shared;
    i = 0;
    while (i < shared->num_philosophers)
    {
        if (check_philosopher_death(&data->philosophers[i]))
        {
            pthread_mutex_lock(&shared->log_lock);
            set_simulation_end(shared, 1);
            pthread_mutex_unlock(&shared->log_lock);
            return (1);
        }
        i++;
    }
    if (check_all_meals(shared, data->philosophers))
    {
        pthread_mutex_lock(&shared->log_lock);
        set_simulation_end(shared, 1);
        pthread_mutex_unlock(&shared->log_lock);
        return (1);
    }
    return (0);
}

void *monitor_routine(void *arg)
{
    t_thread_data *data = (t_thread_data *)arg;
    t_shared *shared = data->shared;
    
    if (shared->meals_required == 0)
    {
        set_simulation_end(shared, 1);
        return (NULL);
    }
    while (!is_simulation_end(shared))
    {
        if (check_termination_conditions(data))
        {
            set_simulation_end(shared, 1);
            break;
        }
        usleep(200);
    }
    return (NULL);
}
