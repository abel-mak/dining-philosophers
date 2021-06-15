/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_three.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-mak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/31 16:02:02 by abel-mak          #+#    #+#             */
/*   Updated: 2021/06/14 15:10:14 by abel-mak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_THREE_H
# define PHILO_THREE_H

# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <semaphore.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <sys/wait.h>

typedef struct s_philo
{
	int				eat;
	struct timeval	last_eaten;
	sem_t			*update_eat;
	int				id;
}	t_philo;

typedef struct s_global_info
{
	int				num_philo;
	time_t			time_to_eat;
	time_t			time_to_die;
	time_t			time_to_sleep;
	int				n_time_eat;
	struct timeval	sim_start;
	sem_t			*forks;
	sem_t			*die_sem;
	sem_t			*stop;
	sem_t			*eatcount;
}	t_global_info;

t_global_info	g_info;

void		init_simulation(int *pid);
void		init_sema(void);
void		*ft_end(void *arg);
void		*ft_monitor_eat(void *arg);
void		ft_log(char *msg, t_philo *philo);
void		*check_died(void *arg);
void		ft_philo_life(t_philo *philo, int *l);
void		ft_philo(t_philo *philo);
void		ft_sleep(int milliseconds);
time_t		ft_get_elapsedtime(struct timeval initial);
int			ft_atoi(const char *str);
int			set_global_info(int argc, char **argv);
sem_t		*ft_sem_mutex(char *name);
size_t		ft_strlen(char *str);
char		*ft_itoa(int n);
int			is_digit(char *arg);

#endif
