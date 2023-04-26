/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyagiz <eyagiz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 10:21:15 by eyagiz            #+#    #+#             */
/*   Updated: 2023/04/26 17:10:39 by eyagiz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/webserv.hpp"
#include "../includes/error.hpp"

int main(int ac, char **av)
{
	Error err(0);
	if(ac != 2)
	{
		err.setAndPrint(1);
		return -1;
	}
	else
	{
		std::ifstream file(av[1]);
		if(!file.is_open())
		{
			err.setAndPrint(2);
			return -1;
		}
		file.close();
	}
	Webserv webserv((ac == 1) ? "" : av[1]); // Webserv ac 1 ise boş bırak değilse conf dosyasını default consa atacak
	
	
}