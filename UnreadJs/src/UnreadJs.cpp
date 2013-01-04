//============================================================================
// Name        : UnreadJs.cpp
// Author      : H.Sakai
// Version     : V1.00
// Copyright   : Copyright 2012 SORAYUME Co., Ltd. Allrights Reserved.
// Description : JavaScript難読化ツール in C++, Ansi-style
//============================================================================
#include <iostream>
#include <string>
#include <vector>
#include "tools/JsCompressor.h"
#include "utils/Util.h"
#include <boost/shared_ptr.hpp>

using namespace std;

int main(int argc, char *argv[])
{
	// 引数が不足している場合はエラー
	if (argc < 4)
	{
		cout << "[ERROR] not arguments" << endl;
	}
	else
	{
		string inFile(argv[1]);																					// 入力ファイル名
		string outFile(argv[2]);																				// 出力ファイル名
		string isFileMode(argv[3]);																				// ファイル選択モード文字列
		string prefix((argc == 4) ? "" : argv[4]);															// メソッドにつける修飾子
		int execCount;																							// 難読化実行数
		
		vector<string> filelist;																					// 実行ファイルリスト
		vector<string> outList;																					// 出力ファイルリスト
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
			
			filelist = getFileList(inFile);
			execCount = filelist.size();
			
			outList.resize(execCount);
			for (int i = 0; i < execCount; i++)
			{
				outList[i] = outFile + "/" + filelist[i];
				filelist[i] = inFile + "/" + filelist[i];
			}
		}
		
		for (int i = 0; i < execCount; i++)
		{
			// 難読化処理実行
			typedef boost::shared_ptr<JsCompressor> JsCompressorPtr;
			JsCompressorPtr compressor(new JsCompressor());
			compressor->execute(filelist[i], outList[i], prefix);
		}
	}
	
	return 0;
}
