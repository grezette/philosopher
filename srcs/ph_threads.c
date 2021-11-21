#include "../include/philo.h"

void
	activity(t_all_var *all_var, int i, char *str, long long int duration)
{
	long long int	now;
	long long int	elapsed;

	elapsed = 0;
	now = this_moment(all_var);
	if (!all_var->error && !all_var->death && !all_var->all_philo_ate)
	{
		pthread_mutex_lock(&all_var->print);
		printf("%lld %d %s\n", now - all_var->start, i + 1, str);
		pthread_mutex_unlock(&all_var->print);
		while (elapsed < duration && !all_var->error)
		{
			usleep(duration / 10);
			elapsed = this_moment(all_var) - now;
		}
		if (!all_var->error && str[3] == 'e')
		{
			all_var->philo[i].last_meal = this_moment(all_var);
			all_var->philo[i].nb_philo_ate++;
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
		pthread_mutex_lock(&all_var->philo[i].fork);
		activity(all_var, i, "has taken a fork", 0);
		pthread_mutex_lock(&all_var->philo[next].fork);
		activity(all_var, i, "has taken a fork", 0);
	}
	else if (action == SLEEP)
	{
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
	while (!all_var->death && !all_var->all_philo_ate && !all_var->error)
	{
		activity(all_var, philo->thread_id, "is thinking", 0);
		if (all_var->arg.nb_p == 1)
			break ;
		if (all_var->death || all_var->all_philo_ate || all_var->error)
			break ;
		forking(all_var, philo->thread_id, TAKE_FORK, true);
		if (all_var->death || all_var->all_philo_ate || all_var->error)
			return (forking(all_var, philo->thread_id, SLEEP, false));
		activity(all_var, philo->thread_id, "is eating", all_var->arg.te);
		if (all_var->death || all_var->all_philo_ate || all_var->error)
			return (forking(all_var, philo->thread_id, SLEEP, false));
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
	while (!all_var->death && !all_var->all_philo_ate && !all_var->error)
	{
		i = -1;
		while (++i < all_var->arg.nb_p)
		{
			if ((this_moment(all_var) - all_var->philo[i].last_meal)
				> all_var->arg.td && !all_var->error)
			{
				activity(all_var, i, "died", 0);
				all_var->death = true;
				break ;
			}
			else if (all_var->arg.must_eat && min_nb_eat(all_var)
				>= all_var->arg.nb_e && !all_var->error)
			{
				all_var->all_philo_ate = true;
				break ;
			}
		}
	}
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
	i = all_var->arg.nb_p;
	while (--i >= 0)
		pthread_join(all_var->philo[i].pthread, NULL);
	return (0);
}
