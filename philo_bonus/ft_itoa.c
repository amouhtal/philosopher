/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amouhtal <amouhtal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/30 14:14:42 by amouhtal          #+#    #+#             */
/*   Updated: 2021/07/12 16:22:10 by amouhtal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

size_t	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*news;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (s1 == NULL)
		return (0);
	news = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2)) + 1);
	if (!news)
		return (0);
	while (s1[i])
	{
		news[i] = s1[i];
		i++;
	}
	while (s2[j])
	{
		news[i] = s2[j];
		j++;
		i++;
	}
	news[i] = '\0';
	return (news);
}

static	int	count_char(long n)
{
	int	count;

	count = 0;
	if (n < 0)
	{
		n = n * -1;
		count++;
	}
	while (n > 9)
	{
		n = n / 10;
		count++;
	}
	count++;
	return (count);
}

static	char	*resume(long l_n, char *char_int)
{
	int	count;
	int	i;

	i = 0;
	count = count_char(l_n);
	if (!char_int)
		return (NULL);
	if (l_n < 0)
	{
		char_int[i] = '-';
		l_n = l_n * -1;
		i++;
	}
	char_int[count] = '\0';
	count--;
	while (l_n > 9)
	{
		char_int[count] = l_n % 10 + 48;
		l_n = l_n / 10;
		count--;
	}
	char_int[count] = l_n % 10 + 48;
	return (char_int);
}

char	*ft_itoa(int n)
{
	char	*char_int;
	char	*ret;

	char_int = malloc(sizeof(char) * count_char((long)n) + 1);
	ret = resume((long)n, char_int);
	return (ret);
}
