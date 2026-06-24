/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abroslav <abroslav@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 14:03:55 by abroslav          #+#    #+#             */
/*   Updated: 2026/04/19 17:57:37 by abroslav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_philo	t_philo;

typedef struct s_table
{
	int				nbr_philos;
	long			time_die;
	long			time_eat;
	long			time_sleep;
	long			start_time;
	int				must_eat;
	int				dead;
	int				meals_done;
	pthread_mutex_t	dead_lock;
	pthread_mutex_t	meals_lock;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	*forks;
	t_philo			*philos;
}	t_table;

typedef struct s_philo
{
	int				id_philo;
	int				done_meals;
	long			last_meal;
	pthread_mutex_t	*fork_left;
	pthread_mutex_t	*fork_right;
	pthread_mutex_t	*meal_lock;
	pthread_t		thread;
	t_table			*table;
}	t_philo;

/* ================= PHILOSOPHERS LIFE ========================= */
void	grab_forks(t_philo *philo);
void	release_forks(t_philo *philo);
void	philo_eating(t_philo *philo);

/* ================== SETUP & INIT ============================= */
int		setup_philos(t_table *table);
int		setup_mutex(t_table *table);
int		parsing(t_table *table, int ac, char **av);

/* =================== SIMULATION & MONITORING ================= */
void	run_checker(t_table *table);
int		check_death(t_philo *philo);
int		is_dead(t_table *table);

/* =================== CLEANUP ================================  */
void	cleanup(t_table *table);

/* =================== PHILOSOPHERS THREAD FUNCTION ============  */
void	*philo_life(void *arg);

/* ==================== UTILS ==================================  */
int		ft_atoi(const char *str);
long	get_timestamp(void);
long	elapsed_ms(long start);
void	smart_sleep(long ms, t_philo *philo);
void	safe_print(t_philo *philo, char *msg);

#endif
