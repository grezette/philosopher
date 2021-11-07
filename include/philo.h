#ifndef PHILO_H
#define PHILO_H

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>
#include <stdbool.h>

typedef struct	s_philo
{
	int			nb_p;
	int			td;
	int			te;
	int			ts;
	bool		must_eat;
	int			nb_e;
}				t_philo;

int	philo_pars(t_philo *philo, int ac, char **av);
int	philo_free(t_philo philo);

#endif
