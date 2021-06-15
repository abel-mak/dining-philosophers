/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-mak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/01 12:54:17 by abel-mak          #+#    #+#             */
/*   Updated: 2021/06/14 16:00:37 by abel-mak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_ONE_H
# define PHILO_ONE_H

# include <stdio.h>
# include <sys/time.h>
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>

typedef struct s_philo
{
	int				eat;
	pthread_mutex_t	update_eat;
	pthread_mutex_t	right_fork;
	struct timeval	last_eaten;
	int				id;
}				t_philo;

typedef struct s_global_info
{
	int				num_philo;
	time_t			time_to_eat;
	time_t			time_to_die;
	time_t			time_to_sleep;
	int				n_time_eat;
	int				all_eat_count;
	int				end;
	pthread_mutex_t	die_mutex;
	pthread_mutex_t	log_mutex;
	struct timeval	sim_start;
}				t_global_info;

t_global_info	g_info;

void	ft_sleep(int milliseconds);
int		ft_atoi(const char *str);
time_t	ft_get_elapsedtime(struct timeval initial);
void	*check_died(void *arg);
void	ft_log(char *msg, t_philo *philo);
void	ft_philo_life(t_philo *philo, pthread_mutex_t *left_fork);
void	*ft_philo(void *arg);
int		set_global_info(int argc, char **argv);
void	init_simulation(t_philo *philos);
int		is_digit(char *arg);
void	ft_lonely_philo(t_philo *philo);

#endif
