/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_management.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 15:43:43 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/29 00:58:02 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int launch_philosopher_threads(t_shared *shared, pthread_t *threads, t_philosopher *philosophers)
{
    int i;

    i = 0;
    while (i < shared->num_philosophers)
    {
        if (pthread_create(&threads[i], NULL, philosopher_routine, (void *)&philosophers[i]) != 0)
        {
            while (--i >= 0)
                pthread_detach(threads[i]);
            return (0);
        }
        i++;
    }
    return (1);
}

int launch_monitor_thread(pthread_t *monitor_thread, t_shared *shared)
{
    if (pthread_create(monitor_thread, NULL, monitor_routine, (void *)shared) != 0)
        return (0);
    return (1);
}

int launch_threads(t_thread_data *data)
{
    int i;
    
    i = 0;
    if (!launch_philosopher_threads(data->shared, data->threads, data->philosophers))
    {
        printf("Error: Failed to create philosopher threads.\n");
        return (0);
    }
    if (!launch_monitor_thread(&data->monitor_thread, data->shared))
    {
        printf("Error: Failed to create monitor thread.\n");
        while (i < data->shared->num_philosophers)
        {
            pthread_detach(data->threads[i]);
            i++;
        }
        return (0);
    }
    return (1);
}
