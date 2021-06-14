#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include "hsp3plugin.h"
#include "hspvar_float.h"

#include <CL/cl.h>

#include "errmsg.h"
#include "RGB.h"

/*
#define MAX_SOURCE_SIZE 0x200000
#define MAX_PLATFORM_IDS 16 //platform_idの最大値
#define MAX_DEVICE_IDS 32 //devicesの最大値

int gplatkz = 0;//プラットフォームの数  2
int gdevkz = 0;//デバイスの数  4


int dev_num = 0;

cl_platform_id platform_id[MAX_PLATFORM_IDS];
cl_context* context;
cl_command_queue* command_queue;
cl_mem mem_obj;//上限4096撤廃
cl_program program;
cl_kernel kernel;
cl_event dmyevent;
cl_event* ddmyevent = &dmyevent;
int clsetdev = 0;
void gtflt(float* ff0);
void gtint(int* ii0);
void gtdbl(double* dd0);
void retmeserr(cl_int ret);//clEnqueueNDRangeKernel で失敗した時出すエラーメッセージをまとめた関数
void retmeserr2(cl_int ret);//clReadで失敗した時出すエラーメッセージをまとめた関数
*/



const int MAX_PLATFORM_IDS = 32;//platform_idの最大値
const int MAX_DEVICE_IDS = 2048;//一度に取得できるdeviceの最大値
const int CLCALL_LOOP = 32;//HCLCallの引数の可能な数
int CL_EVENT_MAX = 65536;//cl_eventを記憶して置ける最大数
int COMMANDQUEUE_PER_DEVICE = 4;//1デバイスあたりのコマンドキュー、設定で変更できる
int dev_num = 0;//全プラットフォームのデバイスの合計数
int bufferout[1024 * 4];//ほぼHCLGetDeviceInfoの返り値用
char hspcharout[1024 * 2];//HSPの文字列出力用バッファ
cl_device_id* device_id;
cl_context* context;
cl_command_queue* command_queue;
cl_mem mem_obj;
cl_program program;
cl_kernel kernel;
cl_event* cppeventlist;//c++で管理するevent object。HSPからいじれるのはここだけ。HCLinitで実体化。メモリリーク予防目的。ここの中にあるeventのみ情報を保持し、それ以外のeventは必ずreleaseして破棄する
cl_event* event_wait_list;//HCLinitで実体化。次にeventでwaitしたいcl関数を使う際にあらかじめこれを設定しておいておくイメージ


struct EventStruct
{
	int k;//kernel idまたはコピーサイズやら
	int devno;
	int queno;
};

EventStruct* evinfo;

int clsetdev = 0;//OpenCLで現在メインとなっているデバイスno
int clsetque = 0;//OpenCLで現在メインとなっているque
int cmd_properties = CL_QUEUE_PROFILING_ENABLE;//OpenCLのコマンドキュー生成時に使うプロパティ番号
int num_event_wait_list = 0;//NDRangeKernel とかで使うやつ。使う度に0になる
int thread_start = 0;//0はEnqueueまちがない、1以降はthreadに投げたがまだEnqueueされてない数







/*
struct cldevinfoStruct {
	int plat;
	cl_device_id dev;
	char name[1024];
	cl_device_type coretype;
	char version[1024];
	char cversion[1024];
	char vendor[1024];
	char EXTENSIONS[1024];
	cl_ulong localmemsize;
	cl_ulong globalmemsize;
	cl_ulong maxmemalloc;
	cl_uint maxfrq;
	size_t maxworkgsz;
	size_t maxworkisz0;
	size_t maxworkisz1;
	size_t maxworkisz2;
	cl_uint maxcompunit;
	int devtype;
	size_t m2dx;
	size_t m2dy;
	size_t m3dx;
	size_t m3dy;
	size_t m3dz;
	bool localmemtype;
	bool compile;
	bool CLyukflg;
};
cldevinfoStruct* gpus;

int onexitflg = 0;
*/








/*


void gtflt(float* ff0)
{
	int chk = code_getprm();							// パラメーターを取得(型は問わない)
	if (chk <= PARAM_END) {
		return;										// パラメーター省略時の処理
	}
	int type = mpval->flag;							// パラメーターの型を取得
	switch (type) {
	case 2:								// パラメーターが文字列だった時
	{
		char* str = (char*)mpval->pt;
		*ff0 = float(atof(str));
		break;
	}
	case 3:									// パラメーターが実数だった時
	{
		double* ptr = (double*)mpval->pt;
		*ff0 = float(*ptr);
		break;
	}
	case 4:									// パラメーターが整数だった時
	{
		int* ptr = (int*)mpval->pt;
		*ff0 = float(*ptr);
		break;
	}
	case 8:									// パラメーターがfloatだった時
	{
		float* ptr = (float*)mpval->pt;
		*ff0 = float(*ptr);
		break;
	}
	default:
		puterror(HSPERR_TYPE_MISMATCH);			// サポートしていない型ならばエラー
	}
}


void gtint(int* ii0)
{
	int chk = code_getprm();							// パラメーターを取得(型は問わない)
	if (chk <= PARAM_END) {
		return;										// パラメーター省略時の処理
	}
	int type = mpval->flag;							// パラメーターの型を取得
	switch (type) {
	case 2:								// パラメーターが文字列だった時
	{
		char* str = (char*)mpval->pt;
		*ii0 = atoi(str);
		break;
	}
	case 3:									// パラメーターが実数だった時
	{
		double* ptr = (double*)mpval->pt;
		*ii0 = int(*ptr);
		break;
	}
	case 4:									// パラメーターが整数だった時
	{
		int* ptr = (int*)mpval->pt;
		*ii0 = int(*ptr);
		break;
	}
	case 8:									// パラメーターがfloatだった時
	{
		float* ptr = (float*)mpval->pt;
		*ii0 = int(*ptr);
		break;
	}
	default:
		puterror(HSPERR_TYPE_MISMATCH);			// サポートしていない型ならばエラー
	}
}


void gtdbl(double* dd0)
{
	int chk = code_getprm();							// パラメーターを取得(型は問わない)
	if (chk <= PARAM_END) {
		return;										// パラメーター省略時の処理
	}
	int type = mpval->flag;							// パラメーターの型を取得
	switch (type) {
	case 2:								// パラメーターが文字列だった時
	{
		char* str = (char*)mpval->pt;
		*dd0 = atof(str);
		break;
	}
	case 3:									// パラメーターが実数だった時
	{
		double* ptr = (double*)mpval->pt;
		*dd0 = double(*ptr);
		break;
	}
	case 4:									// パラメーターが整数だった時
	{
		int* ptr = (int*)mpval->pt;
		*dd0 = double(*ptr);
		break;
	}
	case 8:									// パラメーターがfloatだった時
	{
		float* ptr = (float*)mpval->pt;
		*dd0 = double(*ptr);
		break;
	}
	default:
		puterror(HSPERR_TYPE_MISMATCH);			// サポートしていない型ならばエラー
	}
}

*/
/*
void retmeserr(cl_int ret)
{
	switch (ret) {							//分岐
	case CL_INVALID_PROGRAM_EXECUTABLE:
		MessageBox(NULL, "デバイス上で実行可能な、正常にビルドされたプログラムが一つもありません", "エラー", 0);

		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_COMMAND_QUEUE:
		MessageBox(NULL, "デバイスidが無効なデバイスになっています", "エラー", 0);

		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_KERNEL:
		MessageBox(NULL, "カーネルidが間違っています", "エラー", 0);

		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_CONTEXT:
		MessageBox(NULL, "カーネルidが違うデバイスidで登録されています、あるいは第一引数と event_wait_list 内のイベントと関連付けられたデバイスが同じでない", "エラー", 0);

		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_KERNEL_ARGS:
		MessageBox(NULL, "カーネル引数が指定されていません", "エラー", 0);

		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_GLOBAL_WORK_SIZE:
		MessageBox(NULL, "global_work_size が NULL です。あるいは、global_work_sizeの配列のどれかが0です。もしくはカーネルを実行するデバイス上でのglobal_work_sizeが上限値を超えている", "エラー", 0);

		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_GLOBAL_OFFSET:
		MessageBox(NULL, "CL_INVALID_GLOBAL_OFFSET - global_work_offset が NULL でない", "エラー", 0);

		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_WORK_DIMENSION:
		MessageBox(NULL, "work_dim が適切な値でない", "エラー", 0);

		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_WORK_GROUP_SIZE:
		MessageBox(NULL, "global_work_sizeがlocal_work_size で整除できない、またはlocal_work_size[0]*local_work_size[1]*local_work_size[2]が、一つのワークグループ内のワークアイテム数の最大値を超えた", "エラー", 0);

		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_WORK_ITEM_SIZE:
		MessageBox(NULL, "local_work_size[0], ... local_work_size[work_dim - 1] で指定したワークアイテム数が対応する CL_DEVICE_MAX_WORK_ITEM_SIZES[0], ... CL_DEVICE_MAX_WORK_ITEM_SIZES[work_dim - 1] の値こえている、または0を指定した", "エラー", 0);

		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_MEM_OBJECT_ALLOCATION_FAILURE:
		MessageBox(NULL, "kernel の引数に指定されたバッファ/イメージオブジェクトに関連付けられたデータ保存のためのメモリ領域の確保に失敗した", "エラー", 0);

		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_EVENT_WAIT_LIST:
		MessageBox(NULL, "event_wait_list が NULL で num_events_in_wait_list が 0 より大きいとき。あるいは event_wait_list が NULL でなく num_events_in_wait_list が 0 のとき。あるいは event_wait_list 内のイベントオブジェクトが有効なものでない", "エラー", 0);

		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_OUT_OF_RESOURCES:
		MessageBox(NULL, "デバイス上でのリソース確保に失敗した", "エラー", 0);

		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_OUT_OF_HOST_MEMORY:
		MessageBox(NULL, "ホスト上でのリソース確保に失敗した", "エラー", 0);

		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	}
	//上のどれでもなければ
	MessageBox(NULL, "原因不明のエラーです", "エラー", 0);
	puterror(HSPERR_UNSUPPORTED_FUNCTION);
}

*/







