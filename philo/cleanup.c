/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:37:31 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/26 19:50:35 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void destroy_fork_mutexes(pthread_mutex_t *forks_mutex, int num_philosophers)
{
    int i;

    i = 0;
    if (!forks_mutex)
        return ;
    while (i < num_philosophers)
    {
        pthread_mutex_destroy(&forks_mutex[i]);
        i++;
    }
    free(forks_mutex);
}


void destroy_write_mutex(pthread_mutex_t *write_mutex)
{
    pthread_mutex_destroy(write_mutex);
}

void cleanup_shared_resources(t_shared *shared)
{
    destroy_fork_mutexes(shared->forks_mutex, shared->num_philosophers);
    destroy_write_mutex(&shared->write_mutex);
}

void cleanup_simulation(t_shared *shared, t_philosopher *philosophers)
{
    int i;

    i = 0;
    while (i < shared->num_philosophers)
    {
        pthread_mutex_destroy(&philosophers[i].meal_mutex);
        i++;
    }
    cleanup_shared_resources(shared);
    free(philosophers);
}

