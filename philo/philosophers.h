/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 23:49:30 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/12/05 10:25:20 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

#define MEMORY_ALLOCATED    1 // 0001
#define MUTEXES_INITIALIZED 2 // 0010
#define THREADS_INITIALIZED 4 // 0100

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
	pthread_mutex_t     meal_lock;
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
    pthread_mutex_t     simulation_end_lock;
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

// cleaning

void clean_up_simulation(t_thread_data *data, t_shared *shared, int cleanup_flags);

//initialization functions
void initialize_philosopher(t_philosopher *philosopher, int id, t_shared *shared);
int init_forks(t_shared *shared, int *cleanup_flags);
int allocate_thread_data(t_thread_data *data, t_shared *shared, int *cleanup_flags);
int initialize_simulation(t_shared *shared, int argc, char **argv, int *cleanup_flags);

//monitoring functions
int check_philosopher_death(t_philosopher *philosopher);
int check_all_meals(t_shared *shared, t_philosopher *philosophers);
int check_termination_conditions(t_thread_data *data);
void *monitor_routine(void *arg);

//parsing functions
int is_numeric(const char *str);
int parse_and_validate(int argc, char **argv, t_shared *shared);

//routine functions
int take_forks(t_philosopher *philosopher);
void put_forks(t_philosopher *philosopher);
int eat(t_philosopher *philosopher);
void *philosopher_routine(void *arg);

//thread management functions
int launch_threads(t_thread_data *data, int *cleanup_flags);

//utilities functions
long long get_current_time_ms(void);
void log_action(t_philosopher *philosopher, const char *action);
void smart_sleep(int duration, t_shared *shared);
int	safe_atoi(const char *str, int *result);
int	is_simulation_end(t_shared *shared);
void	set_simulation_end(t_shared *shared, int value);

#endif