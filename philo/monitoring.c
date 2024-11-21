/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 17:07:05 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/21 16:14:49 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	has_starvation_occurred(t_philosopher *philosophers, t_shared *shared)
{
    int i;

	i = 0;
    while (i < shared->num_philosophers)
    {
        pthread_mutex_lock(&shared->monitor_lock);
        if (get_current_time() - philosophers[i].last_meal_time > shared->time_to_die)
        {
            log_action(&philosophers[i], "died");
            shared->is_simulation_running = 0;
            pthread_mutex_unlock(&shared->monitor_lock);
            return (1);
        }
        pthread_mutex_unlock(&shared->monitor_lock);
        i++;
    }
    return (0);
}

void	*monitor_health(void *arg)
{
    t_philosopher	*philosophers;
    t_shared		*shared;

	philosophers = (t_philosopher *)arg;
	shared = philosophers[0].shared;
    while (shared->is_simulation_running)
    {
        if (has_starvation_occurred(philosophers, shared))
            break;
        usleep(500);
    }
    return (NULL);
}
