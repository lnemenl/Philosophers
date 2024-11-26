/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkhakimu <rkhakimu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 15:08:49 by rkhakimu          #+#    #+#             */
/*   Updated: 2024/11/26 17:49:49 by rkhakimu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int main(int argc, char **argv)
 {
    t_shared shared;
    t_philosopher *philosophers;

    if (parse_input(argc, argv, &shared))
        return 1;
    philosophers = setup_simulation(&shared);
    if (!philosophers)
        return 1;
    if (manage_threads(philosophers, &shared))
    {
        cleanup_simulation(&shared, philosophers);
        return (1);
    }
    cleanup_simulation(&shared, philosophers);
    return (0);
}
