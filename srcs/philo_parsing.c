/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grezette <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/21 20:39:41 by grezette          #+#    #+#             */
/*   Updated: 2021/11/21 21:12:10 by grezette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static int
	prs_atoi(char *av, int *nb)
{
	int			minus;
	long int	atoi;

	atoi = 0;
	minus = 1;
	if (*av == '-')
	{
		minus = -1;
		av++;
	}
	while ('0' <= *av && *av <= '9')
	{
		atoi = atoi * 10 + *av++ - '0';
		if (atoi - 1 == INT_MAX && minus == -1 && !*av)
			break ;
		else if (atoi > INT_MAX)
			return (-1);
	}
	if (*av)
		return (-1);
	*nb = atoi * minus;
	return (0);
}

static void
	init_philos(int nb_p, t_all_var *all_var)
{
	int	i;

	i = -1;
	all_var->philo = (t_philo *)malloc(sizeof(t_philo) * nb_p);
	if (!all_var->philo)
		all_var->error = true;
	else
	{
		all_var->death = false;
		all_var->all_philo_ate = false;
		all_var->error = false;
		while (++i < nb_p && !all_var->error)
		{
			pthread_mutex_init(&all_var->philo[i].fork, NULL);
			pthread_mutex_init(&all_var->philo[i].m_nb_ate, NULL);
			all_var->philo[i].thread_id = i;
			all_var->philo[i].nb_philo_ate = 0;
			all_var->philo[i].all_var = all_var;
			all_var->philo[i].last_meal = this_moment(all_var);
		}
	}
}

int	initialize(t_all_var *all_var)
{
	all_var->error = false;
	all_var->philo = NULL;
	all_var->start = this_moment(all_var);
	if (all_var->error)
		return (-1);
	pthread_mutex_init(&all_var->print, NULL);
	pthread_mutex_init(&all_var->m_death, NULL);
	pthread_mutex_init(&all_var->m_all_philo_ate, NULL);
	pthread_mutex_init(&all_var->m_error, NULL);
	init_philos(all_var->arg.nb_p, all_var);
	if (all_var->error)
		return (-1);
	return (0);
}

int
	philo_pars(t_arg *arg, int ac, char **av)
{
	if (ac < 5 || ac > 6)
		return (-1);
	if (prs_atoi(*(++av), &arg->nb_p) || arg->nb_p < 1
		|| prs_atoi(*(++av), &arg->td) || arg->td < 1
		|| prs_atoi(*(++av), &arg->te) || arg->te < 0
		|| prs_atoi(*(++av), &arg->ts) || arg->ts < 0)
		return (-1);
	if (ac == 6)
	{
		if (prs_atoi(*(++av), &arg->nb_e) || arg->nb_e < 0)
			return (-1);
		else
			arg->must_eat = true;
	}
	else
		arg->must_eat = false;
	return (0);
}
