/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:37:31 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/23 14:52:51 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	destroy_mutex(pthread_mutex_t *mutex, const char *error_message)
{
    if (pthread_mutex_destroy(mutex) != 0)
        printf("Error: %s\n", error_message);
}

void	destroy_forks(t_shared *shared)
{
    int	i;

	i = 0;
    while (i < shared->num_philosophers)
    {
        destroy_mutex(&shared->forks[i], "Failed to destroy fork mutex");
        i++;
    }
}

void	destroy_shared_mutexes(t_shared *shared)
{
    destroy_mutex(&shared->write_lock, "Failed to destroy write mutex");
    destroy_mutex(&shared->monitor_lock, "Failed to destroy monitor mutex");
}

void	free_resources(t_shared *shared, t_philosopher *philosophers)
{
    free(shared->forks);
    free(philosophers);
}

void	cleanup_simulation(t_shared *shared, t_philosopher *philosophers)
{
    destroy_forks(shared);
    destroy_shared_mutexes(shared);
    free_resources(shared, philosophers);
}
