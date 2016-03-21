/*
 * Util.h
 * 共通関数郡。
 *
 *  Created on: 2012/11/07
 *      Author: H.Sakai
 */
#ifndef UTIL_H_
#define UTIL_H_

#include <string>
#include <list>
#include <vector>
#include <string>

/*
 * 空文字定数
 */
const static std::string EmptyStr = "";

/*
 * 文字列をint型数値に変換する。
 * 
 * @param str 文字列
 * @return 変換した数値
 */
int to_int(std::string str);

/*
 * 指定パスのファイルを読み込み行データを返却する。
 * 
 * @param filePath ファイルパス
 * @return 1行単位で格納したリスト
 */
std::list<std::string> readFileLine(std::string filePath);

/*
 * 指定パスのファイルを読み込み文字列を返却する。
 * 
 * @param filePath ファイルパス
 * @return 1行単位で格納したリスト
 */
std::string readFileLineAsString(std::string filePath);

/*
 * 指定パスのディレクトリやファイルが存在するかどうか判定を実行する。
 * 
 * @param path パス
 * @return true:存在する、false:存在しない
 */
bool isPathExist(std::string path);

/*
 * 指定パスのディレクトリを作成する。
 * 
 * @param path パス
 * @return true:作成成功、false:作成失敗
 */
bool createDir(std::string path);

/*
 * 指定ディレクトリのに含まれるファイル名リストを取得する。
 * 
 * @param dirPath ディレクトリパス
 * @param buffer 格納バッファ
 * @return ファイル名リスト
 */
void getFileList(std::string dirPath, std::vector<std::string> *buffer);

#endif /* UTIL_H_ */
