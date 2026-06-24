/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abroslav <abroslav@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/18 20:05:42 by abroslav          #+#    #+#             */
/*   Updated: 2026/06/24 21:50:00 by abroslav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	*handle_single_philosopher(t_philosopher *philosopher)
{
	pthread_mutex_lock(philosopher->left_fork);
	thread_safe_display(philosopher, "has taken a fork");
	precision_sleep(philosopher->table->death_timeout_ms, philosopher);
	pthread_mutex_unlock(philosopher->left_fork);
	return (NULL);
}

static int	verify_meal_quota_completed(t_philosopher *philosopher)
{
	int		meals_requirement;
	int		meals_eaten;
	int		quota_met;

	meals_requirement = philosopher->table->must_complete_meals;
	if (meals_requirement <= 0)
	{
		return (0);
	}
	pthread_mutex_lock(philosopher->meals_sync_lock);
	meals_eaten = philosopher->total_meals_eaten;
	pthread_mutex_unlock(philosopher->meals_sync_lock);
	if (meals_eaten >= meals_requirement)
	{
		quota_met = 1;
	}
	else
	{
		quota_met = 0;
	}
	return (quota_met);
}

static void	execute_thinking_phase(t_philosopher *philosopher)
{
	long		thinking_duration;
	int			philosopher_count;
	int			is_even_count;

	thread_safe_display(philosopher, "is thinking");
	philosopher_count = philosopher->table->philosopher_count;
	if (philosopher_count % 2 == 0)
	{
		return ;
	}
	is_even_count = 0;
	thinking_duration = philosopher->table->eating_duration_ms * 2
		- philosopher->table->sleeping_duration_ms;
	if (thinking_duration < 0)
	{
		thinking_duration = 0;
	}
	precision_sleep(thinking_duration, philosopher);
}

void	*philosopher_routine(void *arg)
{
	t_philosopher	*philosopher;
	int				is_quota_complete;
	int				death_occurred;

	philosopher = (t_philosopher *)arg;
	if (philosopher->table->philosopher_count == 1)
	{
		return (handle_single_philosopher(philosopher));
	}
	if (philosopher->philosopher_index % 2 == 0)
	{
		precision_sleep(
			philosopher->table->eating_duration_ms / 2,
			philosopher
		);
	}
	while (1)
	{
		death_occurred = query_death_status(philosopher->table);
		if (death_occurred != 0)
		{
			break ;
		}
		execute_eating_phase(philosopher);
		is_quota_complete = verify_meal_quota_completed(philosopher);
		if (is_quota_complete != 0)
		{
			break ;
		}
		thread_safe_display(philosopher, "is sleeping");
		precision_sleep(
			philosopher->table->sleeping_duration_ms,
			philosopher
		);
		execute_thinking_phase(philosopher);
	}
	return (NULL);
}
