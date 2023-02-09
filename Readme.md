--------------------------------------------------------------------------  
【  ソフト名   】HSPCL32N.dll  
【 バージョン  】3.0  
【    作者     】pippi  
【  必要環境１ 】Windows7 以降  
【  必要環境２ 】HSP Ver3.5以降  
【  必要環境３ 】OpenCL対応グラフィックボードまたはCPUまたはCellプロセッサー  
			GeForce 400 Series以降  
			RADEON HD 6xxx以降  
			HD Graphics 2500/4000以降(Ivy Bridge以降)  
【  取扱種別   】フリープラグイン  
【    内容     】HSP3(32bit)用OpenCLプラグイン  
【     HP      】http://toropippi.web.fc2.com/  
--------------------------------------------------------------------------  
  
## ■使用方法  
１、sampleフォルダにある「HSPCL32N.dll」をHSPインストールフォルダ「C:\hsp35」ないしは「C:\Program Files (x86)\hsp35」に順ずる階層へコピーして下さい。  
２、sampleフォルダにある「hspcl32n.as」をHSPインストールフォルダの「common」フォルダの中へ入れて下さい。  
３、ヘルプデータをコピーしたい場合は、「doclib」フォルダ自体をそのままHSPインストールフォルダの中へ上書きして下さい。  
  
  
## ■概要  
  OpenCLをHSPから簡単に触れるようにしたプラグインです。HSPは計算速度が遅いのが課題ですが、このプラグインがあればGPU上で計算(GPGPU)させることができ、とてつもない高速化が可能になります。  
  現在HSPCLシリーズは3種類あります。  
  
### HSPCL32 ver2.0(HSPコンテスト2013) → HSPCL32Nへ改名  
  ここで公開しているものです。  
  HSPCL32 ver2.0はOpenCL機能しかないプラグインです。  
  
### HSPCL32 ver4.02  
  HSPコンテスト2014版はコチラ
  http://dev.onionsoft.net/seed/info.ax?id=929  
  ただしこれは「varsize」関数がかち合ってhsp35以降ではエラーが出るようになりました。  
  
  そこでyoudaiさんによりhspcl32.asの中身を修正して頂いた安定バージョンが公開されました。  
  http://youdaizone.starfree.jp/hsp3/hspcl32m_42/index.html  
  現在はコチラを使用するのが良いでしょう。  
  
  なおver4.02以降更新の予定はございません。  
  
### HSPCL64
  HSPCL32Nの64bit版です。  
  https://github.com/toropippi/HSPCL64  
  HSPCL32Nと比較し、GPU上で確保できるメモリサイズ4GBまでの制約がなくなりました。  
  
## ■インストール  
HSPインストールフォルダ(hsp36フォルダ)にHSPCL32N.dllをコピー  
hsp36フォルダ内のcommonフォルダにhspcl32n.asをコピー  
hsp36フォルダ内にdoclibフォルダをそのままコピーで上書き  
アンインストールはゴミ箱へ削除  
  
## ■注意点  
このプラグインではカーネルコードによるメモリアクセス違反に対して防護する機能がありません。  
メモリアクセス違反によるエラーがシステムに影響を及ぼすことがあります。  
  
最悪、ブルースクリーンになったり、GPUからの信号が途絶え画面が落ちたり、GPUがフリーズしたりなどの現象が起こります。  
これに関するいかなる損失も、責任を負えません。  
  
ただ私は100回以上ブルスクを出してきましたが、これでGPUが壊れたことは1回もありませんでした。  
  
## ■連絡先  
efghiqippi@yahoo.co.jp  
  
## ■免責  
このプラグインの使用により発生した如何なる問題について当方は一切の責任を負いません。  
商用問わず配布、転載、改造は無断かつ自由にして構いません（大歓迎）  
  
## ■License  
HSPCL32N Copyright (c) 2021 pippi  
Released under the MIT License  
see https://opensource.org/licenses/MIT  
  

## ■更新履歴  
	Ver 3.0  
	2021/10/06  
	HSPCL64 ver 1.1から命令を全て逆輸入(結構一気にかわった)  
	以降HSPCL64と命令を同時に更新する方針  
	  
	Ver 2.2  
	2021/6/14  
	ほとんどの命令をHSPCL64 1.0から逆輸入  
	以前の命令と互換性を保つため関数型とコマンド型どちらでも命令を実行できるように変更  
	  
	Ver 2.1  
	2021/5/28  
	命令をHSPCL32 ver2.0から引き継ぎ。バージョン表記も引き継ぎ  
	  
	Ver 2.0  
	2013/8/29  
	HCLbye実行時の不具合を修正  
	HCLDoKrn1〜3まで追加  
	HCLWaitTaskでタスク待できるように  
	メモリアクセス違反以外の大概のプログラムミスに対しエラーメッセージを出すようにチェック機能搭載  
	その他エラーメッセージがでるように機能追加  
	リリース前最終調整  
	  
	Ver 1.11  
	2013/8/10   
	カーネル実行においてlocal_work_sizeとglobal_work_sizeに配列を指定できるように変更  
	  
	Ver 1.10  
	2013/5/14  
	プログラムビルド、カーネル、メモリ周りの実装  
	  
	Ver 1.0  
	2013/5/9  
	コンテキストの作成命令  