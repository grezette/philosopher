#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <limits.h>
# include <sys/time.h>
# include <stdbool.h>

enum e_philo_state
{
	TAKE_FORK,
	SLEEP
};

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
	int				nb_philo_ate;
	pthread_t		pthread;
	void			*all_var;
	pthread_mutex_t	fork;
	long long int	last_meal;
}					t_philo;

typedef struct s_all_var
{
	t_arg			arg;
	long long int	start;
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

int				principal_algo(t_all_var *all_var);

/*Utils*/

int				min_nb_eat(t_all_var *all_var);
long long int	this_moment(t_all_var *all_var);

#endif
