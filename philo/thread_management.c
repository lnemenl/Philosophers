/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_management.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 15:43:43 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/12/02 14:15:58 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int launch_threads(t_thread_data *data, int *cleanup_flags)
{
    int thread_created_count;

    thread_created_count = 0;
    while (thread_created_count < data->shared->num_philosophers)
    {
        if (pthread_create(&data->threads[thread_created_count], NULL,
                           philosopher_routine, (void *)&data->philosophers[thread_created_count]) != 0)
        {
            printf("Error: Failed to create philosopher thread %d.\n", thread_created_count + 1);
            *cleanup_flags |= THREADS_INITIALIZED;
            clean_up_simulation(data, data->shared, *cleanup_flags);
            return (0);
        }
        thread_created_count++;
    }
    *cleanup_flags |= THREADS_INITIALIZED;
    if (pthread_create(&data->monitor_thread, NULL, monitor_routine, (void *)data) != 0)
    {
        printf("Error: Failed to create monitor thread.\n");
        clean_up_simulation(data, data->shared, *cleanup_flags);
        return (0);
    }
    return (1);
}
