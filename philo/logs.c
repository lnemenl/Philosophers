/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 23:22:55 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/12/19 19:38:18 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	ft_putnbr_fd(long long n, int fd)
{
	char	c;

	if (n >= 10)
		ft_putnbr_fd(n / 10, fd);
	c = (n % 10) + '0';
	write(fd, &c, 1);
}

static void	ft_putstr_fd(char *s, int fd)
{
	int	i;

	i = 0;
	while (s[i])
	{
		write(fd, &s[i], 1);
		i++;
	}
}

void	log_action(t_philosopher *philosopher, const char *action)
{
	t_shared	*shared;
	long long	timestamp;

	shared = philosopher->shared_data;
	timestamp = get_current_time_ms();
	pthread_mutex_lock(&shared->log_lock);
	if (!is_simulation_end(shared))
	{
		ft_putnbr_fd(timestamp - shared->start_time, 1);
		write(1, " ", 1);
		ft_putnbr_fd(philosopher->id, 1);
		write(1, " ", 1);
		ft_putstr_fd((char *)action, 1);
		write(1, "\n", 1);
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
