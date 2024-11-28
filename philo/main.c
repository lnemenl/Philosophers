/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 15:08:49 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/29 00:47:51 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int main(int argc, char **argv)
{
    t_shared shared;
    t_thread_data data;

    if (!initialize_simulation(&shared, argc, argv))
    {
        print_error("Error: Failed to initialize simulation.\n");
        return (1);
    }
    if (!allocate_thread_data(&data, &shared))
    {
        print_error("Error: Failed to allocate thread data.\n");
        return (1);
    }
    if (!launch_threads(&data))
    {
        print_error("Error: Failed to launch threads.\n");
        clean_up(&data);
        return (1);
    }
    clean_up(&data);
    return (0);
}
