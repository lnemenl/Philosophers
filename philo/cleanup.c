/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:37:31 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/20 15:34:06 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void destroy_forks(t_shared *shared)
{
    int i;

    i = 0;
    while (i < shared->num_philosophers)
    {
        pthread_mutex_destroy(&shared->forks[i]);
        i++;
    }
}

void destroy_shared_mutexes(t_shared *shared)
{
    pthread_mutex_destroy(&shared->write_lock);
    pthread_mutex_destroy(&shared->monitor_lock);
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
