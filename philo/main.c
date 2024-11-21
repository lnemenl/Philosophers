/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 15:08:49 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/21 12:54:07 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int main(int argc, char **argv)
{
    t_shared        shared;
    t_philosopher   *philosophers;
    pthread_t       *threads;

    if (initialize_simulation(argc, argv, &shared))
        return (1);
    if (handle_one_philosopher(&shared))
        return (0);
    philosophers = init_philosophers(&shared);
    if (!philosophers)
    {
        printf("Error: Philosopher initialization failed\n");
        return (1);
    }
    if (start_threads(&shared, philosophers, &threads))
    {
        cleanup_simulation(&shared, philosophers);
        return (1);
    }
    join_threads(&shared, threads);
    cleanup_simulation(&shared, philosophers);
    return (0);
}
