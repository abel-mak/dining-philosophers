/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-mak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/02 17:20:06 by abel-mak          #+#    #+#             */
/*   Updated: 2021/06/14 16:26:27 by abel-mak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

void	ft_lonely_philo(t_philo *philo)
{
	ft_log("has taken a fork", philo);
	while (1)
		;
}

int	main(int argc, char **argv)
{
	t_philo	*philos;

	if (set_global_info(argc, argv) == 0)
	{
		philos = (t_philo *)malloc(sizeof(t_philo) * g_info.num_philo);
		gettimeofday(&g_info.sim_start, NULL);
		init_simulation(philos);
		while (g_info.end == 0 && (g_info.n_time_eat == -1
				|| g_info.all_eat_count < g_info.num_philo))
			;
		free(philos);
		return (0);
	}
	else
	{
		printf("wrong arguments\n");
		return (1);
	}
}
