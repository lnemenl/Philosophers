/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 23:49:30 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/23 21:29:29 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <sys/time.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>

/* Shared Data */
typedef struct s_shared
{
	int					num_philosophers;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					is_simulation_running;
	int                 *forks;
	pthread_mutex_t		forks_mutex;
	pthread_mutex_t		write_mutex;
}						t_shared;

/* Individual Philosopher Data */
typedef struct s_philosopher
{
	int					id;
	long				last_meal_time;
	t_shared			*shared;
}						t_philosopher;

/* Parsing and Initialization */
int				parse_input(int argc, char **argv, t_shared *shared);
int             initialize_forks(t_shared *shared, int i);
int             initialize_shared_resources(t_shared *shared);
int             setup_philosopher(t_philosopher *philosophers, t_shared *shared, int i);
int             initialize_simulation(int argc, char **argv, t_shared *shared);
t_philosopher   *init_philosophers(t_shared *shared);

/* Simulation Actions */
void            take_forks(t_philosopher *philo);
void            release_forks(t_philosopher *philo);
void            eat(t_philosopher *philo);
void            sleep_and_think(t_philosopher *philo);
void            *philosopher_routine(void *arg);

/* Monitoring */
void			*monitor_health(void *arg);
int             has_starvation_occurred(t_philosopher *philosophers, int num_philosophers, int time_to_die);

/* Thread Management */
int				start_threads(t_shared *shared, t_philosopher *philosophers, pthread_t **threads);
void			join_threads(t_shared *shared, pthread_t *threads);

/* Utilities */
int				ft_atoi(const char *str);
long			get_current_time(void);
void			log_action(t_philosopher *philo, const char *action);
void			wait_or_exit(t_philosopher *philo, int milliseconds);

/* Cleanup */
void			destroy_shared_mutexes(t_shared *shared);
void			cleanup_simulation(t_shared *shared, t_philosopher *philosophers);
void            free_resources(t_shared *shared, t_philosopher *philosophers);

#endif

//need some adjustments to address possible data race according to valgrind --tool=helgrind
