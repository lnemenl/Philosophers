/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:37:31 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/28 02:26:13 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

#include "philosophers.h"

void clean_mutexes(t_table *table)
{
    int i;

    if (!table || !table->forks)
        return;
    i = 0;
    while (i < table->num_philosophers)
    {
        pthread_mutex_destroy(&table->forks[i]);
        i++;
    }
    pthread_mutex_destroy(&table->waiter);
    pthread_mutex_destroy(&table->write_lock);
}

void clean_table(t_table *table)
{
    if (!table)
        return;
    clean_mutexes(table);
    if (table->philosophers)
        free(table->philosophers);
    if (table->forks)
        free(table->forks);
}

int cleanup_mutexes(t_table *table, int count)
{
    if (!table || !table->forks)
        return (0);
    while (--count >= 0)
        pthread_mutex_destroy(&table->forks[count]);
    pthread_mutex_destroy(&table->waiter);
    pthread_mutex_destroy(&table->write_lock);
    return (0);
}
