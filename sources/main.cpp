/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyagiz <eyagiz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 10:21:15 by eyagiz            #+#    #+#             */
/*   Updated: 2023/05/11 18:49:19 by eyagiz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/include.hpp"
#include "../includes/Config.hpp"
#include "../includes/Cluster.hpp"

Config webserv;

int main(int ac, char **av)
{
	Error err(0);
	if(ac != 2)
		err.setAndPrint(1, "NULL");
	else
	{
		webserv.FileChecker(av[1]);
		// webserv.printAll();
		Cluster cluster(webserv.getConfig());
		cluster.setup();
	}
}