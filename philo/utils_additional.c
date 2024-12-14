/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_additional.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 12:29:22 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/12/14 02:47:27 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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

size_t ft_strlen(const char *s)
{
    size_t len;
	len = 0;
    while (s[len])
        len++;
    return (len);
}

void log_error(const char *msg)
{
    int len;

    len = ft_strlen(msg);
    write(STDERR_FILENO, msg, len);
    write(STDERR_FILENO, "\n", 1);
}