/*
static void clini(void)
{
	cl_device_id device_id[MAX_DEVICE_IDS];
	cl_uint ret_num_devices;
	cl_uint ret_num_platforms;
	cl_int ret = clGetPlatformIDs(MAX_PLATFORM_IDS, platform_id, &ret_num_platforms);
	gplatkz = ret_num_platforms;
	int gkw = 0;

	if (gplatkz != 0) {
		for (int i = 0; i < gplatkz; i++) {
			ret = clGetDeviceIDs(platform_id[i], CL_DEVICE_TYPE_ALL, MAX_DEVICE_IDS, device_id, &ret_num_devices);
			gkw += ret_num_devices;
		}
		gpus = new struct cldevinfoStruct[gkw];
		int w = 0;
		for (int i = 0; i < gplatkz; i++) {
			ret = clGetDeviceIDs(platform_id[i], CL_DEVICE_TYPE_ALL, MAX_DEVICE_IDS, device_id, &ret_num_devices);
			for (int j = 0; j<int(ret_num_devices); j++)
			{
				clGetDeviceInfo(device_id[j], CL_DEVICE_NAME, 1024, &gpus[w].name, NULL);
				clGetDeviceInfo(device_id[j], CL_DEVICE_TYPE, sizeof(cl_device_type), &gpus[w].coretype, NULL);
				clGetDeviceInfo(device_id[j], CL_DEVICE_VENDOR, 1024, &gpus[w].vendor, NULL);
				clGetDeviceInfo(device_id[j], CL_DEVICE_EXTENSIONS, 1024, &gpus[w].EXTENSIONS, NULL);
				clGetDeviceInfo(device_id[j], CL_DRIVER_VERSION, 1024, &gpus[w].version, NULL);
				clGetDeviceInfo(device_id[j], CL_DEVICE_VERSION, 1024, &gpus[w].cversion, NULL);
				clGetDeviceInfo(device_id[j], CL_DEVICE_LOCAL_MEM_SIZE, sizeof(cl_ulong), &gpus[w].localmemsize, NULL);
				clGetDeviceInfo(device_id[j], CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(cl_ulong), &gpus[w].globalmemsize, NULL);
				clGetDeviceInfo(device_id[j], CL_DEVICE_MAX_MEM_ALLOC_SIZE, sizeof(cl_ulong), &gpus[w].maxmemalloc, NULL);
				clGetDeviceInfo(device_id[j], CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(cl_uint), &gpus[w].maxfrq, NULL);
				clGetDeviceInfo(device_id[j], CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &gpus[w].maxworkgsz, NULL);

				size_t itj[3];
				clGetDeviceInfo(device_id[j], CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(size_t) * 3, &itj, NULL);
				gpus[w].maxworkisz0 = itj[0]; gpus[w].maxworkisz1 = itj[1]; gpus[w].maxworkisz2 = itj[2];
				clGetDeviceInfo(device_id[j], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &gpus[w].maxcompunit, NULL);
				cl_device_type itj3;
				clGetDeviceInfo(device_id[j], CL_DEVICE_TYPE, sizeof(cl_device_type), &itj3, NULL);
				gpus[w].devtype = int(itj3);
				gpus[w].plat = i;
				gpus[w].dev = device_id[j];

				clGetDeviceInfo(device_id[j], CL_DEVICE_IMAGE2D_MAX_WIDTH, sizeof(size_t), &gpus[w].m2dx, NULL);
				clGetDeviceInfo(device_id[j], CL_DEVICE_IMAGE2D_MAX_HEIGHT, sizeof(size_t), &gpus[w].m2dy, NULL);
				clGetDeviceInfo(device_id[j], CL_DEVICE_IMAGE3D_MAX_WIDTH, sizeof(size_t), &gpus[w].m3dx, NULL);
				clGetDeviceInfo(device_id[j], CL_DEVICE_IMAGE3D_MAX_HEIGHT, sizeof(size_t), &gpus[w].m3dy, NULL);
				clGetDeviceInfo(device_id[j], CL_DEVICE_IMAGE3D_MAX_DEPTH, sizeof(size_t), &gpus[w].m3dz, NULL);
				cl_device_local_mem_type itj4;
				clGetDeviceInfo(device_id[j], CL_DEVICE_LOCAL_MEM_TYPE, sizeof(cl_device_local_mem_type), &itj4, NULL);
				if (itj4 == CL_GLOBAL) { gpus[w].localmemtype = 0; }
				if (itj4 == CL_LOCAL) { gpus[w].localmemtype = 1; }
				cl_bool itj6;
				clGetDeviceInfo(device_id[j], CL_DEVICE_COMPILER_AVAILABLE, sizeof(cl_bool), &itj6, NULL);
				if (itj6 == CL_FALSE) { gpus[w].compile = 0; }
				if (itj6 == CL_TRUE) { gpus[w].compile = 1; }
				w++;
			}
		}
	}//////////デバイス情報を取得しまくっているところ



	cl_device_id cdDeviceID[32];
	size_t size3 = 0;
	int count = 0;
	int countd = 0;
	context = new cl_context[gkw];
	command_queue = new cl_command_queue[gkw];


	char str1[1024];
	char str2[1024];


	for (int k = 0; k < gkw; k++) {//コンテキストとコマンド級ーを作る
		context[k] = clCreateContext(NULL, 1, &gpus[k].dev, NULL, NULL, &ret);

		sprintf(str2, "原因不明");
		switch (ret) {							//分岐
		case CL_INVALID_PLATFORM:
			sprintf(str2, "CL_INVALID_PLATFORM\nコンテキスト作成エラー。\nproperties が NULL でかつプラットフォームがひとつも選択されなかったとき。または、properties 内で指定したプラットフォームが有効なプラットフォームでないとき。");
			break;

		case CL_INVALID_PROPERTY:
			sprintf(str2, "CL_INVALID_PROPERTY\n値が支援されたプロパティ名に対して指定した場合、特性中のコンテキスト・プロパティ名が支援されたプロパティ名でない場合、有効でない、あるいは、同じプロパティ名が二度以上指定される場合。しかしながら、拡張cl_khr_gl_sharingが可能になる場合、上記の特性用のテーブルにリストされたもの以外の属性名が、特性の中で指定される場合、CL_INVALID_PROPERTYが返されます。");
			break;

		case CL_INVALID_VALUE:
			sprintf(str2, "CL_INVALID_VALUE\nコンテキスト作成エラー。\ndevices が NULL のとき num_devices が0のとき。  pfn_notify が NULL で user_data が NULL でないとき。");
			break;

		case CL_INVALID_DEVICE:
			sprintf(str2, "CL_INVALID_DEVICE\nコンテキスト作成エラー。\ndevices に有効でないデバイスが含まれているとき");
			break;

		case CL_DEVICE_NOT_AVAILABLE:
			sprintf(str2, "CL_DEVICE_NOT_AVAILABLE\nコンテキスト作成エラー。\ndevices 内のデバイスが、clGetDeviceIDs で返されたデバイスであるものの現在利用可能でないとき。");
			break;

		case CL_OUT_OF_HOST_MEMORY:
			sprintf(str2, "CL_OUT_OF_HOST_MEMORY\nコンテキスト作成エラー。\nホスト上でのリソース確保に失敗したとき。");
			break;
		}


		if (ret != CL_SUCCESS) {
			sprintf(str1, "%s\n\n%s\n", gpus[k].name, str2);

			context[k] = clCreateContext(NULL, 1, &gpus[k].dev, NULL, NULL, &ret);
			sprintf(str2, "原因不明");
			switch (ret) {							//分岐
			case CL_INVALID_PLATFORM:
				sprintf(str2, "CL_INVALID_PLATFORM\nコンテキスト作成エラー。\nproperties が NULL でかつプラットフォームがひとつも選択されなかったとき。または、properties 内で指定したプラットフォームが有効なプラットフォームでないとき。");
				break;

			case CL_INVALID_PROPERTY:
				sprintf(str2, "CL_INVALID_PROPERTY\n値が支援されたプロパティ名に対して指定した場合、特性中のコンテキスト・プロパティ名が支援されたプロパティ名でない場合、有効でない、あるいは、同じプロパティ名が二度以上指定される場合。しかしながら、拡張cl_khr_gl_sharingが可能になる場合、上記の特性用のテーブルにリストされたもの以外の属性名が、特性の中で指定される場合、CL_INVALID_PROPERTYが返されます。");
				break;

			case CL_INVALID_VALUE:
				sprintf(str2, "CL_INVALID_VALUE\nコンテキスト作成エラー。\ndevices が NULL のとき num_devices が0のとき。  pfn_notify が NULL で user_data が NULL でないとき。");
				break;

			case CL_INVALID_DEVICE:
				sprintf(str2, "CL_INVALID_DEVICE\nコンテキスト作成エラー。\ndevices に有効でないデバイスが含まれているとき");
				break;

			case CL_DEVICE_NOT_AVAILABLE:
				sprintf(str2, "CL_DEVICE_NOT_AVAILABLE\nコンテキスト作成エラー。\ndevices 内のデバイスが、clGetDeviceIDs で返されたデバイスであるものの現在利用可能でないとき。");
				break;

			case CL_OUT_OF_HOST_MEMORY:
				sprintf(str2, "CL_OUT_OF_HOST_MEMORY\nコンテキスト作成エラー。\nホスト上でのリソース確保に失敗したとき。");
				break;
			}
			if (ret != CL_SUCCESS)
			{
				sprintf(str1, "%s\n\n%s\n", gpus[k].name, str2);
			}
		}




		command_queue[k] = clCreateCommandQueue(context[k], gpus[k].dev, CL_QUEUE_PROFILING_ENABLE, &ret);

		switch (ret) {							//分岐
		case CL_INVALID_CONTEXT:
			sprintf(str2, "コマンドキュー生成エラー\ncontext が有効なOpenCLコンテキストでないとき。");
			sprintf(str1, "%s\n\n%s", gpus[k].name, str2);
			break;

		case CL_INVALID_DEVICE:
			sprintf(str2, "コマンドキュー生成エラー\ndevice が有効なOpenCLデバイスでないとき。あるいは、context と関連付けられていないとき。");
			sprintf(str1, "%s\n\n%s", gpus[k].name, str2);
			break;

		case CL_INVALID_VALUE:
			sprintf(str2, "コマンドキュー生成エラー\nproperties に指定した値が有効なものでないとき。");
			sprintf(str1, "%s\n\n%s", gpus[k].name, str2);

			sprintf(str1, "propertiesにアウトオブオーダーを指定したところエラーに成ったので、インオーダー実行として再コマンドキュー生成します");

			command_queue[k] = clCreateCommandQueue(context[k], gpus[k].dev, 0, &ret);
			break;

		case CL_INVALID_QUEUE_PROPERTIES:
			sprintf(str2, "コマンドキュー生成エラー\nproperties に指定した値が、有効であるもののデバイスがサポートしていないとき。");
			sprintf(str1, "%s\n\n%s", gpus[k].name, str2);

			sprintf(str1, "propertiesにアウトオブオーダーを指定したところエラーに成ったので、インオーダー実行として再コマンドキュー生成します");

			command_queue[k] = clCreateCommandQueue(context[k], gpus[k].dev, 0, &ret);
			break;

		case CL_OUT_OF_HOST_MEMORY:
			sprintf(str2, "コマンドキュー生成エラー\nホスト上でのリソース確保に失敗したとき。");
			sprintf(str1, "%s\n\n%s", gpus[k].name, str2);

			break;
		}


		if (ret == CL_SUCCESS) {
			char str3[4];
			gpus[k].CLyukflg = 1;
			sprintf(str2, "コマンドキュー生成成功");
			sprintf(str1, "デバイス名\n%s\n\n%s\nOpenGL連携 %s", gpus[k].name, str2, str3);
		}
		else {
			gpus[k].CLyukflg = 0;
		}
	}

	gdevkz = gkw;
	dev_num = gdevkz;
	stat = gdevkz;
}
*/


