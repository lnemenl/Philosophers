/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 23:49:30 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/15 23:10:24 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <sys/time.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>

typedef struct s_control
{
	int				number_of_philosophers;
	int				stop_simulation;
	pthread_mutex_t	control_mutex;
	pthread_mutex_t	*forks;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
}					t_control;

typedef struct s_philosopher
{
	int				id;
	long			last_meal_time;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_control		*control;
}					t_philosopher;

typedef struct s_simulation
{
	t_control		*control;
	t_philosopher	*philosophers;
}					t_simulation;

int         parse_arguments(int ac, char **av, t_control *control);
t_control   *initialize_control(int num_philosophers);
int         initialize_forks(pthread_mutex_t *forks, int num_philosophers);
int         setup_philosopher(t_philosopher *philosopher, int id, t_control *control);
int         initialize_philosophers(t_philosopher *philosophers, int num_philosophers, t_control *control);
void        *philosopher_routine(void *philosopher_data);
int         start_philosophers(t_simulation *simulation);
void        free_control(t_control *control);
void        free_simulation(t_simulation *simulation);
long        get_current_time_ms();
void        sleep_ms(long milliseconds);
int         ft_atoi(const char *str);
long        ft_atol(const char *str);
void		perform_eating(t_philosopher *philosopher);
void		*monitor_routine(void *data);
int			check_stop_simulation(t_simulation *simulation);
int			check_philosopher_status(t_simulation *simulation);
void		destroy_forks(pthread_mutex_t *forks, int num_initialized);
int			should_stop_simulation(t_philosopher *philosopher);
void		handle_eating(t_philosopher *philosopher);
void		handle_sleeping(t_philosopher *philosopher);
void		handle_thinking(t_philosopher *philosopher);
int			initialize_simulation(t_simulation *simulation);

#endif
