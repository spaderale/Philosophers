/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abroslav <abroslav@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/18 20:26:35 by abroslav          #+#    #+#             */
/*   Updated: 2026/06/24 21:50:00 by abroslav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	acquire_forks_even_first(
	t_philosopher *philosopher
)
{
	pthread_mutex_lock(philosopher->right_fork);
	thread_safe_display(philosopher, "has taken a fork");
	pthread_mutex_lock(philosopher->left_fork);
	thread_safe_display(philosopher, "has taken a fork");
}

static void	acquire_forks_odd_first(
	t_philosopher *philosopher
)
{
	pthread_mutex_lock(philosopher->left_fork);
	thread_safe_display(philosopher, "has taken a fork");
	pthread_mutex_lock(philosopher->right_fork);
	thread_safe_display(philosopher, "has taken a fork");
}

void	acquire_forks_for_eating(t_philosopher *philosopher)
{
	int	philosopher_parity;

	philosopher_parity = philosopher->philosopher_index % 2;
	if (philosopher_parity == 0)
	{
		acquire_forks_even_first(philosopher);
	}
	else
	{
		acquire_forks_odd_first(philosopher);
	}
}

void	release_forks_after_eating(t_philosopher *philosopher)
{
	pthread_mutex_unlock(philosopher->left_fork);
	pthread_mutex_unlock(philosopher->right_fork);
}

void	execute_eating_phase(t_philosopher *philosopher)
{
	long		current_time;

	acquire_forks_for_eating(philosopher);
	pthread_mutex_lock(philosopher->meals_sync_lock);
	current_time = get_current_timestamp_ms();
	philosopher->last_eating_timestamp = current_time;
	philosopher->total_meals_eaten++;
	pthread_mutex_unlock(philosopher->meals_sync_lock);
	thread_safe_display(philosopher, "is eating");
	precision_sleep(
		philosopher->table->eating_duration_ms,
		philosopher
	);
	release_forks_after_eating(philosopher);
}
