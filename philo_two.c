/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_two.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-mak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/26 11:31:58 by abel-mak          #+#    #+#             */
/*   Updated: 2021/05/29 16:27:10 by abel-mak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <sys/time.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>

typedef struct	s_philo
{
	int				eat;
	struct timeval	last_eaten;
	sem_t			*update_eat;
	pthread_t		tid;
	int				id;
}				t_philo;

typedef	struct	s_global_info
{
	int				num_philo;
	time_t			time_to_eat;
	time_t			time_to_die;
	time_t			time_to_sleep;
	int				n_time_eat;
	int				all_eat_count;
	int				end;
	int				all_close;
	struct timeval	sim_start;//start of simulation
	sem_t			*forks;
	sem_t			*die_sem;
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
** dt = (final(now) time - initial time)
*/

time_t	ft_get_elapsedtime(struct timeval initial)
{
	struct timeval now;
	time_t e;

	gettimeofday(&now, NULL);
	e = (
				(now.tv_sec * 1000000 + now.tv_usec) - 
				(initial.tv_sec * 1000000 + initial.tv_usec)
			) / 1000;
	return (e);
}


void	*check_died(void *arg)
{
	t_philo *philo;
	time_t e;
	int run_close;

	run_close = 0;
	philo = arg;
	while (g_info.end == 0)
	{
		sem_wait(philo->update_eat);
		if ((e = ft_get_elapsedtime(philo->last_eaten)) > g_info.time_to_die)
		{
			errno = 0;
			sem_wait(g_info.die_sem);
			printf("%d die_err: %s\n", philo->id, strerror(errno));
			if (g_info.end == 0 && (g_info.n_time_eat == -1
	                     || g_info.all_eat_count < g_info.num_philo))
				printf("%ld %d died last_eaten= %ld ---\n", ft_get_elapsedtime(g_info.sim_start), ((t_philo*)philo)->id, e);
			g_info.end = 1;
			run_close = 1;
			usleep(20);
			sem_post(g_info.die_sem);
			sem_post(philo->update_eat);
			break;
			//sem_post(g_info.die_sem);
			//sem_post(g_info.die_sem);
		}
		sem_post(philo->update_eat);
		usleep(20);
	}
	g_info.all_close++;
//	if (run_close == 1)
//		sem_close(g_info.forks);
//	sem_close(philo->update_eat);
//	pthread_detach(philo->tid);
	//printf("heyyy!\n");
	return (NULL);
}

void	ft_log(char *msg, t_philo *philo)
{
	sem_wait(g_info.die_sem);
	if (g_info.end == 0 && (g_info.n_time_eat == -1
                    || g_info.all_eat_count < g_info.num_philo))
		printf("%ld %d %s\n", 
				ft_get_elapsedtime(g_info.sim_start), philo->id, msg);
	sem_post(g_info.die_sem);
}

void	*ft_philo(void	*arg)
{
	t_philo *philo;
	pthread_t check_tid;
	int r;
	int l;

	l = 0;
	philo = arg;
	pthread_create(&check_tid, NULL, check_died, philo);
	pthread_detach(check_tid);
	while (g_info.end == 0)
	{
		errno = 0;
		r = sem_wait(g_info.forks);
		printf("wait= %d %s\n", r, strerror(errno));
		ft_log("has taken a fork", philo);
		sem_wait(g_info.forks);
		ft_log("has taken a fork", philo);
		sem_wait(philo->update_eat);
		printf("%d ate: %d all_eat_count: %d num_philo: %d\n", philo->id, philo->eat, g_info.all_eat_count, g_info.num_philo);
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
		sem_post(philo->update_eat);
		sem_post(g_info.forks);
		sem_post(g_info.forks);
		ft_log("is sleeping", philo);
		ft_sleep(g_info.time_to_sleep);
		ft_log("is thinking", philo);
	//	if (l == 1)
	//		break;
	}
	return (NULL);
}

int		set_global_info(int argc, char **argv)
{
	if (argc < 5 || argc > 6)
		return (1);
	g_info.num_philo = ft_atoi(argv[1]);
	g_info.time_to_die = ft_atoi(argv[2]);
	g_info.time_to_eat = ft_atoi(argv[3]);
	g_info.time_to_sleep = ft_atoi(argv[4]);
	g_info.all_eat_count = 0;
	printf("num_philo= %d to_die= %ld to_eat= %ld to_sleep= %ld\n"
		, g_info.num_philo, g_info.time_to_die, g_info.time_to_eat, g_info.time_to_sleep);
	g_info.n_time_eat = -1;
	g_info.end = 0;
	//pthread_mutex_init(&g_info.die_mutex, NULL);
	if (argc == 6)
	{
		g_info.n_time_eat = ft_atoi(argv[5]);
		if (g_info.n_time_eat < 0)
			return (1);
	}
	if (g_info.num_philo < 0 || g_info.time_to_die < 0 
			|| g_info.time_to_eat < 0 || g_info.time_to_sleep < 0)
		return (1);
	return (0);
}

char            *ft_itoa(int n);

sem_t *ft_sem_mutex(char *name)
{
	sem_unlink(name);
	return (sem_open(name, O_CREAT, S_IRWXU, 1));
}

int main(int argc, char **argv)
{
	if (set_global_info(argc, argv) == 0)
	{
		//t_philo philos[g_info.num_philo];
		pthread_t	tid[g_info.num_philo];
		t_philo		*philos;
		int i;
		char *name;

		philos = (t_philo*)malloc(sizeof(t_philo) * g_info.num_philo);
		sem_unlink("die");
		sem_unlink("forks");
		g_info.forks = sem_open("forks", O_CREAT | O_EXCL, S_IRWXU,
			   	g_info.num_philo);
		g_info.die_sem = sem_open("die", O_CREAT | O_EXCL, S_IRWXU, 1);
		printf("err: %s\n", strerror(errno));
		i = 0;
		gettimeofday(&g_info.sim_start, NULL);
		while (i < g_info.num_philo)
		{
			philos[i].id = i + 1;
			philos[i].eat = 0;
			philos[i].last_eaten.tv_sec = g_info.sim_start.tv_sec;
			philos[i].last_eaten.tv_usec = g_info.sim_start.tv_usec;
			name = ft_itoa(i + 1);
			philos[i].update_eat = ft_sem_mutex(name);
			philos[i].tid = tid[i];
			free(name);
		//	usleep(10);
			pthread_create(&tid[i], NULL, ft_philo, &philos[i]);
		//	pthread_detach(tid);
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
		while(g_info.end == 0 && (g_info.n_time_eat == -1 
					|| g_info.all_eat_count < g_info.num_philo))
			;
		free(philos);
		printf("==========\n");
		//sem_close(g_info.forks);
		//errno = 0;
		//while (1);
		//printf("sem_unlink= %d\n", sem_unlink("forks"));
		//sem_unlink("die");
		//printf("error= %s\n", strerror(errno));
		//sem_close(g_info.die_sem);
		//printf("close %d\n", sem_close(g_info.forks));
		//*forks = g_info.num_philo;
		//printf("ooo %d\n", *forks);
	}
	else
	{
		gettimeofday(&g_info.sim_start, NULL);
		ft_sleep(1000);
		printf("%ld\n", ft_get_elapsedtime(g_info.sim_start));
		printf("arguments\n");
	}
}
