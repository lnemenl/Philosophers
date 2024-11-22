/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_management.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 15:43:43 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/22 19:34:57 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	create_threads(t_shared *shared, t_philosopher *philosophers, pthread_t *threads)
{
    int i;

	i = 0;
    while (i < shared->num_philosophers)
    {
        if (pthread_create(&threads[i], NULL, philosopher_routine, &philosophers[i]) != 0)
        {
            printf("Error: Failed to create thread for philosopher %d\n", i + 1);
            shared->is_simulation_running = 0;
            return (1);
        }
        usleep(100);
        i++;
    }
    if (pthread_create(&threads[i], NULL, monitor_health, philosophers) != 0)
    {
        printf("Error: Failed to create monitor thread\n");
        shared->is_simulation_running = 0;
        return (1);
    }
    return (0);
}

void	join_threads(t_shared *shared, pthread_t *threads)
{
    int i;

	i = 0;
    while (i < shared->num_philosophers + 1)
    {
        pthread_join(threads[i], NULL);
        i++;
    }
}

int	start_threads(t_shared *shared, t_philosopher *philosophers, pthread_t **threads)
{
    *threads = malloc(sizeof(pthread_t) * (shared->num_philosophers + 1));
    if (!(*threads))
    {
        printf("Error: Memory allocation for threads failed\n");
        return (1);
    }
    if (create_threads(shared, philosophers, *threads))
    {
        free(*threads);
        return (1);
    }
    return (0);
}
