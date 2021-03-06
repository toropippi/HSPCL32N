;------------------------
;   カーネル関連
;------------------------

;-------- header --------
%dll
HSPCL32N.dll

%ver
3.0

%date
2021/10/06

%author
pippi

%note
hspcl32n.as をインクルードしてください。

%type
32bitランタイムGPGPU用プラグイン

%group
OpenCLカーネル関連

%port
Win

;-------- ref --------

%index
HCLCreateProgram
カーネルプログラムのビルド

%prm
(str p1,str p2)
str p1 : カーネルソースファイル名 	[in]
str p2 : ビルドオプション,省略可	[in]

%inst
プログラムidが32bit int型で返ります。
p1にはソースのファイル名を入れて下さい。
p2にはビルドオプションを入れてください。
例："-D SCALE=111"

コンパイルされたOpenCLカーネルプログラムは、そのデバイス上でしか使えません。
２つ以上のデバイス上で同じカーネルを実行したいとき、それぞれのデバイスidをHCLSetDeviceでセットしなおしてHCLCreateProgramを実行して下さい。

%href
HCLCreateProgramWithSource
HCLSetDevice
HCLCreateKernel
HCLReleaseProgram

;-------- ref --------

%index
HCLCreateProgramWithSource
カーネルプログラムのビルド

%prm
(str p1,str p2)
str p1 : カーネルソース文字列		[in]
str p2 : ビルドオプション,省略可	[in]

%inst
プログラムidが32bit int型で返ります。
p1にはソースのデータを入れて下さい。
p2にはビルドオプションを入れてください。
例："-D SCALE=111"

コンパイルされたOpenCLカーネルプログラムは、そのデバイス上でしか使えません。
２つ以上のデバイス上で同じカーネルを実行したいとき、それぞれのデバイスidをHCLSetDeviceでセットしなおしてHCLCreateProgramWithSourceを実行して下さい。

%href
HCLCreateProgram
HCLSetDevice
HCLCreateKernel
HCLReleaseProgram

;--------
%index
HCLGetProgramBinary
プログラムバイナリ作成

%prm
(int p1)
int p1 : プログラムid		[in]

%inst
バイナリ文字列がstr型で返ります。

改行コードがCR LFではなく、LFのみの文字列になっている点に注意してください。

%href
HCLCreateProgramWithSource
HCLCreateProgramWithBinary
;--------
%index
HCLCreateProgramWithBinary
プログラムバイナリからプログラム作成

%prm
(str p1,str p2)
str p1 : プログラムバイナリ文字列	[in]
str p2 : ビルドオプション,省略可	[in]

%inst
プログラムidがint32型で返ります。
p1にはソースのデータを入れて下さい。
p2にはビルドオプションを入れてください。
例："-D SCALE=111"

コンパイルされたOpenCLカーネルプログラムは、そのデバイス上でしか使えません。
２つ以上のデバイス上で同じカーネルを実行したいとき、それぞれのデバイスidをHCLSetDeviceでセットしなおしてHCLCreateProgramWithBinaryを実行して下さい。

%href
HCLGetProgramBinary
HCLCreateProgramWithSource
;------------

%index
HCLCreateKernel
カーネル作成

%prm
(int p1,str p2)
int p1 : プログラムid		[in]
str p2 : カーネル関数名		[in]

%inst
カーネルidが32bit int型で返ります。

p2はp1カーネルソース内にある「__kernel 」から始まる関数名を「__kernel 」より後の文字列で指定します。
例えばp1ソース内に「__kernel void vector_add(__global float *A) {}」という行があればp2は "vector_add" を指定します。
以降、変数のセットや計算はこのカーネルidという形で管理、実行できます。

%href
HCLSetDevice
HCLReleaseKernel
HCLSetKernel
HCLSetKrns

;--------

%index
HCLSetKernel
カーネルセット

%prm
int p1,int p2,p3,int p4
int p1 : カーネルid			[in]
int p2 : 引数の順番p(x)の指定		[in]
    p3 : 引数に渡す実体(定数やmem_object)[in]
int p4 : ローカルメモリフラグ,省略可	[in]
%inst
カーネルの引数一つ一つにデータを渡します。

HCLDoKernelで計算する前にこれでカーネルの引数を予めセットしておかなければいけません。


例えばカーネル側のソースが

__kernel void vector_add(__global int *array1,int arg2) {}

というものなら
HCLSetKernel p1,0,mem_object_A	//(←HCLCreateBufferで作成したmem_object id)	;配列
HCLSetKernel p1,1,5	//引数2
と2回に渡り指定します。

p2は、vector_addの引数の一番左を0番として考えます。

１回セットすれば次セットし直すまで適応され続けます。
p3には32bit int型、64bit int型、文字列型変数、double型、float型変数が指定できます。

p4を0以外にすると、その引数はローカルメモリ（共有メモリ）として登録されます。ローカルメモリはグローバルメモリより容量が少ない分高速にアクセスが可能な書込読取可能メモリです。
一つのワークグループ内でしか値を保持できません。初期値は設定不可で0または不定です。
並列計算中、他のスレッドと情報を共有したいときに使います。以下参照。
p4が0以外のとき、p4には確保したいローカルメモリサイズ(byte)をintで指定して下さい。p3は無視されます。


ローカルメモリの使い方は以下のとおりです

