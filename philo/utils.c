/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 17:54:42 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/20 16:43:44 by rkhakimu         ###   ########.fr       */
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

void log_action(t_philosopher *philo, const char *action)
{
    pthread_mutex_lock(&philo->shared->write_lock);
    printf("%ld %d %s\n", get_current_time(), philo->id, action);
    pthread_mutex_unlock(&philo->shared->write_lock);
}

int handle_one_philosopher(t_shared *shared)
{
    if (shared->num_philosophers == 1)
    {
        printf("%ld 1 has taken a fork\n", get_current_time());
        usleep(shared->time_to_die * 1000); // Wait for the philosopher to "die"
        printf("%ld 1 died\n", get_current_time());
        return (1);
    }
    return (0);
}
