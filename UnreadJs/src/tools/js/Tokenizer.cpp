/*
 * Tokenizer.cpp
 *
 *  Created on: 2012/11/07
 *      Author: H.Sakai
 */
#include "Tokenizer.h"
#include <boost/lexical_cast.hpp>

Tokenizer::Tokenizer(std::string content)
{
	_next = NULL;
	
	boost::regex pattern;
	std::string::const_iterator start , end;
	
	boost::regex ptn("//.*?\n|/\\*.*?\\*/|[_\\$a-zA-Z][_\\$\\w]*\\b|[+-]?\\d+|[+-]?\\d+(([.]\\d+)*([eE][+-]?\\d+))?|/((\\\\.)|([^/\n]))*?/[a-zA-Z]*|\"((\\\\.)|([^\"]))*\"|'((\\\\.)|([^']))*'|.");
	pattern = ptn;
	start = content.begin();
	end = content.end();
	
	while (boost::regex_search(start, end, m_result, pattern))
	{
		matchList.push_back(m_result.str(0));
		start = m_result[0].second;
	}
	
	count = 0;
	maxCount = matchList.size();
	next();
}

Tokenizer::~Tokenizer()
{
	delete _next;
}

bool Tokenizer::hasNext()
{
	return _next != NULL;
}

void Tokenizer::reset()
{
	m_result.length(0);
}

TokenPtr Tokenizer::next()
{
	if (maxCount > count)
	{
		_next = new std::string(matchList[count].c_str());
		count++;
	}
	else
	{
		_next = NULL;
	}
	
	TokenPtr t(new Token(((_next != NULL) ? _next->c_str() : "")));
	return t;
}
