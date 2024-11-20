/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_management.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 15:43:43 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/20 16:27:12 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int create_threads(t_shared *shared, t_philosopher *philosophers, pthread_t *threads)
{
    int i;

    i = 0;
    // Create philosopher threads
    while (i < shared->num_philosophers)
    {
        if (pthread_create(&threads[i], NULL, philosopher_routine, &philosophers[i]) != 0)
        {
            printf("Error: Failed to create thread for philosopher %d\n", i + 1);
            return (1); // Return error if thread creation fails
        }
        i++;
    }

    // Create monitor thread
    if (pthread_create(&threads[i], NULL, monitor_health, philosophers) != 0)
    {
        printf("Error: Failed to create monitor thread\n");
        return (1);
    }

    return (0); // Successfully created all threads
}

void join_threads(t_shared *shared, pthread_t *threads)
{
    int i;

    i = 0;
    while (i < shared->num_philosophers + 1)
    {
        pthread_join(threads[i], NULL); // Join philosopher and monitor threads
        i++;
    }
}

int start_threads(t_shared *shared, t_philosopher *philosophers, pthread_t **threads)
{
    // Allocate memory for thread handles
    *threads = malloc(sizeof(pthread_t) * (shared->num_philosophers + 1));
    if (!(*threads))
    {
        printf("Error: Memory allocation for threads failed\n");
        return (1); // Return error if allocation fails
    }

    // Create philosopher and monitor threads
    if (create_threads(shared, philosophers, *threads))
    {
        free(*threads); // Free allocated memory if thread creation fails
        return (1);
    }

    return (0); // Successfully started all threads
}
