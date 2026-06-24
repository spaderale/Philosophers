/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abroslav <abroslav@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/18 18:47:09 by abroslav          #+#    #+#             */
/*   Updated: 2026/06/24 21:50:00 by abroslav         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	convert_string_to_integer(const char *str)
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
	{
		return (-1);
	}
	return (result);
}

long	get_current_timestamp_ms(void)
{
	struct timeval	current_time;
	long			timestamp_milliseconds;

	gettimeofday(&current_time, NULL);
	timestamp_milliseconds = (current_time.tv_sec * 1000)
		+ (current_time.tv_usec / 1000);
	return (timestamp_milliseconds);
}

long	calculate_elapsed_time_ms(long start_point)
{
	long	current_time;
	long	elapsed_duration;

	current_time = get_current_timestamp_ms();
	elapsed_duration = current_time - start_point;
	return (elapsed_duration);
}

void	thread_safe_display(
	t_philosopher *philosopher,
	char *message
)
{
	long		elapsed_time;
	int			is_dead_status;
	int			can_display;

	pthread_mutex_lock(&philosopher->table->display_lock);
	is_dead_status = query_death_status(philosopher->table);
	if (message[0] == 'd')
	{
		can_display = 1;
	}
	else if (is_dead_status == 0)
	{
		can_display = 1;
	}
	else
	{
		can_display = 0;
	}
	if (can_display != 0)
	{
		elapsed_time = calculate_elapsed_time_ms(
			philosopher->table->start_timestamp
		);
		printf("%ld %d %s\n",
			elapsed_time,
			philosopher->philosopher_index,
			message);
	}
	pthread_mutex_unlock(&philosopher->table->display_lock);
}

void	precision_sleep(long milliseconds, t_philosopher *philosopher)
{
	long	start_time;
	long	elapsed_time;
	int	death_check;

	start_time = get_current_timestamp_ms();
	while (1)
	{
		elapsed_time = calculate_elapsed_time_ms(start_time);
		if (elapsed_time >= milliseconds)
		{
			break ;
		}
		pthread_mutex_lock(&philosopher->table->death_flag_lock);
		death_check = philosopher->table->is_philosopher_dead;
		pthread_mutex_unlock(&philosopher->table->death_flag_lock);
		if (death_check != 0)
		{
			return ;
		}
		usleep(500);
	}
}
