/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_management.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 15:43:43 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/12/12 16:27:51 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// int launch_threads(t_thread_data *data, int *cleanup_flags)
// {
//     int i;

//     i = 0;
//     while (i < data->shared->num_philosophers)
//     {
//         if (pthread_create(&data->threads[i], NULL, philosopher_routine, (void *)&data->philosophers[i]) != 0)
//         {
//             printf("Error: Failed to create philosopher thread %d.\n", i + 1);
//             *cleanup_flags |= THREADS_INITIALIZED;
//             while (i > 0)
//             {
//                 i--;
//                 pthread_join(data->threads[i], NULL);
//             }
//             clean_up_simulation(data, data->shared, *cleanup_flags);
//             return (0);
//         }
//         i++;
//     }
//     *cleanup_flags |= THREADS_INITIALIZED;
//     if (pthread_create(&data->monitor_thread, NULL, monitor_routine, (void *)data) != 0)
//     {
//         printf("Error: Failed to create monitor thread.\n");
//         i = 0;
//         while (i < data->shared->num_philosophers)
//         {
//             pthread_join(data->threads[i], NULL);
//             i++;
//         }
//         clean_up_simulation(data, data->shared, *cleanup_flags);
//         return (0);
//     }
//     return (1);
// }


static int create_philosopher_threads(t_thread_data *data, int *cleanup_flags)
{
    int i;

    i = 0;
    while (i < data->shared->num_philosophers)
    {
        if (pthread_create(&data->threads[i], NULL, philosopher_routine,
                           (void *)&data->philosophers[i]) != 0)
        {
            printf("Error: Failed to create philosopher thread %d.\n", i + 1);
            *cleanup_flags |= THREADS_INITIALIZED;
            while (i > 0)
            {
                i--;
                pthread_join(data->threads[i], NULL);
            }
            return (0);
        }
        i++;
    }
    *cleanup_flags |= THREADS_INITIALIZED;
    return (1);
}

static int create_monitor_thread(t_thread_data *data)
{
    int i;

    if (pthread_create(&data->monitor_thread, NULL, monitor_routine, (void *)data) != 0)
    {
        printf("Error: Failed to create monitor thread.\n");
        i = 0;
        while (i < data->shared->num_philosophers)
        {
            pthread_join(data->threads[i], NULL);
            i++;
        }
        return (0);
    }
    return (1);
}

int launch_threads(t_thread_data *data, int *cleanup_flags)
{
    if (!create_philosopher_threads(data, cleanup_flags))
    {
        clean_up_simulation(data, data->shared, *cleanup_flags);
        return (0);
    }
    if (!create_monitor_thread(data))
    {
        clean_up_simulation(data, data->shared, *cleanup_flags);
        return (0);
    }
    return (1);
}
