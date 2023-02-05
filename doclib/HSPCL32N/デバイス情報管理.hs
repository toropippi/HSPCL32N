;------------------------
;  デバイス情報関連
;------------------------

;-------- header --------
%dll
HSPCL32N.dll

%ver
3.0

%date
2023/02/06

%author
pippi

%note
hspcl32n.as をインクルードしてください。

%type
32bitランタイムGPGPU用プラグイン

%group
OpenCLデバイス情報管理

%port
Win

;-------- ref --------

%index
clinit
HSPCL32Nを初期化

%prm

%inst
HSPCL32Nを初期化します。
hspcl32n.asをインクルードしたあとに実行して下さい。

%href
clGetDeviceCount
_ExclSetCommandQueueMax
_ExclSetCommandQueueProperties
_ExclSetEventMax
;--------

%index
clGetPluginVersion
HSPCL32Nバージョン取得

%prm
()

%inst
HSPCL32Nプラグインのバージョンがdouble型で返ります。

%href
;--------

%index
clGetDeviceCount
OpenCLデバイス数取得

%prm
()

%inst
OpenCLデバイスとして認識されるデバイス数が返ります。
clSetDeviceで指定できる番号は0～clGetDeviceCount()-1の範囲となります。

%href
clinit
clSetDevice

;--------


%index
clSetDevice
デバイスセット

%prm
int p1
int p1	デバイスid [in]

%inst

カーネル命令やカーネル登録、メモリ確保を実行するデバイスを指定します。
clSetDeviceで指定できる番号は0～clGetDeviceCount()-1の範囲となります。
例えばGPUを2枚以上搭載しているPCで、それぞれのGPUで全く異なるプログラムを実行されることができます。

以下の命令は設定したデバイスのみで実行されます。


clGetDeviceInfo_s
clGetDeviceInfo_i
clGetDeviceInfo_i64
clGetSettingDevice
clCreateProgram
clCreateProgramWithSource
clReleaseProgram
clGetProgramBinary
clCreateProgramWithBinary
clCreateKernel
clSetKernel
clSetKrns
clGetKernelName
clReleaseKernel
clCreateBuffer
clCreateBufferFrom
clWriteBuffer
clWriteBuffer_NonBlocking
clReadBuffer
clReadBuffer_NonBlocking
clGet_NonBlocking_Status
clCopyBuffer
clFillBuffer
clReleaseBuffer
cldim_i32FromBuffer
cldim_i64FromBuffer
cldim_dpFromBuffer
cldim_fpFromBuffer
clGetSize
clGetAllBufferSize
clGarbageCollectionNow
clIncRefcntCLBufferId
clDecRefcntCLBufferId
clReadIndex_i32
clReadIndex_i64
clReadIndex_dp
clReadIndex_fp
clWriteIndex_i32
clWriteIndex_i64
clWriteIndex_dp
clWriteIndex_fp
clCall
clDoKrn1
clDoKrn2
clDoKrn3
clDoKernel
clDoKrn1_sub
clCall2
clDoXc
clDoXi
clDoXl
clDoXf
clDoXd
clDoXuc
clDoXui
clDoXul
clFinish
clFlush
clWaitForEvent
clWaitForEvents
clBLAS_Set2DShape
clBLAS_sgemm
clBLAS_dgemm
clBLAS_sT
clBLAS_dT
clBLAS_sgemv
clBLAS_dgemv
clBLAS_sdot
clBLAS_ddot
clBLAS_snrm2
clBLAS_dnrm2

clSetDeviceを実行していない場合は、カレントデバイスはデバイス0です。

%href
clGetDeviceInfo_s
clGetDeviceInfo_i
clGetDeviceInfo_i64
clGetSettingDevice
clCreateProgram
clCreateProgramWithSource
clReleaseProgram
clGetProgramBinary
clCreateProgramWithBinary
clCreateKernel
clSetKernel
clSetKrns
clGetKernelName
clReleaseKernel
clCreateBuffer
clCreateBufferFrom
clWriteBuffer
clWriteBuffer_NonBlocking
clReadBuffer
clReadBuffer_NonBlocking
clGet_NonBlocking_Status
clCopyBuffer
clFillBuffer
clReleaseBuffer
cldim_i32FromBuffer
cldim_i64FromBuffer
cldim_dpFromBuffer
cldim_fpFromBuffer
clGetSize
clGetAllBufferSize
clGarbageCollectionNow
clIncRefcntCLBufferId
clDecRefcntCLBufferId
clReadIndex_i32
clReadIndex_i64
clReadIndex_dp
clReadIndex_fp
clWriteIndex_i32
clWriteIndex_i64
clWriteIndex_dp
clWriteIndex_fp
clCall
clDoKrn1
clDoKrn2
clDoKrn3
clDoKernel
clDoKrn1_sub
clCall2
clDoXc
clDoXi
clDoXl
clDoXf
clDoXd
clDoXuc
clDoXui
clDoXul
clFinish
clFlush
clWaitForEvent
clWaitForEvents
clBLAS_Set2DShape
clBLAS_sgemm
clBLAS_dgemm
clBLAS_sT
clBLAS_dT
clBLAS_sgemv
clBLAS_dgemv
clBLAS_sdot
clBLAS_ddot
clBLAS_snrm2
clBLAS_dnrm2
;--------

