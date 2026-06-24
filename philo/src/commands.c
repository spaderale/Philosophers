/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abroslav <abroslav@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/18 20:26:35 by abroslav          #+#    #+#             */
/*   Updated: 2026/04/18 20:38:30 by abroslav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	grab_forks(t_philo *philo)
{
	if (philo->id_philo % 2 == 0)
	{
		pthread_mutex_lock(philo->fork_right);
		safe_print(philo, "has taken a fork");
		pthread_mutex_lock(philo->fork_left);
		safe_print(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->fork_left);
		safe_print(philo, "has taken a fork");
		pthread_mutex_lock(philo->fork_right);
		safe_print(philo, "has taken a fork");
	}
}

void	release_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->fork_left);
	pthread_mutex_unlock(philo->fork_right);
}

void	philo_eating(t_philo *philo)
{
	grab_forks(philo);
	pthread_mutex_lock(philo->meal_lock);
	philo->last_meal = get_timestamp();
	philo->done_meals++;
	pthread_mutex_unlock(philo->meal_lock);
	safe_print(philo, "is eating");
	smart_sleep(philo->table->time_eat, philo);
	release_forks(philo);
}
