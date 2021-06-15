/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_three.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-mak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/29 16:30:55 by abel-mak          #+#    #+#             */
/*   Updated: 2021/06/14 16:22:14 by abel-mak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

void	ft_log(char *msg, t_philo *philo)
{
	sem_wait(g_info.die_sem);
	printf("%ld %d %s\n",
		ft_get_elapsedtime(g_info.sim_start), philo->id, msg);
	sem_post(g_info.die_sem);
}

void	*check_died(void *arg)
{
	t_philo	*philo;

	philo = arg;
	while (1)
	{
		sem_wait(philo->update_eat);
		if ((ft_get_elapsedtime(philo->last_eaten)) > g_info.time_to_die)
		{
			sem_wait(g_info.die_sem);
			printf("%ld %d died\n",
				ft_get_elapsedtime(g_info.sim_start), ((t_philo*)philo)->id);
			sem_post(g_info.stop);
			sem_post(philo->update_eat);
			break ;
		}
		sem_post(philo->update_eat);
		usleep(20);
	}
	return (NULL);
}

void	ft_philo_life(t_philo *philo, int *l)
{
	sem_wait(g_info.forks);
	ft_log("has taken a fork", philo);
	sem_wait(g_info.forks);
	ft_log("has taken a fork", philo);
	sem_wait(philo->update_eat);
	if (g_info.n_time_eat != -1 && philo->eat < g_info.n_time_eat)
		philo->eat++;
	if (philo->eat == g_info.n_time_eat && *l == 0)
	{
		*l = 1;
		sem_post(g_info.eatcount);
	}
	gettimeofday(&philo->last_eaten, NULL);
	ft_log("is eating", philo);
	ft_sleep(g_info.time_to_eat);
	sem_post(philo->update_eat);
	sem_post(g_info.forks);
	sem_post(g_info.forks);
	ft_log("is sleeping", philo);
	ft_sleep(g_info.time_to_sleep);
	ft_log("is thinking", philo);
}

void	ft_philo(t_philo *philo)
{
	pthread_t	check_tid;
	int			l;

	l = 0;
	pthread_create(&check_tid, NULL, check_died, philo);
	pthread_detach(check_tid);
	pthread_create(&check_tid, NULL, ft_end, NULL);
	pthread_detach(check_tid);
	while (1)
	{
		if (g_info.num_philo != 1)
			ft_philo_life(philo, &l);
		else
		{
			ft_log("has taken a fork", philo);
			while (1)
				;
		}
	}
}
