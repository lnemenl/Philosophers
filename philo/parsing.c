/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 15:09:17 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/12/05 10:41:54 by rkhakimu         ###   ########.fr       */
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

int parse_and_validate(int argc, char **argv, t_shared *shared)
{
    int result;

    if (argc != 5 && argc != 6)
        return (0);
    if (!is_numeric(argv[1]) || !safe_atoi(argv[1], &result) || result <= 0)
        return (0);
    shared->num_philosophers = result;
    if (!is_numeric(argv[2]) || !safe_atoi(argv[2], &result) || result <= 0)
        return (0);
    shared->time_to_die = result;
    if (!is_numeric(argv[3]) || !safe_atoi(argv[3], &result) || result <= 0)
        return (0);
    shared->time_to_eat = result;
    if (!is_numeric(argv[4]) || !safe_atoi(argv[4], &result) || result <= 0)
        return (0);
    shared->time_to_sleep = result;
    if (argc == 6)
    {
        if (!is_numeric(argv[5]) || !safe_atoi(argv[5], &result) || result < 0)
            return (0);
        shared->meals_required = result;
        if(shared->meals_required == 0)
            return (0);
    }
    else
    {
        shared->meals_required = -1;
    }
    if (shared->num_philosophers <= 0 || shared->time_to_die <= 0 ||
        shared->time_to_eat <= 0 || shared->time_to_sleep <= 0)
        return (0);

    return (1);
}
