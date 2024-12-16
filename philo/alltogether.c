/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alltogether.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:37:31 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/12/16 15:00:44 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	join_threads(t_thread_data *data, t_shared *shared)
{
	int	i;

	i = 0;
	while (i < shared->num_philosophers)
	{
		pthread_join(data->threads[i], NULL);
		i++;
	}
	pthread_join(data->monitor_thread, NULL);
}

static void	destroy_mutexes(t_thread_data *data, t_shared *shared)
{
	int	i;

	i = 0;
	while (i < shared->num_philosophers)
	{
		pthread_mutex_destroy(&shared->forks[i]);
		if (data && data->philosophers)
			pthread_mutex_destroy(&data->philosophers[i].meal_lock);
		i++;
	}
	pthread_mutex_destroy(&shared->log_lock);
	pthread_mutex_destroy(&shared->simulation_end_lock);
}

static void	free_memory(t_thread_data *data, t_shared *shared)
{
	if (data)
	{
		if (data->threads)
			free(data->threads);
		if (data->philosophers)
			free(data->philosophers);
	}
	if (shared && shared->forks)
	{
		free(shared->forks);
		shared->forks = NULL;
	}
}

void	clean_up_simulation(t_thread_data *data,
							t_shared *shared, int cleanup_flags)
{
	if ((cleanup_flags & THREADS_INITIALIZED) && data && data->threads)
		join_threads(data, shared);
	if ((cleanup_flags & MUTEXES_INITIALIZED) && shared && shared->forks)
		destroy_mutexes(data, shared);
	if (cleanup_flags & MEMORY_ALLOCATED)
		free_memory(data, shared);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 16:36:47 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/12/14 23:26:46 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	initialize_philosopher(t_philosopher *philosopher,
								int id, t_shared *shared)
{
	philosopher->id = id;
	philosopher->meals_eaten = 0;
	philosopher->last_meal_time = get_current_time_ms();
	philosopher->left_fork = &shared->forks[id - 1];
	philosopher->right_fork = &shared->forks[id % shared->num_philosophers];
	philosopher->shared_data = shared;
	if (pthread_mutex_init(&philosopher->meal_lock, NULL) != 0)
		set_simulation_end(shared, 1);
}

int	init_forks(t_shared *shared, int *cleanup_flags)
{
	int	i;

	i = 0;
	while (i < shared->num_philosophers)
	{
		if (pthread_mutex_init(&shared->forks[i], NULL) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&shared->forks[i]);
			*cleanup_flags |= MUTEXES_INITIALIZED;
			clean_up_simulation(NULL, shared, *cleanup_flags);
			return (0);
		}
		i++;
	}
	*cleanup_flags |= MUTEXES_INITIALIZED;
	return (1);
}

static int	initialize_mutexes(t_shared *shared, int *cleanup_flags)
{
	if (pthread_mutex_init(&shared->simulation_end_lock, NULL) != 0)
		return (0);
	shared->forks = malloc(sizeof(pthread_mutex_t) * shared->num_philosophers);
	if (!shared->forks)
	{
		pthread_mutex_destroy(&shared->simulation_end_lock);
		return (0);
	}
	if (pthread_mutex_init(&shared->log_lock, NULL) != 0)
	{
		free(shared->forks);
		pthread_mutex_destroy(&shared->simulation_end_lock);
		return (0);
	}
	*cleanup_flags |= MEMORY_ALLOCATED;
	return (1);
}

static int	handle_single_philosopher(t_shared *shared, int *cleanup_flags)
{
	t_philosopher	single_philo;

	single_philo.id = 1;
	single_philo.shared_data = shared;
	log_action(&single_philo, "has taken a fork");
	usleep(shared->time_to_die * 1000);
	log_action(&single_philo, "died");
	clean_up_simulation(NULL, shared, *cleanup_flags);
	return (0);
}

