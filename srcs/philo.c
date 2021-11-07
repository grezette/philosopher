#include "../include/philo.h"

void	print_args(t_philo philo)
{
	printf("%d | %d | %d | %d | %d | %d", philo.nb_p, philo.td, philo.te, philo.ts, philo.must_eat, philo.nb_e);
}

int	main(int ac, char **av)
{
	t_philo phil;

	if (philo_pars(&phil, ac, av))
		return (write(1, "Error during parsing\n", 21) + philo_free(phil));
	if (phil.must_eat && !philo.nb_e)
		return (0);
	return (0);
}