/*

static void clGetDevInfo(void)
{
	int p1 = clsetdev;
	char out[90000];
	sprintf(out, "プラットフォームid=開業%d開業デバイス名=開業%s開業デバイスid=開業%d開業デバイス種類(DEFAULT 1,CPU 2,GPU 4,ACCELERATOR 8)=開業%d開業OpenCLソフトウェアドライバのバージョン=開業%s開業デバイスがサポートするOpenCLのバージョン=開業%s開業デバイスのベンダー=開業%s開業ローカルメモリ(共有メモリ)のサイズ(byte)=開業%d開業デバイス最大周波数(MHz)=開業%f開業並列演算コアの数=開業%d開業グローバルメモリサイズ(byte)=開業%f開業メモリオブジェクトとして確保できる最大のメモリサイズ(byte)=開業%f開業ひとつのワークグループ内のワークアイテム数の最大値=開業%d開業ワークグループごとのワークアイテム数のそれぞれの次元についての最大値=開業%d,%d,%d開業2Dイメージの幅の最大サイズ=開業%d開業2Dイメージの高さの最大サイズ=開業%d開業3Dイメージの幅の最大サイズ=開業%d開業3Dイメージの高さの最大サイズ=開業%d開業3Dイメージの深さの最大サイズ=開業%d開業ローカルメモリのタイプ（専用のメモリ 有り1 無し0）=開業%d開業プログラムソースをコンパイルするために利用できるコンパイラの有無(有1 無0)=開業%d開業%d開業OpenCL有効(有1 無0)=開業%d開業デバイスがサポートする拡張機能の名称をスペースで区切ったリストの形式の文字列で返します開業%s開業"
		, gpus[p1].plat, gpus[p1].name, p1, int(gpus[p1].coretype), gpus[p1].version, gpus[p1].cversion, gpus[p1].vendor
		, int(gpus[p1].localmemsize), float(gpus[p1].maxfrq)
		, int(gpus[p1].maxcompunit), float(gpus[p1].globalmemsize), float(gpus[p1].maxmemalloc), int(gpus[p1].maxworkgsz)
		, int(gpus[p1].maxworkisz0), int(gpus[p1].maxworkisz1), int(gpus[p1].maxworkisz2)
		, int(gpus[p1].m2dx), int(gpus[p1].m2dy), int(gpus[p1].m3dx), int(gpus[p1].m3dy), int(gpus[p1].m3dz)
		, int(gpus[p1].localmemtype), int(gpus[p1].compile), int(gpus[p1].CLyukflg), gpus[p1].EXTENSIONS);
	PVal* pval;
	APTR aptr;
	aptr = code_getva(&pval);
	code_setva(pval, aptr, HSPVAR_FLAG_STR, &out);
}

*/





int GetMemSize(cl_mem m)
{
	int st;
	cl_int ret = clGetMemObjectInfo(m, CL_MEM_SIZE, 4, &st, NULL);
	if (ret != CL_SUCCESS) retmeserr12(ret);
	return st;
}



//prm3は参照渡しであることに注意
void AutoReadWriteCopySize(int& prm3, PVal* pval, cl_mem prm1)
{
	int host_sz = pval->size;
	int dev_sz = GetMemSize((cl_mem)prm1);
	if (prm3 == -1)
	{
		prm3 = min(dev_sz, host_sz);
	}
	else
	{
		if (prm3 > host_sz)
		{
			std::string ss = "";
			ss += "コピーサイズ>HSP配列変数サイズ です。\nコピーサイズ=";
			ss += std::to_string(prm3);
			ss += "\nHSP配列変数サイズ=";
			ss += std::to_string(host_sz);
			MessageBox(NULL, ss.c_str(), "エラー", 0);
			puterror(HSPERR_UNSUPPORTED_FUNCTION);
		}
		if (prm3 > dev_sz)
		{
			std::string ss = "";
			ss += "コピーサイズ>デバイスメモリサイズ です。\nコピーサイズ=";
			ss += std::to_string(prm3);
			ss += "\nデバイスメモリサイズ=";
			ss += std::to_string(dev_sz);
			MessageBox(NULL, ss.c_str(), "エラー", 0);
			puterror(HSPERR_UNSUPPORTED_FUNCTION);
		}
	}
	return;
}

//DokrnやReadBuffer系でイベント記録するか
//この関数内で code_getdiをしていることに注意
cl_event* EventOutChk(int k)
{
	//outevent関連
	int eventid = code_getdi(-1);
	cl_event* outeventp = NULL;
	if (eventid >= 0)
	{
		clReleaseEvent(cppeventlist[eventid]);
		outeventp = &cppeventlist[eventid];
		evinfo[eventid].k = k;
		evinfo[eventid].devno = clsetdev;
		evinfo[eventid].queno = clsetque;
	}
	return outeventp;
}

cl_event* GetWaitEvlist()
{
	if (num_event_wait_list == 0)
	{
		return NULL;
	}
	else
	{
		return event_wait_list;
	}
}



void Thread_WriteBuffer(cl_command_queue cmd, cl_mem mem, int ofst, int size,
	const void* vptr, int num_event_wait_list__, cl_event* ev_, cl_event* outevent)
{
	//wait event list関連
	cl_int ret = clEnqueueWriteBuffer(cmd, mem, CL_FALSE, ofst,
		size, vptr, num_event_wait_list__, ev_, outevent);
	thread_start--;
	if (ret != CL_SUCCESS) { retmeserr2(ret); }
	return;
}

void Thread_ReadBuffer(cl_command_queue cmd, cl_mem mem, int ofst, int size,
	void* vptr, int num_event_wait_list__, cl_event* ev_, cl_event* outevent)
{
	//wait event list関連
	cl_int ret = clEnqueueReadBuffer(cmd, mem, CL_FALSE, ofst,
		size, vptr, num_event_wait_list__, ev_, outevent);
	thread_start--;
	if (ret != CL_SUCCESS) { retmeserr2(ret); }
	return;
}






