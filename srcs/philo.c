/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grezette <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/21 20:39:29 by grezette          #+#    #+#             */
/*   Updated: 2021/12/04 16:15:54 by grezette         ###   ########.fr       */
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

static int
	philo_free(t_all_var *all_var)
{
	int	i;

	i = -1;
	if (all_var->philo)
	{
		while (++i < all_var->arg.nb_p)
		{
			pthread_mutex_destroy(&all_var->philo[i].fork);
			pthread_mutex_destroy(&all_var->philo[i].m_nb_ate);
		}
	}
	pthread_mutex_destroy(&all_var->print);
	pthread_mutex_destroy(&all_var->m_death);
	pthread_mutex_destroy(&all_var->m_all_philo_ate);
	pthread_mutex_destroy(&all_var->m_error);
	free(all_var->philo);
	return (0);
}

long long int
	this_moment(t_all_var *all_var)
{
	long long int	time;
	struct timeval	now;

	if (gettimeofday(&now, NULL))
	{
		pthread_mutex_lock(&all_var->m_error);
		all_var->error = true;
		pthread_mutex_unlock(&all_var->m_error);
		return (-1);
	}
	else
		time = now.tv_sec * 1000 + now.tv_usec / 1000;
	return (time);
}

int
	min_nb_eat(t_all_var *all_var)
{
	int	i;
	int	min;

	i = -1;
	min = all_var->arg.nb_e;
	while (++i < all_var->arg.nb_p)
	{
		pthread_mutex_lock(&all_var->philo[i].m_nb_ate);
		if (min > all_var->philo[i].nb_philo_ate)
			min = all_var->philo[i].nb_philo_ate;
		pthread_mutex_unlock(&all_var->philo[i].m_nb_ate);
	}
	return (min);
}

void __attribute__((destructor)) lol() {
	while (1) {};
}

int
	main(int ac, char **av)
{
	t_all_var	all_var;

	if (philo_pars(&(all_var.arg), ac, av))
		return (write(1, "Error during parsing\n", 21));
	if (all_var.arg.must_eat && !all_var.arg.nb_e)
		return (0);
	if (initialize(&all_var))
		return (philo_free(&all_var));
	principal_algo(&all_var);
	philo_free(&all_var);
	return (0);
}
