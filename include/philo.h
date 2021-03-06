/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grezette <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/21 20:41:17 by grezette          #+#    #+#             */
/*   Updated: 2021/12/04 20:23:01 by grezette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <limits.h>
# include <sys/time.h>
# include <stdbool.h>

typedef struct s_arg
{
	int			nb_p;
	int			td;
	int			te;
	int			ts;
	bool		must_eat;
	int			nb_e;
}				t_arg;

typedef struct s_philo
{
	int				thread_id;
	pthread_mutex_t	m_nb_ate;
	int				nb_philo_ate;
	pthread_t		pthread;
	void			*var;
	pthread_mutex_t	fork;
	bool			fork_available;
	long long int	last_meal;
}					t_philo;

typedef struct s_all_var
{
	t_arg			arg;
	long long int	start;
	pthread_mutex_t	m_death;
	pthread_mutex_t	m_all_philo_ate;
	pthread_mutex_t	m_error;
	bool			death;
	bool			all_philo_ate;
	bool			error;
	t_philo			*philo;
	pthread_mutex_t	print;
}					t_all_var;

/*Init and parsing*/

int				philo_pars(t_arg *arg, int ac, char **av);
int				initialize(t_all_var *all_var);

/*Main Algo*/

void			principal_algo(t_all_var *all_var, int i);

/*Utils*/

int				min_nb_eat(t_all_var *all_var);
bool			le_z(t_all_var *all_var, bool just_error);
long long int	this_moment(t_all_var *all_var);

#endif
