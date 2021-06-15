/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-mak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/21 11:42:52 by abel-mak          #+#    #+#             */
/*   Updated: 2021/05/31 16:56:01 by abel-mak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** msb : most significant bit
*/
#include <stdlib.h>

size_t	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i] != 0)
		i++;
	return (i);
}

static int	msb(int n)
{
	int	i;

	i = 0;
	if (n == 0)
		return (1);
	while (n != 0)
	{
		n = n / 10;
		i++;
	}
	return (i);
}

static void	rev(char *str)
{
	int		start;
	int		end;
	char	tmp;

	end = ft_strlen(str) - 1;
	start = 0;
	while (start < end)
	{
		tmp = str[start];
		str[start] = str[end];
		str[end] = tmp;
		start++;
		end--;
	}
}

static char	*fill(int n, int isnegative, int sign)
{
	int		reslen;
	int		i;
	char	*result;

	i = 0;
	reslen = msb(n);
	result = (char *)malloc(sizeof(char) * (reslen + isnegative + 1));
	if (result == NULL)
		return (NULL);
	while (i < reslen)
	{
		result[i] = ((n % 10) * sign) + '0';
		n = n / 10;
		i++;
	}
	if (isnegative == 1)
	{
		result[i] = '-';
		result[i + 1] = '\0';
	}
	else
		result[i] = '\0';
	return (result);
}

char	*ft_itoa(int n)
{
	char	*result;
	int		isnegative;
	int		sign;

	sign = 1;
	isnegative = 0;
	if (n < 0)
	{
		sign = -1;
		isnegative = 1;
	}
	result = fill(n, isnegative, sign);
	if (result == NULL)
		return (NULL);
	rev(result);
	return (result);
}
