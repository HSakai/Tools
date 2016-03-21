//============================================================================
// Name        : UnreadJs.cpp
// Author      : H.Sakai
// Version     : V1.00
// Copyright   : Copyright 2012 H.Sakai
// Description : JavaScript難読化ツール in C++, Ansi-style
//============================================================================
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "tools/JsCompressor.h"
#include "utils/Util.h"

int main(int argc, char *argv[])
{
	// 引数が不足している場合はエラー
	if (argc < 4)
	{
		std::cout << "[ERROR] not arguments" << std::endl;
		return 1;
	}

	const std::string inFile(argv[1]);																				// 入力ファイル名
	const std::string outFile(argv[2]);																				// 出力ファイル名
	const std::string isFileMode(argv[3]);																			// ファイル選択モード文字列
	const std::string prefix((argc == 4) ? "" : argv[4]);															// メソッドにつける修飾子
	int execCount;																									// 難読化実行数
	
	std::vector<std::string> filelist;																				// 実行ファイルリスト
	std::vector<std::string> outList;																				// 出力ファイルリスト
	if (isFileMode == "1")
	{
		execCount = 1;
		filelist.push_back(inFile);
		outList.push_back(outFile);
	}
	else
	{
		// 出力先のディレクトリが存在しない場合は先に作成しておく。入力ファイルチェックはJsCompressor内でやっているので。
		if (!isPathExist(outFile))
		{
			createDir(outFile);
		}
		
		getFileList(inFile, &filelist);
		execCount = filelist.size();
		
		outList.resize(execCount);
		for (int i = 0; i < execCount; i++)
		{
			outList[i] = outFile + "/" + filelist[i];
			filelist[i] = inFile + "/" + filelist[i];
		}
	}
	
	std::unique_ptr<JsCompressor> compressor(new JsCompressor());
	for (int i = 0; i < execCount; i++)
	{
		// 難読化処理実行
		compressor->execute(filelist[i], outList[i], prefix);
	}
	
	return 0;
}
