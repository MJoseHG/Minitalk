/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahernan <mahernan@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 11:10:57 by mahernan          #+#    #+#             */
/*   Updated: 2022/08/10 11:11:00 by mahernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <math.h>
#include <signal.h>
#include <stdio.h>

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_RESET "\x1b[0m"

#include <stdlib.h>
#include <unistd.h>

void	ft_putstr_color_fd(char *color, char *s, int fd)
{
	ft_putstr_fd(color, fd);
	ft_putstr_fd(s, fd);
	ft_putstr_fd(ANSI_COLOR_RESET, fd);
}

// int	ft_str_isnumeric(char *str)
// {
// 	int	i;

// 	i = 0;
// 	if (str[i] == '-')
// 		i++;
// 	while (str[i])
// 	{
// 		if (!ft_isdigit(str[i]))
// 			return (0);
// 		i++;
// 	}
// 	return (1);
// }

// void	error_client(char *str)
// {
// 	if (str)
// 		free(str);
// 	ft_putstr_color_fd(ANSI_COLOR_RED,
// 		"Client: Unexpected error.\n", 2);
// 	exit(EXIT_FAILURE);
// }

// int	send_null(int pid, char *str)
// {
// 	static int	i = 0;

// 	if (i++ != 8)
// 	{
// 		if (kill(pid, SIGUSR1) == -1)
// 			error_client(str);
// 		return (0);
// 	}
// 	return (1);
// }

// int	send_bit(int pid, char *str)
// {
// 	static char	*message = 0;
// 	static int	s_pid = 0;
// 	static int	bits = -1;

// 	if (str)
// 		message = ft_strdup(str);
// 	if (!message)
// 		error_client(0);
// 	if (pid)
// 		s_pid = pid;
// 	if (message[++bits / 8])
// 	{
// 		if (message[bits / 8] & (0x80 >> (bits % 8)))
// 		{
// 			if (kill(s_pid, SIGUSR2) == -1)
// 				error_client(message);
// 		}
// 		else if (kill(s_pid, SIGUSR1) == -1)
// 			error_client(message);
// 		return (0);
// 	}
// 	if (!send_null(s_pid, message))
// 		return (0);
// 	free(message);
// 	return (1);
// }

static void	send_byte(int pid, char *message)
{
	int		cnt;
	char	byte;
	while (*message)
	{
		cnt = 7;
		byte = *message;
		while (cnt >= 0)
		{
			if (byte >> cnt & 1)
				kill(pid, SIGUSR2);
			else
				kill(pid,SIGUSR1);
			cnt--;
			usleep(100);
		}
		message++;
	}
}

void	error_client(int pid, char *message)
{
	if (!pid || !*message)
	{
		ft_putstr_color_fd(ANSI_COLOR_RED,
							"Client: Unexpected error.\n",
							2);
		exit(EXIT_FAILURE);
	}
	else if (pid == -1)
	{
		ft_putstr_color_fd(ANSI_COLOR_RED,
							"Client: Unexpected error.\n",
							2);
		exit(EXIT_FAILURE);
	}
}

int	main(int argc, char **argv)
{
	int		pid;
	char	*message;

	if (argc != 3)
	{
		ft_putstr_color_fd(ANSI_COLOR_RED,
							"Client: Invalid arguments.\n",
							2);
		ft_putstr_color_fd(ANSI_COLOR_YELLOW,
							"Correct format: [./client <PID> <STR>].\n",
							2);
		exit(EXIT_FAILURE);
	}
	pid = ft_atoi(argv[1]);
	message = argv[2];
	error_client(pid, message);
	send_byte(pid, message);
	return (0);
}