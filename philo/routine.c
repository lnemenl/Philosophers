/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 17:42:21 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/28 22:33:29 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"


void *philosopher_routine(void *arg)
{
    t_philosopher *philosopher = (t_philosopher *)arg;
    t_shared *shared = philosopher->shared_data;

    while (!shared->simulation_end) {
        // Thinking
        log_action(philosopher, "is thinking");
        if (shared->simulation_end) // Check if simulation ended
            break;

        // Take forks
        if (!take_forks(philosopher))
            break;

        // Eating
        if (!eat(philosopher))
            break;

        // Put forks down
        put_forks(philosopher);

        // Sleeping
        log_action(philosopher, "is sleeping");
        smart_sleep(shared->time_to_sleep, shared);
    }
    return (NULL);
}
