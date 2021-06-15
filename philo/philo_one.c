/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-mak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/20 12:23:25 by abel-mak          #+#    #+#             */
/*   Updated: 2021/06/14 15:59:25 by abel-mak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

void	ft_philo_life(t_philo *philo, pthread_mutex_t *left_fork)
{
	int	l;

	l = 0;
	pthread_mutex_lock(&philo->right_fork);
	ft_log("has taken a fork", philo);
	pthread_mutex_lock(left_fork);
	ft_log("has taken a fork", philo);
	pthread_mutex_lock(&philo->update_eat);
	if (g_info.n_time_eat != -1 && philo->eat < g_info.n_time_eat)
		philo->eat++;
	gettimeofday(&philo->last_eaten, NULL);
	ft_log("is eating", philo);
	ft_sleep(g_info.time_to_eat);
	if (philo->eat == g_info.n_time_eat && l == 0)
	{
		l = 1;
		g_info.all_eat_count++;
	}
	pthread_mutex_unlock(&philo->update_eat);
	pthread_mutex_unlock(left_fork);
	pthread_mutex_unlock(&philo->right_fork);
	ft_log("is sleeping", philo);
	ft_sleep(g_info.time_to_sleep);
	ft_log("is thinking", philo);
}

void	*ft_philo(void *arg)
{
	t_philo			*philo;
	int				i;
	pthread_mutex_t	*left_fork;
	pthread_t		check_tid;

	philo = arg;
	pthread_create(&check_tid, NULL, check_died, philo);
	pthread_detach(check_tid);
	i = philo->id - 1;
	if (philo->id == g_info.num_philo && g_info.num_philo != 1)
		left_fork = &((philo - i)->right_fork);
	else if (g_info.num_philo != 1)
		left_fork = &((philo + 1)->right_fork);
	while (g_info.end == 0)
	{
		if (g_info.num_philo != 1)
			ft_philo_life(philo, left_fork);
		else
			ft_lonely_philo(philo);
	}
	return (NULL);
}

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

int	set_global_info(int argc, char **argv)
{
	if (argc < 5 || argc > 6 || is_digit(argv[1]) == 1
		|| is_digit(argv[2]) == 1 || is_digit(argv[3]) == 1
		|| is_digit(argv[4]) == 1)
		return (1);
	g_info.num_philo = ft_atoi(argv[1]);
	g_info.time_to_die = ft_atoi(argv[2]);
	g_info.time_to_eat = ft_atoi(argv[3]);
	g_info.time_to_sleep = ft_atoi(argv[4]);
	g_info.all_eat_count = 0;
	g_info.n_time_eat = -1;
	g_info.end = 0;
	pthread_mutex_init(&g_info.die_mutex, NULL);
	pthread_mutex_init(&g_info.log_mutex, NULL);
	if (argc == 6)
	{
		g_info.n_time_eat = ft_atoi(argv[5]);
		if (g_info.n_time_eat <= 0 || is_digit(argv[5]) == 1)
			return (1);
	}
	if (g_info.num_philo < 1 || g_info.time_to_die < 1
		|| g_info.time_to_eat < 1 || g_info.time_to_sleep < 1)
		return (1);
	return (0);
}

/*
 * delay is for dead lock and mutex_init
 */

void	init_simulation(t_philo *philos)
{
	int			i;
	pthread_t	tid;

	i = 0;
	while (i < g_info.num_philo)
	{
		philos[i].id = i + 1;
		philos[i].last_eaten.tv_usec = g_info.sim_start.tv_usec;
		philos[i].last_eaten.tv_sec = g_info.sim_start.tv_sec;
		philos[i].eat = 0;
		pthread_mutex_init(&(philos[i].right_fork), NULL);
		pthread_mutex_init(&(philos[i].update_eat), NULL);
		usleep(110);
		pthread_create(&tid, NULL, ft_philo, &philos[i]);
		pthread_detach(tid);
		i++;
	}
}
