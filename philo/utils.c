/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 17:54:42 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/21 12:58:34 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long	get_current_time(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

const char	*skip_whitespace(const char *str)
{
	while ((*str >= 9 && *str <= 13) || *str == ' ')
		str++;
	return (str);
}

int	get_sign(const char **str)
{
	int	sign;
	
	sign = 1;
	if (**str == '-' || **str == '+')
	{
		if (**str == '-')
			sign = -1;
		(*str)++;
	}
	return (sign);
}

void	log_action(t_philosopher *philo, const char *action)
{
	pthread_mutex_lock(&philo->shared->write_lock);
	if (philo->shared->is_simulation_running) // Log only if the simulation is still running
		printf("%ld %d %s\n", get_current_time(), philo->id, action);
	pthread_mutex_unlock(&philo->shared->write_lock);
}

int	handle_one_philosopher(t_shared *shared)
{
	if (shared->num_philosophers == 1)
	{
		printf("%ld 1 has taken a fork\n", get_current_time());
		usleep(shared->time_to_die * 1000);
		printf("%ld 1 died\n", get_current_time());
		return (1);
	}
	return (0);
}
void	wait_or_exit(t_philosopher *philo, int milliseconds)
{
	long start_time = get_current_time();
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

