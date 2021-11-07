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

int
	philo_pars(t_philo *philo, int ac, char **av)
{
	if (ac < 5 || ac > 6)
		return (-1);
	if (prs_atoi(*(++av), &philo->nb_p) || prs_atoi(*(++av), &philo->td)
			|| prs_atoi(*(++av), &philo->te) || prs_atoi(*(++av), &philo->ts))
		return (-1);
	if (ac == 6)
	{
		if (prs_atoi(*(++av), &philo->nb_e) || philo->nb_e < 0)
			return (-1);
		else
			philo->must_eat = true;
	}
	else
		philo->must_eat = false;
	return (0);
}
