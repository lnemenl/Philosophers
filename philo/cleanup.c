/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:37:31 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/27 17:12:17 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	destroy_fork_mutexes(t_shared *shared, int i)
{
    if (i >= shared->num_philosophers)
        return;
    
    pthread_mutex_destroy(&shared->fork_mutexes[i]);
    destroy_fork_mutexes(shared, i + 1);
}

void	destroy_shared_mutexes(t_shared *shared)
{
    destroy_fork_mutexes(shared, 0);
    pthread_mutex_destroy(&shared->write_mutex);
}

void	free_resources(t_shared *shared, t_philosopher *philosophers)
{
    // Remove forks array free
    if (shared->fork_mutexes)
        free(shared->fork_mutexes);
    if (philosophers)
        free(philosophers);
}

void	cleanup_simulation(t_shared *shared, t_philosopher *philosophers)
{
    destroy_shared_mutexes(shared);
    free_resources(shared, philosophers);
}