int	initialize_simulation(t_shared *shared,
							int argc, char **argv, int *cleanup_flags)
{
	if (!parse_and_validate(argc, argv, shared))
	{
		log_error("Error: Invalid arguments.");
		return (0);
	}
	shared->simulation_end = 0;
	if (!initialize_mutexes(shared, cleanup_flags))
		return (0);
	if (shared->num_philosophers == 1)
		return (handle_single_philosopher(shared, cleanup_flags));
	return (1);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 23:22:55 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/12/14 23:24:19 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	ft_putnbr_fd(long long n, int fd)
{
	char	c;

	if (n >= 10)
		ft_putnbr_fd(n / 10, fd);
	c = (n % 10) + '0';
	write(fd, &c, 1);
}

static void	ft_putstr_fd(char *s, int fd)
{
	int	i;

	i = 0;
	while (s[i])
	{
		write(fd, &s[i], 1);
		i++;
	}
}

void	log_action(t_philosopher *philosopher, const char *action)
{
	t_shared	*shared;
	long long	timestamp;

	shared = philosopher->shared_data;
	timestamp = get_current_time_ms();
	pthread_mutex_lock(&shared->log_lock);
	if (!is_simulation_end(shared))
	{
		ft_putnbr_fd(timestamp, 1);
		write(1, " ", 1);
		ft_putnbr_fd(philosopher->id, 1);
		write(1, " ", 1);
		ft_putstr_fd((char *)action, 1);
		write(1, "\n", 1);
	}
	pthread_mutex_unlock(&shared->log_lock);
}

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}
void	log_error(const char *message)
{
	int	len;

	len = ft_strlen(message);
	write(STDERR_FILENO, message, len);
	write(STDERR_FILENO, "\n", 1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 15:08:49 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/12/14 23:30:26 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	initialize_simulation_data(t_shared *shared,
										int argc, char **argv, int *flags)
{
	if (!initialize_simulation(shared, argc, argv, flags))
	{
		clean_up_simulation(NULL, shared, *flags);
		return (0);
	}
	return (1);
}

static int	allocate_and_init_resources(t_shared *shared,
										t_thread_data *data, int *flags)
{
	if (!allocate_thread_data(data, shared, flags))
	{
		log_error("Error: Failed to allocate thread data.");
		clean_up_simulation(NULL, shared, *flags);
		return (0);
	}
	if (!init_forks(shared, flags))
	{
		clean_up_simulation(data, shared, *flags);
		return (0);
	}
	return (1);
}

static int	launch_simulation_threads(t_shared *shared,
										t_thread_data *data, int *flags)
{
	if (!launch_threads(data, flags))
	{
		log_error("Error: Failed to launch threads.");
		clean_up_simulation(data, shared, *flags);
		return (0);
	}
	return (1);
}

int	main(int argc, char **argv)
{
	t_shared		shared;
	t_thread_data	data;
	int				cleanup_flags;

	cleanup_flags = 0;
	if (!initialize_simulation_data(&shared, argc, argv, &cleanup_flags))
		return (1);
	if (!allocate_and_init_resources(&shared, &data, &cleanup_flags))
		return (1);
	if (!launch_simulation_threads(&shared, &data, &cleanup_flags))
		return (1);
	clean_up_simulation(&data, &shared, cleanup_flags);
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
/*   Updated: 2024/12/14 23:36:23 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int check_philosopher_death(t_philosopher *philosopher)
{
    long long current_time;
    long long last_meal;
    t_shared *shared;

    shared = philosopher->shared_data;
    current_time = get_current_time_ms();
    pthread_mutex_lock(&philosopher->meal_lock);
    last_meal = philosopher->last_meal_time;
    pthread_mutex_unlock(&philosopher->meal_lock);
    if (current_time - last_meal >= shared->time_to_die)
    {
        pthread_mutex_lock(&shared->log_lock);
        if (!is_simulation_end(shared))
        {
            set_simulation_end(shared, 1);
            printf("%lld %d died\n", current_time, philosopher->id);
        }
        pthread_mutex_unlock(&shared->log_lock);
        return (1);
    }
    return (0);
}

int	check_all_meals(t_shared *shared, t_philosopher *philosophers)
{
	int	i;

	if (shared->meals_required == -1)
		return (0);
	i = 0;
	while (i < shared->num_philosophers)
	{
		pthread_mutex_lock(&philosophers[i].meal_lock);
		if (philosophers[i].meals_eaten < shared->meals_required)
		{
			pthread_mutex_unlock(&philosophers[i].meal_lock);
			return (0);
		}
		pthread_mutex_unlock(&philosophers[i].meal_lock);
		i++;
	}
	return (1);
}

int	check_termination_conditions(t_thread_data *data)
{
	int			i;
	t_shared	*shared;

	shared = data->shared;
	i = 0;
	while (i < shared->num_philosophers)
	{
		if (check_philosopher_death(&data->philosophers[i]))
		{
			pthread_mutex_lock(&shared->log_lock);
			set_simulation_end(shared, 1);
			pthread_mutex_unlock(&shared->log_lock);
			return (1);
		}
		i++;
	}
	if (check_all_meals(shared, data->philosophers))
	{
		pthread_mutex_lock(&shared->log_lock);
		set_simulation_end(shared, 1);
		pthread_mutex_unlock(&shared->log_lock);
		return (1);
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_thread_data	*data;
	t_shared		*shared;

	data = (t_thread_data *)arg;
	shared = data->shared;
	while (!is_simulation_end(shared))
	{
		if (check_termination_conditions(data))
			break;
		usleep(50);
	}
	return (NULL);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 15:09:17 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/12/13 12:06:12 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (!str || str[0] == '\0')
		return (0);
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static int	validate_argument(const char *arg, int *result)
{
	if (!is_numeric(arg) || !safe_atoi(arg, result) || *result <= 0)
		return (0);
	return (1);
}

static int	validate_optional_argument(const char *arg, t_shared *shared)
{
	int	result;

	if (!is_numeric(arg) || !safe_atoi(arg, &result) || result < 0)
		return (0);
	shared->meals_required = result;
	if (shared->meals_required == 0)
		return (0);
	return (1);
}

int	parse_and_validate(int argc, char **argv, t_shared *shared)
{
	int	result;

	if (argc != 5 && argc != 6)
		return (0);
	if (!validate_argument(argv[1], &result))
		return (0);
	shared->num_philosophers = result;
	if (!validate_argument(argv[2], &result))
		return (0);
	shared->time_to_die = result;
	if (!validate_argument(argv[3], &result))
		return (0);
	shared->time_to_eat = result;
	if (!validate_argument(argv[4], &result))
		return (0);
	shared->time_to_sleep = result;
	if (argc == 6)
	{
		if (!validate_optional_argument(argv[5], shared))
			return (0);
	}
	else
		shared->meals_required = -1;
	return (1);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 23:49:30 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/12/14 23:25:53 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# define MEMORY_ALLOCATED    1 // 0001
# define MUTEXES_INITIALIZED 2 // 0010
# define THREADS_INITIALIZED 4 // 0100

# include <pthread.h>
# include <sys/time.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <limits.h>

typedef struct s_philosopher
{
	int					id;
	int					meals_eaten;
	long long			last_meal_time;
	pthread_mutex_t		meal_lock;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	struct s_shared		*shared_data;
}						t_philosopher;

typedef struct s_shared
{
	int					num_philosophers;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					meals_required;
	int					simulation_end;
	pthread_mutex_t		simulation_end_lock;
	pthread_mutex_t		*forks;
	pthread_mutex_t		log_lock;
}						t_shared;

typedef struct s_thread_data
{
	t_shared			*shared;
	pthread_t			*threads;
	pthread_t			monitor_thread;
	t_philosopher		*philosophers;
}						t_thread_data;

// cleaning
void		clean_up_simulation(t_thread_data *data,
				t_shared *shared, int cleanup_flags);

//initialization functions
void		initialize_philosopher(t_philosopher *philosopher,
				int id, t_shared *shared);
int			init_forks(t_shared *shared, int *cleanup_flags);
int			allocate_thread_data(t_thread_data *data,
				t_shared *shared, int *cleanup_flags);
int			initialize_simulation(t_shared *shared,
				int argc, char **argv, int *cleanup_flags);

//monitoring functions
int			check_philosopher_death(t_philosopher *philosopher);
int			check_all_meals(t_shared *shared, t_philosopher *philosophers);
int			check_termination_conditions(t_thread_data *data);
void		*monitor_routine(void *arg);

//parsing functions
int			is_numeric(const char *str);
int			parse_and_validate(int argc, char **argv, t_shared *shared);

//routine functions
int			take_forks(t_philosopher *philosopher);
void		put_forks(t_philosopher *philosopher);
int			eat(t_philosopher *philosopher);
void		*philosopher_routine(void *arg);

//thread management functions
int			launch_threads(t_thread_data *data, int *cleanup_flags);

//utilities functions
long long	get_current_time_ms(void);
void		smart_sleep(int duration, t_shared *shared);
int			safe_atoi(const char *str, int *result);
int			is_simulation_end(t_shared *shared);
void		set_simulation_end(t_shared *shared, int value);
void		update_last_meal_time(t_philosopher *philosopher);

//logs functions
void		log_action(t_philosopher *philosopher, const char *action);
void		log_error(const char *message);

#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 17:42:21 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/12/15 00:12:26 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	get_forks_order(t_philosopher *philosopher,
							pthread_mutex_t **first_fork,
							pthread_mutex_t **second_fork)
{
	t_shared	*shared;

	shared = philosopher->shared_data;
	if (philosopher->id % shared->num_philosophers == 0)
	{
		*first_fork = philosopher->right_fork;
		*second_fork = philosopher->left_fork;
	}
	else
	{
		*first_fork = philosopher->left_fork;
		*second_fork = philosopher->right_fork;
	}
}

int take_forks(t_philosopher *philosopher)
{
    t_shared        *shared;
    pthread_mutex_t *first_fork;
    pthread_mutex_t *second_fork;

    shared = philosopher->shared_data;
    get_forks_order(philosopher, &first_fork, &second_fork);
    pthread_mutex_lock(first_fork);
    log_action(philosopher, "has taken a fork");
    if (is_simulation_end(shared))
    {
        pthread_mutex_unlock(first_fork);
        return (0);
    }
    update_last_meal_time(philosopher);
    pthread_mutex_lock(second_fork);
    log_action(philosopher, "has taken a fork");
    if (is_simulation_end(shared))
    {
        pthread_mutex_unlock(second_fork);
        pthread_mutex_unlock(first_fork);
        return (0);
    }
    return (1);
}

void	put_forks(t_philosopher *philosopher)
{
	pthread_mutex_unlock(philosopher->right_fork);
	pthread_mutex_unlock(philosopher->left_fork);
}

int	eat(t_philosopher *philosopher)
{
	t_shared	*shared;

	shared = philosopher->shared_data;
	if (is_simulation_end(shared))
		return (0);
	log_action(philosopher, "is eating");
	pthread_mutex_lock(&philosopher->meal_lock);
	philosopher->last_meal_time = get_current_time_ms();
	philosopher->meals_eaten++;
	pthread_mutex_unlock(&philosopher->meal_lock);
	smart_sleep(shared->time_to_eat, shared);
	if (is_simulation_end(shared))
		return (0);
	return (1);
}

void	*philosopher_routine(void *arg)
{
	t_philosopher	*philosopher;
	t_shared		*shared;

	philosopher = (t_philosopher *)arg;
	shared = philosopher->shared_data;
	if (philosopher->id % 2 == 0)
        usleep(1000);
	while (!is_simulation_end(shared))
	{
		log_action(philosopher, "is thinking");
		if (is_simulation_end(shared) || !take_forks(philosopher))
			break ;
		if (is_simulation_end(shared) || !eat(philosopher))
		{
			put_forks(philosopher);
			break ;
		}
		put_forks(philosopher);
		if (!is_simulation_end(shared))
			log_action(philosopher, "is sleeping");
		smart_sleep(shared->time_to_sleep, shared);
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
/*   Updated: 2024/12/14 23:30:39 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	allocate_thread_data(t_thread_data *data,
							t_shared *shared, int *cleanup_flags)
{
	int	i;

	data->threads = malloc(sizeof(pthread_t) * shared->num_philosophers);
	if (!data->threads)
		return (0);
	data->philosophers = malloc(sizeof(t_philosopher)
			* shared->num_philosophers);
	if (!data->philosophers)
	{
		free(data->threads);
		return (0);
	}
	*cleanup_flags |= MEMORY_ALLOCATED;
	i = 0;
	while (i < shared->num_philosophers)
	{
		initialize_philosopher(&data->philosophers[i], i + 1, shared);
		i++;
	}
	data->shared = shared;
	return (1);
}

static int	create_philosopher_threads(t_thread_data *data, int *cleanup_flags)
{
	int	i;

	i = 0;
	while (i < data->shared->num_philosophers)
	{
		if (pthread_create(&data->threads[i], NULL, philosopher_routine,
				(void *)&data->philosophers[i]) != 0)
		{
			printf("Error: Failed to create philosopher thread %d.\n", i + 1);
			*cleanup_flags |= THREADS_INITIALIZED;
			while (i > 0)
			{
				i--;
				pthread_join(data->threads[i], NULL);
			}
			return (0);
		}
		i++;
	}
	*cleanup_flags |= THREADS_INITIALIZED;
	return (1);
}

static int	create_monitor_thread(t_thread_data *data)
{
	int	i;

	if (pthread_create(&data->monitor_thread,
			NULL, monitor_routine, (void *)data) != 0)
	{
		log_error("Error: Failed to create monitor thread.");
		i = 0;
		while (i < data->shared->num_philosophers)
		{
			pthread_join(data->threads[i], NULL);
			i++;
		}
		return (0);
	}
	return (1);
}

int	launch_threads(t_thread_data *data, int *cleanup_flags)
{
	if (!create_philosopher_threads(data, cleanup_flags))
	{
		clean_up_simulation(data, data->shared, *cleanup_flags);
		return (0);
	}
	if (!create_monitor_thread(data))
	{
		clean_up_simulation(data, data->shared, *cleanup_flags);
		return (0);
	}
	return (1);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_additional.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 12:29:22 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/12/14 23:24:23 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	is_simulation_end(t_shared *shared)
{
	int	result;

	pthread_mutex_lock(&shared->simulation_end_lock);
	result = shared->simulation_end;
	pthread_mutex_unlock(&shared->simulation_end_lock);
	return (result);
}

void	set_simulation_end(t_shared *shared, int value)
{
	pthread_mutex_lock(&shared->simulation_end_lock);
	shared->simulation_end = value;
	pthread_mutex_unlock(&shared->simulation_end_lock);
}

void	update_last_meal_time(t_philosopher *philosopher)
{
	pthread_mutex_lock(&philosopher->meal_lock);
	philosopher->last_meal_time = get_current_time_ms();
	pthread_mutex_unlock(&philosopher->meal_lock);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 17:54:42 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/12/15 00:11:21 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long long	get_current_time_ms(void)
{
	struct timeval	tv;
	static long long	start = -1;
	long long		current;

	if (gettimeofday(&tv, NULL) != 0)
	{
		log_error("Error: Failed to get current time");
		return (0);
	}
	current = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	if (start == -1)
		start = current;
	return (current - start);
}

void	smart_sleep(int duration, t_shared *shared)
{
	long long	start_time;

	start_time = get_current_time_ms();
	while (!is_simulation_end(shared)
		&& (get_current_time_ms() - start_time < duration))
		usleep(10);
}

int	safe_atoi(const char *str, int *result)
{
	long long	num;
	int			sign;
	int			i;

	num = 0;
	sign = 1;
	i = 0;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i])
	{
		num = num * 10 + (str[i] - '0');
		if ((sign == 1 && (num > INT_MAX)) || (sign == -1 && (-num < INT_MIN)))
			return (0);
		i++;
	}
	*result = (int)(num * sign);
	return (1);
}
