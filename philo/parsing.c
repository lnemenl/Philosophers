/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 15:09:17 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/12/02 07:46:29 by rkhakimu         ###   ########.fr       */
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
    if (shared->num_philosophers <= 0 || shared->time_to_die <= 0 ||
        shared->time_to_eat <= 0 || shared->time_to_sleep <= 0)
    {
        printf("Error: All parameters must be positive integers.\n");
        return (0);
    }
    if (shared->meals_required < -1)
        return (0);
    // if (shared->time_to_die <= shared->time_to_eat + shared->time_to_sleep)
    // {
    //     printf("Error: time_to_die must be greater than the sum of time_to_eat and time_to_sleep.\n");
    //     return (0);
    // }
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
    int value;

    if (argc != 5 && argc != 6)
        return (0);
    if (!is_numeric(argv[1]) || !safe_atoi(argv[1], &value) || value <= 0)
        return (0);
    shared->num_philosophers = value;
    if (!is_numeric(argv[2]) || !safe_atoi(argv[2], &value) || value <= 0)
        return (0);
    shared->time_to_die = value;
    if (!is_numeric(argv[3]) || !safe_atoi(argv[3], &value) || value <= 0)
        return (0);
    shared->time_to_eat = value;
    if (!is_numeric(argv[4]) || !safe_atoi(argv[4], &value) || value <= 0)
        return (0);
    shared->time_to_sleep = value;
    if (argc == 6)
    {
        if (!is_numeric(argv[5]) || !safe_atoi(argv[5], &value) || value < 0)
            return (0);
        shared->meals_required = value;
    }
    else
        shared->meals_required = -1;
    if (!validate_arguments(shared))
        return (0);
    return (1);
}
