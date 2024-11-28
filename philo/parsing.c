/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 15:09:17 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/29 01:56:11 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int is_numeric(const char *str)
{
    int i;
    
    i = 0;
    if (!str || str[0] == '\0')
        return (0);
    if (str[i] == '+' || str[i] == '-')
        i++;
    while (str[i])
    {
        if (str[i] < '0' || str[i] > '9')
            return (0);
        i++;
    }
    return (1);
}

int validate_arguments(t_shared *shared)
{
    if (shared->num_philosophers <= 0 || shared->time_to_die <= 0 || shared->time_to_eat <= 0 || shared->time_to_sleep <= 0)
    {
        printf("Error: All parameters must be positive integers.\n");
        return (0);
    }
    if (shared->meals_required < -1)
    {
        printf("Error: Meals required must be -1 or a positive integer.\n");
        return (0);
    }
    return (1);
}

int parse_shared_parameters(t_shared *shared, char **argv)
{
    int result;

    if (!safe_atoi(argv[1], &result) || result <= 0)
        return (0);
    shared->num_philosophers = result;
    if (!safe_atoi(argv[2], &result) || result <= 0)
        return (0);
    shared->time_to_die = result;
    if (!safe_atoi(argv[3], &result) || result <= 0)
        return (0);
    shared->time_to_eat = result;
    if (!safe_atoi(argv[4], &result) || result <= 0)
        return (0);
    shared->time_to_sleep = result;
    if (argv[5])
    {
        if (!safe_atoi(argv[5], &result) || result < 0)
            return (0);
        shared->meals_required = result;
    }
    else
        shared->meals_required = -1;
    return (1);
}

int parse_arguments(int argc, char **argv, t_shared *shared)
{
    int i;
    int value;

    if (argc != 5 && argc != 6)
        return (0);
    i = 1;
    while (i < argc)
    {
        if (!is_numeric(argv[i]) || !safe_atoi(argv[i], &value))
            return (0);
        if (i == 1)
            shared->num_philosophers = value;
        else if (i == 2)
            shared->time_to_die = value;
        else if (i == 3)
            shared->time_to_eat = value;
        else if (i == 4)
            shared->time_to_sleep = value;
        else if (i == 5)
            shared->meals_required = value;
        i++;
    }
    if (argc == 5)
        shared->meals_required = -1;
    return (validate_arguments(shared));
}
