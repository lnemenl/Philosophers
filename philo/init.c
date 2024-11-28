/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 16:36:47 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/28 02:40:02 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int allocate_resources(t_table *table)
{
    table->philosophers = NULL;
    table->forks = NULL;

    table->philosophers = malloc(sizeof(t_philosopher) * table->num_philosophers);
    if (!table->philosophers)
        return (0);
    table->forks = malloc(sizeof(pthread_mutex_t) * table->num_philosophers);
    if (!table->forks)
    {
        free(table->philosophers);
        table->philosophers = NULL;
        return (0);
    }
    return (1);
}

static int init_basic_locks(t_table *table)
{
    if (pthread_mutex_init(&table->write_lock, NULL) != 0)
        return (0);
    if (pthread_mutex_init(&table->waiter, NULL) != 0)
    {
        pthread_mutex_destroy(&table->write_lock);
        return (0);
    }
    return (1);
}

void init_philosophers(t_table *table)
{
    int i;

    if (!table || table->num_philosophers <= 0)
        return;
    i = 0;
    while (i < table->num_philosophers)
    {
        table->philosophers[i].id = i + 1;
        table->philosophers[i].last_meal_time = table->start_time;
        table->philosophers[i].meals_eaten = 0;
        table->philosophers[i].left_fork_id = i;
        table->philosophers[i].right_fork_id = (i + 1) % table->num_philosophers;
        table->philosophers[i].table = table;
        i++;
    }
}

int init_mutexes(t_table *table)
{
    int i;

    i = 0;
    while (i < table->num_philosophers)
    {
        if (pthread_mutex_init(&table->forks[i], NULL) != 0)
        {
            cleanup_mutexes(table, i);
            return (0);
        }
        i++;
    }
    return (1);
}

int init_table(t_table *table, int argc, char **argv)
{
    if (!parse_arguments(table, argc, argv))
        return (0);
    if (!allocate_resources(table))
        return (0);
    if (!init_basic_locks(table))
    {
        free(table->philosophers);
        free(table->forks);
        return (0);
    }
    if (!init_mutexes(table))
    {
        pthread_mutex_destroy(&table->write_lock);
        pthread_mutex_destroy(&table->waiter);
        free(table->philosophers);
        free(table->forks);
        return (0);
    }
    table->simulation_end = 0;
    table->start_time = get_current_time();
    init_philosophers(table);
    return (1);
}
