/*
 * JsCompressor.cpp
 *
 *  Created on: 2012/11/06
 *      Author: H.Sakai
 */
#include "stdio.h"
#include "JsCompressor.h"
#include "../utils/Util.h"
#include "js/Scope.h"
#include <iostream>
#include <list>
#include <algorithm>
#include <stack>
#include <boost/shared_ptr.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>

#define CHARS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ$"
#define CHARS_SECOND "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ$0123456789"

typedef boost::shared_ptr<Scope> ScopePtr;

std::vector<TokenPtr> JsCompressor::filterWhitespace(TokenizerPtr tokenizer)
{
	std::vector<TokenPtr> filterList;
	
	TokenPtr token;
	token = tokenizer->next();
	
	TokenPtr token2;
	
	while (true)
	{
		if (!token)
		{
			break;
		}
		
		if (filterList.size() > 0)
		{
			token2 = filterList[filterList.size() - 1];
		}
		
		TokenPtr token1;
		token1 = token;
		
		if (tokenizer->hasNext())
		{
			token = tokenizer->next();
		}
		else
		{
			token.reset();
		}
		
		if (token1)
		{
			if (token1->getType() == "WHITESPACE" || token1->getType() == "COMMENT" || token1->getType() == "DOCCOMMENT")
			{
				if (token && token2)
				{
					std::string s = token->getType();
					std::string s1 = token2->getType();
					
					if(s == "IDENT" || s == "FUNCTION" || s == "VAR" || s == "KEYWORD"
							|| s == "INTEGER" || s == "FLOAT" || s == "REGEX" || s == "QUOTED")
					{
						if(s1 == "IDENT" || s1 == "FUNCTION" || s1 == "VAR" || s1 == "KEYWORD"
								|| s1 == "INTEGER" || s1 == "FLOAT" || s1 == "REGEX" || s1 == "QUOTED")
						{
							TokenPtr blank(new Token(" "));
							filterList.push_back(blank);
						}
						else
						{
							if(token2->getType() == "RC" || token2->getToken() == "]")
							{
								TokenPtr newline(new Token("\n"));
								filterList.push_back(newline);
							}
						}
					}
				}
			}
			else
			{
				filterList.push_back(token1);
			}
		}
	}
	
	return filterList;
}

void JsCompressor::getPublicIdents(std::vector<TokenPtr> list, IdentMap resultMap)
{
	std::vector<TokenPtr>::iterator itr = list.begin();
	while(itr != list.end())
	{
		TokenPtr token = *itr;
		std::string type = token->getType();
		if (type == "IDENT")
		{
			resultMap->insert(std::map<std::string, TokenPtr>::value_type(type, token));
		}
		
		++itr;
	}
}

void JsCompressor::renamePrivateIdents(std::vector<TokenPtr> list, IdentMap map, std::string s)
{
	std::map<std::string, std::string> tmp;
	std::vector<TokenPtr>::iterator itr = list.begin();
	while(itr != list.end())
	{
		TokenPtr token = *itr;
		std::string type = token->getType();
		std::string tokenText = token->getToken();
		if (type == "IDENT" && boost::starts_with(tokenText, s))
		{
			std::map<std::string, std::string>::iterator itr;
			itr = tmp.find(tokenText);
			if (itr != tmp.end())
			{
				token->setToken(tmp[tokenText]);
			}
			else
			{
				std::string s1;
				s1 = generateIdent(map, false);
				tmp[tokenText] = s1;
				token->setToken(s1);
			}
		}
		
		++itr;
	}
}

std::string JsCompressor::generateIdent(IdentMap map, bool flag)
{
	std::string ident;
	std::map<std::string, TokenPtr>::iterator itr;
	std::vector<std::string> keywords;
	keywords = Token::keywords();
	std::vector<std::string>::iterator keyItr;
	
	ident = "";
	do
	{
		std::string if1;
		if1 = boost::lexical_cast<std::string>(( keyItr != Token::keywords().end() ));
		ident = generateIdent(flag);
	} while ((itr = map->find(ident)) != map->end()
			|| (keyItr = find(keywords.begin(), keywords.end(), ident)) != keywords.end());
	
	return ident;
}

