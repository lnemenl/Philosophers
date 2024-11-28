/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 23:49:30 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/28 03:11:18 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <sys/time.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>

/* Philosopher Data */

typedef struct s_philosopher
{
	int				id;
	long long		last_meal_time;
	int				meals_eaten;
	int				left_fork_id;
	int				right_fork_id;
	pthread_t		thread;
	struct s_table	*table;
}					t_philosopher;

/* Shared Data */

typedef struct s_table
{
	t_philosopher	*philosophers;
	pthread_mutex_t	*forks;
	pthread_mutex_t	waiter;
	pthread_mutex_t	write_lock;
	pthread_t		monitor_thread;
	long long		start_time;
	int				num_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat_count;
	int				simulation_end;
}					t_table;

/* Cleaning */
void				clean_mutexes(t_table *table);
void				clean_table(t_table *table);
int					cleanup_mutexes(t_table *table, int count);

/* Initialization */
int					init_table(t_table *table, int argc, char **argv);
void				init_philosophers(t_table *table);
int					init_mutexes(t_table *table);

/* Monitoring */
void				*monitor_routine(void *arg);
int					get_simulation_status(t_table *table);
int					monitor_status(t_table *table, int philo_index);

/* Parsing */
int					parse_arguments(t_table *table, int argc, char **argv);
int					validate_arguments(t_table *table);

/* Routine */
void				*philosopher_routine(void *arg);
int					think(t_philosopher *philo);
int					eat(t_philosopher *philo);
int					sleep_philosopher(t_philosopher *philo);

/* Threads */
int					create_philosophers(t_table *table);
int					create_monitor(t_table *table);
int					end_simulation(t_table *table);

/* Utilities */
long long			get_current_time(void);
void				print_status(t_philosopher *philo, const char *status);
void				smart_sleep(long long duration, t_table *table);

/* Mutex Safety: Provides error-checked mutex operations */
int					safe_mutex_lock(pthread_mutex_t *mutex);
int					safe_mutex_unlock(pthread_mutex_t *mutex);

/* Argument Parsing Utility */
int					ft_atoi(const char *str);

/* Single Philosopher Case */
void				*handle_single_philosopher(t_philosopher *philo);

#endif