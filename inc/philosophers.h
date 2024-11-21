/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 23:49:30 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/21 16:31:19 by rkhakimu         ###   ########.fr       */
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
    pthread_mutex_t		*forks;
    pthread_mutex_t		write_lock;
    pthread_mutex_t		monitor_lock;
}						t_shared;

/* Individual Philosopher Data */
typedef struct s_philosopher
{
    int					id;
    long				last_meal_time;
    pthread_mutex_t		*left_fork;
    pthread_mutex_t		*right_fork;
    t_shared			*shared;
}						t_philosopher;

/* Parsing and Initialization */
int				parse_input(int argc, char **argv, t_shared *shared);
int				initialize_simulation(int argc, char **argv, t_shared *shared);
t_philosopher	*init_philosophers(t_shared *shared);

/* Simulation Actions */
void			*philosopher_routine(void *arg);
void			take_forks(t_philosopher *philo);
void			release_forks(t_philosopher *philo);
void			eat(t_philosopher *philo);
void			sleep_and_think(t_philosopher *philo);

/* Monitoring */
void			*monitor_health(void *arg);

/* Thread Management */
int				start_threads(t_shared *shared, t_philosopher *philosophers, pthread_t **threads);
void			join_threads(t_shared *shared, pthread_t *threads);

/* Utilities */
long			get_current_time(void);
int				ft_atoi(const char *str);
void			log_action(t_philosopher *philo, const char *action);
void			wait_or_exit(t_philosopher *philo, int milliseconds);
void			stop_simulation(t_shared *shared);

/* Cleanup */
void			cleanup_simulation(t_shared *shared, t_philosopher *philosophers);

#endif
