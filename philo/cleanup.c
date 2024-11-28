/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:37:31 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/29 00:25:51 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void join_threads(t_thread_data *data)
{
    int i;
    
    i = 0;
    while (i < data->shared->num_philosophers)
    {
        pthread_join(data->threads[i], NULL);
        i++;
    }
    pthread_join(data->monitor_thread, NULL);
}

void destroy_mutexes(t_shared *shared)
{
    int i;
    
    i = 0;
    while (i < shared->num_philosophers)
    {
        pthread_mutex_destroy(&shared->forks[i]);
        i++;
    }
    pthread_mutex_destroy(&shared->log_lock);
}

void free_resources(t_thread_data *data)
{
    free(data->shared->forks);
    free(data->threads);
    free(data->philosophers);
}

void clean_up(t_thread_data *data)
{
    join_threads(data);
    destroy_mutexes(data->shared);
    free_resources(data);  
}
