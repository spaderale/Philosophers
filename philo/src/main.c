/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abroslav <abroslav@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 14:14:32 by abroslav          #+#    #+#             */
/*   Updated: 2026/04/28 23:29:14 by abroslav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	cleanup(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->nbr_philos)
	{
		pthread_mutex_destroy(&table->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&table->dead_lock);
	pthread_mutex_destroy(&table->meals_lock);
	pthread_mutex_destroy(&table->print_lock);
	free(table->forks);
	free(table->philos);
}

static void	set_time(t_table *table)
{
	int	i;

	table->start_time = get_timestamp();
	i = 0;
	while (i < table->nbr_philos)
	{
		table->philos[i].last_meal = table->start_time;
		i++;
	}
}

static int	threads_start(t_table *table)
{
	int	i;

	set_time(table);
	i = 0;
	while (i < table->nbr_philos)
	{
		if (pthread_create(&table->philos[i].thread, NULL,
				philo_life, &table->philos[i]))
			return (1);
		i++;
	}
	return (0);
}

static void	threads_join(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->nbr_philos)
	{
		pthread_join(table->philos[i].thread, NULL);
		i++;
	}
}

int	main(int ac, char **av)
{
	t_table	table;

	if (ac != 5 && ac != 6)
	{
		write(2, "Error: Wrong number os arguments\n", 33);
		return (1);
	}
	if (parsing(&table, ac, av))
	{
		write(2, "Error: Invalid arguments\n", 25);
		return (1);
	}
	if (setup_mutex(&table) || setup_philos(&table))
	{
		write(2, "Error: Initialization failed\n", 29);
		return (1);
	}
	if (threads_start(&table))
		return (cleanup(&table), 1);
	run_checker(&table);
	threads_join(&table);
	cleanup(&table);
	return (0);
}
