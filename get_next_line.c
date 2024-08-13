/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: susamani <susamani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 10:32:56 by susamani          #+#    #+#             */
/*   Updated: 2024/07/19 11:19:16 by susamani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

// La función ft_free está diseñada para liberar la memoria de una cadena
// que fue asignada dinámicamente con malloc. Establece el puntero a NULL
char	*ft_free(char **str)
{
	free(*str);
	*str = NULL;
	return (NULL);
}
//Función:Extrae línea del 'repo' y actualiza el repo con el resto del string

static char	*extract_and_refresh_line(char **repo)
{
	char			*extracted_line;
	char			*newline_position;
	char			*new_repo;
	unsigned int	tail;

	if (!*repo)
		return (NULL);
	newline_position = ft_strchr(*repo, '\n');
	if (newline_position)
	{
		extracted_line = ft_substr(*repo, 0, newline_position - *repo + 1);
		tail = (newline_position - *repo) + 1;
		new_repo = ft_substr(*repo, tail, ft_strlen(newline_position + 1));
		ft_free(repo);
		*repo = new_repo;
	}
	else
	{
		extracted_line = ft_strdup(*repo);
		ft_free(repo);
	}
	return (extracted_line);
}

//Función: será llamada por 'gnl' para leer más datos del archivo
//y los almacena en 'repo'
static char	*read_and_store(int fd, char **repo)
{
	char	*buffer;
	int		bytes_read;

	buffer = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
		return (ft_free(repo));
	bytes_read = read(fd, buffer, BUFFER_SIZE);
	while (bytes_read > 0)
	{
		buffer[bytes_read] = '\0';
		*repo = ft_strjoin(*repo, buffer);
		if (ft_strchr(buffer, '\n'))
			break ;
		bytes_read = read(fd, buffer, BUFFER_SIZE);
	}
	free(buffer);
	if (bytes_read < 0)
		return (ft_free(repo));
	if (*repo && **repo == '\0')
		return (ft_free(repo));
	return (*repo);
}

char	*get_next_line(int fd)
{
	static char	*repo;
	char		*extracted_line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!repo)
		repo = ft_strdup("");
	repo = read_and_store(fd, &repo);
	if (!repo)
		return (NULL);
	extracted_line = extract_and_refresh_line(&repo);
	if (!extracted_line && repo && repo[0] == '\0')
	{
		free(extracted_line);
		return (NULL);
	}
	return (extracted_line);
}

// *Compilar
// gcc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c 
// get_next_line_utils.c 
// -o gnl && ./gnl text.txt

// MAIN NORMAL
// int	main(int argc, char **argv)
// {
// 	int		fd;
// 	char	*test_str;

// 	if (argc != 2)
// 	{
// 		write(1, "Wrong argument count: Must be exactly 2.\n", 41);
// 		return (1);
// 	}
// 	test_str = NULL;
// 	fd = open(argv[1], O_RDONLY);
// 	if (fd == -1)
// 	{
// 		write (1, "Error: Could not open file\n", 27);
// 		return (1);
// 	}
// 	while ((test_str = get_next_line(fd)))
// 	{
// 		printf("Result: %s", test_str);
// 		free(test_str);
// 	}
// 	close (fd);
// 	return (0);
// }

// MAIN PARA ENTRADA POR TECLADO
// int	main(void)
// {
// 	char	*extracted_line;

// 	while ((extracted_line = get_next_line (0)) != NULL)
// 	{
// 		printf("%s", extracted_line);
// 		free(extracted_line);
// 	}
// 	return (0);
// }
