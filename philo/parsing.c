/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 15:09:17 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/28 02:33:08 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int is_valid_int(const char *str)
{
    int num = ft_atoi(str);
    return (num >= 0); // Additional checks for overflows can be added if needed
}

int parse_arguments(t_table *table, int argc, char **argv)
{
    if (argc != 5 && argc != 6)
    {
        printf("Error: Invalid number of arguments. Expected 5 or 6 arguments.\n");
        return (0);
    }
    if (!is_valid_int(argv[1]) || !is_valid_int(argv[2]) || !is_valid_int(argv[3]) || !is_valid_int(argv[4]))
    {
        printf("Error: Arguments must be positive integers.\n");
        return (0);
    }
    table->num_philosophers = ft_atoi(argv[1]);
    table->time_to_die = ft_atoi(argv[2]);
    table->time_to_eat = ft_atoi(argv[3]);
    table->time_to_sleep = ft_atoi(argv[4]);
    table->must_eat_count = -1;
    if (argc == 6)
    {
        if (!is_valid_int(argv[5]))
        {
            printf("Error: The optional argument must be a positive integer.\n");
            return (0);
        }
        table->must_eat_count = ft_atoi(argv[5]);
    }
    if (!validate_arguments(table))
    {
        printf("Error: Invalid argument values. Ensure all values are within valid ranges.\n");
        return (0);
    }

    return (1);
}

int validate_arguments(t_table *table)
{
    if (table->num_philosophers < 1)
    {
        printf("Error: Number of philosophers must be at least 1.\n");
        return (0);
    }
    if (table->time_to_die <= 0 || table->time_to_eat <= 0 || table->time_to_sleep <= 0)
    {
        printf("Error: Times must be greater than 0.\n");
        return (0);
    }
    if (table->must_eat_count < -1) // -1 indicates no limit on meals
    {
        printf("Error: Number of times each philosopher must eat must be -1 or greater.\n");
        return (0);
    }
    return (1);
}
