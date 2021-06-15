/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-mak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/31 16:22:34 by abel-mak          #+#    #+#             */
/*   Updated: 2021/06/14 16:16:43 by abel-mak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

void	init_simulation(int *pid)
{
	int		i;
	t_philo	*philo;
	char	*name;

	i = 0;
	gettimeofday(&g_info.sim_start, NULL);
	while (i < g_info.num_philo)
	{
		usleep(65);
		pid[i] = fork();
		if (pid[i] == 0)
		{
			philo = (t_philo *)malloc(sizeof(t_philo) * g_info.num_philo);
			philo->id = i + 1;
			philo->eat = 0;
			philo->last_eaten.tv_sec = g_info.sim_start.tv_sec;
			philo->last_eaten.tv_usec = g_info.sim_start.tv_usec;
			name = ft_itoa(philo->id);
			philo->update_eat = ft_sem_mutex(name);
			free(name);
			ft_philo(philo);
			exit(0);
		}
		i++;
	}
}

void	init_sema(void)
{
	sem_unlink("die");
	sem_unlink("forks");
	sem_unlink("stop");
	sem_unlink("eat_counter");
	g_info.forks = sem_open("forks", O_CREAT | O_EXCL, S_IRWXU,
			g_info.num_philo);
	g_info.die_sem = sem_open("die", O_CREAT | O_EXCL, S_IRWXU, 1);
	g_info.stop = sem_open("stop", O_CREAT | O_EXCL, S_IRWXU, 0);
	g_info.eatcount = sem_open("eat_counter", O_CREAT | O_EXCL, S_IRWXU, 0);
}
