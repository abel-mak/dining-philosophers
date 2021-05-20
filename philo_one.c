/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-mak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/20 12:23:25 by abel-mak          #+#    #+#             */
/*   Updated: 2021/05/20 17:32:02 by abel-mak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct	s_data
{
	int *forks;
	int	id;
}				t_data;

void	*philo(void *arg)
{
	t_data *data;
	int *forks;
	int id;

	data = arg;
	forks = data->forks;
	id = data->id;
	printf("i'm philo number %d and forks: %d\n", id, *(forks));
	return (NULL);
}

int main(int argc, char **argv)
{
	if (argc > 1)
	{
		struct timeval past;
		struct timeval curr;
		int	num_philo;
		int i;
		pthread_t tid[atoi(argv[1])];//thread id
		t_data *data;
		int forks;

		num_philo = atoi(argv[1]); //num_philo is also number of forks
		forks = num_philo;
		i = 0;
		while (i < num_philo)
		{
			data = (t_data*)malloc(sizeof(t_data));
			data->forks = &forks; 
			data->id = i + 1;
			pthread_create(&tid[i], NULL, philo, data);
			pthread_join(tid[i], NULL);
			i++;
		}
	}
	else
		printf("arguments\n");
	//gettimeofday(&past, NULL);
	//usleep(1000000);
	//gettimeofday(&curr, NULL);
	//printf("%ld\n", curr.tv_sec - past.tv_sec);
	return (0);
}
