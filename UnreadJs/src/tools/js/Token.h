/*
 * Token.h
 * 
 * テキストのトークン分割するためのトークの基本情報オブジェクト。
 *
 *  Created on: 2012/11/07
 *      Author: H.Sakai
 */
#ifndef TOKEN_H_
#define TOKEN_H_

#include "stdio.h"
#include <string>
#include <vector>
#include <memory>

class Token {
private:
	std::string token;
	std::string type;
	static std::string _IDENT;
	static std::string _WHITESPACE;
	static std::string _COMMENT;
	static std::string _CONDITIONAL_COMMENT;
	static std::string _DOCCOMMENT;
	static std::string _FLOAT;
	static std::string _INTEGER;
	static std::string _SYMBOL;
	static std::string _QUOTED;
	static std::string _REGEX;
	static std::string _FUNCTION;
	static std::string _DOT;
	static std::string _VAR;
	static std::string _LP;
	static std::string _RP;
	static std::string _RC;
	static std::string _LC;
	static std::string _KEYWORD;
	static std::string _reComment;
	static std::string _reIdent;
	static std::string _reInt;
	static std::string _reFloat;
	static std::string _reSingleQuoted;
	static std::string _reDoubleQuoted;
	static std::string _reRegex;
	static std::vector<std::string> _keywords;
public:
	typedef std::shared_ptr<Token> TokenPtr;
	Token(std::string s);
	virtual ~Token();
	std::string getToken();
	void setToken(const std::string s);
	std::string getType();
	std::string getTokenType(std::string s);
	std::string toString();
	static void writeTokensToFile(std::vector<TokenPtr> *textList, std::string filePath);
	static std::string IDENT();
	static std::string WHITESPACE();
	static std::string COMMENT();
	static std::string CONDITIONAL_COMMENT();
	static std::string DOCCOMMENT();
	static std::string FLOAT();
	static std::string INTEGER();
	static std::string SYMBOL();
	static std::string QUOTED();
	static std::string REGEX();
	static std::string FUNCTION();
	static std::string DOT();
	static std::string VAR();
	static std::string LP();
	static std::string RP();
	static std::string RC();
	static std::string LC();
	static std::string KEYWORD();
	static std::string reComment();
	static std::string reIdent();
	static std::string reInt();
	static std::string reFloat();
	static std::string reSingleQuoted();
	static std::string reDoubleQuoted();
	static std::string reRegex();
	static std::vector<std::string> keywords();
};

#endif /* TOKEN_H_ */
