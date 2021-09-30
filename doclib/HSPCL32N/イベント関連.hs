;------------------------
;   イベント関連
;------------------------

;-------- header --------
%dll
HSPCL32N.dll

%ver
2.2

%date
2021/06/14

%author
pippi

%note
hspcl32n.as をインクルードしてください。

%type
32bitランタイムGPGPU用プラグイン

%group
OpenCLイベント関連

%port
Win

;-------- ref --------

%index
clSetWaitEvent
event待ちに使うeventを1つセット

%prm
int p1
int p1 : event id [in]

%inst
p1に実行完了を行いたいevent idを入れて下さい。
下記命令を次に実行する際のみに、そのevent待ちが行われます。

clDoKernel
clDoKrn1
clDoKrn1_sub
clDoKrn2
clDoKrn3
clCopyBuffer
clWriteBuffer
clReadBuffer
clWriteBuffer_NonBlocking
clReadBuffer_NonBlocking
clFillBuffer_i32
clFillBuffer_fp
clFillBuffer_dp


使い方
clSetCommandQueue 0
clWriteBuffer memA,data,,,,0,0 //eventid=0
clSetCommandQueue 1
clSetWaitEvent 0
clDoKrn1 krnid,65536,256

これで、clWriteBufferの転送が終了したあとにkrnidのカーネルの実行が始まることが保証されます。(clSetWaitEvent 0をしてないと転送とカーネル実行が同時に起こる可能性がある)

%href
clSetWaitEvents
;--------

%index
clSetWaitEvents
event待ちに使うeventを複数セット

%prm
array p1
array p1 : event idが格納されたint型配列 [in]

%inst
p1にevent idが格納されたint型配列を入れて下さい。
下記命令を次に実行する際のみに、そのevent待ちが行われます。

clDoKernel
clDoKrn1
clDoKrn1_sub
clDoKrn2
clDoKrn3
clCopyBuffer
clWriteBuffer
clReadBuffer
clWriteBuffer_NonBlocking
clReadBuffer_NonBlocking
clFillBuffer_i32
clFillBuffer_fp
clFillBuffer_dp


%href
clSetWaitEvent

;--------

%index
clGetEventLogs
event情報取得

%prm
(int p1.int p2)
int p1 ： event id	[in]
int p2 ： parameter	[in]

%inst
p1にevent idを指定してp2で0～7のパラメーターを指定することで下記情報を取得できます。
返り値は必ずint型です。

0:eventと紐付いているOpenCLコマンドの種類
#define global CL_COMMAND_NDRANGE_KERNEL                   0x11F0
#define global CL_COMMAND_TASK                             0x11F1
#define global CL_COMMAND_NATIVE_KERNEL                    0x11F2
#define global CL_COMMAND_READ_BUFFER                      0x11F3
#define global CL_COMMAND_WRITE_BUFFER                     0x11F4
#define global CL_COMMAND_COPY_BUFFER                      0x11F5
#define global CL_COMMAND_READ_IMAGE                       0x11F6
#define global CL_COMMAND_WRITE_IMAGE                      0x11F7
#define global CL_COMMAND_COPY_IMAGE                       0x11F8
#define global CL_COMMAND_COPY_IMAGE_TO_BUFFER             0x11F9
#define global CL_COMMAND_COPY_BUFFER_TO_IMAGE             0x11FA
#define global CL_COMMAND_MAP_BUFFER                       0x11FB
#define global CL_COMMAND_MAP_IMAGE                        0x11FC
#define global CL_COMMAND_UNMAP_MEM_OBJECT                 0x11FD
#define global CL_COMMAND_MARKER                           0x11FE
#define global CL_COMMAND_ACQUIRE_GL_OBJECTS               0x11FF
#define global CL_COMMAND_RELEASE_GL_OBJECTS               0x1200
#define global CL_COMMAND_READ_BUFFER_RECT                 0x1201
#define global CL_COMMAND_WRITE_BUFFER_RECT                0x1202
#define global CL_COMMAND_COPY_BUFFER_RECT                 0x1203
#define global CL_COMMAND_USER                             0x1204
#define global CL_COMMAND_BARRIER                          0x1205
#define global CL_COMMAND_MIGRATE_MEM_OBJECTS              0x1206
#define global CL_COMMAND_FILL_BUFFER                      0x1207
#define global CL_COMMAND_FILL_IMAGE                       0x1208
そのeventがCL_COMMAND_NDRANGE_KERNELだったか、CL_COMMAND_WRITE_BUFFERだったか、CL_COMMAND_Read_BUFFERだったかの判定につかえます。

