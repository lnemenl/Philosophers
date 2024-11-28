/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 17:07:05 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/28 03:47:56 by rkhakimu         ###   ########.fr       */
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

static int has_philosopher_died(t_philosopher *philo, t_table *table)
{
    long long current_time;

    current_time = get_current_time();
    if (current_time - philo->last_meal_time > table->time_to_die)
    {
        print_status(philo, "died");
        table->simulation_end = 1;
        return (1);
    }
    return (0);
}

static int all_philosophers_satisfied(t_table *table)
{
    int i;

    if (table->must_eat_count == -1)
        return (0);

    i = 0;
    while (i < table->num_philosophers)
    {
        if (table->philosophers[i].meals_eaten < table->must_eat_count)
            return (0);
        i++;
    }
    return (1);
}

int monitor_status(t_table *table, int philo_index)
{
    safe_mutex_lock(&table->write_lock);
    if (has_philosopher_died(&table->philosophers[philo_index], table))
    {
        safe_mutex_unlock(&table->write_lock);
        return (1);
    }
    if (all_philosophers_satisfied(table))
    {
        table->simulation_end = 1;
        safe_mutex_unlock(&table->write_lock);
        return (1);
    }
    safe_mutex_unlock(&table->write_lock);
    return (0);
}

void *monitor_routine(void *arg)
{
    t_table *table = (t_table *)arg;
    int i;

    while (!get_simulation_status(table))
    {
        i = 0;
        while (i < table->num_philosophers)
        {
            if (monitor_status(table, i))
            {
                printf("Monitor detected termination condition\n");
                return (NULL);
            }
            i++;
        }
        usleep(500); // More frequent checks
    }
    printf("Monitor thread exiting\n");
    return (NULL);
}