%index
clGetSettingDevice
セットデバイスno取得

%prm
()

%inst
clSetDeviceで指定したデバイスのidが返ります。

%href
clSetDevice
;--------

%index
clGetDeviceInfo_s
デバイス情報取得

%prm
(int p1)
int p1 ： param_name [in]

%inst
clSetDeviceでセットしたデバイスの情報を文字列で取得します。
p1にparam_nameを指定して下さい。戻り値は文字列になります。

p1は下記のURLの表にある値からひとつ選んで指定できます。
日本語サイト：http://wiki.tommy6.net/wiki/clGetDeviceInfo
英語サイト：https://www.khronos.org/registry/cl/sdk/1.0/docs/man/xhtml/clGetDeviceInfo.html

%href
clGetDeviceInfo_i
clGetDeviceInfo_i64
;--------

%index
clGetDeviceInfo_i
デバイス情報取得

%prm
(int p1,int p2)
int p1 ： param_name [in]
int p2 ： index [in]

%inst
clSetDevice でセットしたデバイスの情報を文字列で取得します。
p1にparam_nameを指定して下さい。戻り値はint型の整数になります。
戻り値の数値が配列の場合は、p2にindexを指定することで、p2番目の要素の情報を得ることができます。

p1は下記のURLの表にある値からひとつ選んで指定できます。
日本語サイト：http://wiki.tommy6.net/wiki/clGetDeviceInfo
英語サイト：https://www.khronos.org/registry/cl/sdk/1.0/docs/man/xhtml/clGetDeviceInfo.html

%href
clGetDeviceInfo_i64
clGetDeviceInfo_s
;--------

%index
clGetDeviceInfo_i64
デバイス情報取得

%prm
(int p1,int p2)
int p1 ： param_name [in]
int p2 ： index [in]

%inst
clSetDevice でセットしたデバイスの情報を文字列で取得します。
p1にparam_nameを指定して下さい。戻り値は64bit int型の整数になります。
戻り値の数値が配列の場合は、p2にindexを指定することで、p2番目の要素の情報を得ることができます。

p1は下記のURLの表にある値からひとつ選んで指定できます。
日本語サイト：http://wiki.tommy6.net/wiki/clGetDeviceInfo
英語サイト：https://www.khronos.org/registry/cl/sdk/1.0/docs/man/xhtml/clGetDeviceInfo.html

%href
clGetDeviceInfo_i
clGetDeviceInfo_s
;--------

%index
_ExclSetCommandQueueMax
コマンドキューの最大数をセット

%prm
int p1
int p1	コマンドキューの最大数 [in]


%inst
clinitの実行前に指定することで、1つのデバイスあたりのコマンドキューの最大を増やすことができます。
デフォルトでは1つのデバイスあたりのコマンドキューは0～3までつかえます。

%href
_ExclSetCommandQueueProperties
_ExclSetEventMax
;--------

%index
_ExclSetCommandQueueProperties
コマンドキュープロパティのセット

%prm
int p1
int p1	properties [in]

%inst
clinitの実行前に指定することで、OpenCLのコマンドキューのプロパティの設定を変えることができます。
コマンドキュープロパティはデフォルトではCL_QUEUE_PROFILING_ENABLEが指定されています。
http://wiki.tommy6.net/wiki/clCreateCommandQueue
を参考にして下さい。

%href
_ExclSetCommandQueueMax
_ExclSetEventMax
;--------

%index
_ExclSetEventMax
イベントの最大数をセット

%prm
int p1
int p1	イベントの最大数 [in]

%inst
clinitの実行前に指定することで、記録可能イベントの最大を増やすことができます。
デフォルトでイベントidは0～65535番までつかえます。

%href
_ExclSetCommandQueueMax
_ExclSetCommandQueueProperties
;--------