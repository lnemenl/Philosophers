/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:37:31 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/20 16:50:55 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void destroy_forks(t_shared *shared)
{
    int i;

    i = 0;
    while (i < shared->num_philosophers)
    {
        if (pthread_mutex_destroy(&shared->forks[i]) != 0)
            printf("Error: Failed to destroy fork mutex %d\n", i);
        i++;
    }
}

void destroy_shared_mutexes(t_shared *shared)
{
    if (pthread_mutex_destroy(&shared->write_lock) != 0)
        printf("Error: Failed to destroy write_lock mutex\n");
    if (pthread_mutex_destroy(&shared->monitor_lock) != 0)
        printf("Error: Failed to destroy monitor_lock mutex\n");
}

void free_resources(t_shared *shared, t_philosopher *philosophers)
{
    free(shared->forks);
    free(philosophers);
}

void cleanup_simulation(t_shared *shared, t_philosopher *philosophers)
{
    destroy_forks(shared);
    destroy_shared_mutexes(shared);
    free_resources(shared, philosophers);
}
