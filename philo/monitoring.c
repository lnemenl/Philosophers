/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 17:07:05 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/28 05:20:51 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int get_simulation_status(t_table *table)
{
    int status;

    safe_mutex_lock(&table->write_lock);
    status = table->simulation_end;
    safe_mutex_unlock(&table->write_lock);
    return (status);
}

static int check_philosopher_died(t_table *table, int philo_index)
{
    long long current_time = get_current_time();

    if (current_time - table->philosophers[philo_index].last_meal_time > table->time_to_die)
    {
        print_status(&table->philosophers[philo_index], "died");
        table->simulation_end = 1;
        return (1);
    }
    return (0);
}

static int check_all_ate_enough(t_table *table)
{
    int i;

    if (table->must_eat_count <= 0)
        return (0);

    i = 0;
    while (i < table->num_philosophers)
    {
        if (table->philosophers[i].meals_eaten < table->must_eat_count)
            return (0);
        i++;
    }
    table->simulation_end = 1;
    return (1);
}

int monitor_status(t_table *table, int philo_index)
{
    int status;
    
    status = 0;
    safe_mutex_lock(&table->write_lock);
    if (check_philosopher_died(table, philo_index))
        status = 1;
    else if (check_all_ate_enough(table))
        status = 1;
    safe_mutex_unlock(&table->write_lock);
    return (status);
}

void *monitor_routine(void *arg)
{
    t_table *table = (t_table *)arg;

    while (!get_simulation_status(table))
    {
        for (int i = 0; i < table->num_philosophers; i++)
        {
            if (monitor_status(table, i))
                return (NULL);
        }
        usleep(200);
    }
    printf("Monitor thread exiting.\n");
    return (NULL);
}
