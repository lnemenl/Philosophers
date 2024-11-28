/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 23:49:30 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/28 22:09:48 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <sys/time.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <limits.h>

typedef struct s_philosopher
{
	int					id;							// Philosopher ID
	int					meals_eaten;				// Count of meals eaten
	long long			last_meal_time;				// Timestamp of the last meal
	pthread_mutex_t		*left_fork;					// Pointer to the left fork mutex
	pthread_mutex_t		*right_fork;				// Pointer to the right fork mutex
	struct s_shared		*shared_data;				// Pointer to shared data
}						t_philosopher;

typedef struct s_shared
{
    int					num_philosophers;
    int					time_to_die;
    int					time_to_eat;
    int					time_to_sleep;
    int					meals_required;
    int					simulation_end;
    pthread_mutex_t		*forks;
    pthread_mutex_t		log_lock;
}						t_shared;


typedef struct s_thread_data
{
    t_shared            *shared;
    pthread_t           *threads;
    pthread_t           monitor_thread;
    t_philosopher       *philosophers;
}                       t_thread_data;


#endif