std::string JsCompressor::generateIdent(bool flag)
{
	int i;
	i = state;
	std::string sb( compress_prefix != "" && !flag ? compress_prefix : "" );
	std::string chars = CHARS;
	std::string charsSecond = CHARS_SECOND;
	char c, c1;
	std::string char_s, char_s1;
	
	c = chars.at(i % chars.length());
	char_s = boost::lexical_cast<std::string>(c);
	sb.append(char_s);
	
	i /= chars.length();
	
	do
	{
		c1 = charsSecond.at(i % charsSecond.length());
		char_s1 = boost::lexical_cast<std::string>(c1);
		sb.append(char_s1);
		i /= charsSecond.length();
	} while (i > 1);
	
	state++;
	
	return sb;
}

void JsCompressor::renameLocalIdents(std::vector<TokenPtr> list, IdentMap map)
{
	if (list.size() == 0)
	{
		return;
	}
	
	TokenPtr token, token1, token2, token3;
	std::stack<int> stack;
	Scope* scope = new Scope();
	bool flag, flag1, flag2;
	flag = flag1 = flag2 = false;
	int i;
	i = 0;
	
	std::vector<TokenPtr>::iterator itr;
	itr = list.begin();
	token = *itr;
	++itr;
	
	do
	{
		if (!token)
		{
			break;
		}
		
		token3 = token2;
		token2 = token1;
		token1 = token;
		
		if (itr != list.end())
		{
			token = *itr;
		}
		else
		{
			token.reset();
		}
		
		if (token1)
		{
			if (token1->getToken() == "=")
			{
				flag2 = false;
			}
			if (token1->getType() == "LC")
			{
				i++;
			}
			if (token1->getType() == "RC")
			{
				if (stack.size() > 0)
				{
					int j;
					j = stack.top();
					std::string char_s1 = boost::lexical_cast<std::string>(i);
					std::string char_s2 = boost::lexical_cast<std::string>(j);
					if (j == i)
					{
						stack.pop();
						scope = scope->getPrevious();
					}
				}
				i--;
			}
			
			if (token1->getType() == "FUNCTION" && ( !token3 || token3->getToken() != "new" ))
			{
				flag = true;
				scope = scope->getNext(scope);
				stack.push(i + 1);
			}
			if (token1->getType() == "VAR")
			{
				flag2 = true;
			}
			if (token1->getType() == "LP" && flag)
			{
				flag1 = true;
				flag = false;
			}
			if (token1->getType() == "RP" && flag1)
			{
				flag1 = false;
			}
			if(token1->getType() == "IDENT" && (!token2 || token2->getType() != "DOT") && (!token || token->getToken() != ":" || token2->getToken() == "?"))
			{
				std::string s = scope->getMapping(token1->getToken());
				if(s != "")
				{
					token1->setToken(s);
				}
				else
				{
					if((flag1 || flag2 && stack.size() > 0) && token1->getToken().length() > 2)
					{
						std::string s1 = generateIdent(map, true);
						scope->addMapping(token1->getToken(), s1);
						token1->setToken(s1);
					}
				}
				flag2 = false;
			}
			if(token1->getType() == "KEYWORD")
			{
				flag2 = false;
			}
			
			++itr;
		}
	} while (true);
	
	delete scope;
}

JsCompressor::JsCompressor()
{
	state = 0;
}

JsCompressor::~JsCompressor()
{
}

void JsCompressor::execute(std::string inFile, std::string outFile, std::string prefix)
{
	// 拡張子チェック
	std::string extension;
	if (inFile.length() < 4)
	{
		puts("[ERROR] Input File is too short FileName!!");
		return;
	}
	
	boost::regex pattern(".*.js");
	if(!boost::regex_match(inFile, pattern))
	{
		puts("[ERROR] Input File is not JavaScript File!!");
		return;
	}
	
	std::string source;																									// 入力JavaScriptソース
	
	try
	{
		source = readFileLineAsString(inFile);
	}
	catch(const char* str)
	{
		puts("[ERROR] Input File does not exist!!");
		return;
	}
	
	compress_prefix = prefix;
	
	TokenizerPtr tokenizer(new Tokenizer(source));
	std::vector<TokenPtr> list = filterWhitespace(tokenizer);
	IdentMap identMap(new std::map<std::string, TokenPtr>());
	getPublicIdents(list, identMap);
	
	if (compress_prefix != "")
	{
		renamePrivateIdents(list, identMap, compress_prefix);
	}
	
	identMap->clear();
	getPublicIdents(list, identMap);
	renameLocalIdents(list, identMap);
	
	// 難読化結果を出力
	Token::writeTokensToFile(list, outFile);
}
