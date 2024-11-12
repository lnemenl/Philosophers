/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 23:49:30 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/12 12:07:37 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
#define PHILOSOPHERS_H

#include <pthread.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct s_control
{
	int				number_of_philosophers;
	int				active_group;
	pthread_mutex_t	group_mutex;
	pthread_mutex_t	*forks;
}					t_control;

typedef struct s_philosopher
{
	int				id;
	long			last_meal_time;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_control		*control;
}					t_philosopher;

typedef struct s_simulation
{
	t_control		*control;
	t_philosopher	*philosopher;
}					t_simulation;


#endif
