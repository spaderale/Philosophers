/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abroslav <abroslav@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/18 18:29:07 by abroslav          #+#    #+#             */
/*   Updated: 2026/04/18 20:45:20 by abroslav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_dead(t_table *table)
{
	int	val;

	pthread_mutex_lock(&table->dead_lock);
	val = table->dead;
	pthread_mutex_unlock(&table->dead_lock);
	return (val);
}

int	check_death(t_philo *philo)
{
	long	since_meal;

	pthread_mutex_lock(philo->meal_lock);
	since_meal = get_timestamp() - philo->last_meal;
	pthread_mutex_unlock(philo->meal_lock);
	if (since_meal >= philo->table->time_die)
		return (1);
	return (0);
}

static int	meals_enough(t_table *table)
{
	int	i;
	int	count;

	if (table->must_eat < 0)
		return (0);
	i = 0;
	count = 0;
	while (i < table->nbr_philos)
	{
		pthread_mutex_lock(&table->meals_lock);
		if (table->philos[i].done_meals >= table->must_eat)
			count++;
		pthread_mutex_unlock(&table->meals_lock);
		i++;
	}
	if (count == table->nbr_philos)
		return (1);
	return (0);
}

static void	set_dead(t_table *table)
{
	pthread_mutex_lock(&table->dead_lock);
	table->dead = 1;
	pthread_mutex_unlock(&table->dead_lock);
}

void	run_checker(t_table *table)
{
	int	i;

	while (1)
	{
		i = 0;
		while (i < table->nbr_philos)
		{
			if (check_death(&table->philos[i]))
			{
				safe_print(&table->philos[i], "died");
				set_dead(table);
				return ;
			}
			i++;
		}
		if (meals_enough(table))
		{
			set_dead(table);
			return ;
		}
		usleep(1000);
	}
}
