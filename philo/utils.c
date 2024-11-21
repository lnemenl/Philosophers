/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 17:54:42 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/21 16:27:12 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long	get_current_time(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

void	log_action(t_philosopher *philo, const char *action)
{
    pthread_mutex_lock(&philo->shared->write_lock);
    if (philo->shared->is_simulation_running)
        printf("%ld %d %s\n", get_current_time(), philo->id, action);
    pthread_mutex_unlock(&philo->shared->write_lock);
}

void	wait_or_exit(t_philosopher *philo, int milliseconds)
{
    long	start_time;

	start_time = get_current_time();
    while (get_current_time() - start_time < milliseconds)
    {
        if (!philo->shared->is_simulation_running)
            return;
        usleep(100);
    }
}

void	stop_simulation(t_shared *shared)
{
    pthread_mutex_lock(&shared->monitor_lock);
    shared->is_simulation_running = 0;
    pthread_mutex_unlock(&shared->monitor_lock);
}
