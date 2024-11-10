/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   turns.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 23:49:22 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/10 23:49:25 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philosophers.h>

int check_turn(int philosopher_id, int *turn_counter, pthread_mutex_t *turn_mutex)
{
    int is_turn;
    pthread_mutex_lock(turn_mutex);
    is_turn = (*turn_counter == philosopher_id);
    pthread_mutex_unlock(turn_mutex);
    return (is_turn);
}

void    update_turn(int *turn_counter, pthread_mutex_t *turn_mutex, int number_of_philosophers)
{
    pthread_mutex_lock(turn_mutex);
    *turn_counter = (*turn_counter % number_of_philosophers) + 1;
    pthread_mutex_unlock(turn_mutex);
}