std::string readFileIntoString(const std::string& path) {
	std::ifstream input_file(path);
	if (!input_file.is_open()) {
		MessageBox(NULL, "ファイルが存在しません", "エラー", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		exit(EXIT_FAILURE);
	}
	return std::string((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
}

cl_program WithSource_func(cl_context contxt, std::string s_source, std::string s_option)
{
	size_t sz = s_source.length();
	auto sp = s_source.c_str();
	cl_program program = clCreateProgramWithSource(contxt, 1, (const char**)&sp, (const size_t*)&sz, NULL);
	cl_int err0 = clBuildProgram(program, 1, &device_id[clsetdev], s_option.c_str(), NULL, NULL);
	if (err0 != CL_SUCCESS) retmeserr7(device_id[clsetdev], program);
	return program;
}




































































/*------------------------------------------------------------*/

static int cmdfunc(int cmd)
{
	//		実行処理 (命令実行時に呼ばれます)
	//
	code_next();							// 次のコードを取得(最初に必ず必要です)

	switch (cmd) {							// サブコマンドごとの分岐

	case 0xA0:
		//clini();
		break;

	case 0xA1:
		//clGetDevInfo();
		break;

	case 0xA2:
		//clBye();
		break;

	case 0xA3:
	{
		//buildprogram
		int MAX_SOURCE_SIZE = 1024 * 1024 * 64;
		char* p;
		char pathname[_MAX_PATH];
		p = code_gets();								// 文字列を取得
		strncpy(pathname, p, _MAX_PATH - 1);			// 取得した文字列をコピー

		FILE* fp;
		char* source_str;
		size_t source_size;
		if ((fp = fopen(pathname, "r")) == NULL) {
			puterror(HSPERR_FILE_IO);
		}
		else {
			source_str = (char*)malloc(MAX_SOURCE_SIZE);
			source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
			fclose(fp);
			// Build the program
			program = clCreateProgramWithSource(context[clsetdev], 1, (const char**)&source_str, (const size_t*)&source_size, NULL);
			cl_int err0 = clBuildProgram(program, 1, &device_id[clsetdev], NULL, NULL, NULL);
			if (err0 != CL_SUCCESS) {
				size_t len;
				char buffer[2048 * 64];
				clGetProgramBuildInfo(program, device_id[clsetdev],
					CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
				MessageBox(NULL, (LPCSTR)buffer, "Error on OpenCL code", MB_OK);
				puterror(HSPERR_UNKNOWN_CODE);
			}

			PVal* pval;
			APTR aptr;
			aptr = code_getva(&pval);
			code_setva(pval, aptr, HSPVAR_FLAG_INT, &program);
		}
		break;
	}

	case 0xA4:
	{
		//clCreateKernel();
		p1 = code_getdi(0);		// パラメータ1:数値
		char* ps;
		char pathname[_MAX_PATH];
		ps = code_gets();								// 文字列を取得
		strncpy(pathname, ps, _MAX_PATH - 1);			// 取得した文字列をコピー
		cl_int ret;

		kernel = clCreateKernel((cl_program)p1, pathname, &ret);

		PVal* pval;
		APTR aptr;
		aptr = code_getva(&pval);
		code_setva(pval, aptr, HSPVAR_FLAG_INT, &kernel);
		if (ret != CL_SUCCESS)retmeserr8(ret);
		break;
	}

	case 0xA6:
	{
		//clCreateBuffer();
		PVal* pval;
		APTR aptr;
		aptr = code_getva(&pval);
		p3 = code_getdi(0);		// パラメータ1:数値
		cl_int errcode_ret;
		if (p3 > 1024 * 1024 * 512) {
			MessageBox(NULL, "512Mを超えるサイズは指定できません", "エラー", 0);
			puterror(HSPERR_UNSUPPORTED_FUNCTION);
		}

		mem_obj = clCreateBuffer(context[clsetdev], CL_MEM_READ_WRITE,
			p3, NULL, &errcode_ret);

		int dnryndtyd = 0;
		cl_int ret = clEnqueueWriteBuffer(command_queue[clsetdev * COMMANDQUEUE_PER_DEVICE + clsetque], mem_obj, 0, 1,
			1, &dnryndtyd, 0, NULL, NULL);

		if (ret != CL_SUCCESS) retmeserr9(ret);
		clFinish(command_queue[clsetdev * COMMANDQUEUE_PER_DEVICE + clsetque]);
		code_setva(pval, aptr, HSPVAR_FLAG_INT, &mem_obj);
		break;
	}

	case 0xAE:
	{
		//clfdim();
		PVal* pval1;
		APTR aptr1;
		aptr1 = code_getva(&pval1);
		exinfo->HspFunc_dim(pval1, 8, 0, code_getdi(0), 0, 0, 0);
		break;
	}










































	//////////////////////ここから64移植/////////////////
	//////////////////////ここから64移植/////////////////
	//////////////////////ここから64移植/////////////////
	//////////////////////ここから64移植/////////////////
	//////////////////////ここから64移植/////////////////
	//////////////////////ここから64移植/////////////////
	//////////////////////ここから64移植/////////////////
	//////////////////////ここから64移植/////////////////
	//////////////////////ここから64移植/////////////////


	case 0x50://HCLinit
	{
		cl_int errcode_ret;
		cl_platform_id platform_id[MAX_PLATFORM_IDS];
		cl_uint ret_num_devices;
		cl_uint ret_num_platforms;

		cl_device_id* _device_id;
		_device_id = new cl_device_id[MAX_DEVICE_IDS];

		clGetPlatformIDs(MAX_PLATFORM_IDS, platform_id, &ret_num_platforms);

		dev_num = 0;
		for (int i = 0; i < (int)ret_num_platforms; i++) {
			clGetDeviceIDs(platform_id[i], CL_DEVICE_TYPE_ALL, MAX_DEVICE_IDS, _device_id, &ret_num_devices);
			dev_num += ret_num_devices;
		}

		if (dev_num == 0)
		{
			MessageBox(NULL, "No OpenCL Devices\nHSPCL32Nは使えません", "error", 0);
			puterror(HSPERR_UNSUPPORTED_FUNCTION);
			break;
		}

		device_id = new cl_device_id[dev_num];

		int dev_num_ = 0;
		for (int i = 0; i < (int)ret_num_platforms; i++) {
			clGetDeviceIDs(platform_id[i], CL_DEVICE_TYPE_ALL, MAX_DEVICE_IDS, &_device_id[0], &ret_num_devices);

			for (int i = 0; i < (int)ret_num_devices; i++)
			{
				device_id[dev_num_ + i] = _device_id[i];
			}
			dev_num_ += ret_num_devices;
		}
		//////////デバイス情報を取得しまくっているところ

		//ここでコンテキストとコマンドキュー生成
		context = new cl_context[dev_num];
		command_queue = new cl_command_queue[dev_num * COMMANDQUEUE_PER_DEVICE];

		for (int k = 0; k < dev_num; k++)
		{//コンテキストとコマンド級ーを作る
			context[k] = clCreateContext(NULL, 1, &device_id[k], NULL, NULL, &errcode_ret);
			if (errcode_ret != CL_SUCCESS) retmeserr4(errcode_ret);

			for (int i = 0; i < COMMANDQUEUE_PER_DEVICE; i++)
			{
				command_queue[k * COMMANDQUEUE_PER_DEVICE + i] =
					clCreateCommandQueue(context[k], device_id[k], cmd_properties, &errcode_ret);
				if (errcode_ret != CL_SUCCESS) retmeserr3(errcode_ret);
			}
		}

		//最後にevent変数生成

		cppeventlist = new cl_event[CL_EVENT_MAX];
		event_wait_list = new cl_event[CL_EVENT_MAX];
		evinfo = new EventStruct[CL_EVENT_MAX];
		for (int i = 0; i < CL_EVENT_MAX; i++)
		{
			cppeventlist[i] = NULL;
			event_wait_list[i] = NULL;
		}

		break;
	}

	case 0x8C://HCLdim_i32FromBuffer
	{
		//引数1
		PVal* pval1;
		APTR aptr1;
		aptr1 = code_getva(&pval1);
		//引数2
		int prm2 = code_geti();
		int sz = GetMemSize((cl_mem)prm2);//サイズ
		int n = min((1 << 30), sz);
		//HSP変数初期化
		exinfo->HspFunc_dim(pval1, HSPVAR_FLAG_INT, 0, (n + 3) / 4 * 4, 0, 0, 0);
		//転送
		cl_int ret = clEnqueueReadBuffer(command_queue[clsetdev * COMMANDQUEUE_PER_DEVICE + clsetque], (cl_mem)prm2, CL_TRUE, 0,
			n, pval1->pt, 0, NULL, NULL);
		if (ret != CL_SUCCESS) { retmeserr2(ret); }
		break;
	}

	case 0x8D://HCLdim_fpFromBuffer
	{
		//引数1
		PVal* pval1;
		APTR aptr1;
		aptr1 = code_getva(&pval1);
		//引数2
		int prm2 = code_geti();
		int sz = GetMemSize((cl_mem)prm2);//サイズ
		int n = min((1 << 30), sz);
		//HSP変数初期化
		exinfo->HspFunc_dim(pval1, 8, 0, (n + 7) / 8 * 8, 0, 0, 0);
		//転送
		cl_int ret = clEnqueueReadBuffer(command_queue[clsetdev * COMMANDQUEUE_PER_DEVICE + clsetque], (cl_mem)prm2, CL_TRUE, 0,
			n, pval1->pt, 0, NULL, NULL);
		if (ret != CL_SUCCESS) { retmeserr2(ret); }
		break;
	}

	case 0x8E://HCLdim_dpFromBuffer
	{
		//引数1
		PVal* pval1;
		APTR aptr1;
		aptr1 = code_getva(&pval1);
		//引数2
		int prm2 = code_geti();
		int sz = GetMemSize((cl_mem)prm2);//サイズ
		int n = min((1 << 30), sz);
		//HSP変数初期化
		exinfo->HspFunc_dim(pval1, HSPVAR_FLAG_DOUBLE, 0, (n + 7) / 8 * 8, 0, 0, 0);
		//転送
		cl_int ret = clEnqueueReadBuffer(command_queue[clsetdev * COMMANDQUEUE_PER_DEVICE + clsetque], (cl_mem)prm2, CL_TRUE, 0,
			n, pval1->pt, 0, NULL, NULL);
		if (ret != CL_SUCCESS) { retmeserr2(ret); }
		break;
	}

	case 0x53:	// HCLSetDevice
	{
		clsetdev = code_getdi(0);
		break;
	}

	case 0x55:	// HCLGetDeviceInfo
	{
		cl_device_info devinfoi = code_getdi(0);
		size_t szt = 0;

		clGetDeviceInfo(device_id[clsetdev], devinfoi, sizeof(bufferout), &bufferout, &szt);
		int* datp = (int*)&bufferout[0];
		PVal* pval;
		APTR aptr;
		aptr = code_getva(&pval);
		code_setva(pval, aptr, HSPVAR_FLAG_INT, &datp);

		PVal* pval2;
		APTR aptr2;
		aptr2 = code_getva(&pval2);
		code_setva(pval2, aptr2, HSPVAR_FLAG_INT, &szt);
		break;
	}

	case 0x59://HCLReleaseProgram
	{
		//引数1 kernel
		int prm1 = code_geti();
		clReleaseProgram((cl_program)prm1);
		break;
	}

	case 0x5B:	// HCLSetKernel
	{
		int prm1 = code_geti();
		p2 = code_getdi(0);		// パラメータ2:数値、引数

		int chk = code_getprm();							// パラメーターを取得(型は問わない)

		int type = mpval->flag;							// パラメーターの型を取得
		void* ppttr;
		int sizeofff = -1;
		switch (type) {
		case HSPVAR_FLAG_STR:								// パラメーターが文字列だった時
		{
			ppttr = (char*)mpval->pt;
			sizeofff = 1;
			break;
		}
		case HSPVAR_FLAG_DOUBLE:									// パラメーターが実数だった時
		{
			ppttr = (double*)mpval->pt;
			sizeofff = 8;
			break;
		}
		case 8:								// パラメーターがfloatだった時
		{
			ppttr = (float*)mpval->pt;
			sizeofff = 4;
			break;
		}
		case HSPVAR_FLAG_INT:									// パラメーターが整数だった時
		{
			ppttr = (int*)mpval->pt;
			sizeofff = 4;
			break;
		}
		default:
			puterror(HSPERR_TYPE_MISMATCH);			// サポートしていない型ならばエラー
			sizeofff = -1;
			break;
		}

		int p4 = code_getdi(0);		// パラメータ4:ローカルメモリーフラグ
		int clret = 0;

		if (prm1 == 0)
		{
			MessageBox(NULL, "カーネルidが0です", "エラー", 0);
			puterror(HSPERR_UNSUPPORTED_FUNCTION);
		}
		else
		{
			//prm1カーネルid、p2は引数位置、p3には実態、p4はローカルメモリフラグ
			if (p4 == 0) { clret = clSetKernelArg((cl_kernel)prm1, p2, sizeofff, ppttr); }
			if (p4 != 0) { clret = clSetKernelArg((cl_kernel)prm1, p2, p4, NULL); }
		}

		break;
	}

	case 0x5C://HCLSetKrns
	{
		int prm1 = code_geti();		// パラメータ1:カーネル
		if (prm1 == 0) {
			MessageBox(NULL, "カーネルidが0です", "エラー", 0);
			puterror(HSPERR_UNSUPPORTED_FUNCTION);
		}

		void* ppttr;
		int sizeofff;
		int chk;
		int type;

		for (int i = 0; i < 32; i++) {
			chk = code_getprm();							// パラメーターを取得(型は問わない)
			if (chk <= PARAM_END) {
				break;										// パラメーター省略時の処理
			}
			type = mpval->flag;							// パラメーターの型を取得
			switch (type) {
			case HSPVAR_FLAG_STR:								// パラメーターが文字列だった時
			{
				ppttr = (char*)mpval->pt;
				sizeofff = 1;
				break;
			}
			case HSPVAR_FLAG_DOUBLE:									// パラメーターが実数だった時
			{
				ppttr = (double*)mpval->pt;
				sizeofff = 8;
				break;
			}

			case 8:								// パラメーターがfloatだった時
			{
				ppttr = (float*)mpval->pt;
				sizeofff = 4;
				break;
			}
			case HSPVAR_FLAG_INT:									// パラメーターが整数だった時
			{
				ppttr = (int*)mpval->pt;
				sizeofff = 4;
				break;
			}
			default:
				puterror(HSPERR_TYPE_MISMATCH);			// サポートしていない型ならばエラー
			}

			//p1カーネルid、p2は引数位置、p3には実態、p4はローカルメモリフラグ
			clSetKernelArg((cl_kernel)prm1, i, sizeofff, ppttr);
		}
		break;
	}

	case 0x5D://HCLReleaseKernel
	{
		//引数1 kernel
		int prm1 = code_geti();
		clReleaseKernel((cl_kernel)prm1);
		break;
	}

	case 0x60:	// HCLWriteBuffer
	{
		//引数1
		int prm1 = code_geti();
		//引数2。HSP側の配列変数
		PVal* pval = code_getpval();
		//引数3、コピーサイズ
		int prm3 = code_getdi(-1);
		//引数4、コピー先のofset
		int prm4 = code_getdi(0);
		//引数5、コピー元のofset
		int prm5 = code_getdi(0);//パラメータ5
		int p7 = code_getdi(1);		//ブロッキングモード
		cl_bool TorF = ((p7 == 0) ? CL_FALSE : CL_TRUE);
		//引数省略ならサイズは自動
		AutoReadWriteCopySize(prm3, pval, (cl_mem)prm1);////prm3は参照渡しであることに注意

		//outevent関連
		cl_event* outevent = EventOutChk(prm3);
		//Intel CPUでは正しくブロッキングできないので、event作成してきちんとwaitしてもらう
		cl_event tmpev;

		//wait event list関連
		cl_event* ev_ = GetWaitEvlist();

		cl_int ret = clEnqueueWriteBuffer(command_queue[clsetdev * COMMANDQUEUE_PER_DEVICE + clsetque], (cl_mem)prm1, TorF, prm4,
			prm3, (char*)((pval->pt) + prm5), num_event_wait_list, ev_, &tmpev);
		if (ret != CL_SUCCESS) { retmeserr2(ret); }

		//Intel CPUでは正しくブロッキングできないので、event作成してきちんとwaitしてもらう
		if (TorF == CL_TRUE)
		{
			cl_int ret = clWaitForEvents(1, &tmpev);
			if (ret != CL_SUCCESS) retmeserr6(ret);
		}

		if (outevent != NULL)
		{
			*outevent = tmpev;
		}
		else
		{
			clReleaseEvent(tmpev);
		}

		num_event_wait_list = 0;
		break;
	}

	case 0x61:	// HCLReadBuffer
	{
		//引数1
		int prm1 = code_geti();
		//引数2。HSP側の配列変数
		PVal* pval = code_getpval();
		//引数3、コピーサイズ
		int prm3 = code_getdi(-1);
		//引数4、コピー先のofset
		int prm4 = code_getdi(0);
		//引数5、コピー元のofset
		int prm5 = code_getdi(0);//パラメータ5
		int p7 = code_getdi(1);		//ブロッキングモード
		cl_bool TorF = ((p7 == 0) ? CL_FALSE : CL_TRUE);
		//引数省略ならサイズは自動
		AutoReadWriteCopySize(prm3, pval, (cl_mem)prm1);////prm3は参照渡しであることに注意

		//outevent関連
		cl_event* outevent = EventOutChk(prm3);
		//wait event list関連
		cl_event* ev_ = GetWaitEvlist();
		cl_int ret = clEnqueueReadBuffer(command_queue[clsetdev * COMMANDQUEUE_PER_DEVICE + clsetque], (cl_mem)prm1, TorF, prm4,
			prm3, (char*)((pval->pt) + prm5), num_event_wait_list, ev_, outevent);
		if (ret != CL_SUCCESS) { retmeserr2(ret); }

		num_event_wait_list = 0;
		break;
	}

	//コンパイル時に-pthread が必要
	case 0x86:	// HCLWriteBuffer_NonBlocking
	{
		//引数1
		int prm1 = code_geti();
		//引数2。HSP側の配列変数
		PVal* pval = code_getpval();
		//引数3、コピーサイズ
		int prm3 = code_getdi(-1);
		//引数4、コピー先のofset
		int prm4 = code_getdi(0);
		//引数5、コピー元のofset
		int prm5 = code_getdi(0);//パラメータ5
		cl_bool p7 = code_getdi(1);		//ブロッキングモード
		//引数省略ならサイズは自動
		AutoReadWriteCopySize(prm3, pval, (cl_mem)prm1);////prm3は参照渡しであることに注意

		//outevent関連
		cl_event* outevent = EventOutChk(prm3);

		cl_event* ev_;
		if (num_event_wait_list == 0)
		{
			ev_ = NULL;
		}
		else
		{
			ev_ = new cl_event[num_event_wait_list];
			for (int i = 0; i < num_event_wait_list; i++)
			{
				ev_[i] = event_wait_list[i];
			}
		}

		cl_command_queue cmd = command_queue[clsetdev * COMMANDQUEUE_PER_DEVICE + clsetque];
		void* vptr = (char*)((pval->pt) + prm5);
		mem_obj = (cl_mem)prm1;
		//ここで別スレッドになげる
		thread_start++;
		std::thread th(Thread_WriteBuffer, cmd, mem_obj, prm4, prm3, vptr, num_event_wait_list, ev_, outevent);
		th.detach();
		num_event_wait_list = 0;
		break;
	}

	case 0x87:	// HCLReadBuffer_NonBlocking
	{
		//引数1
		int prm1 = code_geti();
		//引数2。HSP側の配列変数
		PVal* pval = code_getpval();
		//引数3、コピーサイズ
		int prm3 = code_getdi(-1);
		//引数4、コピー先のofset
		int prm4 = code_getdi(0);
		//引数5、コピー元のofset
		int prm5 = code_getdi(0);//パラメータ5
		cl_bool p7 = code_getdi(1);		//ブロッキングモード
		//引数省略ならサイズは自動
		AutoReadWriteCopySize(prm3, pval, (cl_mem)prm1);////prm3は参照渡しであることに注意

		//outevent関連
		cl_event* outevent = EventOutChk(prm3);

		cl_event* ev_;
		if (num_event_wait_list == 0)
		{
			ev_ = NULL;
		}
		else
		{
			ev_ = new cl_event[num_event_wait_list];
			for (int i = 0; i < num_event_wait_list; i++)
			{
				ev_[i] = event_wait_list[i];
			}
		}

		cl_command_queue cmd = command_queue[clsetdev * COMMANDQUEUE_PER_DEVICE + clsetque];
		void* vptr = (char*)((pval->pt) + prm5);
		mem_obj = (cl_mem)prm1;
		//ここで別スレッドになげる
		thread_start++;
		std::thread th(Thread_ReadBuffer, cmd, mem_obj, prm4, prm3, vptr, num_event_wait_list, ev_, outevent);
		th.detach();
		break;
	}

	case 0x62:	// HCLCopyBuffer
	{
		int prm2 = code_geti();//コピー先メモリオブジェクトid
		int prm3 = code_geti();//コピー元メモリオブジェクトid
		int prm4 = code_getdi(-1);// コピーサイズ
		int prm5 = code_getdi(0);// コピー先オフセット
		int prm6 = code_getdi(0);// コピー元オフセット
		//引数省略ならサイズは自動
		int sz2 = GetMemSize((cl_mem)prm2);
		int sz3 = GetMemSize((cl_mem)prm3);
		if (prm4 == -1)
		{
			prm4 = min(sz2, sz3);
		}
		else
		{
			if (prm4 > sz2)
			{
				std::string ss = "";
				ss += "コピーサイズ>コピー先メモリサイズ です。\nコピーサイズ=";
				ss += std::to_string(prm4);
				ss += "\nコピー先サイズ=";
				ss += std::to_string(sz2);
				MessageBox(NULL, ss.c_str(), "エラー", 0);
				puterror(HSPERR_UNSUPPORTED_FUNCTION);
			}
			if (prm4 > sz3)
			{
				std::string ss = "";
				ss += "コピーサイズ>コピー元メモリサイズ です。\nコピーサイズ=";
				ss += std::to_string(prm4);
				ss += "\nコピー元サイズ=";
				ss += std::to_string(sz3);
				MessageBox(NULL, ss.c_str(), "エラー", 0);
				puterror(HSPERR_UNSUPPORTED_FUNCTION);
			}
		}

		//outevent関連
		cl_event* outevent = EventOutChk(prm4);
		//wait event list関連
		cl_event* ev_ = GetWaitEvlist();
		cl_int ret = clEnqueueCopyBuffer(command_queue[clsetdev * COMMANDQUEUE_PER_DEVICE + clsetque],
			(cl_mem)prm3, (cl_mem)prm2, prm6, prm5, prm4, num_event_wait_list, ev_, outevent);
		num_event_wait_list = 0;
		if (ret != CL_SUCCESS)retmeserr2(ret);
		break;
	}

	//GPU上で実行
	case 0x63://HCLFillBuffer_i32
	{
		//引数1 buffer
		int prm1 = code_geti();
		//引数2 pattern
		int pattern = code_getdi(0);
		//引数3、offset(byte)
		int prm4 = code_getdi(0);
		//引数4、size(byte)
		int prm5 = code_getdi(-1);//パラメータ5
		if (prm5 == -1)prm5 = GetMemSize((cl_mem)prm1);

		//outevent関連
		cl_event* outevent = EventOutChk(prm5);
		//wait event list関連
		cl_event* ev_ = GetWaitEvlist();

		cl_int ret = clEnqueueFillBuffer(command_queue[clsetdev * COMMANDQUEUE_PER_DEVICE + clsetque],
			(cl_mem)prm1, &pattern, 4, prm4, prm5, num_event_wait_list, ev_, outevent);
		if (ret != CL_SUCCESS) { retmeserr(ret); }
		num_event_wait_list = 0;
		break;
	}

	case 0x64://HCLFillBuffer_fp
	{
		//引数1 buffer
		int prm1 = code_geti();
		//引数2 pattern
		float pattern = code_getd();
		//引数3、offset(byte)
		int prm4 = code_getdi(0);
		//引数4、size(byte)
		int prm5 = code_getdi(-1);//パラメータ5
		if (prm5 == -1)prm5 = GetMemSize((cl_mem)prm1);

		//outevent関連
		cl_event* outevent = EventOutChk(prm5);
		//wait event list関連
		cl_event* ev_ = GetWaitEvlist();

		cl_int ret = clEnqueueFillBuffer(command_queue[clsetdev * COMMANDQUEUE_PER_DEVICE + clsetque],
			(cl_mem)prm1, &pattern, 4, prm4, prm5, num_event_wait_list, ev_, outevent);
		if (ret != CL_SUCCESS) { retmeserr(ret); }
		num_event_wait_list = 0;
		break;
	}

	//GPU上で実行
	case 0x8B://HCLFillBuffer_dp
	{
		//引数1 buffer
		int prm1 = code_geti();
		//引数2 pattern
		double pattern = code_getdd(0.0);
		//引数3、offset(byte)
		int prm4 = code_getdi(0);
		//引数4、size(byte)
		int prm5 = code_getdi(-1);//パラメータ5
		if (prm5 == -1)prm5 = GetMemSize((cl_mem)prm1);

		//outevent関連
		cl_event* outevent = EventOutChk(prm5);
		//wait event list関連
		cl_event* ev_ = GetWaitEvlist();

		cl_int ret = clEnqueueFillBuffer(command_queue[clsetdev * COMMANDQUEUE_PER_DEVICE + clsetque],
			(cl_mem)prm1, &pattern, 8, prm4, prm5, num_event_wait_list, ev_, outevent);
		if (ret != CL_SUCCESS) { retmeserr(ret); }
		num_event_wait_list = 0;
		break;
	}

	case 0x65://HCLReleaseBuffer
	{
		int prm2 = code_geti();		// パラメータ1:memobj
		cl_int ret = clReleaseMemObject((cl_mem)prm2);
		if (ret != CL_SUCCESS) {
			MessageBox(NULL, "メモリ開放ができませんでした", "エラー", 0);
			puterror(HSPERR_UNSUPPORTED_FUNCTION);
		}
		break;
	}

	case 0x69://HCLWriteIndex_i32
	{
		int memid = code_geti();
		int b = code_geti();//idx
		int data = code_geti();//内容
		cl_int ret = clEnqueueWriteBuffer(command_queue[clsetdev * COMMANDQUEUE_PER_DEVICE + clsetque], (cl_mem)memid, CL_TRUE, b * 4, 4, &data, 0, NULL, NULL);
		if (ret != CL_SUCCESS) { retmeserr2(ret); }
		break;
	}

	case 0x6A://HCLWriteIndex_fp
	{
		int memid = code_geti();
		int b = code_geti();//idx
		float data = code_getd();//内容
		cl_int ret = clEnqueueReadBuffer(command_queue[clsetdev * COMMANDQUEUE_PER_DEVICE + clsetque], (cl_mem)memid, CL_TRUE, b * 4, 4, &data, 0, NULL, NULL);
		if (ret != CL_SUCCESS) { retmeserr2(ret); }
		break;
	}

	case 0x6B://HCLWriteIndex_dp
	{
		int memid = code_geti();
		int b = code_geti();//idx
		double data = code_getd();//内容
		cl_int ret = clEnqueueReadBuffer(command_queue[clsetdev * COMMANDQUEUE_PER_DEVICE + clsetque], (cl_mem)memid, CL_TRUE, b * 8, 8, &data, 0, NULL, NULL);
		if (ret != CL_SUCCESS) { retmeserr2(ret); }
		break;
	}

	// HCLCall
	// str source,int global_size,int local_size,array a,array b,array c・・・・・
	case 0x6C:
	{
		cl_int ret;
		char* c_source;
		c_source = code_gets();								// 文字列を取得
		std::string s_sourse = std::string(c_source);
		program = WithSource_func(context[clsetdev], s_sourse, "");
		ret = clCreateKernelsInProgram(program, 1, &kernel, NULL);//プログラムの中の最初にでてくるカーネルを取得
		if (ret != CL_SUCCESS)retmeserr8(ret);
		//次にglobal_sizeとlocal_size
		size_t global_size = code_getdi(1);	//並列数
		size_t local_size = code_getdi(1);
		//次に引数取得↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
		cl_mem clm[CLCALL_LOOP];
		void* host_ptr[CLCALL_LOOP];
		int copysize[CLCALL_LOOP];
		for (int i = 0; i < CLCALL_LOOP; i++) { copysize[i] = 0; clm[i] = NULL; host_ptr[i] = NULL; }
		//一時配列設定おわった


		for (int i = 0; i < CLCALL_LOOP; i++) {
			//まずcl_mem型かそれ以外かを判定したい
			bool memorval = false;//falseはvalという意味、falseならcl_memを作らない
			bool trygetva = true;//getvaがうまくいけばtrue

			void* ppttr;//①
			int sizeofff;//②
			//この①②があれば転送できる
			int chk;
			int type;
			PVal* pval;

			try
			{
				APTR aptr = code_getva(&pval);
			}
			catch (...)
			{
				trygetva = false;
			}


			if (trygetva == true)
			{
				ppttr = (char*)pval->pt;
				int asz = max(pval->len[1], 1) * max(pval->len[2], 1) * max(pval->len[3], 1) * max(pval->len[4], 1);
				sizeofff = (pval->size) / asz;

				//配列がありそうなら
				if (asz > 1)
				{
					if (pval->offset == 0)//添え字がちゃんと0なら
					{
						memorval = true;
						sizeofff = pval->size;
					}
					else//添え字に意味がありそうなら
					{
						//sizeofff = (pval->size) / asz;
						ppttr = ((char*)ppttr) + ((int)pval->offset * sizeofff);
					}
				}
			}
			else
			{
				chk = code_getprm();
				if (chk <= PARAM_END) {
					break;										// パラメーター省略時の処理
				}
				type = mpval->flag;							// パラメーターの型を取得
				switch (type) {
				case HSPVAR_FLAG_STR:								// パラメーターが文字列だった時
				{
					ppttr = (char*)mpval->pt;
					sizeofff = 1;
					break;
				}
				case HSPVAR_FLAG_DOUBLE:									// パラメーターが実数だった時
				{
					ppttr = (double*)mpval->pt;
					sizeofff = 8;
					break;
				}

				case 8:								// パラメーターがfloatだった時
				{
					ppttr = (float*)mpval->pt;
					sizeofff = 4;
					break;
				}
				case HSPVAR_FLAG_INT:									// パラメーターが整数だった時
				{
					ppttr = (int*)mpval->pt;
					sizeofff = 4;
					break;
				}
				default:
					puterror(HSPERR_TYPE_MISMATCH);			// サポートしていない型ならばエラー
				}
			}




			if (memorval == false) //引数が普通のスカラーなら
			{
				//p1カーネルid、p2は引数位置、p3にはサイズ、p4は実体
				clSetKernelArg(kernel, i, sizeofff, ppttr);
			}
			else //引数がcl_memなら
			{
				clm[i] = clCreateBuffer(context[clsetdev], CL_MEM_READ_WRITE, pval->size, NULL, &ret);
				copysize[i] = pval->size;
				host_ptr[i] = ppttr;
				if (ret != CL_SUCCESS) retmeserr9(ret);
				//vramを確保し転送
				ret = clEnqueueWriteBuffer(command_queue[clsetdev * COMMANDQUEUE_PER_DEVICE + clsetque], clm[i], CL_TRUE, 0,
					copysize[i], host_ptr[i], 0, NULL, NULL);
				if (ret != CL_SUCCESS) { retmeserr2(ret); }
				//ブロッキングありで転送
				//p1カーネルid、p2は引数位置、p3にはサイズ、p4は実体
				clSetKernelArg(kernel, i, 4, &clm[i]);
			}
		}

		//やっと引数設定が終わった↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑

		//次に関数の実行
		if (local_size != 0)
		{
			ret = clEnqueueNDRangeKernel(command_queue[clsetdev * COMMANDQUEUE_PER_DEVICE + clsetque],
				kernel, 1, NULL, &global_size, &local_size, 0, NULL, NULL);
		}
		else
		{
			ret = clEnqueueNDRangeKernel(command_queue[clsetdev * COMMANDQUEUE_PER_DEVICE + clsetque],
				kernel, 1, NULL, &global_size, NULL, 0, NULL, NULL);
		}
		if (ret != CL_SUCCESS) { retmeserr(ret); }

		//GPU→CPUに変数を戻してくる
		for (int i = 0; i < CLCALL_LOOP; i++)
		{
			if (copysize[i] != 0)
			{
				ret = clEnqueueReadBuffer(command_queue[clsetdev * COMMANDQUEUE_PER_DEVICE + clsetque], clm[i], CL_TRUE, 0,
					copysize[i], host_ptr[i], 0, NULL, NULL);
				if (ret != CL_SUCCESS) { retmeserr2(ret); }
				//vramの解放
				ret = clReleaseMemObject(clm[i]);
				if (ret != CL_SUCCESS) {
					MessageBox(NULL, "メモリ開放ができませんでした", "エラー", 0);
					puterror(HSPERR_UNSUPPORTED_FUNCTION);
				}
				clm[i] = NULL; host_ptr[i] = NULL;
			}
		}

		//後片付け
		clReleaseKernel(kernel);
		clReleaseProgram(program);

		//これにて全行程終了のはず！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
		break;
	}

	case 0x6D:	// HCLDoKrn1 int p1,int p2,int p3,int p4
	{
		int prm1 = code_geti();//カ－ネルid
		size_t globalsize = code_getdi(1);
		size_t localsize = code_getdi(0);

		cl_int ret;
		//outevent関連
		cl_event* outevent = EventOutChk(prm1);
		//wait event list関連
		cl_event* ev_ = GetWaitEvlist();

		if (localsize != 0) {
			ret = clEnqueueNDRangeKernel(command_queue[clsetdev * COMMANDQUEUE_PER_DEVICE + clsetque],
				(cl_kernel)prm1, 1, NULL, &globalsize, &localsize, num_event_wait_list, ev_, outevent);
		}
		else
		{
			ret = clEnqueueNDRangeKernel(command_queue[clsetdev * COMMANDQUEUE_PER_DEVICE + clsetque],
				(cl_kernel)prm1, 1, NULL, &globalsize, NULL, num_event_wait_list, ev_, outevent);
		}
		if (ret != CL_SUCCESS) { retmeserr(ret); }
		num_event_wait_list = 0;
		break;
	}

	case 0x52:	// HCLDoKrn2 int p1,int p2,int p3,int p4,int p5,int p6
	{
		int prm1 = code_geti();//カ－ネルid
		size_t globalsize[2];
		size_t localsize[2];
		globalsize[0] = code_geti();
		globalsize[1] = code_geti();
		localsize[0] = code_geti();
		localsize[1] = code_geti();

		cl_int ret;
		//outevent関連
		cl_event* outevent = EventOutChk(prm1);
		//wait event list関連
		cl_event* ev_ = GetWaitEvlist();
		ret = clEnqueueNDRangeKernel(command_queue[clsetdev * COMMANDQUEUE_PER_DEVICE + clsetque],
			(cl_kernel)prm1, 2, NULL, globalsize, localsize, num_event_wait_list, ev_, outevent);
		if (ret != CL_SUCCESS) { retmeserr(ret); }
		num_event_wait_list = 0;
		break;
	}

	case 0x7E:	// HCLDoKrn3 int p1,int p2,int p3,int p4,int p5,int p6,int p7,int p8
	{
		int prm1 = code_geti();//カ－ネルid
		size_t globalsize[3];
		size_t localsize[3];
		globalsize[0] = code_geti();
		globalsize[1] = code_geti();
		globalsize[2] = code_geti();
		localsize[0] = code_geti();
		localsize[1] = code_geti();
		localsize[2] = code_geti();

		cl_int ret;
		//outevent関連
		cl_event* outevent = EventOutChk(prm1);
		//wait event list関連
		cl_event* ev_ = GetWaitEvlist();
		ret = clEnqueueNDRangeKernel(command_queue[clsetdev * COMMANDQUEUE_PER_DEVICE + clsetque],
			(cl_kernel)prm1, 3, NULL, globalsize, localsize, num_event_wait_list, ev_, outevent);
		if (ret != CL_SUCCESS) { retmeserr(ret); }
		num_event_wait_list = 0;
		break;
	}

	case 0x6E:	// HCLDoKernel int p1,int p2,array p3,array p4
	{
		int prm2 = code_geti();		// カーネル
		p3 = code_getdi(1);		// work_dim
		size_t p4[3] = { 1,1,1 };
		size_t ptryes[3] = { 1,1,1 };

		if (p3 == 1)
		{
			p4[0] = code_getdi(1);
			ptryes[0] = code_getdi(0);
		}
		else
		{
			if ((p3 == 2) | (p3 == 3)) {
				PVal* pval1;
				APTR aptr1;	//配列変数の取得
				aptr1 = code_getva(&pval1);//	入力変数の型と実体のポインタを取得
				HspVarProc* phvp1;
				void* ptr1;
				phvp1 = exinfo->HspFunc_getproc(pval1->flag);	//型を処理するHspVarProc構造体へのポインタ
				ptr1 = phvp1->GetPtr(pval1);					//データ（pval1）の実態がある先頭ポインタを取得。

				p4[0] = *(int*)ptr1;
				p4[1] = *((int*)ptr1 + 1);
				if (p3 == 3)
					p4[2] = *((int*)ptr1 + 2);

				PVal* pval2;
				APTR aptr2;	//配列変数の取得
				aptr2 = code_getva(&pval2);//	入力変数の型と実体のポインタを取得
				HspVarProc* phvp2;
				void* ptr2;
				phvp2 = exinfo->HspFunc_getproc(pval2->flag);	//型を処理するHspVarProc構造体へのポインタ
				ptr2 = phvp2->GetPtr(pval2);					//データ（pval1）の実態がある先頭ポインタを取得。

				ptryes[0] = *(int*)ptr2;
				ptryes[1] = *((int*)ptr2 + 1);
				if (p3 == 3)
					ptryes[2] = *((int*)ptr2 + 2);
			}
		}

		cl_int ret;

		//outevent関連
		cl_event* outevent = EventOutChk(prm2);
		//wait event list関連
		cl_event* ev_ = GetWaitEvlist();

		if (ptryes[0] != 0)
		{
			ret = clEnqueueNDRangeKernel(command_queue[clsetdev * COMMANDQUEUE_PER_DEVICE + clsetque],
				(cl_kernel)prm2, p3, NULL, p4, ptryes, num_event_wait_list, ev_, outevent);
		}
		else {
			ret = clEnqueueNDRangeKernel(command_queue[clsetdev * COMMANDQUEUE_PER_DEVICE + clsetque],
				(cl_kernel)prm2, p3, NULL, p4, NULL, num_event_wait_list, ev_, outevent);
		}
		if (ret != CL_SUCCESS) { retmeserr(ret); }

		num_event_wait_list = 0;
		break;
	}

	case 0x6F://HCLDoKrn1_sub
	{
		int prm2 = code_geti();		// パラメータ1:カーネル
		size_t p4 = code_getdi(1);	//並列数
		size_t p5 = code_getdi(1);//ローカルサイズ
		if (p5 == 0) { p5 = 64; }
		size_t p4_1 = (p4 / p5) * p5;//p5で割り切れる数字に
		size_t p4_2 = p4 - p4_1;//問題の端数
		cl_int ret;

		//outevent関連
		cl_event* outevent = EventOutChk(prm2);
		//wait event list関連
		cl_event* ev_ = GetWaitEvlist();

		if (p4_1 != 0) {
			ret = clEnqueueNDRangeKernel(command_queue[clsetdev * COMMANDQUEUE_PER_DEVICE + clsetque],
				(cl_kernel)prm2, 1, NULL, &p4_1, &p5, num_event_wait_list, ev_, outevent);//1回目は無事終わる
			if (ret != CL_SUCCESS) { retmeserr(ret); }
		}
		if (p4_2 != 0) {
			p5 = p4_2;
			ret = clEnqueueNDRangeKernel(command_queue[clsetdev * COMMANDQUEUE_PER_DEVICE + clsetque],
				(cl_kernel)prm2, 1, &p4_1, &p4_2, &p5, num_event_wait_list, ev_, NULL);
			if (ret != CL_SUCCESS) { retmeserr(ret); }
		}
		num_event_wait_list = 0;
		break;
	}

	case 0x70:	// HCLFinish//現在のデバイスの現在のコマンドキューの中にあるタスクを全部待つ
	{
		cl_int ret = clFinish(command_queue[clsetdev * COMMANDQUEUE_PER_DEVICE + clsetque]);
		if (ret != CL_SUCCESS) { retmeserr10(ret); }
		break;
	}

	case 0x71:	// HCLFlush//コマンドキュー
	{
		cl_int ret = clFlush(command_queue[clsetdev * COMMANDQUEUE_PER_DEVICE + clsetque]);
		if (ret != CL_SUCCESS) retmeserr11(ret);
		break;
	}

	case 0x72:	// _ExHCLSetCommandQueueMax
	{
		if (dev_num != 0)
		{
			MessageBox(NULL, "この命令はclinitの前に実行してください", "エラー", 0);
			puterror(HSPERR_UNSUPPORTED_FUNCTION);
		}
		COMMANDQUEUE_PER_DEVICE = code_geti();
		break;
	}

	case 0x73:	// _ExHCLSetCommandQueueProperties
	{
		if (dev_num != 0)
		{
			MessageBox(NULL, "この命令はclinitの前に実行してください", "エラー", 0);
			puterror(HSPERR_UNSUPPORTED_FUNCTION);
		}
		cmd_properties = code_geti();
		break;
	}

	case 0x74://HCLSetCommandQueue
	{
		clsetque = code_getdi(0);
		if (clsetque >= COMMANDQUEUE_PER_DEVICE)
		{
			MessageBox(NULL, "指定したコマンドキューのidが大きすぎます。\n_ExHCLSetCommandQueueMaxで最大値を変更してください。", "エラー", 0);
			puterror(HSPERR_UNSUPPORTED_FUNCTION);
		}
		break;
	}

	case 0x76:	// _ExHCLSetEventMax
	{
		if (dev_num != 0)
		{
			MessageBox(NULL, "この命令はclinitの前に実行してください", "エラー", 0);
			puterror(HSPERR_UNSUPPORTED_FUNCTION);
		}
		CL_EVENT_MAX = code_geti();
		break;
	}

	case 0x77:	//HCLSetWaitEvent int p1
	{
		p1 = code_geti();
		num_event_wait_list = 1;
		event_wait_list[0] = cppeventlist[p1];
		break;
	}

	case 0x78:	//HCLSetWaitEvents array a
	{
		PVal* pval1;
		APTR aptr1;	//配列変数の取得
		aptr1 = code_getva(&pval1);//	入力変数の型と実体のポインタを取得

		num_event_wait_list = pval1->len[1];//全体の要素数

		for (int i = 0; i < num_event_wait_list; i++)
		{
			p2 = *((int*)pval1->pt + i);
			event_wait_list[i] = cppeventlist[p2];
		}
		break;
	}

	case 0x7C:	// HCLWaitForEvent
	{
		int n = code_geti();
		cl_int ret = clWaitForEvents(1, &cppeventlist[n]);
		if (ret != CL_SUCCESS) retmeserr6(ret);
		break;
	}

	case 0x7D:	// HCLWaitForEvents array a
	{
		PVal* pval1;
		APTR aptr1;	//配列変数の取得
		aptr1 = code_getva(&pval1);//	入力変数の型と実体のポインタを取
		int n = pval1->len[1];//全体の要素数
		for (int i = 0; i < n; i++)
		{
			p2 = *((int*)pval1->pt + i);
			event_wait_list[i] = cppeventlist[p2];
		}

		cl_int ret = clWaitForEvents(n, event_wait_list);
		if (ret != CL_SUCCESS) retmeserr6(ret);
		break;
	}

	case 0x89:	// HCLCreateUserEvent
	{
		cl_int ret;
		int event_id = code_geti();
		if (cppeventlist[event_id] != NULL)
			clReleaseEvent(cppeventlist[event_id]);
		cppeventlist[event_id] = clCreateUserEvent(context[clsetdev], &ret);
		if (ret != CL_SUCCESS)retmeserr13(ret);
		break;
	}

	case 0x8A:	// HCLSetUserEventStatus
	{
		cl_int ret;
		int event_id = code_geti();
		cl_int event_stts = code_getdi(CL_SUBMITTED);
		ret = clSetUserEventStatus(cppeventlist[event_id], event_stts);
		if (ret != CL_SUCCESS)retmeserr14(ret);
		break;
	}

	case 0x83:								// newcmd31 //convRGBtoBGR
	{
		_ConvRGBtoBGR();
		break;
	}
	case 0x84:								// newcmd31 //convRGBAtoRGB
	{
		_ConvRGBAtoRGB();
		break;
	}
	case 0x85:								// newcmd31 //convRGBtoRGBA
	{
		_ConvRGBtoRGBA();
		break;
	}

	////////////////////////ここまで/////////////////////
	////////////////////////ここまで/////////////////////
	////////////////////////ここまで/////////////////////
	////////////////////////ここまで/////////////////////
	////////////////////////ここまで/////////////////////
	////////////////////////ここまで/////////////////////
	////////////////////////ここまで/////////////////////
	////////////////////////ここまで/////////////////////
	////////////////////////ここまで/////////////////////
	////////////////////////ここまで/////////////////////
	////////////////////////ここまで/////////////////////

	default:
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
	}
	return RUNMODE_RUN;
}






























































static double ref_doubleval;					// 返値のための変数
static float ref_floatval;						// 返値のための変数
static int ref_int32val;						// 返値のための変数
static char* cptr;								// 返値のための変数



static void* reffunc(int* type_res, int cmd)
{

	//		関数・システム変数の実行処理 (値の参照時に呼ばれます)
	//
	//			'('で始まるかを調べる
	//
	if (*type != TYPE_MARK) puterror(HSPERR_INVALID_FUNCPARAM);
	if (*val != '(') puterror(HSPERR_INVALID_FUNCPARAM);
	code_next();

	bool fDouble = false;
	bool fFloat = false;
	bool fInt = false;
	bool fStr = false;

	switch (cmd) {							// サブコマンドごとの分岐

	case 0xFF:								// float関数
	{
		ref_floatval = 0.0f;
		double dp1 = code_getd();					// 整数値を取得(デフォルトなし)
		ref_floatval = (float)dp1;				// 返値を設定
		*type_res = HspVarFloat_typeid();		// 返値のタイプを指定する
		break;
	}

	//////////////////////////////////////////ここから
	//////////////////////////////////////////ここから
	//////////////////////////////////////////ここから
	//////////////////////////////////////////ここから
	//////////////////////////////////////////ここから
	//////////////////////////////////////////ここから
	//////////////////////////////////////////ここから
	//////////////////////////////////////////ここから
	//////////////////////////////////////////ここから
	//////////////////////////////////////////ここから
	//////////////////////////////////////////ここから
	//////////////////////////////////////////ここから
	//////////////////////////////////////////ここから

	case 0x54://HCLGetDeviceCount
	{
		fInt = true;
		ref_int32val = (int)dev_num;
		break;
	}

	case 0x56://HCLGetSettingDevice
	{
		fInt = true;
		ref_int32val = (int)clsetdev;
		break;
	}

	//str filename,str buildoption=""
	case 0x57:	// HCLCreateProgram
	{
		char* pathname;
		pathname = code_gets();								// 文字列を取得
		std::string s_sourse = readFileIntoString(std::string(pathname));

		char* c_option;
		c_option = code_getds("");								// 文字列を取得
		std::string buildoption;
		buildoption = std::string(c_option);

		// Build the program
		fInt = true;
		ref_int32val = (int)WithSource_func(context[clsetdev], s_sourse, buildoption);
		break;
	}

	case 0x58://HCLCreateProgramWithSource(str "   ")
	{
		char* c_source;
		c_source = code_gets();								// ソースコード
		std::string s_sourse = std::string(c_source);

		char* c_option;
		c_option = code_getds("");								// ビルドオプション文字列を取得
		std::string buildoption;
		buildoption = std::string(c_option);

		// Build the program
		fInt = true;
		ref_int32val = (int)WithSource_func(context[clsetdev], s_sourse, buildoption);
		break;
	}

	//int kernelid,str kansuu_mei
	case 0x5A:	// HCLCreateKernel
	{
		int prm1 = code_geti();
		char* p;
		//char pathname[_MAX_PATH];
		p = code_gets();								// 文字列を取得
		//strncpy(pathname, p, _MAX_PATH - 1);			// 取得した文字列をコピー
		cl_int ret;
		kernel = clCreateKernel((cl_program)prm1, p, &ret);

		fInt = true;
		ref_int32val = (int)kernel;
		if (ret != CL_SUCCESS) retmeserr8(ret);
		break;
	}

	//int bufsize
	case 0x5E:	// HCLCreateBuffer
	{
		int prm1 = code_geti();
		cl_int ret;
		mem_obj = clCreateBuffer(context[clsetdev], CL_MEM_READ_WRITE, prm1, NULL, &ret);
		if (ret != CL_SUCCESS) retmeserr9(ret);

		fInt = true;
		ref_int32val = (int)mem_obj;
		break;
	}

	case 0x5F://HCLCreateBufferFrom
	{
		PVal* pval1 = code_getpval();
		size_t sz = pval1->size;
		cl_int ret;
		mem_obj = clCreateBuffer(context[clsetdev], CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sz, (pval1->pt), &ret);
		if (ret != CL_SUCCESS) retmeserr9(ret);

		fInt = true;
		ref_int32val = (int)mem_obj;
		break;
	}

	case 0x66://HCLReadIndex_i32
	{
		fInt = true;
		int memid = code_geti();
		int b = code_geti();//idx
		cl_int ret = clEnqueueReadBuffer(command_queue[clsetdev * COMMANDQUEUE_PER_DEVICE + clsetque], (cl_mem)memid, CL_TRUE, b * 4, 4, &ref_int32val, 0, NULL, NULL);
		if (ret != CL_SUCCESS) { retmeserr2(ret); }
		break;
	}

	case 0x67://HCLReadIndex_fp
	{
		fFloat = true;
		int memid = code_geti();
		int b = code_geti();//idx
		cl_int ret = clEnqueueReadBuffer(command_queue[clsetdev * COMMANDQUEUE_PER_DEVICE + clsetque], (cl_mem)memid, CL_TRUE, b * 8, 8, &ref_floatval, 0, NULL, NULL);
		if (ret != CL_SUCCESS) { retmeserr2(ret); }
		break;
	}

	case 0x68://HCLReadIndex_dp
	{
		fDouble = true;
		int memid = code_geti();
		int b = code_geti();//idx
		cl_int ret = clEnqueueReadBuffer(command_queue[clsetdev * COMMANDQUEUE_PER_DEVICE + clsetque], (cl_mem)memid, CL_TRUE, b * 8, 8, &ref_doubleval, 0, NULL, NULL);
		if (ret != CL_SUCCESS) { retmeserr2(ret); }
		break;
	}

	case 0x75://HCLGetSettingCommandQueue
	{
		fInt = true;
		ref_int32val = (int)clsetque;
		break;
	}

	//複合関数 64bit整数*8パターンが出力
	case 0x7A:// HCLGetEventLogs
	{
		//0:そのイベントがCL_COMMAND_NDRANGE_KERNELだったか、CL_COMMAND_WRITE_BUFFERだったか
		//1:kernel_idかコピーサイズ
		//2:そのeventを実行したdevice no
		//3:そのeventを実行したque no
		//4:CL_PROFILING_COMMAND_QUEUEDの時間                 0x1280
		//5:CL_PROFILING_COMMAND_SUBMITの時間                 0x1281
		//6:CL_PROFILING_COMMAND_STARTの時間                  0x1282
		//7:CL_PROFILING_COMMAND_ENDの時間                    0x1283
		int eventid = code_geti();
		int secprm = code_geti();
		cl_int ret = CL_SUCCESS;
		cl_ulong tmpret;
		fInt = true;

		switch (secprm)
		{
		case 0:
			ret = clGetEventInfo(cppeventlist[eventid], CL_EVENT_COMMAND_TYPE, 4, &ref_int32val, NULL);
			break;
		case 1:
			ref_int32val = evinfo[eventid].k;
			break;
		case 2:
			ref_int32val = evinfo[eventid].devno;
			break;
		case 3:
			ref_int32val = evinfo[eventid].queno;
			break;
		case 4:
			ret = clGetEventProfilingInfo(cppeventlist[eventid], CL_PROFILING_COMMAND_QUEUED + 0, 8, &tmpret, NULL);
			stat = (int)((unsigned long long)tmpret>>(unsigned long long)32);
			ref_int32val = (unsigned long long)tmpret % ((unsigned long long)1<<(unsigned long long)32);
			break;
		case 5:
			ret = clGetEventProfilingInfo(cppeventlist[eventid], CL_PROFILING_COMMAND_QUEUED + 1, 8, &tmpret, NULL);
			stat = (int)((unsigned long long)tmpret >> (unsigned long long)32);
			ref_int32val = (unsigned long long)tmpret % ((unsigned long long)1 << (unsigned long long)32);
			break;
		case 6:
			ret = clGetEventProfilingInfo(cppeventlist[eventid], CL_PROFILING_COMMAND_QUEUED + 2, 8, &tmpret, NULL);
			stat = (int)((unsigned long long)tmpret >> (unsigned long long)32);
			ref_int32val = (unsigned long long)tmpret % ((unsigned long long)1 << (unsigned long long)32);
			break;
		case 7:
			ret = clGetEventProfilingInfo(cppeventlist[eventid], CL_PROFILING_COMMAND_QUEUED + 3, 8, &tmpret, NULL);
			stat = (int)((unsigned long long)tmpret >> (unsigned long long)32);
			ref_int32val = (unsigned long long)tmpret % ((unsigned long long)1 << (unsigned long long)32);
			break;
		default:
			break;
		}

		if (ret != CL_SUCCESS) retmeserr5(ret);
		break;
	}

	case 0x7B://HCLGetEventStatus
	{
		fInt = true;
		int eventid = code_geti();
		cl_int ret;
		ret = clGetEventInfo(cppeventlist[eventid], CL_EVENT_COMMAND_EXECUTION_STATUS, 4, &ref_int32val, NULL);
		if (ret != CL_SUCCESS) retmeserr5(ret);
		break;
	}

	case 0x79:  //HCLGetKernelName
	{
		fStr = true;
		kernel = (cl_kernel)code_geti();
		size_t szt = 0;
		cl_int ret = clGetKernelInfo(kernel, CL_KERNEL_FUNCTION_NAME, sizeof(hspcharout), &hspcharout, &szt);
		hspcharout[szt] = 0;
		cptr = hspcharout;
		break;
	}


	case 0xAF://clGetDevName
	{
		fStr = true;
		size_t szt = 0;
		clGetDeviceInfo(device_id[clsetdev], CL_DEVICE_NAME, 1024, &hspcharout, &szt);
		hspcharout[szt] = 0;
		cptr = hspcharout;
		break;
	}


	case 0x88:	//HCLGet_NonBlocking_Status
	{
		fInt = true;
		ref_int32val = thread_start;
		break;
	}

	////////////////////////////////////////////////////////////ここまで
	////////////////////////////////////////////////////////////ここまで
	////////////////////////////////////////////////////////////ここまで
	////////////////////////////////////////////////////////////ここまで
	////////////////////////////////////////////////////////////ここまで
	////////////////////////////////////////////////////////////ここまで
	////////////////////////////////////////////////////////////ここまで
	////////////////////////////////////////////////////////////ここまで
	////////////////////////////////////////////////////////////ここまで
	////////////////////////////////////////////////////////////ここまで
	////////////////////////////////////////////////////////////ここまで
	////////////////////////////////////////////////////////////ここまで
	////////////////////////////////////////////////////////////ここまで
	////////////////////////////////////////////////////////////ここまで
	////////////////////////////////////////////////////////////ここまで
	////////////////////////////////////////////////////////////ここまで
	////////////////////////////////////////////////////////////ここまで
	////////////////////////////////////////////////////////////ここまで
	////////////////////////////////////////////////////////////ここまで
	////////////////////////////////////////////////////////////ここまで


	default:
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
	}

	//			'('で終わるかを調べる
	//

	if (*type != TYPE_MARK) puterror(HSPERR_INVALID_FUNCPARAM);
	if (*val != ')') puterror(HSPERR_INVALID_FUNCPARAM);
	code_next();


	if (fDouble) {
		*type_res = HSPVAR_FLAG_DOUBLE;
		return (void*)&ref_doubleval;
	}

	if (fFloat) {
		*type_res = HSPVAR_FLAG_INT;
		return (void*)&ref_floatval;
	}

	if (fInt) {
		*type_res = HSPVAR_FLAG_INT;
		return (void*)&ref_int32val;
	}

	if (fStr) {
		*type_res = HSPVAR_FLAG_STR;
		return (void*)cptr;
	}

	return (void*)cptr;
}









































































static int termfunc(int option)
{
	//		終了処理 (アプリケーション終了時に呼ばれます)
	//
	return 0;
}


int WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, PVOID pvReserved)
{
	//		DLLエントリー (何もする必要はありません)
	//
	return TRUE;
}

EXPORT void WINAPI hsp3cmdinit(HSP3TYPEINFO* info)
{
	//		プラグイン初期化 (実行・終了処理を登録します)
	//
	hsp3sdk_init(info);		// SDKの初期化(最初に行なって下さい)

	info->cmdfunc = cmdfunc;		// 実行関数(cmdfunc)の登録
	info->reffunc = reffunc;		// 参照関数(reffunc)の登録
	info->termfunc = termfunc;		// 終了関数(termfunc)の登録

	registvar(-1, HspVarFloat_Init);		// 新しい型の追加
}