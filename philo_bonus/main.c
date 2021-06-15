/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-mak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/31 16:25:34 by abel-mak          #+#    #+#             */
/*   Updated: 2021/06/14 16:17:32 by abel-mak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

int	is_digit(char *arg)
{
	int	i;

	i = 0;
	while (arg[i] != '\0')
	{
		if (arg[i] < '0' || arg[i] > '9')
			return (1);
		i++;
	}
	return (0);
}

void	*ft_end(void *arg)
{
	t_philo	*philo;

	philo = arg;
	sem_wait(g_info.stop);
	sem_post(g_info.stop);
	free(philo);
	exit(0);
	return (NULL);
}

void	*ft_monitor_eat(void *arg)
{
	int		i;

	i = 0;
	(void)arg;
	if (g_info.n_time_eat == -1)
		return (NULL);
	while (i < g_info.num_philo)
	{
		sem_wait(g_info.eatcount);
		i++;
	}
	sem_post(g_info.stop);
	return (NULL);
}

int	main(int argc, char **argv)
{
	int			*pid;
	pthread_t	tid;
	int			i;

	if (set_global_info(argc, argv) == 0)
	{
		pid = (int *)malloc(sizeof(int) * g_info.num_philo);
		init_sema();
		init_simulation(pid);
		pthread_create(&tid, NULL, ft_monitor_eat, NULL);
		i = 0;
		while (i < g_info.num_philo)
		{
			waitpid(pid[i], NULL, 0);
			i++;
		}
		free(pid);
	}
	else
	{
		printf("wrong arguments!\n");
		return (1);
	}
}