例
カーネルコード
__kernel void vector_add(__global double *A, __global double *B, __local double block[] , int b, int n) {
	block[0]=1;//共有メモリの0番目に1を代入
・・・・・

に対するHSPスクリプトでのHCLSetKernelは
HCLSetKernel p1,0,mem_object_dpA
HCLSetKernel p1,1,mem_object_dpB
HCLSetKernel p1,2,0,64 (←p3には0を指定、p4は64byteつまりdouble型*8個の共有メモリを作れという意味、OpenCLカーネルソース内ではblock[0]〜block[7]が同じワークグループ内で共有して使える)
HCLSetKernel p1,3,10
HCLSetKernel p1,4,1024



%href
HCLSetDevice
HCLCreateKernel
HCLSetKrns
HCLDoKernel
HCLDoKrn1
HCLDoKrn2
HCLDoKrn3

;--------

%index
HCLSetKrns
カーネルセット

%prm
int p1,p2,,,,,
int p1 : カーネルid				[in]
p2以降 : 引数に渡す実体(定数やmem_object)	[in]

%inst
カーネルの引数をまとめて指定します。

HCLSetKernel p1,0,mem_object_dpA
HCLSetKernel p1,1,mem_object_dpB

なら

HCLSetkrns p1,mem_object_dpA,mem_object_dpB

となります。
なおローカルメモリのサイズ指定はできません。

%href
HCLSetDevice
HCLCreateKernel
HCLSetKernel
HCLDoKernel
HCLDoKrn1
HCLDoKrn2
HCLDoKrn3

;--------

%index
HCLGetKernelName
カーネル名取得

%prm
(int p1)
int p1 : カーネルid			[in]

%inst
カーネルの名前を文字列で返します。

%href
HCLCreateKernel
;--------

%index
HCLCall
カーネル文字列実行

%prm
str p1,int p2,int p3,p4,p5,p6,,,,,
str p1:カーネル文字列				[in]
int p2:グローバルサイズ(1次元並列処理数)	[in]
int p3:ローカルサイズ(1次元並列処理数)		[in]
p4以降:引数に渡す実体(arrayやvar intなどの数値)	[in,out]
%inst

HCLCreateProgram,HCLCreateKernel,HCLSetKernel,HCLCreateBuffer等をせずカーネルを実行して結果を得ます。
例えばOpenCLで配列に値を書き込む処理を書いて実行すると、p4以降に指定したHSP配列変数に結果が書き込まれます。
p1にはソースコードの文字列
p2にはグローバルサイズ（実行したい並列処理数）
p3にはローカルサイズ
p4以降にはカーネルに渡す引数を指定して下さい。

p4以降の引数の数とOpenCLカーネル内の引数の数が合わないとエラーになります。


内部でHCLCreateProgram,HCLCreateKernel,HCLSetKernel,HCLCreateBuffer,HCLWriteBuffer,HCLReadBuffer,HCLReleaseKernel,HCLReleaseProgram,HCLReleaseBufferを使用しています。
この命令で確保したVRAM等は、この命令が終わるまでに必ず破棄されます。
HCLDoKernelと違い、タスクが完了するまで次の命令にうつりません。
オーバーヘッドも大きいので、速度が求められる場合には向きません。

%href
HCLDoKernel
HCLCall2
HCLDokrn1
HCLDokrn2
HCLDokrn3
;--------

%index
HCLCall2
カーネル文字列実行

%prm
str p1,int p2,int p3,p4,p5,p6,,,,,
str p1:カーネル文字列				[in]
int p2:グローバルサイズ(1次元並列処理数)	[in]
int p3:ローカルサイズ(1次元並列処理数)		[in]
p4以降:引数に渡す実体(arrayやvar intなどの数値)	[in,out]
%inst

HCLCreateProgram,HCLCreateKernel,HCLSetKernelをせずカーネルを実行して結果を得ます。
今回のHCLCall2は引数を直接(?)指定できるので、より直観的に書くことができます。
つまりHCLCallのHSP配列→cl mem版です。

p1にはソースコードの文字列
p2にはグローバルサイズ（実行したい並列処理数）
p3にはローカルサイズ
p4以降にはカーネルに渡す引数を指定して下さい。

p4以降の引数の数とOpenCLカーネル内の引数の数が合わないとエラーになります。

内部でHCLCreateProgram,HCLCreateKernel,HCLSetKernelを使用しています。
HCLCallと違いタスクが完了する前に次の命令にうつります(ブロッキングモードoff)。

HCLCallやHCLCall2では第一引数の入力文字列がプラグイン内部でハッシュ化され保存されており、全く同じ文字列の場合、前回のビルドを自動で使いまわすことができるようになっています。
つまり毎回カーネルソースをコンパイルしているわけではないので高速です。
ただHCLDokrn1,2,3命令のように引数指定と分離できてるわけではないので、細かいことを言えば引数指定の分オーバーヘッドはどうしてもあります。

%href
HCLDoKernel
HCLCall
HCLDokrn1
HCLDokrn2
HCLDokrn3

;--------

%index
HCLDoKernel
カーネル実行

%prm
int p1,int p2,array p3,array p4,int p5
int p1 : カーネルid			[in]
int p2 : work_dim(1〜3)			[in]
array p3 : global_work_size		[in]
array p4 : local_work_size		[in]
int p5 : event_id,省略可		[in]
%inst

p1には実行したいカーネルid
p2には1〜3 (work dimension＝グローバルワークサイズとローカルワークサイズの次元)
p3には次元数に応じたグローバルワークサイズ(並列処理数)を記憶する変数
p4には次元数に応じたローカルワークサイズを記憶する変数
を指定してください。
p4の変数の内容が0の場合、グローバルワークアイテムをどのようにワークグループに分割するかは OpenCL 実装が決定します。
p5はevent_idで-1〜65535の値を指定できます。省略時デフォルトでは-1です。
カーネルの実行状態を取得するにはeventを記録する必要があり、0〜65535のときそのevent番号とカーネルが紐付けされます。
以降、その番号でevent内容の取得(実行状況、実行開始時間、実行終了時間など)が行えます。
番号の上書きもできますが、上書きされる前のevent情報は破棄されます。

この命令自体は実行が完了するまで待つ命令ではなく、OpenCLコマンドをキューに入れるだけであり、実際のカーネルの実行終了を待つにはeventを使うかHCLFinish等で待つことになります。
これは一見複雑なように思えますが、GPUが計算している最中にCPUが別のタスクに処理を回せるという利点があります。

■HCLDoKernelについて
この命令はOpenCLの入門用として、また簡易にOpenCLを利用できることを目的に作成した命令です。
HCLCallでは自動的に処理していた「HSPで用意した配列変数をVRAM等に移す処理」「スレッドの次元」を
自ら設定しなければなりません。
しかし設定できる部分が少ないHCLCallと比較して処理の高速化や自由度の高いことが可能になります。


HSPユーザーとしてHCLDoKernel（OpneCLプラグイン）を利用する時、理解の上で躓きやすい点をリストアップしました。
そのためにOpenCLを利用する上での独特な処理を説明します。

	1.OpenCL用の命令ソースを別個用意する必要がある。（簡易なC言語にて表記）
	そしてそのソースをHSP上で固有の命令（HCLCreateProgram）で読み込み。

	2.そのソースの関数を「カーネル」と言われる命令単位をHSP上で固有の命令（HCLCreateKernel）で作成する。


	3.カーネル関数への引数をセットする際、その引数は　CL_mem_object idという固有のオブジェクト形式である必要がある。
	CL_mem_object　idは32bit int型の数値である。
	そのオブジェクトを用意するにはHSP上で固有の命令HCLCreateBufferにて作成する。
	またそのオブジェクトにHSP上で用意した配列を入れ込みたい時はHSP上で固有の命令HCLWriteBufferにて入れ込む。
　
	4.先ほどカーネル関数に入れ込むための引数を固有のオブジェクト形式にて用意した。
	その引数をカーネル関数に引数をセットするとき固有の命令（HCLSetKernelやHCLSetKrns）を使用しなければならない。 

	5.そして引数をセットしたカーネルを固有の命令（HCLDoKernel）で実行。

	6.そしてその結果を参照する時はHSP上の固有の命令（HCLReadBuffer）でデータを戻してこなければならない。

このような処理が必要となるのはホスト側（CPU側）とデバイス側（GPU側）の処理/メモリ管理が別個となっているからです。
なお便宜上GPU側と書いていますが、OpenCLデバイスがIntel CPUやAMD CPUの場合もありえます。その場合でもメモリ管理が別個であることは変わりないです。※つまりSVMは使えない(ver1.x時点)


■インデクス空間について
GPGPUプログラミングにおいて、グローバルワークサイズやローカルワークサイズはとても重要な概念になってきます。
OpenCLだけでなくCUDAでも考え方をします。


html{
<img src="./doclib/HSPCL32N/thumbs/d22.png">
}html


この図全体がNDRange

NDRange サイズ Gx＝global_work_size[0]＝グローバルワークアイテムxのサイズ＝この図だと4
NDRange サイズ Gy＝global_work_size[1]＝グローバルワークアイテムyのサイズ＝この図だと6

ワークグループサイズSx＝local_work_size[0]＝ローカルワークアイテムxのサイズ＝この図だと2
ワークグループサイズSy＝local_work_size[1]＝ローカルワークアイテムyのサイズ＝この図だと3

ワークアイテム＝スレッド＝この図で言うと24個ある水色の箱。

赤枠内の黄色いところ(グループ内)で共有メモリが使えます。
「VRAM」と「共有メモリ」は違います。

VRAM＝ビデオメモリー＝デバイスメモリー＝グローバルメモリー＝GDDR6（アクセス一番遅い）＝カーネルソースで__globalで指定した変数
共有メモリー＝１次キャッシュ＝ローカルメモリー（少し早い）＝カーネルソースで__localで指定した変数
プライベートメモリー＝レジスタ（一番早い）＝カーネルソースで何も指定しなかった時の変数(uint ic　とかはこれをさしている)

プログラマはOpenCLでデータ並列処理を行うためにインデクス空間の次元数、ワークサイズの総数、ローカルサイズを指定することでインデクス空間を定義しなければなりません。(後述)

以下にOpenCL Cでのグローバルidなどの使い方について記載します。

例
work_dim=2
global_work_size=4,6
local_work_size=2,3
を指定したとします。

html{
<img src="./doclib/HSPCL32N/thumbs/d22.png">
}html

並列実行されるスレッド数は4*6の24個となります。

今度はカーネル側のサンプルです。
カーネルソースでは
__kernel void vector_add(__global double *A, __global double *B, __local double block[] , int bekii, int n) {
	int i0 = get_global_id(0);
	int i1 = get_global_id(1);
・・・・・・・・・計算処理部分・・・・・・・・
}

となっているとします。
カーネルは24個並列作動しますがそれぞれを見分けるidがあり、それがget_global_idで取得できます。

get_global_id(0)は0〜3を
get_global_id(1)は0〜5をそれぞれのワークアイテムに戻します。（以下の図参照）

get_local_id はワークグループ内の識別idを戻します。
get_local_id(0)は0〜1を
get_local_id(1)は0〜2をそれぞれのワークアイテムに戻します。（以下の図参照）

get_group_idはワークグループのグループ識別番号を戻します。
get_group_id(0)は0〜1
get_group_id(1)は0〜1をそれぞれのワークアイテムに戻します。（以下の図参照）

get_global_sizeはグローバルサイズで、どのワークアイテムでも戻す値は同じ値で
get_global_size(0)は6
get_global_size(1)は3
となります。

get_local_sizeはローカルサイズで、どのワークアイテムでも戻す値は同じ値で
get_local_size(0)は2
get_local_size(1)は3
となります

get_num_groupsはグループ数で、どのワークアイテムでも戻す値は同じ値で
get_num_groups(0)は3
get_num_groups(1)は1
となります


以下、24スレッドの各値の図

html{
<img src="./doclib/HSPCL32N/thumbs/d2.png">
}html




■global_work_sizeの指定
並列数を指定するものだと思ってください。
100要素の配列があって1スレッドが1つの要素にアクセスするような処理の場合、1次元100並列で動かすので、global_work_size[0]は100とします。

画像のようなデータに処理を施す場合は、2次元で256*256並列などということもできます。

■local_work_sizeの指定
先ほどのglobal_work_sizeをどのように分割するか、というのが直観的な説明かと思います。
つまり割り切れれば何でもいいわけですが、計算速度に直結するので重要な項目です。

結論から言うと64,128,256のどれかを指定すれば外れはありません。
global_work_sizeが半端な数で上記の数で割り切れない場合は、global_work_sizeを増やしてでも割り切れる数字にしたほうが良いです。

・詳細説明
global_work_sizeが1024でlocal_work_sizeを64にしたとします。
分割した後の1グループ(=1つのワークグループ)は64のスレッドを持っている状態です。1024/64=16なので16グループ存在します。
そして1グループは以下の機能が使えます。

    ・共有メモリがあれば、共有メモリを共有できる。

この機能のために、同じグループに属するスレッドは必ず同一のCU(SM)で実行されます。
一般的にNVIDIAのGPUだと1SMあたり64,128,192のCUDA Coreがあり、AMDのGPUだと1CUあたり64のPEがあり、そのCUDA coreやPEが処理を行ないます。

NVIDIAのTuringアーキテクチャでいうと1SMあたり64CUDA Coreあり、これが16+16+16+16と分かれています。
この1×16CUDA coreが1つのWarp(*1)を2cycleかけて処理します。
*1:Warpとは32スレッドのまとまりのこと

1グループ=64スレッドなので、2つのWarpにおさまります。
全部で16グループあるので全部でWarpが32あることになります。Warp番号0〜31が割り振られているとします。

またここでは1SM内で16グループ=32Warp全部が実行されるとして、あるFMA(c=a*b+c)を1つ実行するとすると
先ほどの?@16+?A16+?B16+?C16のCUDA Coreが
1,2cycle目
?@:Warp0
?A:Warp1
?B:Warp2
?C:Warp3

3,4cycle目
?@:Warp4
?A:Warp5
?B:Warp6
?C:Warp7

5,6cycle目
?@:Warp8
?A:Warp9
?B:Warp10
?C:Warp11

7,8cycle目
?@:Warp12
?A:Warp13
?B:Warp14
?C:Warp15

9,10cycle目
?@:Warp16
?A:Warp17
?B:Warp18
?C:Warp19

11,12cycle目
?@:Warp20
?A:Warp21
?B:Warp22
?C:Warp23

13,14cycle目
?@:Warp24
?A:Warp25
?B:Warp26
?C:Warp27

15,16cycle目
?@:Warp28
?A:Warp29
?B:Warp30
?C:Warp31


このように実行され、1024個のFMAが実行できたことになります。

実際はSM数が70あったりでもっと話は複雑なのですが、このような流れであることはCUDAでもCompute Shaderでも変わりません。
AMD GPUのGCNアーキテクチャだと1CU=16PE+16PE+16PE+16PEで16PEが4cycleで1wavefront(64thread)を実行します。

なので、local_work_sizeが32で割り切れなければWarpにうまくおさまらないので空回りするCoreがでてきてしまい、計算が非効率になってしまいます。
AMD GPUのことも考えると64で割り切れることが必要になります。

なので冒頭に書いた64,128,256が良いという結論になり、512,1024は一部のGPUでエラーが出るのでそこまで大きい数は指定しないほうが無難、という考えになります。
あくまで私の考えなので、答えはないものだとは思います。究極的にはlocal_work_sizeを全パターン試して最速のを採用するのが良いです。


■使用注意
(1)
一部のGPUでは、グローバルワークサイズはローカルワークサイズで整除できなければいけません。
dim globalsize,3
dim localsize,3
globalsize=2048,2048,7
localsize=8,8,4
であった場合、globalsize.2はlocalsize.2で割り切れないためにエラーとなります。ただしAMDのグラボや最新のNVidiaのグラボ、一部のCPUではエラーにならないこともあります。エラーの場合
「global_work_sizeがlocal_work_size で整除できない、またはlocal_work_size[0]*local_work_size[1]*local_work_size[2]が、一つのワークグループ内のワークアイテム数の最大値を超えた」
というメッセージが出ます。


■カーネルソースについて
(1)
使える代表的な演算子
=	代入
!=	条件文
==	条件文
>	条件文
<	条件文
>=	条件文
<=	条件文
>>	右シフト
<<	左シフト
％	a％b　でaをbで割った余りを返す（事情により全角）
/	割り算
*	掛け算
+	足し算
-	引き算
--	-=1と同意味
++	+=1と同意味
&var	変数varのポインタ。使いドコロとしては自作関数の出力値に変数をセットしたいとき（むしろそれだけ）
*var	varがポインタであることが前提。「*var」はvarの指すポインタ位置の変数。使いドコロとしては関数の出力値の宣言など
->	間接メンバ参照演算子

(2)
使える代表的な型
uはunsignedの意味
1bit変数
・bool 0〜1
1byte変数
・uchar 0〜255
・char -128〜127
2byte変数
・ushort 0〜65536
・short -32768〜32767
4byte変数
・float -∞〜+∞
・uint 0〜約40億
・int -約20億〜+約20億
8byte変数
・double -∞〜+∞
・ulong 0〜約1800京
・long -約900京〜+約900京

(3)
型変換
uint a;
uchar b=1;
a=b;//これでもいいが
a=(uint)b;//このほうがエラーを防げる

同じく
float ftmp;
int i0=123;
ftmp=(float)i0;//ftmpには123.0000が代入される

(4)
関数
「__global 」はVRAM上の変数を使うという意味
「__local」はローカルメモリ上の変数を使うという意味

(5)
使える関数、命令、演算子など詳細に知りたい方は
https://www.khronos.org/files/opencl-quick-reference-card.pdf
へ


■最後に
これだけではまだ理解できないかと思いますが、本プラグイン付属のサンプルのコメントや、ドキュメントも読んでいただけたらなと思います。
そしてOpenCLの全貌を理解するのに少しでも役に立てばいいと思い参考となるリンクを載せておきます。

http://seesaawiki.jp/w/mikk_ni3_92/d/OpenCL
http://neareal.net/index.php?Programming%2FOpenCL%2FOpenCLSharp%2FTutorial%2F4_SimpleOpenCLTask-1
http://www.02.246.ne.jp/~torutk/cxx/opencl/
http://www.matlab.nitech.ac.jp/papers/pdf/degree/2011_bachelor_sawada.pdf
http://www.cc.u-tokyo.ac.jp/support/press/news/VOL12/No6/201011_gpgpu.pdf
http://www.cc.u-tokyo.ac.jp/support/press/news/VOL12/No1/201001gpgpu.pdf
http://ameblo.jp/xcc/entry-10799610964.html
http://www.bohyoh.com/CandCPP/FAQ/FAQ00046.html
http://kmotk.jugem.jp/?eid=89
http://d.hatena.ne.jp/oho_sugu/20100928/1285685048
http://sssiii.seesaa.net/category/15158044-1.html
http://sssiii.seesaa.net/article/309874057.html


%href
HCLSetKernel
HCLSetKrns
HCLCall
HCLDokrn1
HCLDokrn1_sub
HCLDokrn2
HCLDokrn3

;--------

%index
HCLDoKrn1
一次元でカーネル実行

%prm
int p1,int p2,int p3,int p4
int p1 : カーネルid			[in]
int p2 : global_work_size		[in]
int p3 : local_work_size		[in]
int p4 : event_id,省略可		[in]

%inst
動作はwork_dimが1の場合のHCLDoKernelと同じです。

p3が0の場合、グローバルワークアイテムをどのようにワークグループに分割するかは OpenCL 実装が決定します。
p4はevent_idで-1〜65535の値を指定できます。省略時デフォルトでは-1です。詳細はHCLDokernelを参照ください。

■この命令を使う前に
LV3	HCLDoKrn1〜3を利用したOpenCL
OpenCL（GPGPU）の初期理解を補助するためにレベルを３段階に分けておりましたが
HCLDoKrn1〜3はLV3となっております。


%href
HCLDoKernel
HCLCall
HCLDoKrn1_sub
HCLDoKrn2
HCLDoKrn3
HCLSetKernel
HCLSetKrns
;--------


;--------

%index
HCLDoKrn1_sub
一次元でカーネル実行

%prm
int p1,int p2,int p3,int p4
int p1 : カーネルid			[in]
int p2 : global_work_size		[in]
int p3 : local_work_size		[in]
int p4 : event_id,省略可		[in]

%inst

この命令ではglobal_work_sizeがlocal_work_sizeで割り切れない場合、エラーを出さずにカーネルを2回に渡り実行するものです。

HCLDoKrn1ではglobal_work_sizeがlocal_work_sizeで割り切れなければいけませんでした。
もしlocal_work_sizeに0を指定してOpenCL実装にまかせても、global_work_sizeが素数の場合local_work_sizeが1にされてしまうことがあり、この場合計算が非効率になってしまいます。

この命令では1回目にlocal_work_sizeで割り切れる分だけのglobal_work_sizeを実行し、2回目にあまりの端数＝local_work_size＝global_work_sizeとして実行します。このとき「get_global_id(0)」が続きから始まるようになっています。
ただし「get_group_id(0)」の値は0になってしまいます。
p3に0は指定できません。
p4のevent idは省略時デフォルトで-1で、0〜65535の値を指定できますが記録されるのは「端数じゃない方のカーネル」のみです。


%href
HCLDoKernel
HCLSetKernel
HCLSetKrns
HCLDoKrn1
HCLDoKrn2
HCLDoKrn3

;--------


%index
HCLDoKrn2
ニ次元でカーネル実行

%prm
int p1,int p2,int p3,int p4,int p5
int p1 : カーネルid			[in]
int p2 : global_work_size.0		[in]
int p3 : global_work_size.1		[in]
int p4 : local_work_size.0		[in]
int p5 : local_work_size.1		[in]
int p6 : event_id,省略可		[in]
%inst
work_dimが2の場合のHCLDoKernelと同じです。

p4が0の場合、グローバルワークアイテムをどのようにワークグループに分割するかは OpenCL 実装が決定します。

%href
HCLDoKernel
HCLSetKernel
HCLSetKrns
HCLDoKrn1
HCLDoKrn1_sub
HCLDoKrn3

;--------

%index
HCLDoKrn3
三次元でカーネル実行

%prm
int p1,int p2,int p3,int p4,int p5,int p6,int p7
int p1 : カーネルid			[in]
int p2 : global_work_size.0		[in]
int p3 : global_work_size.1		[in]
int p4 : global_work_size.2		[in]
int p5 : local_work_size.0		[in]
int p6 : local_work_size.1		[in]
int p7 : local_work_size.2		[in]
int p8 : event_id,省略可		[in]

%inst
work_dimが3の場合のHCLDoKernelと同じです。

p5が0の場合、グローバルワークアイテムをどのようにワークグループに分割するかは OpenCL 実装が決定します。

%href
HCLDoKernel
HCLSetKernel
HCLSetKrns
HCLDoKrn1
HCLDoKrn1_sub
HCLDoKrn2

;--------

%index
HCLFinish
OpenCLコマンド待ち

%prm

%inst

同じコマンドキュー内に入れられたOpenCLコマンドの実行がすべて終わるまで待ちます。
ここで言うOpenCLコマンドとは
HCLDoKernel
HCLDoKrn1
HCLDoKrn1_sub
HCLDoKrn2
HCLDoKrn3
HCLCall2
HCLDoXc
HCLDoXi
HCLDoXl
HCLDoXf
HCLDoXd
HCLDoXuc
HCLDoXui
HCLDoXul
HCLCopyBuffer
HCLWriteBuffer
HCLReadBuffer
HCLFillBuffer
HCLBLAS_Set2DShape
HCLBLAS_Get2DShape
HCLBLAS_sgemm
HCLBLAS_dgemm
HCLBLAS_sT
HCLBLAS_dT
HCLBLAS_sgemv
HCLBLAS_dgemv
HCLBLAS_sdot
HCLBLAS_ddot
HCLBLAS_snrm2
HCLBLAS_dnrm2

の命令で発行したものになります。

%href
HCLDoKernel
HCLDoKrn1
HCLDoKrn1_sub
HCLDoKrn2
HCLDoKrn3
HCLCall2
HCLDoXc
HCLDoXi
HCLDoXl
HCLDoXf
HCLDoXd
HCLDoXuc
HCLDoXui
HCLDoXul
HCLCopyBuffer
HCLWriteBuffer
HCLReadBuffer
HCLFillBuffer
HCLBLAS_Set2DShape
HCLBLAS_Get2DShape
HCLBLAS_sgemm
HCLBLAS_dgemm
HCLBLAS_sT
HCLBLAS_dT
HCLBLAS_sgemv
HCLBLAS_dgemv
HCLBLAS_sdot
HCLBLAS_ddot
HCLBLAS_snrm2
HCLBLAS_dnrm2
HCLFlush
;--------

%index
HCLFlush
OpenCLコマンドを発行

%prm

%inst

HCLSetDeviceで指定しているデバイスのすべてのコマンドキューに入れられた全てのOpenCLコマンドを発行します。

ここで言うOpenCLコマンドとは

HCLDoKernel
HCLDoKrn1
HCLDoKrn1_sub
HCLDoKrn2
HCLDoKrn3
HCLCall2
HCLDoXc
HCLDoXi
HCLDoXl
HCLDoXf
HCLDoXd
HCLDoXuc
HCLDoXui
HCLDoXul
HCLCopyBuffer
HCLWriteBuffer
HCLReadBuffer
HCLFillBuffer
HCLBLAS_Set2DShape
HCLBLAS_Get2DShape
HCLBLAS_sgemm
HCLBLAS_dgemm
HCLBLAS_sT
HCLBLAS_dT
HCLBLAS_sgemv
HCLBLAS_dgemv
HCLBLAS_sdot
HCLBLAS_ddot
HCLBLAS_snrm2
HCLBLAS_dnrm2
の命令で発行したものになります。

%href
HCLDoKernel
HCLDoKrn1
HCLDoKrn1_sub
HCLDoKrn2
HCLDoKrn3
HCLCall2
HCLDoXc
HCLDoXi
HCLDoXl
HCLDoXf
HCLDoXd
HCLDoXuc
HCLDoXui
HCLDoXul
HCLCopyBuffer
HCLWriteBuffer
HCLReadBuffer
HCLFillBuffer
HCLBLAS_Set2DShape
HCLBLAS_Get2DShape
HCLBLAS_sgemm
HCLBLAS_dgemm
HCLBLAS_sT
HCLBLAS_dT
HCLBLAS_sgemv
HCLBLAS_dgemv
HCLBLAS_sdot
HCLBLAS_ddot
HCLBLAS_snrm2
HCLBLAS_dnrm2
HCLFinish
;--------

%index
HCLSetCommandQueue
コマンドキュー番号をセット

%prm
int p1
int p1 : コマンドキュー番号	[in]

%inst

HCLSetDeviceで指定しているデバイスのうち、使えるコマンドキューは0〜3まであります。
デフォルトでは0です。
下記命令は指定したコマンドキューにOpenCLコマンドとして入り実行されます。

HCLDoKernel
HCLDoKrn1
HCLDoKrn1_sub
HCLDoKrn2
HCLDoKrn3
HCLCopyBuffer
HCLWriteBuffer
HCLReadBuffer
HCLFillBuffer

同じデバイスでも違うコマンドキューに入れられたOpenCLコマンドは、順不同で実行されます。
例えば同じメモリにデータを書き込むカーネルを別々のコマンドキューにいれて実行すると、カーネルが同時に実行される可能性があり、メモリにはそれぞれのカーネルが書き込んだ値が混在している可能性があります。
複雑で不便なように思いますが、OpenCLコマンドのオーバーラップ実行が可能になり、使い方によっては速度面で有利になります。

%href
HCLDoKernel
HCLDoKrn1
HCLDoKrn1_sub
HCLDoKrn2
HCLDoKrn3
HCLCopyBuffer
HCLWriteBuffer
HCLReadBuffer
HCLFillBuffer
HCLFlush
HCLFinish
HCLGetSettingCommandQueue
_ExHCLSetCommandQueueMax
_ExHCLSetCommandQueueProperties
;--------

%index
HCLGetSettingCommandQueue
セットしているコマンドキュー番号を取得

%prm
()

%inst
HCLSetCommandQueueでセットした番号を返します。
;--------
%index
HCLReleaseKernel
カーネル破棄

%prm
int p1
int p1 : カーネルid			[in]

%inst

登録カーネルを破棄します。

%href
HCLCreateKernel
;--------

%index
HCLReleaseProgram
プログラム破棄

%prm
int p1
int p1 : プログラムid			[in]
%inst

登録コンパイル済みプログラムを破棄します。

%href
HCLCreateProgram
;--------


%index
HCLDoXc
短縮記法カーネル実行

%prm
str p1,p2,p3,p4,p5,,,,,
str p1:短縮記法カーネル文字列		[in]
p2以降:引数に渡す実体(arrayやvar intなどの数値)	[in]
%inst

カーネルを実行します。

まずHCLDoXc,HCLDoXi,HCLDoXl,HCLDoXuc,HCLDoXui,HCLDoXul,HCLDoXf,HCLDoXdの8種類の違いですが
c,i,l,uc,ui,ul,f,dがそれぞれchar,int,long,uchar,uint,ulong,float,doubleに対応しています。
この型情報は、基本的にglobal変数の型の解釈として使われます。

■HCLDoX系命令について
第一引数の文字列はOpenCL カーネルコードになります。

ただ普通と違うのは__global float *Aなどと宣言してないことです。
文字列のコードをプラグイン内部で解釈し、1文字の大文字はglobal変数、1文字の小文字はprivate変数として自動的に宣言が追加されます。

A,B,Cとaという1文字変数を短縮記カーネルで使うと
	__global int *A,__global int *B,__global int *C,int a
という宣言がプラグイン内部によって追加されていることになります。


■型については
global変数の場合、HCLDoXiはint型に決定されます。
private変数の場合、HSP側で入力した引数の型がそのまま採用されます。

■並列実行数
global_sizeとlocal_sizeですが、local_sizeは64固定、global_sizeは
グローバル変数Aに対応するBufferのサイズから決定されます。
例えばHCLDoXi命令で、cl　memとしてサイズが256*4=1024byteのcl memをp2に指定した場合
HCLDoXiなのでint型と解釈しておりsizeof(int)=4で割って
global_size=1024/4=256
ということになります。

■小文字変数について(1文字)
例外としては「i」「j」「k」「x」「y」「z」があります。
iは
	int i = get_global_id(0);
というように宣言されており今回の「a」のような使い方はできません。
j,k,x,y,zはprivate変数の宣言には使われず、普通にコード内で
	float x=1.2;
と使うことができます。

■小文字変数について(2文字以上)
「li」という変数は
	int li = get_local_id(0);
と宣言されています。


■大文字変数について(2文字以上)
1文字の大文字はglobal変数と解釈されますが、型については例えばHCLDoXdなら全てdoubleと決め打ちされてしまいます。
そこでglobal変数の型を明示的に、簡単に記せるよう以下のような規則を設けています。
	C0 〜C9 	:	global変数をchar型と解釈
	UC0〜UC9	:	global変数をunsigned char型と解釈
	I0 〜I9 	:	global変数をint型と解釈
	UI0〜UI9	:	global変数をunsigned int型と解釈
	L0 〜L9 	:	global変数をlong long型と解釈
	UL0〜UL9	:	global変数をunsigned long long型と解釈
	F0 〜F9 	:	global変数をfloat型と解釈
	D0 〜D9 	:	global変数をdouble型と解釈


ただしコード内に必ず「A」や「B」などの半角1文字の大文字を使っていないといけません。
それはglobal_sizeを決める際に、その1文字変数が基準になるからです。

HCLDoX系命令に引数を与える順番ですが
１ 半角1文字大文字変数A-Z
２ C D F I L UC UI UL
３ 半角1文字小文字変数a-z
の順になります。


またS0〜S9という文字列も意味を持ちます。
SはShared memoryのSであり
	S0	:	1要素のShared memory
	S1	:	2要素のShared memory
	S2	:	4要素のShared memory
	S3	:	8要素のShared memory
	S4	:	16要素のShared memory
	S5	:	32要素のShared memory
	S6	:	64要素のShared memory
	S7	:	128要素のShared memory
	S8	:	256要素のShared memory
	S9	:	512要素のShared memory

型はHCLDoXfならfloat型と決定されます。


OUTという文字も意味を持ちます。
まずHCLDoX系命令は関数として使うこともでき、新しくcl_memを作成し返すことができます。
カーネルコード側でOUTと書いてあるところが、出力メモリバッファにあたります。
カーネルコード内ではHCLDoXfの場合OUTはfloat型でありメモリのサイズは「A※」と同じものが作られます。

※1文字大文字変数でアルファベット順で最初にくるもの(つまりHCLDoX系命令の第2引数にあたるもの)と同じサイズ、型として作成されるという規則があります。

もしカーネルコード中にOUTを使用していても、HCLDoX系命令を命令形(関数ではなく)として使った場合OUTは一度作成されますがプラグイン内部ですぐ破棄されます。

■デフォルト関数
デフォルト設定されている関数があり
	uint RND(uint s) {
		s*=1847483629;
		s=(s^61)^(s>>16);
		s*=9;
		s=s^(s>>4);
		s*=0x27d4eb2d;
		s=s^(s>>15);
		return s;
	}

これによりRND()関数をデフォルトで使うことができます。

また#defineで下記文言が登録されており使うことができます。
	#define REP(j, n) for(int j = 0; j < (int)(n); j++)
	#define BARRIER barrier(CLK_LOCAL_MEM_FENCE);


■コードの使い回しについて
HCLDoX系命令もHCLCallもHCLCall2も、入力文字列はハッシュ化され、過去に同じ文字列でカーネルを実行したことがあるならば
文字列のコンパイルをスキップしkernel idを使い回すことでオーバーヘッドを極力へらす仕様になっています。
ただし、異なるデバイスidでコンパイルしたものは同じコード文字列であっても別物と解釈します。

したがって同じデバイスで同じコード文字列を何度も実行しても、最初の1回のみ大きなオーバーヘッドがあるだけで
2回目以降の実行はHCLDokrn1,2,3と同じくらい、気にならない程度のオーバーヘッドになるはずです。
例えば1秒に10000回HCLDoX系命令を実行するなら別ですが・・・その場合HCLDokrn1,2,3系命令のほうが明らかにオーバーヘッドという観点では高速になるでしょう。(もちろんGPU上のカーネルコードの実行速度は変わらない)



%href
HCLDoXc
HCLDoXi
HCLDoXl
HCLDoXf
HCLDoXd
HCLDoXuc
HCLDoXui
HCLDoXul
;--------


%index
HCLDoXi
短縮記法カーネル実行

%prm
str p1,p2,p3,p4,p5,,,,,
str p1:短縮記法カーネル文字列		[in]
p2以降:引数に渡す実体(arrayやvar intなどの数値)	[in]
%inst

カーネルを実行します。

まずHCLDoXc,HCLDoXi,HCLDoXl,HCLDoXuc,HCLDoXui,HCLDoXul,HCLDoXf,HCLDoXdの8種類の違いですが
c,i,l,uc,ui,ul,f,dがそれぞれchar,int,long,uchar,uint,ulong,float,doubleに対応しています。
この型情報は、基本的にglobal変数の型の解釈として使われます。

■HCLDoX系命令について
第一引数の文字列はOpenCL カーネルコードになります。

ただ普通と違うのは__global float *Aなどと宣言してないことです。
文字列のコードをプラグイン内部で解釈し、1文字の大文字はglobal変数、1文字の小文字はprivate変数として自動的に宣言が追加されます。

A,B,Cとaという1文字変数を短縮記カーネルで使うと
	__global int *A,__global int *B,__global int *C,int a
という宣言がプラグイン内部によって追加されていることになります。


■型については
global変数の場合、HCLDoXiはint型に決定されます。
private変数の場合、HSP側で入力した引数の型がそのまま採用されます。

■並列実行数
global_sizeとlocal_sizeですが、local_sizeは64固定、global_sizeは
グローバル変数Aに対応するBufferのサイズから決定されます。
例えばHCLDoXi命令で、cl　memとしてサイズが256*4=1024byteのcl memをp2に指定した場合
HCLDoXiなのでint型と解釈しておりsizeof(int)=4で割って
global_size=1024/4=256
ということになります。

■小文字変数について(1文字)
例外としては「i」「j」「k」「x」「y」「z」があります。
iは
	int i = get_global_id(0);
というように宣言されており今回の「a」のような使い方はできません。
j,k,x,y,zはprivate変数の宣言には使われず、普通にコード内で
	float x=1.2;
と使うことができます。

■小文字変数について(2文字以上)
「li」という変数は
	int li = get_local_id(0);
と宣言されています。


■大文字変数について(2文字以上)
1文字の大文字はglobal変数と解釈されますが、型については例えばHCLDoXdなら全てdoubleと決め打ちされてしまいます。
そこでglobal変数の型を明示的に、簡単に記せるよう以下のような規則を設けています。
	C0 〜C9 	:	global変数をchar型と解釈
	UC0〜UC9	:	global変数をunsigned char型と解釈
	I0 〜I9 	:	global変数をint型と解釈
	UI0〜UI9	:	global変数をunsigned int型と解釈
	L0 〜L9 	:	global変数をlong long型と解釈
	UL0〜UL9	:	global変数をunsigned long long型と解釈
	F0 〜F9 	:	global変数をfloat型と解釈
	D0 〜D9 	:	global変数をdouble型と解釈


ただしコード内に必ず「A」や「B」などの半角1文字の大文字を使っていないといけません。
それはglobal_sizeを決める際に、その1文字変数が基準になるからです。

HCLDoX系命令に引数を与える順番ですが
１ 半角1文字大文字変数A-Z
２ C D F I L UC UI UL
３ 半角1文字小文字変数a-z
の順になります。


またS0〜S9という文字列も意味を持ちます。
SはShared memoryのSであり
	S0	:	1要素のShared memory
	S1	:	2要素のShared memory
	S2	:	4要素のShared memory
	S3	:	8要素のShared memory
	S4	:	16要素のShared memory
	S5	:	32要素のShared memory
	S6	:	64要素のShared memory
	S7	:	128要素のShared memory
	S8	:	256要素のShared memory
	S9	:	512要素のShared memory

型はHCLDoXfならfloat型と決定されます。


OUTという文字も意味を持ちます。
まずHCLDoX系命令は関数として使うこともでき、新しくcl_memを作成し返すことができます。
カーネルコード側でOUTと書いてあるところが、出力メモリバッファにあたります。
カーネルコード内ではHCLDoXfの場合OUTはfloat型でありメモリのサイズは「A※」と同じものが作られます。

※1文字大文字変数でアルファベット順で最初にくるもの(つまりHCLDoX系命令の第2引数にあたるもの)と同じサイズ、型として作成されるという規則があります。

もしカーネルコード中にOUTを使用していても、HCLDoX系命令を命令形(関数ではなく)として使った場合OUTは一度作成されますがプラグイン内部ですぐ破棄されます。

■デフォルト関数
デフォルト設定されている関数があり
	uint RND(uint s) {
		s*=1847483629;
		s=(s^61)^(s>>16);
		s*=9;
		s=s^(s>>4);
		s*=0x27d4eb2d;
		s=s^(s>>15);
		return s;
	}

これによりRND()関数をデフォルトで使うことができます。

また#defineで下記文言が登録されており使うことができます。
	#define REP(j, n) for(int j = 0; j < (int)(n); j++)
	#define BARRIER barrier(CLK_LOCAL_MEM_FENCE);


■コードの使い回しについて
HCLDoX系命令もHCLCallもHCLCall2も、入力文字列はハッシュ化され、過去に同じ文字列でカーネルを実行したことがあるならば
文字列のコンパイルをスキップしkernel idを使い回すことでオーバーヘッドを極力へらす仕様になっています。
ただし、異なるデバイスidでコンパイルしたものは同じコード文字列であっても別物と解釈します。

したがって同じデバイスで同じコード文字列を何度も実行しても、最初の1回のみ大きなオーバーヘッドがあるだけで
2回目以降の実行はHCLDokrn1,2,3と同じくらい、気にならない程度のオーバーヘッドになるはずです。
例えば1秒に10000回HCLDoX系命令を実行するなら別ですが・・・その場合HCLDokrn1,2,3系命令のほうが明らかにオーバーヘッドという観点では高速になるでしょう。(もちろんGPU上のカーネルコードの実行速度は変わらない)



%href
HCLDoXc
HCLDoXi
HCLDoXl
HCLDoXf
HCLDoXd
HCLDoXuc
HCLDoXui
HCLDoXul
;--------


%index
HCLDoXl
短縮記法カーネル実行

%prm
str p1,p2,p3,p4,p5,,,,,
str p1:短縮記法カーネル文字列		[in]
p2以降:引数に渡す実体(arrayやvar intなどの数値)	[in]
%inst

カーネルを実行します。

まずHCLDoXc,HCLDoXi,HCLDoXl,HCLDoXuc,HCLDoXui,HCLDoXul,HCLDoXf,HCLDoXdの8種類の違いですが
c,i,l,uc,ui,ul,f,dがそれぞれchar,int,long,uchar,uint,ulong,float,doubleに対応しています。
この型情報は、基本的にglobal変数の型の解釈として使われます。

■HCLDoX系命令について
第一引数の文字列はOpenCL カーネルコードになります。

ただ普通と違うのは__global float *Aなどと宣言してないことです。
文字列のコードをプラグイン内部で解釈し、1文字の大文字はglobal変数、1文字の小文字はprivate変数として自動的に宣言が追加されます。

A,B,Cとaという1文字変数を短縮記カーネルで使うと
	__global int *A,__global int *B,__global int *C,int a
という宣言がプラグイン内部によって追加されていることになります。


■型については
global変数の場合、HCLDoXiはint型に決定されます。
private変数の場合、HSP側で入力した引数の型がそのまま採用されます。

■並列実行数
global_sizeとlocal_sizeですが、local_sizeは64固定、global_sizeは
グローバル変数Aに対応するBufferのサイズから決定されます。
例えばHCLDoXi命令で、cl　memとしてサイズが256*4=1024byteのcl memをp2に指定した場合
HCLDoXiなのでint型と解釈しておりsizeof(int)=4で割って
global_size=1024/4=256
ということになります。

■小文字変数について(1文字)
例外としては「i」「j」「k」「x」「y」「z」があります。
iは
	int i = get_global_id(0);
というように宣言されており今回の「a」のような使い方はできません。
j,k,x,y,zはprivate変数の宣言には使われず、普通にコード内で
	float x=1.2;
と使うことができます。

■小文字変数について(2文字以上)
「li」という変数は
	int li = get_local_id(0);
と宣言されています。


■大文字変数について(2文字以上)
1文字の大文字はglobal変数と解釈されますが、型については例えばHCLDoXdなら全てdoubleと決め打ちされてしまいます。
そこでglobal変数の型を明示的に、簡単に記せるよう以下のような規則を設けています。
	C0 〜C9 	:	global変数をchar型と解釈
	UC0〜UC9	:	global変数をunsigned char型と解釈
	I0 〜I9 	:	global変数をint型と解釈
	UI0〜UI9	:	global変数をunsigned int型と解釈
	L0 〜L9 	:	global変数をlong long型と解釈
	UL0〜UL9	:	global変数をunsigned long long型と解釈
	F0 〜F9 	:	global変数をfloat型と解釈
	D0 〜D9 	:	global変数をdouble型と解釈


ただしコード内に必ず「A」や「B」などの半角1文字の大文字を使っていないといけません。
それはglobal_sizeを決める際に、その1文字変数が基準になるからです。

HCLDoX系命令に引数を与える順番ですが
１ 半角1文字大文字変数A-Z
２ C D F I L UC UI UL
３ 半角1文字小文字変数a-z
の順になります。


またS0〜S9という文字列も意味を持ちます。
SはShared memoryのSであり
	S0	:	1要素のShared memory
	S1	:	2要素のShared memory
	S2	:	4要素のShared memory
	S3	:	8要素のShared memory
	S4	:	16要素のShared memory
	S5	:	32要素のShared memory
	S6	:	64要素のShared memory
	S7	:	128要素のShared memory
	S8	:	256要素のShared memory
	S9	:	512要素のShared memory

型はHCLDoXfならfloat型と決定されます。


OUTという文字も意味を持ちます。
まずHCLDoX系命令は関数として使うこともでき、新しくcl_memを作成し返すことができます。
カーネルコード側でOUTと書いてあるところが、出力メモリバッファにあたります。
カーネルコード内ではHCLDoXfの場合OUTはfloat型でありメモリのサイズは「A※」と同じものが作られます。

※1文字大文字変数でアルファベット順で最初にくるもの(つまりHCLDoX系命令の第2引数にあたるもの)と同じサイズ、型として作成されるという規則があります。

もしカーネルコード中にOUTを使用していても、HCLDoX系命令を命令形(関数ではなく)として使った場合OUTは一度作成されますがプラグイン内部ですぐ破棄されます。

■デフォルト関数
デフォルト設定されている関数があり
	uint RND(uint s) {
		s*=1847483629;
		s=(s^61)^(s>>16);
		s*=9;
		s=s^(s>>4);
		s*=0x27d4eb2d;
		s=s^(s>>15);
		return s;
	}

これによりRND()関数をデフォルトで使うことができます。

また#defineで下記文言が登録されており使うことができます。
	#define REP(j, n) for(int j = 0; j < (int)(n); j++)
	#define BARRIER barrier(CLK_LOCAL_MEM_FENCE);


■コードの使い回しについて
HCLDoX系命令もHCLCallもHCLCall2も、入力文字列はハッシュ化され、過去に同じ文字列でカーネルを実行したことがあるならば
文字列のコンパイルをスキップしkernel idを使い回すことでオーバーヘッドを極力へらす仕様になっています。
ただし、異なるデバイスidでコンパイルしたものは同じコード文字列であっても別物と解釈します。

したがって同じデバイスで同じコード文字列を何度も実行しても、最初の1回のみ大きなオーバーヘッドがあるだけで
2回目以降の実行はHCLDokrn1,2,3と同じくらい、気にならない程度のオーバーヘッドになるはずです。
例えば1秒に10000回HCLDoX系命令を実行するなら別ですが・・・その場合HCLDokrn1,2,3系命令のほうが明らかにオーバーヘッドという観点では高速になるでしょう。(もちろんGPU上のカーネルコードの実行速度は変わらない)



%href
HCLDoXc
HCLDoXi
HCLDoXl
HCLDoXf
HCLDoXd
HCLDoXuc
HCLDoXui
HCLDoXul
;--------


%index
HCLDoXf
短縮記法カーネル実行

%prm
str p1,p2,p3,p4,p5,,,,,
str p1:短縮記法カーネル文字列		[in]
p2以降:引数に渡す実体(arrayやvar intなどの数値)	[in]
%inst

カーネルを実行します。

まずHCLDoXc,HCLDoXi,HCLDoXl,HCLDoXuc,HCLDoXui,HCLDoXul,HCLDoXf,HCLDoXdの8種類の違いですが
c,i,l,uc,ui,ul,f,dがそれぞれchar,int,long,uchar,uint,ulong,float,doubleに対応しています。
この型情報は、基本的にglobal変数の型の解釈として使われます。

■HCLDoX系命令について
第一引数の文字列はOpenCL カーネルコードになります。

ただ普通と違うのは__global float *Aなどと宣言してないことです。
文字列のコードをプラグイン内部で解釈し、1文字の大文字はglobal変数、1文字の小文字はprivate変数として自動的に宣言が追加されます。

A,B,Cとaという1文字変数を短縮記カーネルで使うと
	__global int *A,__global int *B,__global int *C,int a
という宣言がプラグイン内部によって追加されていることになります。


■型については
global変数の場合、HCLDoXiはint型に決定されます。
private変数の場合、HSP側で入力した引数の型がそのまま採用されます。

■並列実行数
global_sizeとlocal_sizeですが、local_sizeは64固定、global_sizeは
グローバル変数Aに対応するBufferのサイズから決定されます。
例えばHCLDoXi命令で、cl　memとしてサイズが256*4=1024byteのcl memをp2に指定した場合
HCLDoXiなのでint型と解釈しておりsizeof(int)=4で割って
global_size=1024/4=256
ということになります。

■小文字変数について(1文字)
例外としては「i」「j」「k」「x」「y」「z」があります。
iは
	int i = get_global_id(0);
というように宣言されており今回の「a」のような使い方はできません。
j,k,x,y,zはprivate変数の宣言には使われず、普通にコード内で
	float x=1.2;
と使うことができます。

■小文字変数について(2文字以上)
「li」という変数は
	int li = get_local_id(0);
と宣言されています。


■大文字変数について(2文字以上)
1文字の大文字はglobal変数と解釈されますが、型については例えばHCLDoXdなら全てdoubleと決め打ちされてしまいます。
そこでglobal変数の型を明示的に、簡単に記せるよう以下のような規則を設けています。
	C0 〜C9 	:	global変数をchar型と解釈
	UC0〜UC9	:	global変数をunsigned char型と解釈
	I0 〜I9 	:	global変数をint型と解釈
	UI0〜UI9	:	global変数をunsigned int型と解釈
	L0 〜L9 	:	global変数をlong long型と解釈
	UL0〜UL9	:	global変数をunsigned long long型と解釈
	F0 〜F9 	:	global変数をfloat型と解釈
	D0 〜D9 	:	global変数をdouble型と解釈


ただしコード内に必ず「A」や「B」などの半角1文字の大文字を使っていないといけません。
それはglobal_sizeを決める際に、その1文字変数が基準になるからです。

HCLDoX系命令に引数を与える順番ですが
１ 半角1文字大文字変数A-Z
２ C D F I L UC UI UL
３ 半角1文字小文字変数a-z
の順になります。


またS0〜S9という文字列も意味を持ちます。
SはShared memoryのSであり
	S0	:	1要素のShared memory
	S1	:	2要素のShared memory
	S2	:	4要素のShared memory
	S3	:	8要素のShared memory
	S4	:	16要素のShared memory
	S5	:	32要素のShared memory
	S6	:	64要素のShared memory
	S7	:	128要素のShared memory
	S8	:	256要素のShared memory
	S9	:	512要素のShared memory

型はHCLDoXfならfloat型と決定されます。


OUTという文字も意味を持ちます。
まずHCLDoX系命令は関数として使うこともでき、新しくcl_memを作成し返すことができます。
カーネルコード側でOUTと書いてあるところが、出力メモリバッファにあたります。
カーネルコード内ではHCLDoXfの場合OUTはfloat型でありメモリのサイズは「A※」と同じものが作られます。

※1文字大文字変数でアルファベット順で最初にくるもの(つまりHCLDoX系命令の第2引数にあたるもの)と同じサイズ、型として作成されるという規則があります。

もしカーネルコード中にOUTを使用していても、HCLDoX系命令を命令形(関数ではなく)として使った場合OUTは一度作成されますがプラグイン内部ですぐ破棄されます。

■デフォルト関数
デフォルト設定されている関数があり
	uint RND(uint s) {
		s*=1847483629;
		s=(s^61)^(s>>16);
		s*=9;
		s=s^(s>>4);
		s*=0x27d4eb2d;
		s=s^(s>>15);
		return s;
	}

これによりRND()関数をデフォルトで使うことができます。

また#defineで下記文言が登録されており使うことができます。
	#define REP(j, n) for(int j = 0; j < (int)(n); j++)
	#define BARRIER barrier(CLK_LOCAL_MEM_FENCE);


■コードの使い回しについて
HCLDoX系命令もHCLCallもHCLCall2も、入力文字列はハッシュ化され、過去に同じ文字列でカーネルを実行したことがあるならば
文字列のコンパイルをスキップしkernel idを使い回すことでオーバーヘッドを極力へらす仕様になっています。
ただし、異なるデバイスidでコンパイルしたものは同じコード文字列であっても別物と解釈します。

したがって同じデバイスで同じコード文字列を何度も実行しても、最初の1回のみ大きなオーバーヘッドがあるだけで
2回目以降の実行はHCLDokrn1,2,3と同じくらい、気にならない程度のオーバーヘッドになるはずです。
例えば1秒に10000回HCLDoX系命令を実行するなら別ですが・・・その場合HCLDokrn1,2,3系命令のほうが明らかにオーバーヘッドという観点では高速になるでしょう。(もちろんGPU上のカーネルコードの実行速度は変わらない)



%href
HCLDoXc
HCLDoXi
HCLDoXl
HCLDoXf
HCLDoXd
HCLDoXuc
HCLDoXui
HCLDoXul
;--------


%index
HCLDoXd
短縮記法カーネル実行

%prm
str p1,p2,p3,p4,p5,,,,,
str p1:短縮記法カーネル文字列		[in]
p2以降:引数に渡す実体(arrayやvar intなどの数値)	[in]
%inst

カーネルを実行します。

まずHCLDoXc,HCLDoXi,HCLDoXl,HCLDoXuc,HCLDoXui,HCLDoXul,HCLDoXf,HCLDoXdの8種類の違いですが
c,i,l,uc,ui,ul,f,dがそれぞれchar,int,long,uchar,uint,ulong,float,doubleに対応しています。
この型情報は、基本的にglobal変数の型の解釈として使われます。

■HCLDoX系命令について
第一引数の文字列はOpenCL カーネルコードになります。

ただ普通と違うのは__global float *Aなどと宣言してないことです。
文字列のコードをプラグイン内部で解釈し、1文字の大文字はglobal変数、1文字の小文字はprivate変数として自動的に宣言が追加されます。

A,B,Cとaという1文字変数を短縮記カーネルで使うと
	__global int *A,__global int *B,__global int *C,int a
という宣言がプラグイン内部によって追加されていることになります。


■型については
global変数の場合、HCLDoXiはint型に決定されます。
private変数の場合、HSP側で入力した引数の型がそのまま採用されます。

■並列実行数
global_sizeとlocal_sizeですが、local_sizeは64固定、global_sizeは
グローバル変数Aに対応するBufferのサイズから決定されます。
例えばHCLDoXi命令で、cl　memとしてサイズが256*4=1024byteのcl memをp2に指定した場合
HCLDoXiなのでint型と解釈しておりsizeof(int)=4で割って
global_size=1024/4=256
ということになります。

■小文字変数について(1文字)
例外としては「i」「j」「k」「x」「y」「z」があります。
iは
	int i = get_global_id(0);
というように宣言されており今回の「a」のような使い方はできません。
j,k,x,y,zはprivate変数の宣言には使われず、普通にコード内で
	float x=1.2;
と使うことができます。

■小文字変数について(2文字以上)
「li」という変数は
	int li = get_local_id(0);
と宣言されています。


■大文字変数について(2文字以上)
1文字の大文字はglobal変数と解釈されますが、型については例えばHCLDoXdなら全てdoubleと決め打ちされてしまいます。
そこでglobal変数の型を明示的に、簡単に記せるよう以下のような規則を設けています。
	C0 〜C9 	:	global変数をchar型と解釈
	UC0〜UC9	:	global変数をunsigned char型と解釈
	I0 〜I9 	:	global変数をint型と解釈
	UI0〜UI9	:	global変数をunsigned int型と解釈
	L0 〜L9 	:	global変数をlong long型と解釈
	UL0〜UL9	:	global変数をunsigned long long型と解釈
	F0 〜F9 	:	global変数をfloat型と解釈
	D0 〜D9 	:	global変数をdouble型と解釈


ただしコード内に必ず「A」や「B」などの半角1文字の大文字を使っていないといけません。
それはglobal_sizeを決める際に、その1文字変数が基準になるからです。

HCLDoX系命令に引数を与える順番ですが
１ 半角1文字大文字変数A-Z
２ C D F I L UC UI UL
３ 半角1文字小文字変数a-z
の順になります。


またS0〜S9という文字列も意味を持ちます。
SはShared memoryのSであり
	S0	:	1要素のShared memory
	S1	:	2要素のShared memory
	S2	:	4要素のShared memory
	S3	:	8要素のShared memory
	S4	:	16要素のShared memory
	S5	:	32要素のShared memory
	S6	:	64要素のShared memory
	S7	:	128要素のShared memory
	S8	:	256要素のShared memory
	S9	:	512要素のShared memory

型はHCLDoXfならfloat型と決定されます。


OUTという文字も意味を持ちます。
まずHCLDoX系命令は関数として使うこともでき、新しくcl_memを作成し返すことができます。
カーネルコード側でOUTと書いてあるところが、出力メモリバッファにあたります。
カーネルコード内ではHCLDoXfの場合OUTはfloat型でありメモリのサイズは「A※」と同じものが作られます。

※1文字大文字変数でアルファベット順で最初にくるもの(つまりHCLDoX系命令の第2引数にあたるもの)と同じサイズ、型として作成されるという規則があります。

もしカーネルコード中にOUTを使用していても、HCLDoX系命令を命令形(関数ではなく)として使った場合OUTは一度作成されますがプラグイン内部ですぐ破棄されます。

■デフォルト関数
デフォルト設定されている関数があり
	uint RND(uint s) {
		s*=1847483629;
		s=(s^61)^(s>>16);
		s*=9;
		s=s^(s>>4);
		s*=0x27d4eb2d;
		s=s^(s>>15);
		return s;
	}

これによりRND()関数をデフォルトで使うことができます。

また#defineで下記文言が登録されており使うことができます。
	#define REP(j, n) for(int j = 0; j < (int)(n); j++)
	#define BARRIER barrier(CLK_LOCAL_MEM_FENCE);


■コードの使い回しについて
HCLDoX系命令もHCLCallもHCLCall2も、入力文字列はハッシュ化され、過去に同じ文字列でカーネルを実行したことがあるならば
文字列のコンパイルをスキップしkernel idを使い回すことでオーバーヘッドを極力へらす仕様になっています。
ただし、異なるデバイスidでコンパイルしたものは同じコード文字列であっても別物と解釈します。

したがって同じデバイスで同じコード文字列を何度も実行しても、最初の1回のみ大きなオーバーヘッドがあるだけで
2回目以降の実行はHCLDokrn1,2,3と同じくらい、気にならない程度のオーバーヘッドになるはずです。
例えば1秒に10000回HCLDoX系命令を実行するなら別ですが・・・その場合HCLDokrn1,2,3系命令のほうが明らかにオーバーヘッドという観点では高速になるでしょう。(もちろんGPU上のカーネルコードの実行速度は変わらない)



%href
HCLDoXc
HCLDoXi
HCLDoXl
HCLDoXf
HCLDoXd
HCLDoXuc
HCLDoXui
HCLDoXul
;--------


%index
HCLDoXuc
短縮記法カーネル実行

%prm
str p1,p2,p3,p4,p5,,,,,
str p1:短縮記法カーネル文字列		[in]
p2以降:引数に渡す実体(arrayやvar intなどの数値)	[in]
%inst

カーネルを実行します。

まずHCLDoXc,HCLDoXi,HCLDoXl,HCLDoXuc,HCLDoXui,HCLDoXul,HCLDoXf,HCLDoXdの8種類の違いですが
c,i,l,uc,ui,ul,f,dがそれぞれchar,int,long,uchar,uint,ulong,float,doubleに対応しています。
この型情報は、基本的にglobal変数の型の解釈として使われます。

■HCLDoX系命令について
第一引数の文字列はOpenCL カーネルコードになります。

ただ普通と違うのは__global float *Aなどと宣言してないことです。
文字列のコードをプラグイン内部で解釈し、1文字の大文字はglobal変数、1文字の小文字はprivate変数として自動的に宣言が追加されます。

A,B,Cとaという1文字変数を短縮記カーネルで使うと
	__global int *A,__global int *B,__global int *C,int a
という宣言がプラグイン内部によって追加されていることになります。


■型については
global変数の場合、HCLDoXiはint型に決定されます。
private変数の場合、HSP側で入力した引数の型がそのまま採用されます。

■並列実行数
global_sizeとlocal_sizeですが、local_sizeは64固定、global_sizeは
グローバル変数Aに対応するBufferのサイズから決定されます。
例えばHCLDoXi命令で、cl　memとしてサイズが256*4=1024byteのcl memをp2に指定した場合
HCLDoXiなのでint型と解釈しておりsizeof(int)=4で割って
global_size=1024/4=256
ということになります。

■小文字変数について(1文字)
例外としては「i」「j」「k」「x」「y」「z」があります。
iは
	int i = get_global_id(0);
というように宣言されており今回の「a」のような使い方はできません。
j,k,x,y,zはprivate変数の宣言には使われず、普通にコード内で
	float x=1.2;
と使うことができます。

■小文字変数について(2文字以上)
「li」という変数は
	int li = get_local_id(0);
と宣言されています。


■大文字変数について(2文字以上)
1文字の大文字はglobal変数と解釈されますが、型については例えばHCLDoXdなら全てdoubleと決め打ちされてしまいます。
そこでglobal変数の型を明示的に、簡単に記せるよう以下のような規則を設けています。
	C0 〜C9 	:	global変数をchar型と解釈
	UC0〜UC9	:	global変数をunsigned char型と解釈
	I0 〜I9 	:	global変数をint型と解釈
	UI0〜UI9	:	global変数をunsigned int型と解釈
	L0 〜L9 	:	global変数をlong long型と解釈
	UL0〜UL9	:	global変数をunsigned long long型と解釈
	F0 〜F9 	:	global変数をfloat型と解釈
	D0 〜D9 	:	global変数をdouble型と解釈


ただしコード内に必ず「A」や「B」などの半角1文字の大文字を使っていないといけません。
それはglobal_sizeを決める際に、その1文字変数が基準になるからです。

HCLDoX系命令に引数を与える順番ですが
１ 半角1文字大文字変数A-Z
２ C D F I L UC UI UL
３ 半角1文字小文字変数a-z
の順になります。


またS0〜S9という文字列も意味を持ちます。
SはShared memoryのSであり
	S0	:	1要素のShared memory
	S1	:	2要素のShared memory
	S2	:	4要素のShared memory
	S3	:	8要素のShared memory
	S4	:	16要素のShared memory
	S5	:	32要素のShared memory
	S6	:	64要素のShared memory
	S7	:	128要素のShared memory
	S8	:	256要素のShared memory
	S9	:	512要素のShared memory

型はHCLDoXfならfloat型と決定されます。


OUTという文字も意味を持ちます。
まずHCLDoX系命令は関数として使うこともでき、新しくcl_memを作成し返すことができます。
カーネルコード側でOUTと書いてあるところが、出力メモリバッファにあたります。
カーネルコード内ではHCLDoXfの場合OUTはfloat型でありメモリのサイズは「A※」と同じものが作られます。

※1文字大文字変数でアルファベット順で最初にくるもの(つまりHCLDoX系命令の第2引数にあたるもの)と同じサイズ、型として作成されるという規則があります。

もしカーネルコード中にOUTを使用していても、HCLDoX系命令を命令形(関数ではなく)として使った場合OUTは一度作成されますがプラグイン内部ですぐ破棄されます。

■デフォルト関数
デフォルト設定されている関数があり
	uint RND(uint s) {
		s*=1847483629;
		s=(s^61)^(s>>16);
		s*=9;
		s=s^(s>>4);
		s*=0x27d4eb2d;
		s=s^(s>>15);
		return s;
	}

これによりRND()関数をデフォルトで使うことができます。

また#defineで下記文言が登録されており使うことができます。
	#define REP(j, n) for(int j = 0; j < (int)(n); j++)
	#define BARRIER barrier(CLK_LOCAL_MEM_FENCE);


■コードの使い回しについて
HCLDoX系命令もHCLCallもHCLCall2も、入力文字列はハッシュ化され、過去に同じ文字列でカーネルを実行したことがあるならば
文字列のコンパイルをスキップしkernel idを使い回すことでオーバーヘッドを極力へらす仕様になっています。
ただし、異なるデバイスidでコンパイルしたものは同じコード文字列であっても別物と解釈します。

したがって同じデバイスで同じコード文字列を何度も実行しても、最初の1回のみ大きなオーバーヘッドがあるだけで
2回目以降の実行はHCLDokrn1,2,3と同じくらい、気にならない程度のオーバーヘッドになるはずです。
例えば1秒に10000回HCLDoX系命令を実行するなら別ですが・・・その場合HCLDokrn1,2,3系命令のほうが明らかにオーバーヘッドという観点では高速になるでしょう。(もちろんGPU上のカーネルコードの実行速度は変わらない)



%href
HCLDoXc
HCLDoXi
HCLDoXl
HCLDoXf
HCLDoXd
HCLDoXuc
HCLDoXui
HCLDoXul
;--------


%index
HCLDoXui
短縮記法カーネル実行

%prm
str p1,p2,p3,p4,p5,,,,,
str p1:短縮記法カーネル文字列		[in]
p2以降:引数に渡す実体(arrayやvar intなどの数値)	[in]
%inst

カーネルを実行します。

まずHCLDoXc,HCLDoXi,HCLDoXl,HCLDoXuc,HCLDoXui,HCLDoXul,HCLDoXf,HCLDoXdの8種類の違いですが
c,i,l,uc,ui,ul,f,dがそれぞれchar,int,long,uchar,uint,ulong,float,doubleに対応しています。
この型情報は、基本的にglobal変数の型の解釈として使われます。

■HCLDoX系命令について
第一引数の文字列はOpenCL カーネルコードになります。

ただ普通と違うのは__global float *Aなどと宣言してないことです。
文字列のコードをプラグイン内部で解釈し、1文字の大文字はglobal変数、1文字の小文字はprivate変数として自動的に宣言が追加されます。

A,B,Cとaという1文字変数を短縮記カーネルで使うと
	__global int *A,__global int *B,__global int *C,int a
という宣言がプラグイン内部によって追加されていることになります。


■型については
global変数の場合、HCLDoXiはint型に決定されます。
private変数の場合、HSP側で入力した引数の型がそのまま採用されます。

■並列実行数
global_sizeとlocal_sizeですが、local_sizeは64固定、global_sizeは
グローバル変数Aに対応するBufferのサイズから決定されます。
例えばHCLDoXi命令で、cl　memとしてサイズが256*4=1024byteのcl memをp2に指定した場合
HCLDoXiなのでint型と解釈しておりsizeof(int)=4で割って
global_size=1024/4=256
ということになります。

■小文字変数について(1文字)
例外としては「i」「j」「k」「x」「y」「z」があります。
iは
	int i = get_global_id(0);
というように宣言されており今回の「a」のような使い方はできません。
j,k,x,y,zはprivate変数の宣言には使われず、普通にコード内で
	float x=1.2;
と使うことができます。

■小文字変数について(2文字以上)
「li」という変数は
	int li = get_local_id(0);
と宣言されています。


■大文字変数について(2文字以上)
1文字の大文字はglobal変数と解釈されますが、型については例えばHCLDoXdなら全てdoubleと決め打ちされてしまいます。
そこでglobal変数の型を明示的に、簡単に記せるよう以下のような規則を設けています。
	C0 〜C9 	:	global変数をchar型と解釈
	UC0〜UC9	:	global変数をunsigned char型と解釈
	I0 〜I9 	:	global変数をint型と解釈
	UI0〜UI9	:	global変数をunsigned int型と解釈
	L0 〜L9 	:	global変数をlong long型と解釈
	UL0〜UL9	:	global変数をunsigned long long型と解釈
	F0 〜F9 	:	global変数をfloat型と解釈
	D0 〜D9 	:	global変数をdouble型と解釈


ただしコード内に必ず「A」や「B」などの半角1文字の大文字を使っていないといけません。
それはglobal_sizeを決める際に、その1文字変数が基準になるからです。

HCLDoX系命令に引数を与える順番ですが
１ 半角1文字大文字変数A-Z
２ C D F I L UC UI UL
３ 半角1文字小文字変数a-z
の順になります。


またS0〜S9という文字列も意味を持ちます。
SはShared memoryのSであり
	S0	:	1要素のShared memory
	S1	:	2要素のShared memory
	S2	:	4要素のShared memory
	S3	:	8要素のShared memory
	S4	:	16要素のShared memory
	S5	:	32要素のShared memory
	S6	:	64要素のShared memory
	S7	:	128要素のShared memory
	S8	:	256要素のShared memory
	S9	:	512要素のShared memory

型はHCLDoXfならfloat型と決定されます。


OUTという文字も意味を持ちます。
まずHCLDoX系命令は関数として使うこともでき、新しくcl_memを作成し返すことができます。
カーネルコード側でOUTと書いてあるところが、出力メモリバッファにあたります。
カーネルコード内ではHCLDoXfの場合OUTはfloat型でありメモリのサイズは「A※」と同じものが作られます。

※1文字大文字変数でアルファベット順で最初にくるもの(つまりHCLDoX系命令の第2引数にあたるもの)と同じサイズ、型として作成されるという規則があります。

もしカーネルコード中にOUTを使用していても、HCLDoX系命令を命令形(関数ではなく)として使った場合OUTは一度作成されますがプラグイン内部ですぐ破棄されます。

■デフォルト関数
デフォルト設定されている関数があり
	uint RND(uint s) {
		s*=1847483629;
		s=(s^61)^(s>>16);
		s*=9;
		s=s^(s>>4);
		s*=0x27d4eb2d;
		s=s^(s>>15);
		return s;
	}

これによりRND()関数をデフォルトで使うことができます。

また#defineで下記文言が登録されており使うことができます。
	#define REP(j, n) for(int j = 0; j < (int)(n); j++)
	#define BARRIER barrier(CLK_LOCAL_MEM_FENCE);


■コードの使い回しについて
HCLDoX系命令もHCLCallもHCLCall2も、入力文字列はハッシュ化され、過去に同じ文字列でカーネルを実行したことがあるならば
文字列のコンパイルをスキップしkernel idを使い回すことでオーバーヘッドを極力へらす仕様になっています。
ただし、異なるデバイスidでコンパイルしたものは同じコード文字列であっても別物と解釈します。

したがって同じデバイスで同じコード文字列を何度も実行しても、最初の1回のみ大きなオーバーヘッドがあるだけで
2回目以降の実行はHCLDokrn1,2,3と同じくらい、気にならない程度のオーバーヘッドになるはずです。
例えば1秒に10000回HCLDoX系命令を実行するなら別ですが・・・その場合HCLDokrn1,2,3系命令のほうが明らかにオーバーヘッドという観点では高速になるでしょう。(もちろんGPU上のカーネルコードの実行速度は変わらない)



%href
HCLDoXc
HCLDoXi
HCLDoXl
HCLDoXf
HCLDoXd
HCLDoXuc
HCLDoXui
HCLDoXul
;--------


%index
HCLDoXul
短縮記法カーネル実行

%prm
str p1,p2,p3,p4,p5,,,,,
str p1:短縮記法カーネル文字列		[in]
p2以降:引数に渡す実体(arrayやvar intなどの数値)	[in]
%inst

カーネルを実行します。

まずHCLDoXc,HCLDoXi,HCLDoXl,HCLDoXuc,HCLDoXui,HCLDoXul,HCLDoXf,HCLDoXdの8種類の違いですが
c,i,l,uc,ui,ul,f,dがそれぞれchar,int,long,uchar,uint,ulong,float,doubleに対応しています。
この型情報は、基本的にglobal変数の型の解釈として使われます。

■HCLDoX系命令について
第一引数の文字列はOpenCL カーネルコードになります。

ただ普通と違うのは__global float *Aなどと宣言してないことです。
文字列のコードをプラグイン内部で解釈し、1文字の大文字はglobal変数、1文字の小文字はprivate変数として自動的に宣言が追加されます。

A,B,Cとaという1文字変数を短縮記カーネルで使うと
	__global int *A,__global int *B,__global int *C,int a
という宣言がプラグイン内部によって追加されていることになります。


■型については
global変数の場合、HCLDoXiはint型に決定されます。
private変数の場合、HSP側で入力した引数の型がそのまま採用されます。

■並列実行数
global_sizeとlocal_sizeですが、local_sizeは64固定、global_sizeは
グローバル変数Aに対応するBufferのサイズから決定されます。
例えばHCLDoXi命令で、cl　memとしてサイズが256*4=1024byteのcl memをp2に指定した場合
HCLDoXiなのでint型と解釈しておりsizeof(int)=4で割って
global_size=1024/4=256
ということになります。

■小文字変数について(1文字)
例外としては「i」「j」「k」「x」「y」「z」があります。
iは
	int i = get_global_id(0);
というように宣言されており今回の「a」のような使い方はできません。
j,k,x,y,zはprivate変数の宣言には使われず、普通にコード内で
	float x=1.2;
と使うことができます。

■小文字変数について(2文字以上)
「li」という変数は
	int li = get_local_id(0);
と宣言されています。


■大文字変数について(2文字以上)
1文字の大文字はglobal変数と解釈されますが、型については例えばHCLDoXdなら全てdoubleと決め打ちされてしまいます。
そこでglobal変数の型を明示的に、簡単に記せるよう以下のような規則を設けています。
	C0 〜C9 	:	global変数をchar型と解釈
	UC0〜UC9	:	global変数をunsigned char型と解釈
	I0 〜I9 	:	global変数をint型と解釈
	UI0〜UI9	:	global変数をunsigned int型と解釈
	L0 〜L9 	:	global変数をlong long型と解釈
	UL0〜UL9	:	global変数をunsigned long long型と解釈
	F0 〜F9 	:	global変数をfloat型と解釈
	D0 〜D9 	:	global変数をdouble型と解釈


ただしコード内に必ず「A」や「B」などの半角1文字の大文字を使っていないといけません。
それはglobal_sizeを決める際に、その1文字変数が基準になるからです。

HCLDoX系命令に引数を与える順番ですが
１ 半角1文字大文字変数A-Z
２ C D F I L UC UI UL
３ 半角1文字小文字変数a-z
の順になります。


またS0〜S9という文字列も意味を持ちます。
SはShared memoryのSであり
	S0	:	1要素のShared memory
	S1	:	2要素のShared memory
	S2	:	4要素のShared memory
	S3	:	8要素のShared memory
	S4	:	16要素のShared memory
	S5	:	32要素のShared memory
	S6	:	64要素のShared memory
	S7	:	128要素のShared memory
	S8	:	256要素のShared memory
	S9	:	512要素のShared memory

型はHCLDoXfならfloat型と決定されます。


OUTという文字も意味を持ちます。
まずHCLDoX系命令は関数として使うこともでき、新しくcl_memを作成し返すことができます。
カーネルコード側でOUTと書いてあるところが、出力メモリバッファにあたります。
カーネルコード内ではHCLDoXfの場合OUTはfloat型でありメモリのサイズは「A※」と同じものが作られます。

※1文字大文字変数でアルファベット順で最初にくるもの(つまりHCLDoX系命令の第2引数にあたるもの)と同じサイズ、型として作成されるという規則があります。

もしカーネルコード中にOUTを使用していても、HCLDoX系命令を命令形(関数ではなく)として使った場合OUTは一度作成されますがプラグイン内部ですぐ破棄されます。

■デフォルト関数
デフォルト設定されている関数があり
	uint RND(uint s) {
		s*=1847483629;
		s=(s^61)^(s>>16);
		s*=9;
		s=s^(s>>4);
		s*=0x27d4eb2d;
		s=s^(s>>15);
		return s;
	}

これによりRND()関数をデフォルトで使うことができます。

また#defineで下記文言が登録されており使うことができます。
	#define REP(j, n) for(int j = 0; j < (int)(n); j++)
	#define BARRIER barrier(CLK_LOCAL_MEM_FENCE);


■コードの使い回しについて
HCLDoX系命令もHCLCallもHCLCall2も、入力文字列はハッシュ化され、過去に同じ文字列でカーネルを実行したことがあるならば
文字列のコンパイルをスキップしkernel idを使い回すことでオーバーヘッドを極力へらす仕様になっています。
ただし、異なるデバイスidでコンパイルしたものは同じコード文字列であっても別物と解釈します。

したがって同じデバイスで同じコード文字列を何度も実行しても、最初の1回のみ大きなオーバーヘッドがあるだけで
2回目以降の実行はHCLDokrn1,2,3と同じくらい、気にならない程度のオーバーヘッドになるはずです。
例えば1秒に10000回HCLDoX系命令を実行するなら別ですが・・・その場合HCLDokrn1,2,3系命令のほうが明らかにオーバーヘッドという観点では高速になるでしょう。(もちろんGPU上のカーネルコードの実行速度は変わらない)



%href
HCLDoXc
HCLDoXi
HCLDoXl
HCLDoXf
HCLDoXd
HCLDoXuc
HCLDoXui
HCLDoXul

;----------------

%index
HCLBLAS_sgemm
sgemmカーネル実行 C=A*B

%prm
int p1,int p2,int p3,int p4,int p5,int p6
int p1:[C]CL_mem_object id			[in]
int p2:[A]CL_mem_object id			[in]
int p3:[B]CL_mem_object id			[in]
int p4:[C]転置フラグ,省略可			[in]
int p5:[A]転置フラグ,省略可			[in]
int p6:[B]転置フラグ,省略可			[in]

%inst

C=A×Bの行列行列積を行ないます。
プラグイン内部に埋め込まれているカーネルで実行されます。

転置フラグは0で転置なし、1で転置ありになります。
A,BにはHCLBLAS_Set2DShape
であらかじめcl memに行、列のサイズを設定しておく必要があります。

命令として実行することもできますが、関数として実行することもできます。
その際は
C=HCLBLAS_sgemm(A,B,0,0,0)
のように使います。
この場合、Cには新たにHCLCreateBufferで確保されたmem idが返されます。


%href
HCLBLAS_Set2DShape
HCLBLAS_Get2DShape
HCLBLAS_sgemm
HCLBLAS_dgemm
;----------------

%index
HCLBLAS_dgemm
dgemmカーネル実行 C=A*B

%prm
int p1,int p2,int p3,int p4,int p5,int p6
int p1:[C]CL_mem_object id			[in]
int p2:[A]CL_mem_object id			[in]
int p3:[B]CL_mem_object id			[in]
int p4:[C]転置フラグ,省略可			[in]
int p5:[A]転置フラグ,省略可			[in]
int p6:[B]転置フラグ,省略可			[in]

%inst

C=A×Bの行列行列積を行ないます。
プラグイン内部に埋め込まれているカーネルで実行されます。

転置フラグは0で転置なし、1で転置ありになります。
A,BにはHCLBLAS_Set2DShape
であらかじめcl memに行、列のサイズを設定しておく必要があります。

命令として実行することもできますが、関数として実行することもできます。
その際は
C=HCLBLAS_sgemm(A,B,0,0,0)
のように使います。
この場合、Cには新たにHCLCreateBufferで確保されたmem idが返されます。


%href
HCLBLAS_Set2DShape
HCLBLAS_Get2DShape
HCLBLAS_sgemm
HCLBLAS_dgemm
;--------
%index
HCLBLAS_Set2DShape
cl mem idに行と列を設定する

%prm
int p1,int p2,int p3
int p1:CL_mem_object id			[in]
int p2:行(raw)の数			[in]
int p3:列(col)の数			[in]

%inst
cl memに行、列のサイズを設定します。
HCLBLAS_sgemm命令やHCLBLAS_dgemm命令を使う際にあらかじめ行、列を正しく設定しておく必要があります。


%href
HCLBLAS_Set2DShape
HCLBLAS_Get2DShape
HCLBLAS_sgemm
HCLBLAS_dgemm
;--------

%index
HCLBLAS_Get2DShape
cl mem idに行or列を取得する

%prm
(int p1,int p2)
int p1:CL_mem_object id			[in]
int p2:0 or 1

%inst
cl memの行、列のサイズを取得します。
p2が0なら行、1なら列が返されます。

%href
HCLBLAS_Set2DShape
HCLBLAS_Get2DShape
HCLBLAS_sgemm
HCLBLAS_dgemm
;--------

%index
HCLBLAS_sT
cl mem idをfloat型で行列転置

%prm
int p1,int p2
int p1:CL_mem_object id			[in]
int p2:CL_mem_object id			[in]

%inst
■命令として使った場合
p1をソースとしてfloat型として解釈し、行列転置したものをp2に書き込みます。
■関数として使った場合
p1をソースとしてfloat型として解釈し、行列転置したメモリbufferを作成しメモリidを返します。
p2は指定できません。

%href
HCLBLAS_dT
;--------
%index
HCLBLAS_dT
cl mem idをdouble型で行列転置

%prm
int p1,int p2
int p1:CL_mem_object id			[in]
int p2:CL_mem_object id			[in]

%inst
■命令として使った場合
p1をソースとしてdouble型として解釈し、行列転置したものをp2に書き込みます。
■関数として使った場合
p1をソースとしてdouble型として解釈し、行列転置したメモリbufferを作成しメモリidを返します。
p2は指定できません。

%href
HCLBLAS_sT
;--------
%index
HCLBLAS_sgemv
sgemvカーネル実行 y=A*x

%prm
int p1,int p2,int p3,int p4,int p5,int p6
int p1:[y]CL_mem_object id			[in]
int p2:[A]CL_mem_object id			[in]
int p3:[x]CL_mem_object id			[in]

%inst

y=A×xの行列ベクトル積を行ないます。
プラグイン内部に埋め込まれているカーネルで実行されます。

AにはHCLBLAS_Set2DShape
であらかじめcl memに行、列のサイズを設定しておく必要があります。

x,yはHCLBLAS_Set2DShapeでサイズ指定されていても無視されます。


命令として実行することもできますが、関数として実行することもできます。
その際は
y=HCLBLAS_sgemm(A,x)
のように使います。
この場合、yには新たにHCLCreateBufferで確保されたmem idが返されます。


%href
HCLBLAS_sgemv
HCLBLAS_dgemv
;--------
%index
HCLBLAS_dgemv
dgemvカーネル実行 y=A*x

%prm
int p1,int p2,int p3,int p4,int p5,int p6
int p1:[y]CL_mem_object id			[in]
int p2:[A]CL_mem_object id			[in]
int p3:[x]CL_mem_object id			[in]

%inst

y=A×xの行列ベクトル積を行ないます。
プラグイン内部に埋め込まれているカーネルで実行されます。

AにはHCLBLAS_Set2DShape
であらかじめcl memに行、列のサイズを設定しておく必要があります。

x,yはHCLBLAS_Set2DShapeでサイズ指定されていても無視されます。


命令として実行することもできますが、関数として実行することもできます。
その際は
y=HCLBLAS_sgemm(A,x)
のように使います。
この場合、yには新たにHCLCreateBufferで確保されたmem idが返されます。


%href
HCLBLAS_sgemv
HCLBLAS_dgemv
;--------
%index
HCLBLAS_sdot
x1・x2のドット積(ベクトル内積)を計算

%prm
int p1,int p2,int p3
int p1:CL_mem_object id			[in]
int p2:CL_mem_object id			[in]
int p3:CL_mem_object id			[in]

%inst
■命令として使った場合
p2,p3をfloat型のベクトルとして解釈し内積計算したものをp1に書き込みます。
■関数として使った場合
p1,p2をfloat型のベクトルとして解釈し内積計算したものの結果が格納されているmem idを返します。
p3は使いません。

%href
HCLBLAS_ddot
;--------
%index
HCLBLAS_ddot
x1・x2のドット積(ベクトル内積)を計算

%prm
int p1,int p2,int p3
int p1:CL_mem_object id			[in]
int p2:CL_mem_object id			[in]
int p3:CL_mem_object id			[in]

%inst
■命令として使った場合
p2,p3をdouble型のベクトルとして解釈し内積計算したものをp1に書き込みます。
■関数として使った場合
p1,p2をdouble型のベクトルとして解釈し内積計算したものの結果が格納されているmem idを返します。
p3は使いません。

%href
HCLBLAS_sdot
;--------
%index
HCLBLAS_snrm2
ベクトルxのL2ノルムを計算

%prm
int p1,int p2
int p1:CL_mem_object id			[in]
int p2:CL_mem_object id			[in]

%inst
■命令として使った場合
p2をfloat型のベクトルとして解釈しL2ノルムを計算したものをp1に書き込みます。
■関数として使った場合
p1をfloat型のベクトルとして解釈しL2ノルムを計算したものの結果が格納されているmem idを返します。
p2は使いません。

%href
HCLBLAS_dnrm2
;--------
%index
HCLBLAS_dnrm2
ベクトルxのL2ノルムを計算

%prm
int p1,int p2
int p1:CL_mem_object id			[in]
int p2:CL_mem_object id			[in]

%inst
■命令として使った場合
p2をdouble型のベクトルとして解釈しL2ノルムを計算したものをp1に書き込みます。
■関数として使った場合
p1をdouble型のベクトルとして解釈しL2ノルムを計算したものの結果が格納されているmem idを返します。
p2は使いません。

%href
HCLBLAS_snrm2
;--------