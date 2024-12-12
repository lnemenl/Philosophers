/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 17:54:42 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/12/12 13:11:13 by rkhakimu         ###   ########.fr       */
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
    long long timestamp;

    shared = philosopher->shared_data;
    timestamp = get_current_time_ms();
    pthread_mutex_lock(&shared->log_lock);
    if (!is_simulation_end(shared))
        printf("%lld %d %s\n", timestamp, philosopher->id, action);
    pthread_mutex_unlock(&shared->log_lock);
}

void smart_sleep(int duration, t_shared *shared)
{
    long long start_time;
    
    start_time = get_current_time_ms();
    while (!is_simulation_end(shared) && (get_current_time_ms() - start_time < duration))
        usleep(50);
}

int	safe_atoi(const char *str, int *result)
{
	long long	num;
	int			sign;
	int			i;

	num = 0;
	sign = 1;
	i = 0;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i])
	{
		num = num * 10 + (str[i] - '0');
		if ((sign == 1 && num > INT_MAX) || (sign == -1 && -num < INT_MIN))
			return (0);
		i++;
	}
	*result = (int)(num * sign);
	return (1);
}

int	is_simulation_end(t_shared *shared)
{
	int	result;
	
	pthread_mutex_lock(&shared->simulation_end_lock);
	result = shared->simulation_end;
	pthread_mutex_unlock(&shared->simulation_end_lock);
	return (result);
}

void	set_simulation_end(t_shared *shared, int value)
{
	pthread_mutex_lock(&shared->simulation_end_lock);
	shared->simulation_end = value;
	pthread_mutex_unlock(&shared->simulation_end_lock);
}
