/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ph_threads.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grezette <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/21 20:39:21 by grezette          #+#    #+#             */
/*   Updated: 2021/11/27 17:52:19 by grezette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

bool
	le_z(t_all_var *all_var, bool just_error)
{
	if (!just_error)
	{
		pthread_mutex_lock(&all_var->m_death);
		if (all_var->death)
		{
			pthread_mutex_unlock(&all_var->m_death);
			return (true);
		}
		pthread_mutex_unlock(&all_var->m_death);
		pthread_mutex_lock(&all_var->m_all_philo_ate);
		if (all_var->all_philo_ate)
		{
			pthread_mutex_unlock(&all_var->m_all_philo_ate);
			return (true);
		}
		pthread_mutex_unlock(&all_var->m_all_philo_ate);
	}
	pthread_mutex_lock(&all_var->m_error);
	if (all_var->error)
	{
		pthread_mutex_unlock(&all_var->m_error);
		return (true);
	}
	pthread_mutex_unlock(&all_var->m_error);
	return (false);
}

void
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
		pthread_mutex_lock(&all_var->print);
		now = this_moment(all_var);
		printf("%lld %d %s\n", now - all_var->start, i + 1, str);
		pthread_mutex_unlock(&all_var->print);
		while (elapsed < duration && !le_z(all_var, true))
		{
			usleep(duration / 50);
			elapsed = this_moment(all_var) - now;
		}
	}
}

void *
	forking(t_all_var *all_var, int i, int action, bool not_end)
{
	int	next;

	next = i + 1;
	if (i + 1 == all_var->arg.nb_p)
		next = 0;
	if (action == TAKE_FORK)
	{
		while (!le_z(all_var, false))
		{
			if (i % 2)
				pthread_mutex_lock(&all_var->philo[i].fork);
			pthread_mutex_lock(&all_var->philo[next].fork);
			if (i % 2 == 0)
				pthread_mutex_lock(&all_var->philo[i].fork);
			if (all_var->philo[i].fork_available && all_var->philo[next].fork_available)
			{
				all_var->philo[i].fork_available = false;
				all_var->philo[next].fork_available = false;
				pthread_mutex_unlock(&all_var->philo[i].fork);
				pthread_mutex_unlock(&all_var->philo[next].fork);
				break ;
			}
			pthread_mutex_unlock(&all_var->philo[i].fork);
			pthread_mutex_unlock(&all_var->philo[next].fork);
		}
		if (le_z(all_var, false))
			return (NULL);
		activity(all_var, i, "has taken a fork", 0);
		activity(all_var, i, "has taken a fork", 0);
	}
	else if (action == SLEEP)
	{
		if (i % 2)
			pthread_mutex_lock(&all_var->philo[i].fork);
		pthread_mutex_lock(&all_var->philo[next].fork);
		if (i % 2 == 0)
			pthread_mutex_lock(&all_var->philo[i].fork);
		all_var->philo[i].fork_available = true;
		all_var->philo[next].fork_available = true;
		pthread_mutex_unlock(&all_var->philo[i].fork);
		pthread_mutex_unlock(&all_var->philo[next].fork);
		if (not_end)
			activity(all_var, i, "is sleeping", all_var->arg.ts);
	}
	return (NULL);
}

void
	*philosopher(void *var)
{
	t_philo		*philo;
	t_all_var	*all_var;

	philo = (t_philo *)var;
	all_var = philo->all_var;
//pthread_mutex_lock(&all_var->print);
//printf("philo %d : %lld\nDiff = %lld\n\n", philo->thread_id, philo->last_meal, philo->last_meal - all_var->start);
//pthread_mutex_unlock(&all_var->print);
	while (!le_z(all_var, false))
	{
		activity(all_var, philo->thread_id, "is thinking", 0);
		if (all_var->arg.nb_p == 1)
			break ;
		if (le_z(all_var, false))
			break ;
		forking(all_var, philo->thread_id, TAKE_FORK, true);
		if (le_z(all_var, false))
			break ;
		activity(all_var, philo->thread_id, "is eating", all_var->arg.te);
		if (le_z(all_var, false))
			break ;
		forking(all_var, philo->thread_id, SLEEP, true);
	}
	return (NULL);
}

static void
	*thread_manager(void *var)
{
	t_all_var	*all_var;
	int			i;

	all_var = (t_all_var *)var;
//pthread_mutex_lock(&all_var->print);
//printf("start : %lld\n\n", all_var->start);
//pthread_mutex_unlock(&all_var->print);
	while (!le_z(all_var, false))
	{
		i = -1;
		while (++i < all_var->arg.nb_p)
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
				break ;
			}
			pthread_mutex_unlock(&all_var->philo[i].m_nb_ate);
			if (all_var->arg.must_eat && min_nb_eat(all_var)
				>= all_var->arg.nb_e && !le_z(all_var, true))
			{
				pthread_mutex_lock(&all_var->m_all_philo_ate);
				all_var->all_philo_ate = true;
				pthread_mutex_unlock(&all_var->m_all_philo_ate);
				break ;
			}
		}
	}
	i = all_var->arg.nb_p;
	while (--i >= 0)
		pthread_join(all_var->philo[i].pthread, NULL);
	return (NULL);
}

int
	principal_algo(t_all_var *all_var)
{
	int			i;
	pthread_t	pthread;

	i = -1;
	while (++i < all_var->arg.nb_p)
		pthread_create(&all_var->philo[i].pthread, NULL, philosopher,
			&all_var->philo[i]);
	pthread_create(&pthread, NULL, thread_manager, all_var);
	pthread_join(pthread, NULL);
	return (0);
}
