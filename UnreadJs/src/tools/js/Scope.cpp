/*
 * Scope.cpp
 *
 *  Created on: 2012/11/08
 *      Author: H.Sakai
 */
#include "Scope.h"
#include "../../utils/Util.h"

Scope::Scope():previous(0) {
	std::map<std::string, std::string> map;
	mapping = new std::map<std::string, std::string>();
}

Scope::Scope( Scope* prev )
{
	mapping = new std::map<std::string, std::string>();
	previous = prev;
}

Scope::~Scope()
{
	if (previous != NULL)
	{
		delete previous;
	}
}

void Scope::addMapping(std::string src, std::string dest)
{
	mapping->insert( std::map<std::string, std::string>::value_type(src, dest) );
}

std::string Scope::getMapping(std::string src)
{
	std::map<std::string, std::string>::iterator itr;
	itr = mapping->find(src);
	std::string dest;
	
	if (!mapping->empty()
			&& itr != mapping->end())
	{
		dest = (*itr).second;
	}
	
	if (dest != EmptyStr)
	{
		return dest;
	}
	
	if (previous != NULL)
	{
		return previous->getMapping(src);
	}

	return EmptyStr;
}

Scope* Scope::getNext(Scope* thisInst)
{
	Scope* next = new Scope(thisInst);
	return next;
}

Scope* Scope::getPrevious()
{
	return previous;
}
