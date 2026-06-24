/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abroslav <abroslav@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/18 18:29:07 by abroslav          #+#    #+#             */
/*   Updated: 2026/06/24 21:50:00 by abroslav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	query_death_status(t_dining_table *table)
{
	int		death_state;

	pthread_mutex_lock(&table->death_flag_lock);
	death_state = table->is_philosopher_dead;
	pthread_mutex_unlock(&table->death_flag_lock);
	return (death_state);
}

int	check_philosopher_death(t_philosopher *philosopher)
{
	long		current_time;
	long		last_meal_time;
	long		time_since_meal;
	int			death_occurred;

	pthread_mutex_lock(philosopher->meals_sync_lock);
	last_meal_time = philosopher->last_eating_timestamp;
	pthread_mutex_unlock(philosopher->meals_sync_lock);
	current_time = get_current_timestamp_ms();
	time_since_meal = current_time - last_meal_time;
	if (time_since_meal >= philosopher->table->death_timeout_ms)
	{
		death_occurred = 1;
	}
	else
	{
		death_occurred = 0;
	}
	return (death_occurred);
}

static int	count_philosophers_finished_meals(t_dining_table *table)
{
	int	i;
	int	count;
	int	meals_required;
	int	meals_eaten;

	meals_required = table->must_complete_meals;
	if (meals_required < 0)
	{
		return (0);
	}
	i = 0;
	count = 0;
	while (i < table->philosopher_count)
	{
		pthread_mutex_lock(&table->meals_completion_lock);
		meals_eaten = table->philosophers[i].total_meals_eaten;
		pthread_mutex_unlock(&table->meals_completion_lock);
		if (meals_eaten >= meals_required)
		{
			count++;
		}
		i++;
	}
	return (count);
}

static int	verify_all_meals_complete(t_dining_table *table)
{
	int	finished_count;
	int	total_philosophers;
	int	all_complete;

	finished_count = count_philosophers_finished_meals(table);
	total_philosophers = table->philosopher_count;
	if (finished_count == total_philosophers)
	{
		all_complete = 1;
	}
	else
	{
		all_complete = 0;
	}
	return (all_complete);
}

static void	set_death_flag(t_dining_table *table)
{
	pthread_mutex_lock(&table->death_flag_lock);
	table->is_philosopher_dead = 1;
	pthread_mutex_unlock(&table->death_flag_lock);
}

void	start_death_monitor(t_dining_table *table)
{
	int	i;
	int	death_found;
	int	all_finished;

	while (1)
	{
		i = 0;
		while (i < table->philosopher_count)
		{
			death_found = check_philosopher_death(&table->philosophers[i]);
			if (death_found != 0)
			{
				thread_safe_display(&table->philosophers[i], "died");
				set_death_flag(table);
				return ;
			}
			i++;
		}
		all_finished = verify_all_meals_complete(table);
		if (all_finished != 0)
		{
			set_death_flag(table);
			return ;
		}
		usleep(1000);
	}
}
