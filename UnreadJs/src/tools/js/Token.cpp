/*
 * Token.cpp
 *
 *  Created on: 2012/11/07
 *      Author: H.Sakai
 */
#include <boost/regex.hpp>
#include "stdio.h"
#include "Token.h"
#include <algorithm>
#include <fstream>
#include <boost/algorithm/string.hpp>

// 定数初期化
std::string Token::_IDENT = "IDENT";
std::string Token::_WHITESPACE = "WHITESPACE";
std::string Token::_COMMENT = "COMMENT";
std::string Token::_CONDITIONAL_COMMENT = "CONDITIONAL_COMMENT";
std::string Token::_DOCCOMMENT = "DOCCOMMENT";
std::string Token::_FLOAT= "FLOAT";
std::string Token::_INTEGER = "INTEGER";
std::string Token::_SYMBOL = "SYMBOL";
std::string Token::_QUOTED = "QUOTED";
std::string Token::_REGEX = "REGEX";
std::string Token::_FUNCTION = "FUNCTION";
std::string Token::_DOT = "DOT";
std::string Token::_VAR = "VAR";
std::string Token::_LP = "LP";
std::string Token::_RP = "RP";
std::string Token::_RC = "RC";
std::string Token::_LC = "LC";
std::string Token::_KEYWORD = "KEYWORD";
std::string Token::_reComment = "//.*?\n|/\\*.*?\\*/";
std::string Token::_reIdent = "[_\\$a-zA-Z][_\\$\\w]*\\b";
std::string Token::_reInt = "[+-]?\\d+";
std::string Token::_reFloat = "[+-]?\\d+(([.]\\d+)*([eE][+-]?\\d+))?";
std::string Token::_reSingleQuoted = "'((\\\\.)|([^']))*'";
std::string Token::_reDoubleQuoted = "\"((\\\\.)|([^\"]))*\"";
std::string Token::_reRegex = "/((\\\\.)|([^/\n]))*?/[a-zA-Z]*";
std::vector<std::string> Token::_keywords;

Token::Token(std::string s)
{
	type = "unknown";
	setToken(s);
}

Token::~Token()
{
}

std::string Token::getToken()
{
	return token;
}

void Token::setToken(const std::string s)
{
	token = s;
	if (s != "")
	{
		type = getTokenType(s);
	}
	else
	{
		type = "UNDEFINED";
	}
}

std::string Token::getType()
{
	return type;
}

std::string Token::getTokenType(std::string s)
{
	// 小文字変換
	std::string lowerStr;
	lowerStr = "" + s;
	transform (lowerStr.begin (), lowerStr.end (), lowerStr.begin (), tolower);
	
	if(lowerStr == "function")
	{
		return "FUNCTION";
	}
	else if(lowerStr == "var")
	{
		return "VAR";
	}
	else if(lowerStr == ".")
	{
		return "DOT";
	}
	else if(lowerStr == "{")
	{
		return "LC";
	}
	else if(lowerStr == "}")
	{
		return "RC";
	}
	else if(lowerStr == "(")
	{
		return "LP";
	}
	else if(lowerStr == ")")
	{
		return "RP";
	}
	else if (boost::starts_with(s, "/*@") && boost::ends_with(s, "@*/"))
	{
		return "CONDITIONAL_COMMENT";
	}
	else if (boost::starts_with(s, "/**") && boost::ends_with(s, "*/"))
	{
		return "DOCCOMMENT";
	}
	else if (boost::starts_with(s, "//") || boost::starts_with(s, "/*") && boost::ends_with(s, "*/"))
	{
		return "COMMENT";
	}
	
	// パターン1の検索
	boost::regex pattern1("[_\\$a-zA-Z][_\\$\\w]*\\b");
	if(boost::regex_match(s.c_str(), pattern1))
	{
		std::vector<std::string>::iterator containsItem = std::find(_keywords.begin(), _keywords.end(), lowerStr);
		if(containsItem != _keywords.end())
		{
			return "KEYWORD";
		}
		else
		{
			return "IDENT";
		}
	}
	
	// パターン2の検索
	boost::regex pattern2("[+-]?\\d+");
	if(boost::regex_match(s, pattern2))
	{
		return "INTEGER";
	}
	
	// パターン3の検索
	boost::regex pattern3("[+-]?\\d+(([.]\\d+)*([eE][+-]?\\d+))?");
	if(boost::regex_match(s, pattern3))
	{
		return "FLOAT";
	}
	
	// パターン4の検索
	boost::regex pattern4("/((\\\\.)|([^/\n]))*?/[a-zA-Z]*");
	if(boost::regex_match(s, pattern4))
	{
		return "REGEX";
	}
	
	// パターン5の検索
	boost::regex pattern5("'((\\\\.)|([^']))*'|\"((\\\\.)|([^\"]))*\"");
	if(boost::regex_match(s, pattern5))
	{
		return "QUOTED";
	}
	
	// パターン6の検索
	boost::regex pattern6("\\t|\\s");
	if(boost::regex_match(s, pattern6))
	{
		return "WHITESPACE";
	}
	else
	{
		return "SYMBOL";
	}
}

