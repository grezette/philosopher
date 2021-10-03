#include "../include/philo.h"

int	main(int ac, char **av)
{
	t_philo phil;

	if (philo_pars(&phil, ac, av))
		return (write(1, "Error during parsing\n", 21) + philo_free(phil));
	return (0);
}
