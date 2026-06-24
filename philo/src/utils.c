/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abroslav <abroslav@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/18 18:47:09 by abroslav          #+#    #+#             */
/*   Updated: 2026/04/19 17:58:42 by abroslav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *str)
{
	int	result;
	int	i;

	i = 0;
	result = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	if (str[i] != '\0')
		return (-1);
	return (result);
}

long	get_timestamp(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

long	elapsed_ms(long start)
{
	return (get_timestamp() - start);
}

void	safe_print(t_philo *philo, char *msg)
{
	pthread_mutex_lock(&philo->table->print_lock);
	if (!is_dead(philo->table) || msg[0] == 'd')
		printf("%ld %d %s\n", elapsed_ms(philo->table->start_time),
			philo->id_philo, msg);
	pthread_mutex_unlock(&philo->table->print_lock);
}

void	smart_sleep(long ms, t_philo *philo)
{
	long	start;

	start = get_timestamp();
	while (elapsed_ms(start) < ms)
	{
		pthread_mutex_lock(&philo->table->dead_lock);
		if (philo->table->dead)
		{
			pthread_mutex_unlock(&philo->table->dead_lock);
			return ;
		}
		pthread_mutex_unlock(&philo->table->dead_lock);
		usleep(500);
	}
}
