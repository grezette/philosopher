#include "../include/philo.h"

static int
	philo_free(t_all_var *all_var)
{
	int	i;

	i = -1;
	if (all_var->philo)
		while (++i < all_var->arg.nb_p)
			pthread_mutex_destroy(&all_var->philo[i].fork);
	pthread_mutex_destroy(&all_var->print);
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
		all_var->error = true;
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
		if (min > all_var->philo[i].nb_philo_ate)
			min = all_var->philo[i].nb_philo_ate;
	return (min);
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
	if (principal_algo(&all_var))
		return (philo_free(&all_var));
	philo_free(&all_var);
	return (0);
}
