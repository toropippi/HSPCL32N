;------------------------
;   変数型制御
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
OpenCL変数型制御

%port
Win

;-------- ref --------
%index
clfdim
float型の配列変数を作成

%prm
p1,p2...
p1=変数 : 配列を割り当てる変数名	[in]
p2=0～ : 要素の最大			[in]

%inst
任意の要素を持つfloat型の配列変数を作成します。

clfdim命令はスクリプト内のどこででも定義・再定義することが可能です。
また、配列変数を作成すると内容はすべて0にクリアされます。

%href
float
;--------

%index
clfloat
float型に変換

%prm
(p1)
p1 : 変換元の値または変数 [in]

%inst
p1で指定された値をfloat型にしたものを返します。
仮数部が23bitしかないためint型から変換する際は、16777216より大きい値or-16777216より小さい値において、1刻みの情報が破棄されることに注意して下さい。

%href
clfdim
;--------