/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   at.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 23:49:30 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/27 18:11:22 by rkhakimu         ###   ########.fr       */
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
	pthread_mutex_t		*fork_mutexes;
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
int             initialize_shared_resources(t_shared *shared);
int             setup_philosopher(t_philosopher *philosophers, t_shared *shared, int i);
int             initialize_simulation(int argc, char **argv, t_shared *shared);
int				init_fork_mutex(t_shared *shared, int i);
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
void			destroy_fork_mutexes(t_shared *shared, int i);
#endif

//need some adjustments to address possible data race according to valgrind --tool=helgrind
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:37:31 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/27 17:12:17 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	destroy_fork_mutexes(t_shared *shared, int i)
{
    if (i >= shared->num_philosophers)
        return;
    
    pthread_mutex_destroy(&shared->fork_mutexes[i]);
    destroy_fork_mutexes(shared, i + 1);
}

void	destroy_shared_mutexes(t_shared *shared)
{
    destroy_fork_mutexes(shared, 0);
    pthread_mutex_destroy(&shared->write_mutex);
}

void	free_resources(t_shared *shared, t_philosopher *philosophers)
{
    // Remove forks array free
    if (shared->fork_mutexes)
        free(shared->fork_mutexes);
    if (philosophers)
        free(philosophers);
}

