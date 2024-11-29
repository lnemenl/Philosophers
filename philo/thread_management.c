/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_management.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 15:43:43 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/29 03:05:31 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int launch_philosopher_threads(t_thread_data *data)
{
    int i;

    i = 0;
    while (i < data->shared->num_philosophers)
    {
        if (pthread_create(&data->threads[i], NULL, philosopher_routine, (void *)&data->philosophers[i]) != 0)
        {
            printf("Error: Failed to create philosopher thread %d.\n", i + 1);
            while (--i >= 0)
                pthread_join(data->threads[i], NULL);
            return (0);
        }
        i++;
    }
    return (1);
}

int launch_monitor_thread(t_thread_data *data)
{
    if (pthread_create(&data->monitor_thread, NULL, monitor_routine, (void *)data) != 0)
    {
        printf("Error: Failed to create monitor thread.\n");
        return (0);
    }
    return (1);
}

int launch_threads(t_thread_data *data)
{
    int thread_created_count;

    thread_created_count = 0;
    while (thread_created_count < data->shared->num_philosophers)
    {
        if (pthread_create(&data->threads[thread_created_count], NULL,
                           philosopher_routine, (void *)&data->philosophers[thread_created_count]) != 0)
        {
            printf("Error: Failed to create philosopher thread %d.\n", thread_created_count + 1);
            handle_initialization_failure(data, thread_created_count);
            return (0);
        }
        thread_created_count++;
    }
    if (pthread_create(&data->monitor_thread, NULL, monitor_routine, (void *)data) != 0)
    {
        printf("Error: Failed to create monitor thread.\n");
        handle_initialization_failure(data, thread_created_count);
        return (0);
    }
    return (1);
}
