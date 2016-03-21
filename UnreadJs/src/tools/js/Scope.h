/*
 * Scope.h
 * 
 * プログラムのスコープを管理するクラス。
 *
 *  Created on: 2012/11/08
 *      Author: H.Sakai
 */
#ifndef SCOPE_H_
#define SCOPE_H_

#include <map>
#include <string>
#include <memory>

class Scope {
private:
	Scope* previous;
	std::map<std::string, std::string>* mapping;
public:
	Scope();
	Scope( Scope* scope );
	virtual ~Scope();
	/*
	 * ソースのマッピングを追加する。
	 * 
	 * @param src キーソース
	 * @param dest 追加ソース
	 */
	void addMapping(std::string src, std::string dest);
	/*
	 * ソースのマッピングを取得する。
	 * 
	 * @param src キーソース
	 * @return ソース
	 */
	std::string getMapping(std::string src);
	/*
	 * 次のスコープを取得する。
	 * 
	 * @param thisInst 自身のポインタ
	 * @return 次のスコープクラスポインタ
	 */
	Scope* getNext(Scope* thisInst);
	/*
	 * 一つ前のスコープを取得する。
	 * 
	 * @return 一つ前のスコープポインタ
	 */
	Scope* getPrevious();																	// ソースのマッピング
};

#endif /* SCOPE_H_ */
