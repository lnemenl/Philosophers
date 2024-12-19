/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 23:22:55 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/12/19 20:54:50 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	log_action(t_philosopher *philosopher, const char *action, long long current_time)
{
	t_shared	*shared;

	shared = philosopher->shared_data;
	pthread_mutex_lock(&shared->log_lock);
	if (!is_simulation_end(shared))
	{
		printf("%lld %d %s\n", current_time - shared->start_time, philosopher->id, action);
	}
	pthread_mutex_unlock(&shared->log_lock);
}

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	log_error(const char *message)
{
	int	len;

	len = ft_strlen(message);
	write(STDERR_FILENO, message, len);
	write(STDERR_FILENO, "\n", 1);
}