std::string Token::toString()
{
	return "<" + getType() + ">" + getToken()  + "</" + getType() + ">";
}

void Token::writeTokensToFile(std::vector<TokenPtr> textList, std::string filePath)
{
	std::string outText;
	std::vector<TokenPtr>::iterator itr = textList.begin();
	while (itr != textList.end())
	{
		TokenPtr token = *itr;
		outText += token->getToken();
		++itr;
	}
	
	std::ofstream ofs;
	ofs.open(filePath.c_str());
	ofs << outText << std::endl;
	ofs.close();
}

// static変数取得メソッド(代入防止用）
std::string Token::IDENT() { return Token::_IDENT; }
std::string Token::WHITESPACE() { return Token::_WHITESPACE; }
std::string Token::COMMENT() { return Token::_COMMENT; }
std::string Token::CONDITIONAL_COMMENT() { return Token::_CONDITIONAL_COMMENT; }
std::string Token::DOCCOMMENT() { return Token::_DOCCOMMENT; }
std::string Token::FLOAT() { return Token::_FLOAT; }
std::string Token::INTEGER() { return Token::_INTEGER; }
std::string Token::SYMBOL() { return Token::_SYMBOL; }
std::string Token::QUOTED() { return Token::_QUOTED; }
std::string Token::REGEX() { return Token::_REGEX; }
std::string Token::FUNCTION() { return Token::_FUNCTION; }
std::string Token::DOT() { return Token::_DOT; }
std::string Token::VAR() { return Token::_VAR; }
std::string Token::LP() { return Token::_LP; }
std::string Token::RP() { return Token::_RP; }
std::string Token::RC() { return Token::_RC; }
std::string Token::LC() { return Token::_LC; }
std::string Token::KEYWORD() { return Token::_KEYWORD; }
std::string Token::reComment() { return Token::_reComment; }
std::string Token::reIdent() { return Token::_reIdent; }
std::string Token::reInt() { return Token::_reInt; }
std::string Token::reFloat() { return Token::_reFloat; }
std::string Token::reSingleQuoted() { return Token::_reSingleQuoted; }
std::string Token::reDoubleQuoted() { return Token::_reDoubleQuoted; }
std::string Token::reRegex() { return Token::_reRegex; }
std::vector<std::string> Token::keywords()
{
	// サイズが0の場合は初期化する
	if (Token::_keywords.size() == 0)
	{
		// 実行速度を考えてあらかじめ容量確保
		Token::_keywords.resize(60);
		Token::_keywords[0] = "arguments";
		Token::_keywords[1] = "abstract";
		Token::_keywords[2] = "boolean";
		Token::_keywords[3] = "break";
		Token::_keywords[4] = "byte";
		Token::_keywords[5] = "case";
		Token::_keywords[6] = "catch";
		Token::_keywords[7] = "char";
		Token::_keywords[8] = "class";
		Token::_keywords[9] = "const";
		Token::_keywords[10] = "continue";
		Token::_keywords[11] = "debugger";
		Token::_keywords[12] = "default";
		Token::_keywords[13] = "delete";
		Token::_keywords[14] = "do";
		Token::_keywords[15] = "double";
		Token::_keywords[16] = "else";
		Token::_keywords[17] = "enum";
		Token::_keywords[18] = "export";
		Token::_keywords[19] = "extends";
		Token::_keywords[20] = "FALSE";
		Token::_keywords[21] = "final";
		Token::_keywords[22] = "finally";
		Token::_keywords[23] = "float";
		Token::_keywords[24] = "for";
		Token::_keywords[25] = "function";
		Token::_keywords[26] = "goto";
		Token::_keywords[27] = "if";
		Token::_keywords[28] = "implements";
		Token::_keywords[29] = "import";
		Token::_keywords[30] = "in";
		Token::_keywords[31] = "instanceof";
		Token::_keywords[32] = "int";
		Token::_keywords[33] = "interface";
		Token::_keywords[34] = "long";
		Token::_keywords[35] = "native";
		Token::_keywords[36] = "new";
		Token::_keywords[37] = "null";
		Token::_keywords[38] = "package";
		Token::_keywords[39] = "private";
		Token::_keywords[40] = "protected";
		Token::_keywords[41] = "public";
		Token::_keywords[42] = "return";
		Token::_keywords[43] = "short";
		Token::_keywords[44] = "static";
		Token::_keywords[45] = "super";
		Token::_keywords[46] = "switch";
		Token::_keywords[47] = "synchronized";
		Token::_keywords[48] = "this";
		Token::_keywords[49] = "throw";
		Token::_keywords[50] = "throws";
		Token::_keywords[51] = "transient";
		Token::_keywords[52] = "TRUE";
		Token::_keywords[53] = "try";
		Token::_keywords[54] = "typeof";
		Token::_keywords[55] = "var";
		Token::_keywords[56] = "void";
		Token::_keywords[57] = "volatile";
		Token::_keywords[58] = "while";
		Token::_keywords[59] = "with";
	}
	
	return Token::_keywords;
}
