/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-mak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/20 12:23:25 by abel-mak          #+#    #+#             */
/*   Updated: 2021/05/23 19:27:00 by abel-mak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

//i supposed that every philo own one fork which is the right
typedef struct	s_philo
{
	pthread_mutex_t right_fork; //right mutex for right fork, and it's own fork the left will be deduced.
	struct timeval	last_eaten;
	pthread_t tid;
	int				id;
}				t_philo;

typedef	struct	s_global_info
{
	int				num_philo;
	time_t			time_to_eat;
	time_t			time_to_die;
	time_t			time_to_sleep;
	int				n_time_eat;
	int				end;
	pthread_mutex_t die_mutex;
	struct timeval	sim_start;//start of simulation

}				t_global_info;

t_global_info g_info;

void	ft_sleep(int milliseconds)
{
	usleep(milliseconds * 1000);
}

int		ft_atoi(const char *str)
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
/*
** difference of time
** dt = (final time - initial time)
*/

time_t	ft_get_elapsedtime(struct timeval initial)
{
	struct timeval now;

	gettimeofday(&now, NULL);
	return ((
				(now.tv_sec * 1000000 + now.tv_usec) - 
				(g_info.sim_start.tv_sec * 1000000 + g_info.sim_start.tv_usec)
			) / 1000);
}

void	*check_died(void *philo)
{
	while(1)
	{
		if (ft_get_elapsedtime(((t_philo*)philo)->last_eaten) > g_info.time_to_die)
		{
			pthread_mutex_lock(&g_info.die_mutex);
			printf("%ld %d died\n", ft_get_elapsedtime(g_info.sim_start), ((t_philo*)philo)->id);
			g_info.end = 1;
			pthread_mutex_unlock(&g_info.die_mutex);
			break;
			//end the simulation
		}
	}
	return (NULL);
}

void	*ft_philo(void *arg)
{
	t_philo *philo;
	int i;
	pthread_mutex_t *left_fork;
	pthread_t	check_tid;

	philo = arg;
	pthread_create(&check_tid, NULL, check_died, philo);
	pthread_detach(check_tid);
	i = philo->id - 1;
	if (philo->id == g_info.num_philo)
		left_fork = &((philo - i)->right_fork);
	else
		left_fork = &((philo + 1)->right_fork);
	while (1)
	{
		pthread_mutex_lock(left_fork);
		printf("%ld %d has taken a fork | left_fork= %p\n", ft_get_elapsedtime(g_info.sim_start), philo->id, left_fork);
		pthread_mutex_lock(&philo->right_fork);
		printf("%ld %d has taken a fork\n", ft_get_elapsedtime(g_info.sim_start), philo->id);
		gettimeofday(&philo->last_eaten, NULL);
		printf("%ld %d is eating\n", ft_get_elapsedtime(g_info.sim_start), philo->id);
		ft_sleep(g_info.time_to_eat);
		pthread_mutex_unlock(&philo->right_fork);
		pthread_mutex_unlock(left_fork);
		printf("%ld %d is sleeping\n", ft_get_elapsedtime(g_info.sim_start), philo->id);
		ft_sleep(g_info.time_to_sleep);
		printf("%ld %d is thinking\n", ft_get_elapsedtime(g_info.sim_start), philo->id);
	}
	return (NULL);
}

int		set_global_info(int argc, char **argv)
{
	if (argc < 5 || argc > 6)
		return (-1);
	g_info.num_philo = ft_atoi(argv[1]);
	g_info.time_to_die = ft_atoi(argv[2]);
	g_info.time_to_eat = ft_atoi(argv[3]);
	g_info.time_to_sleep = ft_atoi(argv[4]);
	g_info.n_time_eat = -1;
	g_info.end = 0;
	pthread_mutex_init(&g_info.die_mutex, NULL);
	if (argc == 6)
	{
		g_info.n_time_eat = ft_atoi(argv[5]);
		if (g_info.n_time_eat < 0)
			return (-1);
	}
	if (g_info.num_philo < 0 || g_info.time_to_die < 0 
			|| g_info.time_to_eat < 0 || g_info.time_to_sleep < 0)
		return (-1);
	return (0);
}

//void	*monitor(void *arg)
//{
//	pthread_mutex_lock((pthread_mutex_t*)arg);
//	return (NULL);
//}

int main(int argc, char **argv)
{
	if (set_global_info(argc, argv) == 0)
	{
		int i;
		pthread_t tid[g_info.num_philo];//thread id
		t_philo philos[g_info.num_philo];
		pthread_t monitor_tid;

		i = 0;
		gettimeofday(&g_info.sim_start, NULL);
		while (i < g_info.num_philo)
		{
			philos[i].id = i + 1;
			philos[i].last_eaten.tv_usec = 0;
			philos[i].last_eaten.tv_sec = 0;
			philos[i].tid = tid[i];
			pthread_mutex_init(&philos[i].right_fork, NULL);
			pthread_create(&tid[i], NULL, ft_philo, &philos[i]);
			usleep(100);
			i++;
		}
		i = 0;
		while (i < g_info.num_philo)
		{
			pthread_detach(tid[i]);
			//pthread_join(tid[i], NULL);
			i++;
		}
		while (g_info.end == 0)
			;
		//pthread_mutex_init(&g_info.end, NULL);
		//pthread_create(&monitor_tid, NULL, monitor, &g_info.end);
		//usleep(100);
		//pthread_mutex_lock(&g_info.end);
	}
	else
	{
		//printf("%ld\n", sizeof(time_t));
		printf("arguments\n");
	}
	return (0);
}
