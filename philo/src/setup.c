/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abroslav <abroslav@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 14:18:36 by abroslav          #+#    #+#             */
/*   Updated: 2026/04/18 20:59:31 by abroslav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	check_valid(int val)
{
	if (val <= 0)
		return (1);
	return (0);
}

int	parsing(t_table *table, int ac, char **av)
{
	table->nbr_philos = ft_atoi(av[1]);
	table->time_die = ft_atoi(av[2]);
	table->time_eat = ft_atoi(av[3]);
	table->time_sleep = ft_atoi(av[4]);
	table->must_eat = -1;
	if (ac == 6)
		table->must_eat = ft_atoi(av[5]);
	if (check_valid(table->nbr_philos) || check_valid(table->time_die)
		|| check_valid(table->time_eat) || check_valid(table->time_sleep))
		return (1);
	if (ac == 6 && check_valid(table->must_eat))
		return (1);
	table->dead = 0;
	table->meals_done = 0;
	return (0);
}

int	setup_mutex(t_table *table)
{
	int	i;

	table->forks = malloc(sizeof(pthread_mutex_t) * table->nbr_philos);
	if (!table->forks)
		return (1);
	i = 0;
	while (i < table->nbr_philos)
	{
		pthread_mutex_init(&table->forks[i], NULL);
		i++;
	}
	pthread_mutex_init(&table->dead_lock, NULL);
	pthread_mutex_init(&table->meals_lock, NULL);
	pthread_mutex_init(&table->print_lock, NULL);
	return (0);
}

static void	assign_philo(t_table *table, int i)
{
	table->philos[i].id_philo = i + 1;
	table->philos[i].done_meals = 0;
	table->philos[i].fork_left = &table->forks[i];
	table->philos[i].fork_right = &table->forks[(i + 1) % table->nbr_philos];
	table->philos[i].meal_lock = &table->meals_lock;
	table->philos[i].table = table;
}

int	setup_philos(t_table *table)
{
	int	i;

	table->philos = malloc(sizeof(t_philo) * table->nbr_philos);
	if (!table->philos)
		return (1);
	i = 0;
	while (i < table->nbr_philos)
	{
		assign_philo(table, i);
		i++;
	}
	return (0);
}
