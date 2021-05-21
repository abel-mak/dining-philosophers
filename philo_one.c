/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-mak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/20 12:23:25 by abel-mak          #+#    #+#             */
/*   Updated: 2021/05/21 18:19:32 by abel-mak         ###   ########.fr       */
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
	int numb_philo; //number of philosophers
	pthread_mutex_t right_fork; //right mutex for right fork, and it's own fork the left will be deduced.
	int				id;
}				t_philo;

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

void	*philo(void *arg)
{
	t_philo *philo;
	int i;
	pthread_mutex_t *left_fork;

	philo = arg;
	i = philo->id - 1;
	if (philo->id == philo->numb_philo)
		left_fork = &(philo - i)->right_fork;
	else
		left_fork = &(philo + 1)->right_fork;
	while (1)
	{
		pthread_mutex_lock(left_fork);
		printf("timestamp_in_ms %d has taken a fork\n", philo->id);
		pthread_mutex_lock(&philo->right_fork);
		printf("timestamp_in_ms %d has taken a fork\n", philo->id);
		printf("timestamp_in_ms %d is eating\n", philo->id);
		ft_sleep(2 * 1000);
		pthread_mutex_unlock(&philo->right_fork);
		pthread_mutex_unlock(left_fork);
		printf("timestamp_in_ms %d is sleeping\n", philo->id);
		ft_sleep(2 * 1000);
		printf("timestamp_in_ms %d is thinking\n", philo->id);
	}
	return (NULL);
}

int main(int argc, char **argv)
{
	if (argc > 1)
	{
		int	num_philo;
		int i;
		pthread_t tid[ft_atoi(argv[1])];//thread id
		t_philo philos[ft_atoi(argv[1])];

		num_philo = ft_atoi(argv[1]); //num_philo is also number of forks
		i = 0;
		while (i < num_philo)
		{
			//data = (t_data*)malloc(sizeof(t_data));
			//data[i].forks = &forks; 
			philos[i].id = i + 1;
			pthread_mutex_init(&philos[i].right_fork, NULL);
			//data[i].mutex = &mutex;
			pthread_create(&tid[i], NULL, philo, &philos[i]);
			i++;
		}
		i = 0;
		while (i < num_philo)
		{
			pthread_join(tid[i], NULL);
			i++;
		}
	}
	else
		printf("arguments\n");
	return (0);
}
