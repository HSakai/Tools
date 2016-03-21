/*
 * Tokenizer.h
 * 
 * テキストを指定トークン分解する。
 *
 *  Created on: 2012/11/07
 *      Author: H.Sakai
 */
#ifndef TOKENIZER_H_
#define TOKENIZER_H_

#include <string>
#include <vector>
#include <boost/regex.hpp>
#include <memory>
#include "stdio.h"
#include "Token.h"

typedef std::shared_ptr<Token> TokenPtr;

class Tokenizer {
private:
	std::string* _next;
	boost::smatch m_result;
	std::vector<std::string> matchList;
	int count, maxCount;
public:
	Tokenizer(std::string content);
	virtual ~Tokenizer();
	bool hasNext();
	void reset();
	TokenPtr next();
};

#endif /* TOKENIZER_H_ */
