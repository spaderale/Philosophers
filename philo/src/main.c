/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abroslav <abroslav@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 14:14:32 by abroslav          #+#    #+#             */
/*   Updated: 2026/06/24 21:50:00 by abroslav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	destroy_and_free_resources(t_dining_table *table)
{
	int	i;

	i = 0;
	while (i < table->philosopher_count)
	{
		pthread_mutex_destroy(&table->fork_mutexes[i]);
		i++;
	}
	pthread_mutex_destroy(&table->death_flag_lock);
	pthread_mutex_destroy(&table->meals_completion_lock);
	pthread_mutex_destroy(&table->display_lock);
	free(table->fork_mutexes);
	free(table->philosophers);
}

static void	initialize_philosopher_start_time(t_dining_table *table)
{
	int	i;

	table->start_timestamp = get_current_timestamp_ms();
	i = 0;
	while (i < table->philosopher_count)
	{
		table->philosophers[i].last_eating_timestamp = table->start_timestamp;
		i++;
	}
}

static int	create_all_philosopher_threads(t_dining_table *table)
{
	int	i;
	int	creation_result;

	initialize_philosopher_start_time(table);
	i = 0;
	while (i < table->philosopher_count)
	{
		creation_result = pthread_create(
			&table->philosophers[i].execution_thread,
			NULL,
			philosopher_routine,
			&table->philosophers[i]
		);
		if (creation_result != 0)
		{
			return (1);
		}
		i++;
	}
	return (0);
}

static void	wait_for_all_philosopher_threads(t_dining_table *table)
{
	int	i;

	i = 0;
	while (i < table->philosopher_count)
	{
		pthread_join(table->philosophers[i].execution_thread, NULL);
		i++;
	}
}

int	main(int ac, char **av)
{
	t_dining_table	table;
	int				validation_error;
	int				initialization_error;
	int				thread_creation_error;

	if (ac != 5 && ac != 6)
	{
		write(2, "Error: Wrong number of arguments\n", 34);
		return (1);
	}
	validation_error = validate_and_parse_arguments(&table, ac, av);
	if (validation_error != 0)
	{
		write(2, "Error: Invalid arguments provided\n", 34);
		return (1);
	}
	initialization_error = initialize_mutex_objects(&table);
	if (initialization_error != 0)
	{
		write(2, "Error: Mutex initialization failed\n", 35);
		return (1);
	}
	initialization_error = initialize_philosophers(&table);
	if (initialization_error != 0)
	{
		write(2, "Error: Philosopher initialization failed\n", 41);
		destroy_and_free_resources(&table);
		return (1);
	}
	thread_creation_error = create_all_philosopher_threads(&table);
	if (thread_creation_error != 0)
	{
		destroy_and_free_resources(&table);
		return (1);
	}
	start_death_monitor(&table);
	wait_for_all_philosopher_threads(&table);
	destroy_and_free_resources(&table);
	return (0);
}
