/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_management.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 15:43:43 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/28 03:38:20 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int handle_thread_error(t_table *table, int i)
{
    printf("Error: Failed to create thread for philosopher %d\n", i);
    safe_mutex_lock(&table->write_lock);
    table->simulation_end = 1;
    safe_mutex_unlock(&table->write_lock);
    while (--i >= 0)
        pthread_join(table->philosophers[i].thread, NULL);
    clean_table(table);
    return (0);
}

int create_philosophers(t_table *table)
{
    int i;

    i = 0;
    while (i < table->num_philosophers)
    {
        if (pthread_create(&table->philosophers[i].thread, NULL,
                           philosopher_routine, &table->philosophers[i]) != 0)
        {
            printf("Error: Thread creation failed for philosopher %d\n", i + 1);
            return (handle_thread_error(table, i));
        }
        usleep(100);
        i++;
    }
    return (1);
}

int create_monitor(t_table *table)
{
    if (pthread_create(&table->monitor_thread, NULL, monitor_routine, table) != 0)
    {
        printf("Error: Monitor thread creation failed.\n");
        return (handle_thread_error(table, table->num_philosophers));
    }
    return (1);
}

// static void join_threads(t_table *table)
// {
//     int i;

//     i = 0;
//     while (i < table->num_philosophers)
//     {
//         pthread_join(table->philosophers[i].thread, NULL);
//         i++;
//     }
//     pthread_join(table->monitor_thread, NULL);
// }

int end_simulation(t_table *table)
{
    int i;

    printf("Starting cleanup\n");
    safe_mutex_lock(&table->write_lock);
    table->simulation_end = 1;
    safe_mutex_unlock(&table->write_lock);
    i = 0;
    while (i < table->num_philosophers)
    {
        pthread_join(table->philosophers[i].thread, NULL);
        printf("Philosopher %d thread joined\n", i + 1);
        i++;
    }
    pthread_join(table->monitor_thread, NULL);
    printf("Monitor thread joined\n");

    clean_table(table);
    printf("Resources cleaned up\n");
    return (1);
}
