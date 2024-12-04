/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:37:31 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/12/04 11:23:07 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void clean_up_simulation(t_thread_data *data, t_shared *shared, int cleanup_flags)
{
    int i;

    i = 0;
    if ((cleanup_flags & THREADS_INITIALIZED) && data && data->threads)
    {
        while (i < shared->num_philosophers)
        {
            pthread_join(data->threads[i], NULL);
            i++;
        }
        pthread_join(data->monitor_thread, NULL);
    }
    i = 0;
    if ((cleanup_flags & MUTEXES_INITIALIZED) && shared && shared->forks)
    {
        while (i < shared->num_philosophers)
        {
            pthread_mutex_destroy(&shared->forks[i]);
            i++;
        }
        i = 0;
        if (data && data->philosophers)
        {
            while (i < shared->num_philosophers)
            {
                pthread_mutex_destroy(&data->philosophers[i].meal_lock);
                i++;
            }
        }
        pthread_mutex_destroy(&shared->log_lock);
        pthread_mutex_destroy(&shared->simulation_end_lock);
    }
    if ((cleanup_flags & MEMORY_ALLOCATED))
    {
        if (data)
        {
            if (data->threads)
                free(data->threads);
            if (data->philosophers)
                free(data->philosophers);
        }
        if (shared && shared->forks)
        {
            free(shared->forks);
            shared->forks = NULL;
        }
    }
}
