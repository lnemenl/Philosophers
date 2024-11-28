/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 17:54:42 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/29 00:50:52 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long long get_current_time_ms(void)
{
    struct timeval tv;

    if (gettimeofday(&tv, NULL) != 0)
    {
        printf("Error: Failed to get current time\n");
        return (0);
    }
    return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void log_action(t_philosopher *philosopher, const char *action)
{
    t_shared *shared;

    shared = philosopher->shared_data;
    pthread_mutex_lock(&shared->log_lock);
    printf("%lld %d %s\n", get_current_time_ms(), philosopher->id, action);
    pthread_mutex_unlock(&shared->log_lock);
}

void smart_sleep(int duration, t_shared *shared)
{
    long long start_time;
    
    start_time = get_current_time_ms();
    while (!shared->simulation_end && (get_current_time_ms() - start_time < duration))
        usleep(50);
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
