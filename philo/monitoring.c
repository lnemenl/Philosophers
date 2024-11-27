/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 17:07:05 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/27 17:12:15 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	has_starvation_occurred(t_philosopher *philosophers, int num_philosophers, int time_to_die)
{
    int		i;
    long	current_time;
    int     starved_philosopher;

    i = 0;
    starved_philosopher = 0;
    while (i < num_philosophers)
    {
        pthread_mutex_lock(&philosophers[i].shared->fork_mutexes[i]);
        current_time = get_current_time();
        if (current_time - philosophers[i].last_meal_time > time_to_die)
        {
            starved_philosopher = i + 1;
            pthread_mutex_unlock(&philosophers[i].shared->fork_mutexes[i]);
            break;
        }
        pthread_mutex_unlock(&philosophers[i].shared->fork_mutexes[i]);
        i++;
    }
    return (starved_philosopher);
}


void	*monitor_health(void *arg)
{
    t_philosopher	*philosophers;
    t_shared		*shared;
    int				starved_philosopher;

    philosophers = (t_philosopher *)arg;
    shared = philosophers[0].shared;

    while (shared->is_simulation_running)
    {
        starved_philosopher = has_starvation_occurred(philosophers, shared->num_philosophers, shared->time_to_die);
        if (starved_philosopher > 0)
        {
            pthread_mutex_lock(&shared->write_mutex);
            if (shared->is_simulation_running)
            {
                printf("%ld %d died\n", get_current_time(), starved_philosopher);
                shared->is_simulation_running = 0;
            }
            pthread_mutex_unlock(&shared->write_mutex);
            return (NULL);
        }
        usleep(1000);
    }
    return NULL;
}
