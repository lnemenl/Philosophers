/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 23:49:15 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/10 23:56:21 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philosophers.h>

void *philosopher_routine(void *arg) 
{
    t_philosopher *philosopher = (t_philosopher *)arg;
    t_control *control = philosopher->control;
    while (1) 
    {
        if (check_turn(philosopher->id, control->turn_counter, control->turn_mutex)) 
        {
            pick_up_forks(philosopher);
            eat(philosopher);
            put_down_forks(philosopher);
            update_turn(control->turn_counter, control->turn_mutex, control->number_of_philosophers);
            usleep(1000 * 500); //To be adjusted
        }
        usleep(100);//To be adjusted
    }
    return NULL;
}
