/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abroslav <abroslav@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 14:18:36 by abroslav          #+#    #+#             */
/*   Updated: 2026/06/24 21:50:00 by abroslav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	validate_positive_integer(int value)
{
	if (value <= 0)
	{
		return (1);
	}
	return (0);
}

static int	parse_philosopher_count(t_dining_table *table, const char *arg)
{
	table->philosopher_count = convert_string_to_integer(arg);
	if (validate_positive_integer(table->philosopher_count) != 0)
	{
		return (1);
	}
	return (0);
}

static int	parse_death_timeout(t_dining_table *table, const char *arg)
{
	table->death_timeout_ms = convert_string_to_integer(arg);
	if (validate_positive_integer(table->death_timeout_ms) != 0)
	{
		return (1);
	}
	return (0);
}

static int	parse_eating_duration(t_dining_table *table, const char *arg)
{
	table->eating_duration_ms = convert_string_to_integer(arg);
	if (validate_positive_integer(table->eating_duration_ms) != 0)
	{
		return (1);
	}
	return (0);
}

static int	parse_sleeping_duration(t_dining_table *table, const char *arg)
{
	table->sleeping_duration_ms = convert_string_to_integer(arg);
	if (validate_positive_integer(table->sleeping_duration_ms) != 0)
	{
		return (1);
	}
	return (0);
}

static int	parse_meal_requirement(t_dining_table *table, const char *arg)
{
	if (arg == NULL)
	{
		table->must_complete_meals = -1;
		return (0);
	}
	table->must_complete_meals = convert_string_to_integer(arg);
	if (validate_positive_integer(table->must_complete_meals) != 0)
	{
		return (1);
	}
	return (0);
}

int	validate_and_parse_arguments(t_dining_table *table, int ac, char **av)
{
	int	parse_result;

	parse_result = parse_philosopher_count(table, av[1]);
	if (parse_result != 0)
	{
		return (1);
	}
	parse_result = parse_death_timeout(table, av[2]);
	if (parse_result != 0)
	{
		return (1);
	}
	parse_result = parse_eating_duration(table, av[3]);
	if (parse_result != 0)
	{
		return (1);
	}
	parse_result = parse_sleeping_duration(table, av[4]);
	if (parse_result != 0)
	{
		return (1);
	}
	if (ac == 6)
	{
		parse_result = parse_meal_requirement(table, av[5]);
	}
	else
	{
		parse_result = parse_meal_requirement(table, NULL);
	}
	if (parse_result != 0)
	{
		return (1);
	}
	table->is_philosopher_dead = 0;
	table->total_meals_finished = 0;
	return (0);
}

int	initialize_mutex_objects(t_dining_table *table)
{
	int	i;

	table->fork_mutexes = malloc(
		sizeof(pthread_mutex_t) * table->philosopher_count
	);
	if (table->fork_mutexes == NULL)
	{
		return (1);
	}
	i = 0;
	while (i < table->philosopher_count)
	{
		pthread_mutex_init(&table->fork_mutexes[i], NULL);
		i++;
	}
	pthread_mutex_init(&table->death_flag_lock, NULL);
	pthread_mutex_init(&table->meals_completion_lock, NULL);
	pthread_mutex_init(&table->display_lock, NULL);
	return (0);
}

static void	configure_philosopher_attributes(
	t_dining_table *table,
	int philosopher_position
)
{
	t_philosopher	*current_philosopher;
	int				next_position;

	current_philosopher = &table->philosophers[philosopher_position];
	next_position = (philosopher_position + 1) % table->philosopher_count;
	current_philosopher->philosopher_index = philosopher_position + 1;
	current_philosopher->total_meals_eaten = 0;
	current_philosopher->left_fork = &table->fork_mutexes[philosopher_position];
	current_philosopher->right_fork = &table->fork_mutexes[next_position];
	current_philosopher->meals_sync_lock = &table->meals_completion_lock;
	current_philosopher->table = table;
}

int	initialize_philosophers(t_dining_table *table)
{
	int	i;

	table->philosophers = malloc(
		sizeof(t_philosopher) * table->philosopher_count
	);
	if (table->philosophers == NULL)
	{
		return (1);
	}
	i = 0;
	while (i < table->philosopher_count)
	{
		configure_philosopher_attributes(table, i);
		i++;
	}
	return (0);
}
