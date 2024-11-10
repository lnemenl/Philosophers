/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pick_up_or_down_fork.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 23:49:07 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/10 23:49:09 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philosophers.h>

void    pick_up_forks(t_philosopher *philosopher)
{
    pthread_mutex_lock(philosopher->left_fork);
    printf("%d has taken a fork\n", philosopher->id);
    pthread_mutex_lock(philosopher->right_fork);
    printf("%d has taken a fork\n", philosopher->id);

}

void    put_down_forks(t_philosopher *philosopher)
{
    pthread_mutex_unlock(philosopher->right_fork);
    pthread_mutex_unlock(philosopher->left_fork);
}

void    eat(t_philosopher *philosopher)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    philosopher->last_meal_time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    printf("%d is eating\n", philosopher->id);
    usleep(1000 * 500);//500 to be adjusted later
}
