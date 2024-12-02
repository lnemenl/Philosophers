/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:37:31 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/29 08:23:28 by rkhakimu         ###   ########.fr       */
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
void destroy_forks(t_shared *shared)
{
    int i;

    i = 0;
    while (i < shared->num_philosophers)
    {
        pthread_mutex_destroy(&shared->forks[i]);
        i++;
    }
    free(shared->forks);
    shared->forks = NULL;
}

void handle_initialization_failure(t_thread_data *data, int thread_created_count)
{
    // Clean up partially created threads
    while (--thread_created_count >= 0)
        pthread_join(data->threads[thread_created_count], NULL);
    if (data->shared && data->shared->forks)
    {
        destroy_forks(data->shared);
        free(data->shared->forks);
        data->shared->forks = NULL;
    }
    if (data->shared)
        pthread_mutex_destroy(&data->shared->log_lock);
    free(data->threads);
    free(data->philosophers);
}

void cleanup_partial_thread_data(t_thread_data *data)
{
    if (data->philosophers)
    {
        free(data->philosophers);
        data->philosophers = NULL;
    }

    if (data->threads)
    {
        free(data->threads);
        data->threads = NULL;
    }
}
