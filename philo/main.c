/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 15:08:49 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/28 02:30:15 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void *handle_single_philosopher(t_philosopher *philo)
{
    pthread_mutex_t *fork;

    fork = &philo->table->forks[philo->left_fork_id];
    safe_mutex_lock(fork);
    print_status(philo, "has taken a fork");
    smart_sleep(philo->table->time_to_die, philo->table);
    print_status(philo, "died");
    safe_mutex_unlock(fork);
    safe_mutex_lock(&philo->table->write_lock);
    philo->table->simulation_end = 1;
    safe_mutex_unlock(&philo->table->write_lock);
    return (NULL);
}

static int initialize_simulation(t_table *table, int argc, char **argv)
{
    if (!init_table(table, argc, argv))
    {
        printf("Error: Failed to initialize the table. Check input parameters.\n");
        return (0);
    }
    if (!create_philosophers(table))
    {
        printf("Error: Failed to create philosopher threads. Cleaning up...\n");
        clean_table(table);
        return (0);
    }
    return (1);
}

int main(int argc, char **argv)
{
    t_table table;

    if (argc != 5 && argc != 6)
    {
        printf("Usage: ./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n");
        return (1);
    }
    if (!initialize_simulation(&table, argc, argv))
        return (1);
    table.start_time = get_current_time();
    if (!create_monitor(&table))
    {
        printf("Error: Failed to create monitor thread. Cleaning up...\n");
        end_simulation(&table);
        return (1);
    }
    pthread_join(table.monitor_thread, NULL);
    end_simulation(&table);
    return (0);
}
