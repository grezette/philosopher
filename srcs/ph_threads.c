/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ph_threads.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grezette <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/21 20:39:21 by grezette          #+#    #+#             */
/*   Updated: 2021/12/03 20:27:33 by grezette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static void
	activity(t_all_var *all_var, int i, char *str, long long int duration)
{
	long long int	now;
	long long int	elapsed;

	elapsed = 0;
	if ((str[0] == 'd' && !le_z(all_var, true)) || !le_z(all_var, false))
	{
		if (str[3] == 'e')
		{
			pthread_mutex_lock(&all_var->philo[i].m_nb_ate);
			all_var->philo[i].last_meal = this_moment(all_var);
			all_var->philo[i].nb_philo_ate++;
			pthread_mutex_unlock(&all_var->philo[i].m_nb_ate);
		}
		now = this_moment(all_var);
		pthread_mutex_lock(&all_var->print);
		printf("%lld %d %s\n", now - all_var->start, i + 1, str);
		pthread_mutex_unlock(&all_var->print);
		while (elapsed < (duration) && !le_z(all_var, false))
		{
			usleep(duration * 100);
			elapsed = this_moment(all_var) - now;
			//printf("elapsed: %lld\n", elapsed);
			//printf("duration: %lld\n", duration);
		}
	}
}

static void
	*forking(t_all_var *all_var, int i, bool action, int next)
{
	if (next == all_var->arg.nb_p)
		next = 0;
	while (!le_z(all_var, false))
	{
		if (i % 2)
			pthread_mutex_lock(&all_var->philo[i].fork);
		pthread_mutex_lock(&all_var->philo[next].fork);
		if (i % 2 == 0)
			pthread_mutex_lock(&all_var->philo[i].fork);
		if (all_var->philo[i].fork_available != action
			&& all_var->philo[next].fork_available != action)
		{
			all_var->philo[i].fork_available = action;
			all_var->philo[next].fork_available = action;
			pthread_mutex_unlock(&all_var->philo[i].fork);
			pthread_mutex_unlock(&all_var->philo[next].fork);
			break ;
		}
		pthread_mutex_unlock(&all_var->philo[i].fork);
		pthread_mutex_unlock(&all_var->philo[next].fork);
	}
	return (NULL);
}

static void
	*philosopher(void *var)
{
	t_philo		*philo;
	t_all_var	*all_var;

	philo = (t_philo *)var;
	all_var = philo->all_var;
	while (!le_z(all_var, false))
	{
		activity(all_var, philo->thread_id, "is thinking", 0);
		if (all_var->arg.nb_p == 1)
			break ;
		if (le_z(all_var, false))
			break ;
		forking(all_var, philo->thread_id, false, philo->thread_id + 1);
		activity(all_var, philo->thread_id, "has taken a fork", 0);
		activity(all_var, philo->thread_id, "has taken a fork", 0);
		if (le_z(all_var, false))
			break ;
		activity(all_var, philo->thread_id, "is eating", all_var->arg.te);
		if (le_z(all_var, false))
			break ;
		forking(all_var, philo->thread_id, true, philo->thread_id + 1);
		activity(all_var, philo->thread_id, "is sleeping", all_var->arg.ts);
	}
	return (NULL);
}

static int
	inside_loop_manager(int i, t_all_var *all_var)
{
	pthread_mutex_lock(&all_var->philo[i].m_nb_ate);
	if ((this_moment(all_var) - all_var->philo[i].last_meal)
		> all_var->arg.td && !le_z(all_var, true))
	{
		pthread_mutex_unlock(&all_var->philo[i].m_nb_ate);
		pthread_mutex_lock(&all_var->m_death);
		all_var->death = true;
		pthread_mutex_unlock(&all_var->m_death);
		activity(all_var, i, "died", 0);
		return (-1);
	}
	pthread_mutex_unlock(&all_var->philo[i].m_nb_ate);
	if (all_var->arg.must_eat && min_nb_eat(all_var)
		>= all_var->arg.nb_e && !le_z(all_var, true))
	{
		pthread_mutex_lock(&all_var->m_all_philo_ate);
		all_var->all_philo_ate = true;
		pthread_mutex_unlock(&all_var->m_all_philo_ate);
		return (-1);
	}
	return (0);
}

int
	principal_algo(t_all_var *all_var)
{
	int			i;

	all_var->start = this_moment(all_var);
	i = -1;
	while (++i < all_var->arg.nb_p)
		all_var->philo[i].last_meal = all_var->start;
	i = -1;
	while (++i < all_var->arg.nb_p)
		pthread_create(&all_var->philo[i].pthread, NULL, philosopher,
			&all_var->philo[i]);
	while (!le_z(all_var, false))
	{
		i = -1;
		while (++i < all_var->arg.nb_p)
			if (inside_loop_manager(i, all_var))
				break ;
	}
	i = all_var->arg.nb_p;
	while (--i >= 0)
		pthread_join(all_var->philo[i].pthread, NULL);
	return (0);
}
