/*
 * Association.hpp
 *
 *  Created on: 13-04-2012
 *      Author: fiedukow
 */

#ifndef ASSOCIATION_HPP_
#define ASSOCIATION_HPP_

typedef int ip_type;
typedef int port_type;


struct HalfAssociation;

/**
 * Structure used as key in main dialogs map.
 */
struct Association
{
	HalfAssociation src;
	HalfAssociation dst;
};

/**
 * Use full structure containing ip & port.
 */
struct HalfAssociation
{
	ip_type 	ip;
	port_type	port;
};


#endif /* ASSOCIATION_HPP_ */
