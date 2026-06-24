/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosop.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abroslav <abroslav@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/18 20:05:42 by abroslav          #+#    #+#             */
/*   Updated: 2026/04/18 20:27:23 by abroslav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	*one_philo(t_philo *philo)
{
	pthread_mutex_lock(philo->fork_left);
	safe_print(philo, "has taken a fork");
	smart_sleep(philo->table->time_die, philo);
	pthread_mutex_unlock(philo->fork_left);
	return (NULL);
}

static int	done_eating(t_philo *philo)
{
	int	done;

	done = 0;
	if (philo->table->must_eat > 0)
	{
		pthread_mutex_lock(philo->meal_lock);
		if (philo->done_meals >= philo->table->must_eat)
			done = 1;
		pthread_mutex_unlock(philo->meal_lock);
	}
	return (done);
}

static void	philo_thinking(t_philo *philo)
{
	long	time_thinking;

	safe_print(philo, "is thinking");
	if (philo->table->nbr_philos % 2 != 0)
	{
		time_thinking = philo->table->time_eat * 2 - philo->table->time_sleep;
		if (time_thinking < 0)
			time_thinking = 0;
		smart_sleep(time_thinking, philo);
	}
}

void	*philo_life(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->table->nbr_philos == 1)
		return (one_philo(philo));
	if (philo->id_philo % 2 == 0)
		smart_sleep(philo->table->time_eat / 2, philo);
	while (!is_dead(philo->table))
	{
		philo_eating(philo);
		if (done_eating(philo))
			break ;
		safe_print(philo, "is sleeping");
		smart_sleep(philo->table->time_sleep, philo);
		philo_thinking(philo);
	}
	return (NULL);
}