1:kernel_idかコピーサイズ
そのeventがCL_COMMAND_NDRANGE_KERNELならkernel_idが、
CL_COMMAND_WRITE_BUFFERやCL_COMMAND_Read_BUFFERやCL_COMMAND_FILL_BUFFERやCL_COMMAND_COPY_BUFFERならコピーサイズがbyteで返ります。

2:そのeventを実行したdevice 番号

3:そのeventを実行したCommandQueue 番号

4:CL_PROFILING_COMMAND_QUEUEDの時間
単位はナノセカンド(ns)
本来返り値は64bit intですが、本関数の返り値はそのうちの下位32bitです。上位32bitはstatに代入されます。

5:CL_PROFILING_COMMAND_SUBMITの時間
単位はナノセカンド(ns)
本来返り値は64bit intですが、本関数の返り値はそのうちの下位32bitです。上位32bitはstatに代入されます。

6:CL_PROFILING_COMMAND_STARTの時間
単位はナノセカンド(ns)
本来返り値は64bit intですが、本関数の返り値はそのうちの下位32bitです。上位32bitはstatに代入されます。

7:CL_PROFILING_COMMAND_ENDの時間
単位はナノセカンド(ns)
本来返り値は64bit intですが、本関数の返り値はそのうちの下位32bitです。上位32bitはstatに代入されます。

%href
clGetEventStatus
;--------

%index
clGetEventStatus
イベント実行状態取得

%prm
(int p1)
int p1 ： event id [in]

%inst

event と関連付けられたコマンドの実行状態をint型で返します。
以下の値のうちひとつが返されます。
CL_QUEUED - コマンドがコマンドキューに挿入された。
CL_SUBMITTED - コマンドキューに関連付けられたデバイスに対し、挿入されたコマンドがホストから送られた。
CL_RUNNING - 現在デバイスがコマンドを実行中。
CL_COMPLETE - コマンドが完了した。
このほかに、負の整数値のエラーコードが返されることがあります（問題のあるメモリアクセスなどでコマンドが異常終了したときなど）。

詳細は
http://wiki.tommy6.net/wiki/clGetEventInfo
ここの
CL_EVENT_COMMAND_EXECUTION_STATUS
を参照して下さい。

%href
;--------

%index
clWaitForEvent
event完了待ち

%prm
int p1
int p1 ： event id [in]

%inst
eventの実行完了を待ちます。

%href
clWaitForEvents

;--------
%index
clWaitForEvents
event完了待ち

%prm
array p1
array p1 ： event idのint型配列 [in]

%inst
eventの実行完了を待ちます。

%href
clWaitForEvent

;--------

%index
clCreateUserEvent
UserEvent作成

%prm
int p1
int p1 ： event id [in]

%inst
p1で指定したevent番号をユーザーイベントとして登録します。
clSetUserEventStatusとclSetWaitEvent等と組み合わせて使います。
clSetUserEventStatusで任意のタイミングでユーザーイベントにCL_COMPLETEをセットすることで
clSetWaitEventにより、あるOpenCLコマンドの実行開始を制御することができます。

初期状態ではCL_SUBMITTEDがセットされています。
注意としてはclGetEventLogs命令でUserEventが扱えないことです。

%href
clSetUserEventStatus
clSetWaitEvent
clSetWaitEvents

;--------

%index
clSetUserEventStatus
UserEventに状態をセット

%prm
int p1,int p2
int p1 ： user event id		[in]
int p2 ： parameter		[in]

%inst
p1で指定したUserEventにp2をセットします。
p2は基本的にCL_COMPLETE(*)を指定します。

(*)
#define global CL_COMPLETE    0x0

それ以外の値をセットする場合は下記参照下さい。
https://www.khronos.org/registry/OpenCL/sdk/2.2/docs/man/html/clSetUserEventStatus.html


%href
clCreateUserEvent
clSetWaitEvent
clSetWaitEvents
;--------