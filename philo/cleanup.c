/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:37:31 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/29 00:23:10 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void join_threads(t_thread_data *data)
{
    int i = 0;

    while (i < data->shared->num_philosophers)
    {
        pthread_join(data->threads[i], NULL);
        i++;
    }
    pthread_join(data->monitor_thread, NULL);
}