void	cleanup_simulation(t_shared *shared, t_philosopher *philosophers)
{
    destroy_shared_mutexes(shared);
    free_resources(shared, philosophers);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 13:54:57 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/21 15:19:33 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static const char	*skip_whitespace(const char *str)
{
    while ((*str >= 9 && *str <= 13) || *str == ' ')
        str++;
    return (str);
}

static int	get_sign(const char **str)
{
    int sign = 1;

    if (**str == '-' || **str == '+')
    {
        if (**str == '-')
            sign = -1;
        (*str)++;
    }
    return (sign);
}

static int	convert_to_int(const char *str, int sign)
{
    long long result = 0;
    long long max = ((unsigned long long)(-1)) / 2;

    while (*str >= '0' && *str <= '9')
    {
        if (result > max / 10 || (result == max / 10 && (*str - '0') > 7))
        {
            if (sign > 0)
                return (-1);
            else
                return (0);
        }
        result = result * 10 + (*str - '0');
        str++;
    }
    return ((int)(sign * result));
}

int	ft_atoi(const char *str)
{
    str = skip_whitespace(str);
    int sign = get_sign(&str);
    return (convert_to_int(str, sign));
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 15:08:49 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/23 20:21:07 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	main(int argc, char **argv)
{
    t_shared        shared;
    t_philosopher   *philosophers;
    pthread_t       *threads;

    if (initialize_simulation(argc, argv, &shared) != 0)
        return (1);
    philosophers = init_philosophers(&shared);
    if (!philosophers)
	{
        printf("Error: Failed to initialize philosophers\n");
        cleanup_simulation(&shared, NULL);
        return (1);
    }
    if (start_threads(&shared, philosophers, &threads) != 0)
	{
        cleanup_simulation(&shared, philosophers);
        return (1);
    }
    join_threads(&shared, threads);
    cleanup_simulation(&shared, philosophers);
    free(threads);
    return (0);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 17:07:05 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/27 17:12:15 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	has_starvation_occurred(t_philosopher *philosophers, int num_philosophers, int time_to_die)
{
    int		i;
    long	current_time;
    int     starved_philosopher;

    i = 0;
    starved_philosopher = 0;
    while (i < num_philosophers)
    {
        pthread_mutex_lock(&philosophers[i].shared->fork_mutexes[i]);
        current_time = get_current_time();
        if (current_time - philosophers[i].last_meal_time > time_to_die)
        {
            starved_philosopher = i + 1;
            pthread_mutex_unlock(&philosophers[i].shared->fork_mutexes[i]);
            break;
        }
        pthread_mutex_unlock(&philosophers[i].shared->fork_mutexes[i]);
        i++;
    }
    return (starved_philosopher);
}


void	*monitor_health(void *arg)
{
    t_philosopher	*philosophers;
    t_shared		*shared;
    int				starved_philosopher;

    philosophers = (t_philosopher *)arg;
    shared = philosophers[0].shared;

    while (shared->is_simulation_running)
    {
        starved_philosopher = has_starvation_occurred(philosophers, shared->num_philosophers, shared->time_to_die);
        if (starved_philosopher > 0)
        {
            pthread_mutex_lock(&shared->write_mutex);
            if (shared->is_simulation_running)
            {
                printf("%ld %d died\n", get_current_time(), starved_philosopher);
                shared->is_simulation_running = 0;
            }
            pthread_mutex_unlock(&shared->write_mutex);
            return (NULL);
        }
        usleep(1000);
    }
    return NULL;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_and_init.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 15:09:17 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/27 17:03:46 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	parse_input(int argc, char **argv, t_shared *shared)
{
	if (argc != 5)
	{
		printf("Error: Invalid number of arguments. Usage: ./philosophers num_philosophers time_to_die time_to_eat time_to_sleep\n");
		return (1);
	}
	shared->num_philosophers = ft_atoi(argv[1]);
	shared->time_to_die = ft_atoi(argv[2]);
	shared->time_to_eat = ft_atoi(argv[3]);
	shared->time_to_sleep = ft_atoi(argv[4]);
	if (shared->num_philosophers <= 0 || shared->time_to_die <= 0 || 
		shared->time_to_eat <= 0 || shared->time_to_sleep <= 0)
	{
		printf("Error: All timing values must be positive integers in ms.\n");
		return (1);
	}
	shared->is_simulation_running = 1;
	return (0);
}

int	init_fork_mutex(t_shared *shared, int i)
{
    if (i >= shared->num_philosophers)
        return (0);
    
    if (pthread_mutex_init(&shared->fork_mutexes[i], NULL) != 0)
    {
        while (i > 0)
        {
            i--;
            pthread_mutex_destroy(&shared->fork_mutexes[i]);
        }
        free(shared->fork_mutexes);
        return (1);
    }   
    return (init_fork_mutex(shared, i + 1));
}
int initialize_shared_resources(t_shared *shared)
{
    shared->fork_mutexes = malloc(sizeof(pthread_mutex_t) * shared->num_philosophers);
    if (!shared->fork_mutexes)
        return (1);
    if (init_fork_mutex(shared, 0) != 0)
        return (1);
    if (pthread_mutex_init(&shared->write_mutex, NULL) != 0)
    {
        destroy_fork_mutexes(shared, 0);
        free(shared->fork_mutexes);
        return (1);
    }
    return (0);
}

int	setup_philosopher(t_philosopher *philosophers, t_shared *shared, int i)
{
	if (i >= shared->num_philosophers)
		return (0);
	philosophers[i].id = i + 1;
	philosophers[i].last_meal_time = get_current_time();
	philosophers[i].shared = shared;
	return (setup_philosopher(philosophers, shared, i + 1));
}

t_philosopher	*init_philosophers(t_shared *shared)
{
	t_philosopher *philosophers = malloc(sizeof(t_philosopher) * shared->num_philosophers);
	if (!philosophers)
		return (NULL);
	if (setup_philosopher(philosophers, shared, 0) != 0)
	{
		free(philosophers);
		return (NULL);
	}
	return (philosophers);
}

int	initialize_simulation(int argc, char **argv, t_shared *shared)
{
	if (parse_input(argc, argv, shared) != 0)
		return (1);
	if (initialize_shared_resources(shared) != 0)
	{
		printf("Error: Failed to initialize shared resources.\n");
		return (1);
	}
	return (0);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 17:42:21 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/27 18:06:15 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	take_forks(t_philosopher *philo)
{
    int	left_fork;
    int	right_fork;

    left_fork = philo->id - 1;
    right_fork = philo->id % philo->shared->num_philosophers;
    pthread_mutex_lock(&philo->shared->fork_mutexes[left_fork]);
    pthread_mutex_lock(&philo->shared->fork_mutexes[right_fork]);
    log_action(philo, "has taken forks");
    pthread_mutex_unlock(&philo->shared->fork_mutexes[left_fork]);
    pthread_mutex_unlock(&philo->shared->fork_mutexes[right_fork]);
}

void	release_forks(t_philosopher *philo)
{
    int	left_fork;
    int	right_fork;

    left_fork = philo->id - 1;
    right_fork = philo->id % philo->shared->num_philosophers;
    pthread_mutex_lock(&philo->shared->fork_mutexes[left_fork]);
    pthread_mutex_lock(&philo->shared->fork_mutexes[right_fork]);
    pthread_mutex_unlock(&philo->shared->fork_mutexes[right_fork]);
    pthread_mutex_unlock(&philo->shared->fork_mutexes[left_fork]);
}

void	eat(t_philosopher *philo)
{
    pthread_mutex_lock(&philo->shared->fork_mutexes[philo->id - 1]);
    pthread_mutex_lock(&philo->shared->fork_mutexes[philo->id % philo->shared->num_philosophers]);
    
    // Update last meal time
    pthread_mutex_lock(&philo->shared->write_mutex);
    philo->last_meal_time = get_current_time();
    pthread_mutex_unlock(&philo->shared->write_mutex);

    log_action(philo, "is eating");
    usleep(philo->shared->time_to_eat * 1000);

    pthread_mutex_unlock(&philo->shared->fork_mutexes[philo->id % philo->shared->num_philosophers]);
    pthread_mutex_unlock(&philo->shared->fork_mutexes[philo->id - 1]);
}


void	sleep_and_think(t_philosopher *philo)
{
    log_action(philo, "is sleeping");
    usleep(philo->shared->time_to_sleep * 1000);
}


void	*philosopher_routine(void *arg)
{
	t_philosopher *philo;

    philo = (t_philosopher *)arg;
	while (philo->shared->is_simulation_running)
	{
		log_action(philo, "is thinking");
		usleep(1000);
		take_forks(philo);
		eat(philo);
		sleep_and_think(philo);
	}
	return (NULL);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_management.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 15:43:43 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/23 20:17:26 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	start_threads(t_shared *shared, t_philosopher *philosophers, pthread_t **threads)
{
	int i;

	i = 0;
	*threads = malloc(sizeof(pthread_t) * (shared->num_philosophers + 1));
	if (!(*threads))
		return (1);
	while (i < shared->num_philosophers)
	{
		if (pthread_create(&(*threads)[i], NULL, philosopher_routine, &philosophers[i]) != 0)
		{
			printf("Error: Failed to create thread for philosopher %d\n", i + 1);
			shared->is_simulation_running = 0;
			return (1);
		}
		i++;
	}
	if (pthread_create(&(*threads)[i], NULL, monitor_health, philosophers) != 0)
	{
		printf("Error: Failed to create monitoring thread\n");
		shared->is_simulation_running = 0;
		return (1);
	}
	return (0);
}

void	join_threads(t_shared *shared, pthread_t *threads)
{
    int i;

	i = 0;
    while (i < shared->num_philosophers + 1)
	{
        pthread_join(threads[i], NULL);
        i++;
    }
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 17:54:42 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/27 17:50:17 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long	get_current_time(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

void	log_action(t_philosopher *philo, const char *action)
{
	pthread_mutex_lock(&philo->shared->write_mutex);
	if (philo->shared->is_simulation_running)
		printf("%ld %d %s\n", get_current_time(), philo->id, action);
	pthread_mutex_unlock(&philo->shared->write_mutex);
}


void	wait_or_exit(t_philosopher *philo, int milliseconds)
{
	long	start_time;

	start_time = get_current_time();
	while (get_current_time() - start_time < milliseconds)
	{
		if (!philo->shared->is_simulation_running)
			break;
		usleep(100);
	}
}
