/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 15:08:49 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/23 20:21:07 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	main(int argc, char **argv)
{
    t_shared        shared;
    t_philosopher   *philosophers;
    pthread_t       *threads;

    if (initialize_simulation(argc, argv, &shared) != 0)
        return (1);
    philosophers = init_philosophers(&shared);
    if (!philosophers)
	{
        printf("Error: Failed to initialize philosophers\n");
        cleanup_simulation(&shared, NULL);
        return (1);
    }
    if (start_threads(&shared, philosophers, &threads) != 0)
	{
        cleanup_simulation(&shared, philosophers);
        return (1);
    }
    join_threads(&shared, threads);
    cleanup_simulation(&shared, philosophers);
    free(threads);
    return (0);
}
