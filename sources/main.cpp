/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyagiz <eyagiz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 10:21:15 by eyagiz            #+#    #+#             */
/*   Updated: 2023/05/30 14:07:13 by eyagiz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Include.hpp"
#include "../includes/Config.hpp"
#include "../includes/Server.hpp"

Config webserv;


int main(int ac, char **av, char **env)
{
	Error err(0);
	if(ac != 2)
		err.setAndPrint(1, "NULL");
	else
	{
		webserv.FileChecker(av[1]);
		webserv.printAll();
		std::cout << "\n\n";
		Server serv(webserv.getConfig(), env);
		serv.run();
	}
}