/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fleizean <fleizean@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 10:21:15 by eyagiz            #+#    #+#             */
/*   Updated: 2023/09/19 15:13:35 by fleizean         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Include.hpp"
#include "../includes/Config.hpp"
#include "../includes/Server.hpp"
#include <signal.h>

Config webserv;

// soruna bakılacak freede hala sıkıntı var büyük ihtimalle getconfig yüzünden direkt olarak serversı publice alabiliriz
void	exitFree(int signal/* , siginfo_t *siginfo, void *unused */)
{
/* 	(void)unused;
 */	(void)signal;
/* 	(void)siginfo;
 */
	for (std::vector<ServerMembers*>::iterator it = webserv.getConfig().begin(); it != webserv.getConfig().end(); ++it)
	{
	    std::vector<Location*>& locations = (*it)->getLocations();
	
	    // İkinci döngü: Location nesnelerini temizle
	    for (std::vector<Location*>::iterator locIt = locations.begin(); locIt != locations.end(); ++locIt)
	    {
	        delete (*locIt);
	    }
	    locations.clear();
	
	    // ServerMembers nesnesini temizle
	    delete (*it);
	}
	
	webserv.getConfig().clear();
	system("leaks webserv");
	exit(0);
}


/**
 * @brief 
 * struct sigaction act bizim sinyal işleme verilerimizi tutacak bir değişken
 * 
 * sigaction(SIGINT, &act, NULL); sinyali yakalamak için kullanılır.
 * SIGINT = CTRL+C
 * &act parametresi bizim oluşturduğumuz sinyal değişkenin verilerini almasını sağlar. Oradan hangi fonksiyona gideceğini
 * -- vesaire söylemiş oluruz.
 * 
 */
int main(int ac, char **av)
{
	Error err(0);
	struct sigaction	act; 

/* 	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = exitFree;*/
	
	act.sa_handler = exitFree;
 	if (ac != 2)
		err.setAndPrint(1, "NULL");
	else
	{
		webserv.FileChecker(av[1]);
		webserv.printAll();
		std::cout << "\n\n";
		Server serv(webserv.getConfig());
		sigaction(SIGINT, &act, NULL);
		serv.run();
	}
}