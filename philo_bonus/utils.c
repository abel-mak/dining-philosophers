/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-mak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/31 16:09:33 by abel-mak          #+#    #+#             */
/*   Updated: 2021/06/14 16:05:44 by abel-mak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

void	ft_sleep(int milliseconds)
{
	usleep(milliseconds * 1000);
}

time_t	ft_get_elapsedtime(struct timeval initial)
{
	struct timeval	now;
	time_t			e;

	gettimeofday(&now, NULL);
	e = (
			(now.tv_sec * 1000000 + now.tv_usec)
			- (initial.tv_sec * 1000000 + initial.tv_usec)
			) / 1000;
	return (e);
}

int	ft_atoi(const char *str)
{
	int		i;
	long	res;
	int		sign;

	sign = 1;
	res = 0;
	i = 0;
	while (str[i] == 32 || (str[i] >= '\t' && str[i] <= '\r'))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (sign == 1 && res > (res * 10) + (sign * (str[i] - '0')))
			return (-1);
		else if (sign == -1 && res < (res * 10 + sign * (str[i] - '0')))
			return (0);
		res = res * 10 + sign * (str[i] - '0');
		i++;
	}
	return (res);
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
	g_info.n_time_eat = -1;
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

sem_t	*ft_sem_mutex(char *name)
{
	sem_unlink(name);
	return (sem_open(name, O_CREAT, S_IRWXU, 1));
}
