/*
 * JsCompressor.h
 * 
 * JavaScriptの難読化による圧縮処理を実行するクラス。
 * executeメソッドの第4引数にてファイル選択かディレクトリ選択か判定し、所定の処理を実行する。
 *
 *  Created on: 2012/11/06
 *      Author: H.Sakai
 */
#ifndef JSCOMPRESSOR_H_
#define JSCOMPRESSOR_H_

#include <string>
#include <vector>
#include <map>
#include "js/Tokenizer.h"
#include "js/Token.h"

typedef boost::shared_ptr<Tokenizer> TokenizerPtr;
typedef boost::shared_ptr< std::map<std::string, TokenPtr> > IdentMap;

class JsCompressor
{
private:
	std::string compress_prefix;																						// 難読化修飾子
	int state;																											// ステータス
	/*
	 * 空白、コメント、ドキュメントコメントのフィルタリング処理を実行する。
	 * 
	 * @param tokenizer トークン分割クラス
	 * @return フィルタリングされたトークンリスト
	 */
	std::vector<TokenPtr> filterWhitespace(TokenizerPtr tokenizer);
	/*
	 * 各トークンの識別子を取得する。
	 * 
	 * @param list トークンリスト
	 * @param 結果格納マップポインタ
	 */
	void getPublicIdents(std::vector<TokenPtr> list, IdentMap resultMap);
	/*
	 * アクセスレベルがprivateの識別子を削除する。
	 * 
	 * @param list トークンリスト
	 * @param map 識別子別トークンマップ
	 * @param s 難読化修飾子
	 */
	void renamePrivateIdents(std::vector<TokenPtr> list, IdentMap map, std::string s);
	/*
	 * 識別子を生成する。
	 * 
	 * @param map 識別子別トークンマップ
	 * @param flag 修飾子未使用フラグ
	 */
	std::string generateIdent(IdentMap map, bool flag);
	/*
	 * 識別子を生成する。
	 * 
	 * @param flag 修飾子未使用フラグ
	 */
	std::string generateIdent(bool flag);
	/*
	 * ローカル変数/関数のリネーム処理を実行する。
	 * 
	 * @param list トークンリスト
	 * @param map 識別子別トークンマップ
	 */
	void renameLocalIdents(std::vector<TokenPtr> list, IdentMap map);
public:
	JsCompressor();
	virtual ~JsCompressor();
	/*
	 * JavaScriptの難読化処理を実行する。
	 * 
	 * @param inFile 入力ファイル(フォルダ):ファイルモードの状態にて
	 * @param outFile 出力ファイル
	 * @param prefix 難読化修飾子
	 */
	void execute(std::string inFile, std::string outFile, std::string prefix);
};

#endif /* JSCOMPRESSOR_H_ */
