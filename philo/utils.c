/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 17:54:42 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/12/18 07:22:15 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long long	get_current_time_ms(void)
{
	struct timeval		tv;
	static long long	start;
	long long			current;

	start = -1;
	if (gettimeofday(&tv, NULL) != 0)
	{
		log_error("Error: Failed to get current time");
		return (0);
	}
	current = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	if (start == -1)
		start = current;
	return (current - start);
}

void	smart_sleep(int duration, t_shared *shared)
{
	long long	start_time;

	start_time = get_current_time_ms();
	while (!is_simulation_end(shared)
		&& (get_current_time_ms() - start_time < duration))
		usleep(10);
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
	pthread_mutex_lock(&philosopher->meal_lock);
	philosopher->last_meal_time = get_current_time_ms();
	pthread_mutex_unlock(&philosopher->meal_lock);
}
