/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:37:31 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/12/03 09:30:28 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void clean_up_simulation(t_thread_data *data, t_shared *shared, int cleanup_flags)
{
    int i;
    
    if (cleanup_flags & THREADS_INITIALIZED)
    {
        i = 0;
        if (data)
        {
            while (i < shared->num_philosophers)
            {
                pthread_join(data->threads[i], NULL);
                i++;
            }
            pthread_join(data->monitor_thread, NULL);
        }
    }
    if (cleanup_flags & MUTEXES_INITIALIZED)
    {
        if (shared && shared->forks)
        {
            i = 0;
            while (i < shared->num_philosophers)
            {
                pthread_mutex_destroy(&shared->forks[i]);
                i++;
            }
        }
        pthread_mutex_destroy(&shared->log_lock);
        pthread_mutex_destroy(&shared->simulation_end_lock);
    }
    if (cleanup_flags & MEMORY_ALLOCATED)
    {
        if (data)
        {
            free(data->threads);
            free(data->philosophers);
        }
        if (shared && shared->forks)
        {
            free(shared->forks);
            shared->forks = NULL;
        }
    }
}
