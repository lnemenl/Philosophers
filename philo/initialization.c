/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 23:48:56 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/10 23:49:02 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philosophers.h>

void    initialize_forks(pthread_mutex_t *forks, int number_of_philosophers)
{
    int i;

    i = 0;
    while (i < number_of_philosophers)
    {
        pthread_mutex_init(&forks[i], NULL);
        i++;
    }
}

void    initialize_philosophers(t_philosopher *philosophers, pthread_mutex_t *forks, int number_of_philosophers, t_control *control)
{
    int i;

    i = 0;
    while (i < number_of_philosophers)
    {
        philosophers[i].id = i + 1;
        philosophers[i].left_fork = &forks[i];
        philosophers[i].right_fork = &forks[(i + 1) % number_of_philosophers];
        philosophers[i].last_meal_time = 0;
        philosophers[i].control = control;
        i++;
    }
}

