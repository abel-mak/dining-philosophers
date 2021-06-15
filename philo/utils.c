/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-mak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/01 13:06:33 by abel-mak          #+#    #+#             */
/*   Updated: 2021/06/14 15:59:26 by abel-mak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

void	ft_sleep(int milliseconds)
{
	usleep(milliseconds * 1000);
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

time_t	ft_get_elapsedtime(struct timeval initial)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	return ((
			(now.tv_sec * 1000000 + now.tv_usec)
			- (initial.tv_sec * 1000000 + initial.tv_usec)
		) / 1000);
}

/*
** this mutex lock was added to make sure that he is not currently eating
** and also that he is not about to update the last he ate
*/

void	*check_died(void *arg)
{
	t_philo	*philo;

	philo = arg;
	while (g_info.end == 0)
	{
		pthread_mutex_lock(&philo->update_eat);
		if (ft_get_elapsedtime(philo->last_eaten) > g_info.time_to_die)
		{
			pthread_mutex_lock(&g_info.die_mutex);
			printf("%ld %d died\n",
				ft_get_elapsedtime(g_info.sim_start), (philo)->id);
			g_info.end = 1;
			break ;
		}
		pthread_mutex_unlock(&philo->update_eat);
		usleep(15);
	}
	return (NULL);
}

void	ft_log(char *msg, t_philo *philo)
{
	pthread_mutex_lock(&g_info.die_mutex);
	printf("%ld %d %s\n", ft_get_elapsedtime(g_info.sim_start), philo->id, msg);
	pthread_mutex_unlock(&g_info.die_mutex);
}
