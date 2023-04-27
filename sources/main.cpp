/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fleizean <fleizean@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 10:21:15 by eyagiz            #+#    #+#             */
/*   Updated: 2023/04/27 19:04:49 by fleizean         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/include.hpp"

WebServer webserv;

int main(int ac, char **av)
{
	Error err(0);
	if(ac != 2)
	{
		err.setAndPrint(1);
		exit(1);
	}
	else
	{
		webserv.FileChecker(av[1]);
	}	
}