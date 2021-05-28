#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "hsp3plugin.h"
#include "hspvar_float.h"

#include <CL/cl.h>


#define MAX_SOURCE_SIZE 0x200000
#define MAX_PLATFORM_IDS 16 //platform_idの最大値
#define MAX_DEVICE_IDS 32 //devicesの最大値

int gplatkz = 0;//プラットフォームの数  2
int gdevkz = 0;//デバイスの数  4

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
	stat = gdevkz;
}


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


static void clBye(void)
{
	if (onexitflg == 0) {
		cl_int ret;

		if (gdevkz != 0) {
			for (int j = 0; j < gdevkz; j++) {
				ret = clFlush(command_queue[j]);
				ret = clFinish(command_queue[j]);
				ret = clReleaseCommandQueue(command_queue[j]);
				ret = clReleaseContext(context[j]);
			}
			delete[] context;
			delete[] command_queue;
			delete[] gpus;
		}
	}
	onexitflg = 1;
}

static void clBuildProgram(void)
{
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
		cl_int err0 = clBuildProgram(program, 1, &gpus[clsetdev].dev, NULL, NULL, NULL);
		if (err0 != CL_SUCCESS) {
			size_t len;
			char buffer[2048 * 64];
			clGetProgramBuildInfo(program, gpus[clsetdev].dev,
				CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
			MessageBox(NULL, (LPCSTR)buffer, "Error on OpenCL code", MB_OK);
			puterror(HSPERR_UNKNOWN_CODE);
		}

		PVal* pval;
		APTR aptr;
		aptr = code_getva(&pval);
		code_setva(pval, aptr, HSPVAR_FLAG_INT, &program);
	}
}

