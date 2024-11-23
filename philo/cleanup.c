/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:37:31 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/23 21:16:30 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	destroy_mutex(pthread_mutex_t *mutex, const char *error_message)
{
    if (pthread_mutex_destroy(mutex) != 0)
        printf("Error: %s\n", error_message);
}

void	destroy_shared_mutexes(t_shared *shared)
{
    destroy_mutex(&shared->forks_mutex, "Failed to destroy forks mutex");
    destroy_mutex(&shared->write_mutex, "Failed to destroy write mutex");
}

void	free_resources(t_shared *shared, t_philosopher *philosophers)
{
	if (shared->forks)
		free(shared->forks);
	if (philosophers)
		free(philosophers);
}

void	cleanup_simulation(t_shared *shared, t_philosopher *philosophers)
{
    destroy_shared_mutexes(shared);
    free_resources(shared, philosophers);
}
