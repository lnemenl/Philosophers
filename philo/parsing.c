/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 15:09:17 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/26 19:08:34 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int validate_argument(const char *arg, int *value)
{
    *value = ft_atoi(arg);
    if (*value <= 0)
        return (1);
    return (0);
}

int parse_input(int argc, char **argv, t_shared *shared)
{
    if (argc != 5 && argc != 6)
    {
        printf("Usage: ./philosophers num_philosophers time_to_die time_to_eat time_to_sleep [num_meals_required]\n");
        return (1);
    }
    if (validate_argument(argv[1], &shared->num_philosophers) ||
        validate_argument(argv[2], &shared->time_to_die) ||
        validate_argument(argv[3], &shared->time_to_eat) ||
        validate_argument(argv[4], &shared->time_to_sleep))
    {
        printf("Error: Arguments must be positive integers.\n");
        return (1);
    }
    if (argc == 6)
    {
        if (validate_argument(argv[5], &shared->num_meals_required))
        {
            printf("Error: num_meals_required must be a positive integer.\n");
            return (1);
        }
    }
    else
        shared->num_meals_required = -1;

    shared->is_simulation_running = 1;
    return (0);
}

