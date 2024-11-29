/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 17:07:05 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/29 03:26:15 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int check_philosopher_death(t_philosopher *philosopher)
{
    long long current_time;
    t_shared *shared;

    shared = philosopher->shared_data;
    current_time = get_current_time_ms();
    if (current_time - philosopher->last_meal_time > shared->time_to_die)
    {
        pthread_mutex_lock(&shared->log_lock);
        if (!shared->simulation_end)
            printf("%lld %d died\n", current_time, philosopher->id);
        pthread_mutex_unlock(&shared->log_lock);
        return (1);
    }
    return (0);
}

int check_all_meals(t_shared *shared, t_philosopher *philosophers)
{
    int i;
    int all_meals_met;

    if (shared->meals_required == -1)
        return (0);
    all_meals_met = 1;
    i = 0;
    while (i < shared->num_philosophers)
    {
        if (philosophers[i].meals_eaten < shared->meals_required)
        {
            all_meals_met = 0;
            break;
        }
        i++;
    }
    return (all_meals_met);
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
            shared->simulation_end = 1;
            pthread_mutex_unlock(&shared->log_lock);
            return (1);
        }
        i++;
    }
    if (check_all_meals(shared, data->philosophers))
    {
        pthread_mutex_lock(&shared->log_lock);
        shared->simulation_end = 1;
        pthread_mutex_unlock(&shared->log_lock);
        return (1);
    }

    return (0);
}

void *monitor_routine(void *arg)
{
    t_thread_data *data;
    t_shared *shared;

    data = (t_thread_data *)arg;
    shared = data->shared;

    while (!shared->simulation_end)
    {
        if (check_termination_conditions(data))
            break;
        usleep(200);
    }
    int i = 0;
    while (i < shared->num_philosophers)
    {
        pthread_join(data->threads[i], NULL);
        i++;
    }
    return (NULL);
}
