/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 23:49:30 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/26 19:59:26 by rkhakimu         ###   ########.fr       */
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
    int					num_meals_required;
    int					is_simulation_running;
    pthread_mutex_t		*forks_mutex;
    pthread_mutex_t		write_mutex;
}						t_shared;

/* Philosopher Data */
typedef struct s_philosopher
{
    int					id;
    long				last_meal_time;
    int					meals_eaten;
    pthread_mutex_t     meal_mutex;
    t_shared			*shared;
}						t_philosopher;

/* Parsing */

int						parse_input(int argc, char **argv, t_shared *shared);
int                     validate_argument(const char *arg, int *value);

/* Initializing */

t_philosopher           *setup_simulation(t_shared *shared);
t_philosopher           *initialize_philosophers(t_shared *shared);
pthread_mutex_t         *initialize_fork_mutexes(int num_philosophers);
int                     initialize_shared_resources(t_shared *shared);
int                     initialize_write_mutex(pthread_mutex_t *write_mutex);


/* Routine */

void					think(t_philosopher *philo);
int                     take_forks(t_philosopher *philo);
void					eat(t_philosopher *philo);
void					release_forks(t_philosopher *philo);
void					sleep_and_rest(t_philosopher *philo);
void                    single_philosopher_routine(t_philosopher *philo);
void                    *general_philosopher_routine(t_philosopher *philo);
void					*philosopher_routine(void *arg);

/* Monitoring */

void					*monitor_health(void *arg);
int                     check_all_meals(t_philosopher *philosophers, t_shared *shared);
void                    check_starvation(t_philosopher *philosophers, t_shared *shared);

/* Thread Management */

int						manage_threads(t_philosopher *philosophers, t_shared *shared);
int						create_philosopher_threads(t_philosopher *philosophers, pthread_t *threads, int num_philosophers);
int						create_monitoring_thread(pthread_t *monitor_thread, t_philosopher *philosophers);
void					join_philosopher_threads(pthread_t *threads, int num_philosophers);
void					join_monitoring_thread(pthread_t monitor_thread);

/* Cleanup */

void					cleanup_simulation(t_shared *shared, t_philosopher *philosophers);
void					destroy_fork_mutexes(pthread_mutex_t *forks_mutex, int num_philosophers);
void					destroy_write_mutex(pthread_mutex_t *write_mutex);
void					cleanup_shared_resources(t_shared *shared);

/* Utilities */

int						ft_atoi(const char *str);
long					get_current_time(void);
void					log_action(t_philosopher *philo, const char *action);

#endif
