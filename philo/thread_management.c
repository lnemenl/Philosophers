/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_management.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 15:43:43 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/26 18:17:19 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"


int create_philosopher_threads(t_philosopher *philosophers, pthread_t *threads, int num_philosophers)
{
    int i;
	
	i = 0;
    while (i < num_philosophers)
	{
        if (pthread_create(&threads[i], NULL, philosopher_routine, &philosophers[i]) != 0)
		{
            printf("Error: Failed to create thread for philosopher %d.\n", i + 1);
            return (1);
        }
        i++;
    }
    return (0);
}

int create_monitoring_thread(pthread_t *monitor_thread, t_philosopher *philosophers)
{
    if (pthread_create(monitor_thread, NULL, monitor_health, philosophers) != 0)
	{
        printf("Error: Failed to create monitoring thread.\n");
        return (1);
    }
    return (0);
}

void join_philosopher_threads(pthread_t *threads, int num_philosophers)
{
    int i;
	
	i = 0;
    while (i < num_philosophers)
	{
        pthread_join(threads[i], NULL);
        i++;
    }
}

void join_monitoring_thread(pthread_t monitor_thread)
{
    pthread_join(monitor_thread, NULL);
}


int manage_threads(t_philosopher *philosophers, t_shared *shared)
{
    pthread_t *threads;
    pthread_t monitor_thread;

    threads = malloc(sizeof(pthread_t) * shared->num_philosophers);
    if (!threads)
	{
        printf("Error: Failed to allocate memory for threads.\n");
        return (1);
    }
    if (create_philosopher_threads(philosophers, threads, shared->num_philosophers))
	{
        free(threads);
        return (1);
    }
    if (create_monitoring_thread(&monitor_thread, philosophers))
	{
        free(threads);
        return (1);
    }
    join_philosopher_threads(threads, shared->num_philosophers);
    join_monitoring_thread(monitor_thread);
    free(threads);
    return (0);
}
