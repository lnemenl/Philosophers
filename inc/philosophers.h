/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 23:49:30 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/29 03:29:00 by rkhakimu         ###   ########.fr       */
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


//cleanup functions
void join_threads(t_thread_data *data);
void destroy_mutexes(t_shared *shared);
void free_resources(t_thread_data *data);
void clean_up(t_thread_data *data);
void destroy_forks(t_shared *shared);
void handle_initialization_failure(t_thread_data *data, int thread_created_count);
void cleanup_partial_thread_data(t_thread_data *data);

//initialization functions
int allocate_memory(t_thread_data *data, t_shared *shared);
void initialize_philosopher(t_philosopher *philosopher, int id, t_shared *shared);
int init_log_mutex(t_shared *shared);
int init_shared_data(t_shared *shared, char **argv);
int init_forks(t_shared *shared);
int allocate_thread_data(t_thread_data *data, t_shared *shared);
int initialize_simulation(t_shared *shared, int argc, char **argv);

//monitoring functions
int check_philosopher_death(t_philosopher *philosopher);
int check_all_meals(t_shared *shared, t_philosopher *philosophers);
int check_termination_conditions(t_thread_data *data);
void *monitor_routine(void *arg);

//parsing functions
int is_numeric(const char *str);
int validate_arguments(t_shared *shared);
int parse_arguments(int argc, char **argv, t_shared *shared);
int parse_shared_parameters(t_shared *shared, char **argv);

//routine functions
int take_forks(t_philosopher *philosopher);
void put_forks(t_philosopher *philosopher);
int eat(t_philosopher *philosopher);
void *philosopher_routine(void *arg);

//thread management functions
int launch_philosopher_threads(t_thread_data *data);
int launch_monitor_thread(t_thread_data *data);
int launch_threads(t_thread_data *data);

//utilities functions
long long get_current_time_ms(void);
void log_action(t_philosopher *philosopher, const char *action);
void smart_sleep(int duration, t_shared *shared);
int safe_mutex_lock(pthread_mutex_t *mutex);
int safe_mutex_unlock(pthread_mutex_t *mutex);
int	safe_atoi(const char *str, int *result);

#endif