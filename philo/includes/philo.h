/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abroslav <abroslav@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 14:03:55 by abroslav          #+#    #+#             */
/*   Updated: 2026/06/24 21:50:00 by abroslav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_philosopher	t_philosopher;

typedef struct s_dining_table
{
	int				philosopher_count;
	int				is_philosopher_dead;
	int				total_meals_finished;
	long			start_timestamp;
	long			death_timeout_ms;
	long			eating_duration_ms;
	long			sleeping_duration_ms;
	int				must_complete_meals;
	pthread_mutex_t	death_flag_lock;
	pthread_mutex_t	meals_completion_lock;
	pthread_mutex_t	display_lock;
	pthread_mutex_t	*fork_mutexes;
	t_philosopher	*philosophers;
}	t_dining_table;

typedef struct s_philosopher
{
	int				philosopher_index;
	int				total_meals_eaten;
	long			last_eating_timestamp;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*meals_sync_lock;
	pthread_t		execution_thread;
	t_dining_table	*table;
}	t_philosopher;

/* ================= PHILOSOPHER ACTIONS ========================= */
void	acquire_forks_for_eating(t_philosopher *philosopher);
void	release_forks_after_eating(t_philosopher *philosopher);
void	execute_eating_phase(t_philosopher *philosopher);

/* ================== INITIALIZATION & SETUP ===================== */
int		initialize_philosophers(t_dining_table *table);
int		initialize_mutex_objects(t_dining_table *table);
int		validate_and_parse_arguments(t_dining_table *table, int ac, char **av);

/* =================== MONITORING & DEATH DETECTION ============== */
void	start_death_monitor(t_dining_table *table);
int		check_philosopher_death(t_philosopher *philosopher);
int		query_death_status(t_dining_table *table);

/* =================== RESOURCE CLEANUP ==========================  */
void	destroy_and_free_resources(t_dining_table *table);

/* =================== PHILOSOPHER THREAD ROUTINE ================  */
void	*philosopher_routine(void *arg);

/* ==================== UTILITY FUNCTIONS ==========================  */
int		convert_string_to_integer(const char *str);
long	get_current_timestamp_ms(void);
long	calculate_elapsed_time_ms(long start_point);
void	precision_sleep(long milliseconds, t_philosopher *philosopher);
void	thread_safe_display(t_philosopher *philosopher, char *message);

#endif
