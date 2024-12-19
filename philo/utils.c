/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 17:54:42 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/12/19 22:43:27 by rkhakimu         ###   ########.fr       */
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

void smart_sleep(int duration, t_shared *shared)
{
    long long start_time = get_current_time_ms();
    long long current_time;

    while (!is_simulation_end(shared))
    {
        current_time = get_current_time_ms();
        if (current_time - start_time >= duration)
            break ;
        usleep(50);
    }
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
		if ((sign == 1 && (num > INT_MAX)) || (sign == -1 && (-num < INT_MIN)))
			return (0);
		i++;
	}
	*result = (int)(num * sign);
	return (1);
}

void	update_last_meal_time(t_philosopher *philosopher)
{
	long long	current_time;

	current_time = get_current_time_ms();
	pthread_mutex_lock(&philosopher->meal_lock);
	philosopher->last_meal_time = current_time;
	pthread_mutex_unlock(&philosopher->meal_lock);
}
