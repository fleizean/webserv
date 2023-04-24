/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyagiz <eyagiz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 10:21:15 by eyagiz            #+#    #+#             */
/*   Updated: 2023/04/24 10:29:29 by eyagiz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/webserv.hpp"

int main(int ac, char **av)
{
    if(ac != 2)
    {
        std::cerr << "Error: Config file not found." << std::endl;
        return -1;
    }
    else
    {
        std::ifstream file(av[1]);
        
        if(!file.is_open())
        {
            std::cerr << "Error: Could not open config file." << std::endl;
            return -1;
        }
        file.close();
    }
}