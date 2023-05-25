/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyagiz <eyagiz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 10:21:15 by eyagiz            #+#    #+#             */
/*   Updated: 2023/05/25 11:02:42 by eyagiz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Include.hpp"
#include "../includes/Config.hpp"
#include "../includes/Server.hpp"

Config webserv;


int main(int ac, char **av)
{
	Error err(0);
	if(ac != 2)
		err.setAndPrint(1, "NULL");
	else
	{
		webserv.FileChecker(av[1]);
		webserv.printAll();
		std::cout << "\n\n";
		Server serv(webserv.getConfig());
		serv.run();
		// Cluster cluster(webserv.getConfig());
		// cluster.setup();
	}
}