/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 17:54:42 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/28 05:18:36 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long long get_current_time(void)
{
    struct timeval tv;

    if (gettimeofday(&tv, NULL) != 0)
    {
        printf("Error: Failed to get current time\n");
        return (0);
    }
    return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void print_status(t_philosopher *philo, const char *status)
{
    long long timestamp;

    safe_mutex_lock(&philo->table->write_lock);
    if (!philo->table->simulation_end)
    {
        timestamp = get_current_time() - philo->table->start_time;
        printf("%lld %d %s\n", timestamp, philo->id, status);
    }
    safe_mutex_unlock(&philo->table->write_lock);
}

void smart_sleep(long long duration, t_table *table)
{
    long long start = get_current_time();

    while (get_current_time() - start < duration)
    {
        if (get_simulation_status(table))
            break;
        usleep(50);
    }
}

int safe_mutex_lock(pthread_mutex_t *mutex)
{
    if (pthread_mutex_lock(mutex) != 0)
    {
        printf("Error: Mutex lock failed\n");
        return (0);
    }
    return (1);
}

int safe_mutex_unlock(pthread_mutex_t *mutex)
{
    if (pthread_mutex_unlock(mutex) != 0)
    {
        printf("Error: Mutex unlock failed\n");
        return (0);
    }
    return (1);
}
