/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 23:49:30 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/20 16:23:40 by rkhakimu         ###   ########.fr       */
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
	int                 num_philosophers;
	int                 time_to_die;
	int                 time_to_eat;
	int                 time_to_sleep;
	int                 meals_required;     // Optional: number_of_times_each_philosopher_must_eat
	int                 is_simulation_running;
	pthread_mutex_t     *forks;             // Array of mutexes for forks
	pthread_mutex_t     write_lock;         // Mutex for logging
	pthread_mutex_t     monitor_lock;       // Mutex to protect simulation state
}               		t_shared;

/* Individual Philosopher Data */
typedef struct s_philosopher
{
	int                 id;
	int                 meals_eaten;
	long                last_meal_time;
	pthread_mutex_t     *left_fork;
	pthread_mutex_t     *right_fork;
	t_shared            *shared;
}               		t_philosopher;

/* Parsing and initialization */
int				parse_input(int argc, char **argv, t_shared *shared);
int				init_shared_resources(t_shared *shared);
t_philosopher	*init_philosophers(t_shared *shared);
int				handle_one_philosopher(t_shared *shared);
int				initialize_simulation(int argc, char **argv, t_shared *shared);

/* Philosopher Routine */
void			*philosopher_routine(void *arg);
void			take_forks(t_philosopher *philo);
void			release_forks(t_philosopher *philo);
void			eat(t_philosopher *philo);
void			sleep_and_think(t_philosopher *philo);

/* Monitoring */
void			*monitor_health(void *arg);
int				check_starvation(t_philosopher *philosophers, t_shared *shared);
int				check_meal_requirement(t_philosopher *philosophers, t_shared *shared);

/* Utility Functions */
long			get_current_time(void);
int				get_sign(const char **str);
const char		*skip_whitespace(const char *str);
void			log_action(t_philosopher *philo, const char *action);

/* Thread Management */
int				create_threads(t_shared *shared, t_philosopher *philosophers, pthread_t *threads);
void			join_threads(t_shared *shared, pthread_t *threads);
int				start_threads(t_shared *shared, t_philosopher *philosophers, pthread_t **threads);

/* Cleanup */
void    		cleanup_simulation(t_shared *shared, t_philosopher *philosophers);
void    		destroy_forks(t_shared *shared);
void    		destroy_shared_mutexes(t_shared *shared);
void    		free_resources(t_shared *shared, t_philosopher *philosophers);

#endif