static void clCreateKernel(void)
{
	p1 = code_getdi(0);		// パラメータ1:数値
	char* p;
	char pathname[_MAX_PATH];
	p = code_gets();								// 文字列を取得
	strncpy(pathname, p, _MAX_PATH - 1);			// 取得した文字列をコピー
	cl_int ret;

	kernel = clCreateKernel((cl_program)p1, pathname, &ret);

	PVal* pval;
	APTR aptr;
	aptr = code_getva(&pval);
	code_setva(pval, aptr, HSPVAR_FLAG_INT, &kernel);

	switch (ret) {							//分岐

	case CL_INVALID_PROGRAM:
		MessageBox(NULL, "program が有効なプログラムオブジェクトでない", "エラー", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;

	case CL_INVALID_PROGRAM_EXECUTABLE:
		MessageBox(NULL, "program に正常にビルドされた実行可能プログラムがない", "エラー", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;

	case CL_INVALID_KERNEL_NAME:
		MessageBox(NULL, "kernel_name が program 内に見つからない", "エラー", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;

	case CL_INVALID_KERNEL_DEFINITION:
		MessageBox(NULL, "kernel_name が与える、引数や引数の型といった __kernel 関数の関数定義が、program がビルドされたすべてのデバイスで同じでない", "エラー", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;

	case CL_INVALID_VALUE:
		MessageBox(NULL, "kernel_name が NULL", "エラー", 0);
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
}

static void clSetKernel(void)
{
	p1 = code_getdi(0);		// パラメータ1:数値
	p2 = code_getdi(0);		// パラメータ1:数値

	int chk = code_getprm();							// パラメーターを取得(型は問わない)
	if (chk <= PARAM_END) {
		return;										// パラメーター省略時の処理
	}
	int type = mpval->flag;							// パラメーターの型を取得
	void* ppttr;
	int sizeofff;
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
	case 8:								// パラメーターが文字列だった時
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

	int p4 = code_getdi(0);		// パラメータ1:数値
	int clret = 0;

	if (p1 == 0) {
		MessageBox(NULL, "カーネルidが0です", "エラー", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
	}
	else {
		//p1カーネルid、p2は引数位置、p3には実態、p4はローカルメモリフラグ
		if (p4 == 0) { clret = clSetKernelArg((cl_kernel)p1, p2, sizeofff, ppttr); }
		if (p4 != 0) { clret = clSetKernelArg((cl_kernel)p1, p2, p4, NULL); }
	}
}

static void clCreateBuffer(void)
{
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
	cl_int ret = clEnqueueWriteBuffer(command_queue[clsetdev], mem_obj, 0, 1,
		1, &dnryndtyd, 0, NULL, NULL);


	switch (errcode_ret) {							//分岐

	case CL_INVALID_CONTEXT:
		MessageBox(NULL, "context が有効なOpenCLコンテキストでない", "エラー", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_VALUE:
		MessageBox(NULL, "読み書き専用メモリが用意できませんでした", "エラー", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_KERNEL:
		MessageBox(NULL, "size が 0 のとき。もしくは size が context 内のデバイスの CL_DEVICE_MAX_MEM_ALLOC_SIZE の値より大きい", "エラー", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_HOST_PTR:
		MessageBox(NULL, "host_ptr が NULL で CL_MEM_USE_HOST_PTR もしくは CL_MEM_COPY_HOST_PTR が flags に指定されているとき。もしくは、CL_MEM_COPY_HOST_PTR や CL_MEM_USE_HOST_PTR が設定されていないのに host_ptr が NULL でない", "エラー", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_MEM_OBJECT_ALLOCATION_FAILURE:
		MessageBox(NULL, "バッファオブジェクトのメモリを確保するのに失敗した", "エラー", 0);
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

	if (ret != CL_SUCCESS) { retmeserr2(ret); }

	clFinish(command_queue[clsetdev]);
	code_setva(pval, aptr, HSPVAR_FLAG_INT, &mem_obj);
}

static void clWriteBuffer(void)
{
	p2 = code_getdi(0);		// パラメータ1:数値

	PVal* pval1;
	APTR aptr1;	//配列変数の取得
	aptr1 = code_getva(&pval1);//	入力変数の型と実体のポインタを取得
	HspVarProc* phvp1;
	void* ptr1;
	phvp1 = exinfo->HspFunc_getproc(pval1->flag);	//型を処理するHspVarProc構造体へのポインタ
	ptr1 = phvp1->GetPtr(pval1);					//データ（pval1）の実態がある先頭ポインタを取得。

	p4 = code_getdi(0);		//サイズ
	p5 = code_getdi(0);		//コピー先オフセットサイズ
	p6 = code_getdi(0);		//コピー元オフセットサイズ
	cl_bool p7 = code_getdi(1);		//ブロッキングモード

	cl_int ret = clEnqueueWriteBuffer(command_queue[clsetdev], (cl_mem)p2, p7, p5,
		p4, (char*)ptr1 + p6, 0, NULL, NULL);

	if (ret != CL_SUCCESS) { retmeserr2(ret); }
}

static void clReadBuffer(void)
{
	p2 = code_getdi(0);		// パラメータ1:数値

	PVal* pval1;
	APTR aptr1;	//配列変数の取得
	aptr1 = code_getva(&pval1);//	入力変数の型と実体のポインタを取得
	HspVarProc* phvp1;
	void* ptr1;
	phvp1 = exinfo->HspFunc_getproc(pval1->flag);	//型を処理するHspVarProc構造体へのポインタ
	ptr1 = phvp1->GetPtr(pval1);					//データ（pval1）の実態がある先頭ポインタを取得。

	p4 = code_getdi(0);		//サイズ
	p5 = code_getdi(0);		//コピー元オフセットサイズ
	p6 = code_getdi(0);		//コピー先オフセットサイズ
	cl_bool p7 = code_getdi(1);		//ブロッキングモード

	cl_int ret = clEnqueueReadBuffer(command_queue[clsetdev], (cl_mem)p2, p7, p5,
		p4, (char*)ptr1 + p6, 0, NULL, NULL);

	if (ret != CL_SUCCESS) { retmeserr2(ret); }

}

static void clCopyBuffer(void)
{
	p2 = code_getdi(0);//コピー先メモリオブジェクトid
	p3 = code_getdi(0);//コピー元メモリオブジェクトid
	size_t p4 = code_getdi(0);		// コピーサイズ
	size_t p5 = code_getdi(0);		// コピー先オフセット
	size_t p6 = code_getdi(0);		// コピー元オフセット
	cl_bool p7 = code_getdi(1);		//ブロッキングモード
	cl_int ret = clEnqueueCopyBuffer(command_queue[clsetdev], (cl_mem)p3, (cl_mem)p2, p6, p5, p4, 0, NULL, NULL);


	switch (ret) {							//分岐
	case CL_INVALID_COMMAND_QUEUE:
		MessageBox(NULL, "command_queue is not a valid command-queue", "エラー", 0);

		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_CONTEXT:
		MessageBox(NULL, "メモリオブジェクトが別のデバイスで作成された可能性があります", "エラー", 0);

		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_MEM_OBJECT:
		MessageBox(NULL, "メモリオブジェクトの実体がありません。メモリオブジェクトが別のデバイスで作成された可能性があります。", "エラー", 0);

		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_VALUE:
		MessageBox(NULL, "アドレスアクセス違反です。書き込み領域or読み込み領域がはみ出してます。", "エラー", 0);

		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_MEM_COPY_OVERLAP:
		MessageBox(NULL, "アドレスアクセス違反です。書き込み領域か読み込み領域がはみ出してます。", "エラー", 0);

		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_MEM_OBJECT_ALLOCATION_FAILURE:
		MessageBox(NULL, "data store のためにallocate memoryするのを失敗しました", "エラー", 0);

		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_OUT_OF_RESOURCES:
		MessageBox(NULL, "デバイス(GPU)上でのリソース確保に失敗した", "エラー", 0);

		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_OUT_OF_HOST_MEMORY:
		MessageBox(NULL, "ホスト(CPU)上でのリソース確保に失敗した", "エラー", 0);

		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	}

	if (p7) {
		cl_int ret = clFinish(command_queue[clsetdev]);
		switch (ret) {
		case CL_INVALID_COMMAND_QUEUE:
			MessageBox(NULL, "第一引数が有効な値ではありません", "エラー", 0);

			puterror(HSPERR_UNSUPPORTED_FUNCTION);
			break;
		case CL_INVALID_CONTEXT:
			MessageBox(NULL, "あるいは第一引数と event_wait_list 内のイベントと関連付けられたデバイスが同じでない", "エラー", 0);

			puterror(HSPERR_UNSUPPORTED_FUNCTION);
			break;
		case CL_INVALID_EVENT:
			MessageBox(NULL, "event_listのイベントオブジェクトが不正", "エラー", 0);

			puterror(HSPERR_UNSUPPORTED_FUNCTION);
			break;
		case CL_OUT_OF_HOST_MEMORY:
			MessageBox(NULL, "ホスト上でのリソース確保に失敗した", "エラー", 0);

			puterror(HSPERR_UNSUPPORTED_FUNCTION);
			break;
		}
	}
}

static void clDoKernel(void)
{
	p2 = code_getdi(0);		// カーネル
	p3 = code_getdi(1);		// work_dim
	size_t p4[3];
	size_t ptryes[3];

	if (p3 == 1) {
		p4[0] = code_getdi(1);
		ptryes[0] = code_getdi(0);
	}
	else {
		if ((p3 == 2) | (p3 == 3)) {
			PVal* pval1;
			APTR aptr1;	//配列変数の取得
			aptr1 = code_getva(&pval1);//	入力変数の型と実体のポインタを取得
			HspVarProc* phvp1;
			void* ptr1;
			phvp1 = exinfo->HspFunc_getproc(pval1->flag);	//型を処理するHspVarProc構造体へのポインタ
			ptr1 = phvp1->GetPtr(pval1);					//データ（pval1）の実態がある先頭ポインタを取得。

			p4[0] = *(size_t*)ptr1;
			p4[1] = *((size_t*)ptr1 + 1);
			p4[2] = *((size_t*)ptr1 + 2);


			PVal* pval2;
			APTR aptr2;	//配列変数の取得
			aptr2 = code_getva(&pval2);//	入力変数の型と実体のポインタを取得
			HspVarProc* phvp2;
			void* ptr2;
			phvp2 = exinfo->HspFunc_getproc(pval2->flag);	//型を処理するHspVarProc構造体へのポインタ
			ptr2 = phvp2->GetPtr(pval2);					//データ（pval1）の実態がある先頭ポインタを取得。

			ptryes[0] = *(size_t*)ptr2;
			ptryes[1] = *((size_t*)ptr2 + 1);
			ptryes[2] = *((size_t*)ptr2 + 2);
		}
	}



	cl_int ret;
	if (ptryes[0] != 0) {
		ret = clEnqueueNDRangeKernel(command_queue[clsetdev], (cl_kernel)p2, p3, NULL, p4, ptryes, 0, NULL, NULL);
	}
	else {
		ret = clEnqueueNDRangeKernel(command_queue[clsetdev], (cl_kernel)p2, p3, NULL, p4, NULL, 0, NULL, NULL);
	}
	if (ret != CL_SUCCESS) { retmeserr(ret); }

}

static void clReleaseKernel(void)
{
	p2 = code_getdi(0);		// パラメータ1:数値

	cl_int ret = clReleaseKernel((cl_kernel)p2);
	if (ret != CL_SUCCESS) {
		MessageBox(NULL, "カーネル開放ができませんでした", "エラー", 0);

		puterror(HSPERR_UNSUPPORTED_FUNCTION);
	}
}

static void clReleaseProgram(void)
{
	p2 = code_getdi(0);		// パラメータ1:数値

	cl_int ret = clReleaseProgram((cl_program)p2);
	if (ret != CL_SUCCESS) {
		MessageBox(NULL, "プログラムデータ開放ができませんでした", "エラー", 0);

		puterror(HSPERR_UNSUPPORTED_FUNCTION);
	}
}

static void clReleaseMemObject(void)
{
	p2 = code_getdi(0);		// パラメータ1:数値

	cl_int ret = 0;
	ret = clReleaseMemObject((cl_mem)p2);

	if (ret != CL_SUCCESS) {
		MessageBox(NULL, "メモリ開放ができませんでした", "エラー", 0);

		puterror(HSPERR_UNSUPPORTED_FUNCTION);
	}
}

static void clfdim(void)
{
	PVal* pval1;
	APTR aptr1;
	aptr1 = code_getva(&pval1);
	exinfo->HspFunc_dim(pval1, 8, 0, code_getdi(0), 0, 0, 0);
}

static void clSetDev(void)//デバイスセット
{
	clsetdev = code_getdi(0);
}

static void clWaitTask(void)//デバイスにある全部のタスク待ち
{
	cl_int ret = clFinish(command_queue[clsetdev]);

	switch (ret) {							//分岐

	case CL_INVALID_COMMAND_QUEUE:
		MessageBox(NULL, "第一引数が有効な値ではありません", "エラー", 0);

		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_CONTEXT:
		MessageBox(NULL, "あるいは第一引数と event_wait_list 内のイベントと関連付けられたデバイスが同じでない", "エラー", 0);

		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_EVENT:
		MessageBox(NULL, "event_listのイベントオブジェクトが不正", "エラー", 0);

		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_OUT_OF_HOST_MEMORY:
		MessageBox(NULL, "ホスト上でのリソース確保に失敗した", "エラー", 0);

		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	}

}

static void clDoKrn1(void)
{
	int p2 = code_getdi(0);		// カーネル
	size_t p4 = code_getdi(1);
	size_t ptryes = code_getdi(0);
	cl_int ret;
	if (ptryes != 0) {
		ret = clEnqueueNDRangeKernel(command_queue[clsetdev], (cl_kernel)p2, 1, NULL, &p4, &ptryes, 0, NULL, NULL);
	}
	else {
		ret = clEnqueueNDRangeKernel(command_queue[clsetdev], (cl_kernel)p2, 1, NULL, &p4, NULL, 0, NULL, NULL);
	}
	if (ret != CL_SUCCESS) { retmeserr(ret); }
}

static void clDoKrn2(void)
{
	p2 = code_getdi(0);		// カーネル
	size_t p4[2];
	p4[0] = code_getdi(1);
	p4[1] = code_getdi(1);
	size_t ptryes[2];
	ptryes[0] = code_getdi(0);
	ptryes[1] = code_getdi(0);

	cl_int ret;
	if (ptryes[0] != 0) {
		ret = clEnqueueNDRangeKernel(command_queue[clsetdev], (cl_kernel)p2, 2, NULL, p4, ptryes, 0, NULL, NULL);
	}
	else {
		ret = clEnqueueNDRangeKernel(command_queue[clsetdev], (cl_kernel)p2, 2, NULL, p4, NULL, 0, NULL, NULL);
	}
	if (ret != CL_SUCCESS) { retmeserr(ret); }
}

static void clDoKrn3(void)
{
	p2 = code_getdi(0);		// カーネル
	size_t p4[3];
	p4[0] = code_getdi(1);
	p4[1] = code_getdi(1);
	p4[2] = code_getdi(1);
	size_t ptryes[3];
	ptryes[0] = code_getdi(0);
	ptryes[1] = code_getdi(0);
	ptryes[2] = code_getdi(0);

	cl_int ret;
	if (ptryes[0] != 0) {
		ret = clEnqueueNDRangeKernel(command_queue[clsetdev], (cl_kernel)p2, 3, NULL, p4, ptryes, 0, NULL, NULL);
	}
	else {
		ret = clEnqueueNDRangeKernel(command_queue[clsetdev], (cl_kernel)p2, 3, NULL, p4, NULL, 0, NULL, NULL);
	}
	if (ret != CL_SUCCESS) { retmeserr(ret); }
}













//----------------------------------------------------------------------------------------ここ以下はOpenGL--------------------------------
//----------------------------------------------------------------------------------------ここ以下はOpenGL--------------------------------
//----------------------------------------------------------------------------------------ここ以下はOpenGL--------------------------------
//----------------------------------------------------------------------------------------ここ以下はOpenGL--------------------------------
//----------------------------------------------------------------------------------------ここ以下はOpenGL--------------------------------
//----------------------------------------------------------------------------------------ここ以下はOpenGL--------------------------------
//----------------------------------------------------------------------------------------ここ以下はOpenGL--------------------------------
//----------------------------------------------------------------------------------------ここ以下はOpenGL--------------------------------
//----------------------------------------------------------------------------------------ここ以下はOpenGL--------------------------------
//----------------------------------------------------------------------------------------ここ以下はOpenGL--------------------------------
//----------------------------------------------------------------------------------------ここ以下はOpenGL--------------------------------
//----------------------------------------------------------------------------------------ここ以下はOpenGL--------------------------------
//----------------------------------------------------------------------------------------ここ以下はOpenGL--------------------------------
//----------------------------------------------------------------------------------------ここ以下はOpenGL--------------------------------
//----------------------------------------------------------------------------------------ここ以下はOpenGL--------------------------------
//----------------------------------------------------------------------------------------ここ以下はOpenGL--------------------------------





static void newcmd48(void)
{
	PVal* pval1;
	APTR aptr1;	//配列変数の取得
	aptr1 = code_getva(&pval1);//	入力変数の型と実体のポインタを取得
	HspVarProc* phvp1;
	void* ptr1;
	phvp1 = exinfo->HspFunc_getproc(pval1->flag);	//型を処理するHspVarProc構造体へのポインタ
	ptr1 = phvp1->GetPtr(pval1);					//データ（pval1）の実態がある先頭ポインタを取得。


	PVal* pval2;
	APTR aptr2;	//配列変数の取得
	aptr2 = code_getva(&pval2);//	入力変数の型と実体のポインタを取得
	HspVarProc* phvp2;
	void* ptr2;
	phvp2 = exinfo->HspFunc_getproc(pval2->flag);	//型を処理するHspVarProc構造体へのポインタ
	ptr2 = phvp2->GetPtr(pval2);					//データ（pval1）の実態がある先頭ポインタを取得。

	p3 = code_getdi(0);

	char* dmptr1 = (char*)ptr1;
	char* dmptr2 = (char*)ptr2;

	for (int i = 0; i < p3; i += 3) {
		dmptr2[i + 2] = dmptr1[i];
		dmptr2[i + 1] = dmptr1[i + 1];
		dmptr2[i] = dmptr1[i + 2];
	}
}
static void newcmd49(void)
{
	PVal* pval1;
	APTR aptr1;	//配列変数の取得
	aptr1 = code_getva(&pval1);//	入力変数の型と実体のポインタを取得
	HspVarProc* phvp1;
	void* ptr1;
	phvp1 = exinfo->HspFunc_getproc(pval1->flag);	//型を処理するHspVarProc構造体へのポインタ
	ptr1 = phvp1->GetPtr(pval1);					//データ（pval1）の実態がある先頭ポインタを取得。

	PVal* pval2;
	APTR aptr2;	//配列変数の取得
	aptr2 = code_getva(&pval2);//	入力変数の型と実体のポインタを取得
	HspVarProc* phvp2;
	void* ptr2;
	phvp2 = exinfo->HspFunc_getproc(pval2->flag);	//型を処理するHspVarProc構造体へのポインタ
	ptr2 = phvp2->GetPtr(pval2);					//データ（pval1）の実態がある先頭ポインタを取得。

	p3 = code_getdi(0) / 4;

	char* dmptr1 = (char*)ptr1;
	char* dmptr2 = (char*)ptr2;
	int idx1 = 0;
	int idx2 = 0;

	for (int i = 0; i < p3; i++) {
		idx1 = i * 3;
		idx2 = i * 4;
		dmptr2[idx1] = dmptr1[idx2];
		dmptr2[idx1 + 1] = dmptr1[idx2 + 1];
		dmptr2[idx1 + 2] = dmptr1[idx2 + 2];
	}
}
static void newcmd50(void)
{
	PVal* pval1;
	APTR aptr1;	//配列変数の取得
	aptr1 = code_getva(&pval1);//	入力変数の型と実体のポインタを取得
	HspVarProc* phvp1;
	void* ptr1;
	phvp1 = exinfo->HspFunc_getproc(pval1->flag);	//型を処理するHspVarProc構造体へのポインタ
	ptr1 = phvp1->GetPtr(pval1);					//データ（pval1）の実態がある先頭ポインタを取得。

	PVal* pval2;
	APTR aptr2;	//配列変数の取得
	aptr2 = code_getva(&pval2);//	入力変数の型と実体のポインタを取得
	HspVarProc* phvp2;
	void* ptr2;
	phvp2 = exinfo->HspFunc_getproc(pval2->flag);	//型を処理するHspVarProc構造体へのポインタ
	ptr2 = phvp2->GetPtr(pval2);					//データ（pval1）の実態がある先頭ポインタを取得。

	p3 = code_getdi(0) / 3;


	int toumeiflg = code_getdi(0);
	char toumei_r = code_getdi(0);
	char toumei_g = code_getdi(0);
	char toumei_b = code_getdi(0);


	char* dmptr1 = (char*)ptr1;
	char* dmptr2 = (char*)ptr2;
	int idx1 = 0;
	int idx2 = 0;
	char tmpr = 0;
	char tmpg = 0;
	char tmpb = 0;

	if (toumeiflg == 0) {
		for (int i = 0; i < p3; i++) {
			idx1 = i * 4;
			idx2 = i * 3;
			tmpr = dmptr1[idx2];
			tmpg = dmptr1[idx2 + 1];
			tmpb = dmptr1[idx2 + 2];
			dmptr2[idx1] = tmpr;
			dmptr2[idx1 + 1] = tmpg;
			dmptr2[idx1 + 2] = tmpb;
			dmptr2[idx1 + 3] = 255;
		}
	}
	else {
		for (int i = 0; i < p3; i++) {
			idx1 = i * 4;
			idx2 = i * 3;
			tmpr = dmptr1[idx2];
			tmpg = dmptr1[idx2 + 1];
			tmpb = dmptr1[idx2 + 2];
			dmptr2[idx1] = tmpr;
			dmptr2[idx1 + 1] = tmpg;
			dmptr2[idx1 + 2] = tmpb;
			dmptr2[idx1 + 3] = 255;
			if (toumei_r == tmpr) { if (toumei_g == tmpg) { if (toumei_b == tmpb) { dmptr2[idx1 + 3] = 0; } } }
		}
	}
}




/*------------------------------------------------------------*/

static int cmdfunc(int cmd)
{
	//		実行処理 (命令実行時に呼ばれます)
	//
	code_next();							// 次のコードを取得(最初に必ず必要です)

	switch (cmd) {							// サブコマンドごとの分岐

	case 0x00:
		clini();
		break;

	case 0x01:
		clGetDevInfo();
		break;

	case 0x02:
		clBye();
		break;

	case 0x03:
		clBuildProgram();
		break;

	case 0x04:
		clCreateKernel();
		break;

	case 0x05:
		clSetKernel();
		break;

	case 0x06:
		clCreateBuffer();
		break;

	case 0x07:
		clWriteBuffer();
		break;

	case 0x08:
		clReadBuffer();
		break;

	case 0x09:
		clCopyBuffer();
		break;

	case 0x0A:
		clDoKernel();
		break;

	case 0x0B:
		clReleaseKernel();
		break;

	case 0x0C:
		clReleaseProgram();
		break;

	case 0x0D:
		clReleaseMemObject();
		break;

	case 0x0E:
		clfdim();
		break;

	case 0x0F:
		clSetDev();
		break;

	case 0x10:
		clWaitTask();
		break;

	case 0x11:
		clDoKrn1();
		break;

	case 0x12:
		clDoKrn2();
		break;

	case 0x13:
		clDoKrn3();
		break;






	case 0x2F:								// newcmd31 //convRGBtoBGR
		newcmd48();
		break;
	case 0x30:								// newcmd31 //convRGBAtoRGB
		newcmd49();
		break;
	case 0x31:								// newcmd31 //convRGBtoRGBA
		newcmd50();
		break;
	default:
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
	}
	return RUNMODE_RUN;
}






















//----------------------------------------------------------------------------------------ここ以下は自分で作った関数--------------------------------
//----------------------------------------------------------------------------------------ここ以下は自分で作った関数--------------------------------
//----------------------------------------------------------------------------------------ここ以下は自分で作った関数--------------------------------
//----------------------------------------------------------------------------------------ここ以下は自分で作った関数--------------------------------
//----------------------------------------------------------------------------------------ここ以下は自分で作った関数--------------------------------
//----------------------------------------------------------------------------------------ここ以下は自分で作った関数--------------------------------
//----------------------------------------------------------------------------------------ここ以下は自分で作った関数--------------------------------
//----------------------------------------------------------------------------------------ここ以下は自分で作った関数--------------------------------
//----------------------------------------------------------------------------------------ここ以下は自分で作った関数--------------------------------
//----------------------------------------------------------------------------------------ここ以下は自分で作った関数--------------------------------
//----------------------------------------------------------------------------------------ここ以下は自分で作った関数--------------------------------
//----------------------------------------------------------------------------------------ここ以下は自分で作った関数--------------------------------
//----------------------------------------------------------------------------------------ここ以下は自分で作った関数--------------------------------

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




void retmeserr2(cl_int ret)
{
	switch (ret) {							//分岐
	case CL_INVALID_COMMAND_QUEUE:
		MessageBox(NULL, "command_queue is not a valid command-queue", "エラー", 0);

		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_CONTEXT:
		MessageBox(NULL, "メモリオブジェクトが別のデバイスで作成された可能性があります", "エラー", 0);

		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_MEM_OBJECT:
		MessageBox(NULL, "メモリオブジェクトの実体がありません。メモリオブジェクトが別のデバイスで作成された可能性があります。", "エラー", 0);

		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_VALUE:
		MessageBox(NULL, "アドレスアクセス違反です。読み込み領域がはみ出してます。", "エラー", 0);

		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_MEM_OBJECT_ALLOCATION_FAILURE:
		MessageBox(NULL, "data store のためにallocate memoryするのを失敗しました", "エラー", 0);

		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_OUT_OF_RESOURCES:
		MessageBox(NULL, "デバイス(GPU)上でのリソース確保に失敗した", "エラー", 0);

		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_OUT_OF_HOST_MEMORY:
		MessageBox(NULL, "ホスト(CPU)上でのリソース確保に失敗した", "エラー", 0);

		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	}
	//上のどれでもなければ
	MessageBox(NULL, "原因不明のエラーです", "エラー", 0);
	puterror(HSPERR_UNSUPPORTED_FUNCTION);

}



























//----------------------------------------------------------------------------------------ここ以下はHSPのサンプルコピー--------------------------------
//----------------------------------------------------------------------------------------ここ以下はHSPのサンプルコピー--------------------------------
//----------------------------------------------------------------------------------------ここ以下はHSPのサンプルコピー--------------------------------
//----------------------------------------------------------------------------------------ここ以下はHSPのサンプルコピー--------------------------------
//----------------------------------------------------------------------------------------ここ以下はHSPのサンプルコピー--------------------------------
//----------------------------------------------------------------------------------------ここ以下はHSPのサンプルコピー--------------------------------
//----------------------------------------------------------------------------------------ここ以下はHSPのサンプルコピー--------------------------------
//----------------------------------------------------------------------------------------ここ以下はHSPのサンプルコピー--------------------------------
//----------------------------------------------------------------------------------------ここ以下はHSPのサンプルコピー--------------------------------
//----------------------------------------------------------------------------------------ここ以下はHSPのサンプルコピー--------------------------------
//----------------------------------------------------------------------------------------ここ以下はHSPのサンプルコピー--------------------------------
//----------------------------------------------------------------------------------------ここ以下はHSPのサンプルコピー--------------------------------
//----------------------------------------------------------------------------------------ここ以下はHSPのサンプルコピー--------------------------------
//----------------------------------------------------------------------------------------ここ以下はHSPのサンプルコピー--------------------------------
//----------------------------------------------------------------------------------------ここ以下はHSPのサンプルコピー--------------------------------

static double ref_fval;						// 返値のための変数
static double dp1;

static void* reffunc(int* type_res, int cmd)
{
	//		関数・システム変数の実行処理 (値の参照時に呼ばれます)
	//
	//			'('で始まるかを調べる
	//
	if (*type != TYPE_MARK) puterror(HSPERR_INVALID_FUNCPARAM);
	if (*val != '(') puterror(HSPERR_INVALID_FUNCPARAM);
	code_next();
	float p9;
	int  p10;
	double p8;
	char p7;
	cl_int ret;
	int p6 = 0;
	int p5 = 0;
	memcpy(&ref_fval, &p6, 4);
	memcpy(&ref_fval, &p5, 4);

	switch (cmd) {							// サブコマンドごとの分岐

	case 0xFF:								// float関数
		ref_fval = 0.0;
		dp1 = code_getd();					// 整数値を取得(デフォルトなし)
		p9 = (float)dp1;				// 返値を設定
		memcpy(&ref_fval, &p9, 4);
		*type_res = HspVarFloat_typeid();		// 返値のタイプを指定する
		break;


	default:
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
	}

	//			'('で終わるかを調べる
	//
	if (*type != TYPE_MARK) puterror(HSPERR_INVALID_FUNCPARAM);
	if (*val != ')') puterror(HSPERR_INVALID_FUNCPARAM);
	code_next();

	return (void*)&ref_fval;
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