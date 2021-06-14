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
#define MAX_PLATFORM_IDS 16 //platform_id�̍ő�l
#define MAX_DEVICE_IDS 32 //devices�̍ő�l

int gplatkz = 0;//�v���b�g�t�H�[���̐�  2
int gdevkz = 0;//�f�o�C�X�̐�  4


int dev_num = 0;

cl_platform_id platform_id[MAX_PLATFORM_IDS];
cl_context* context;
cl_command_queue* command_queue;
cl_mem mem_obj;//���4096�P�p
cl_program program;
cl_kernel kernel;
cl_event dmyevent;
cl_event* ddmyevent = &dmyevent;
int clsetdev = 0;
void gtflt(float* ff0);
void gtint(int* ii0);
void gtdbl(double* dd0);
void retmeserr(cl_int ret);//clEnqueueNDRangeKernel �Ŏ��s�������o���G���[���b�Z�[�W���܂Ƃ߂��֐�
void retmeserr2(cl_int ret);//clRead�Ŏ��s�������o���G���[���b�Z�[�W���܂Ƃ߂��֐�
*/



const int MAX_PLATFORM_IDS = 32;//platform_id�̍ő�l
const int MAX_DEVICE_IDS = 2048;//��x�Ɏ擾�ł���device�̍ő�l
const int CLCALL_LOOP = 32;//HCLCall�̈����̉\�Ȑ�
int CL_EVENT_MAX = 65536;//cl_event���L�����Ēu����ő吔
int COMMANDQUEUE_PER_DEVICE = 4;//1�f�o�C�X������̃R�}���h�L���[�A�ݒ�ŕύX�ł���
int dev_num = 0;//�S�v���b�g�t�H�[���̃f�o�C�X�̍��v��
int bufferout[1024 * 4];//�ق�HCLGetDeviceInfo�̕Ԃ�l�p
char hspcharout[1024 * 2];//HSP�̕�����o�͗p�o�b�t�@
cl_device_id* device_id;
cl_context* context;
cl_command_queue* command_queue;
cl_mem mem_obj;
cl_program program;
cl_kernel kernel;
cl_event* cppeventlist;//c++�ŊǗ�����event object�BHSP���炢�����̂͂��������BHCLinit�Ŏ��̉��B���������[�N�\�h�ړI�B�����̒��ɂ���event�̂ݏ���ێ����A����ȊO��event�͕K��release���Ĕj������
cl_event* event_wait_list;//HCLinit�Ŏ��̉��B����event��wait������cl�֐����g���ۂɂ��炩���߂����ݒ肵�Ă����Ă����C���[�W


struct EventStruct
{
	int k;//kernel id�܂��̓R�s�[�T�C�Y���
	int devno;
	int queno;
};

EventStruct* evinfo;

int clsetdev = 0;//OpenCL�Ō��݃��C���ƂȂ��Ă���f�o�C�Xno
int clsetque = 0;//OpenCL�Ō��݃��C���ƂȂ��Ă���que
int cmd_properties = CL_QUEUE_PROFILING_ENABLE;//OpenCL�̃R�}���h�L���[�������Ɏg���v���p�e�B�ԍ�
int num_event_wait_list = 0;//NDRangeKernel �Ƃ��Ŏg����B�g���x��0�ɂȂ�
int thread_start = 0;//0��Enqueue�܂����Ȃ��A1�ȍ~��thread�ɓ��������܂�Enqueue����ĂȂ���







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
	int chk = code_getprm();							// �p�����[�^�[���擾(�^�͖��Ȃ�)
	if (chk <= PARAM_END) {
		return;										// �p�����[�^�[�ȗ����̏���
	}
	int type = mpval->flag;							// �p�����[�^�[�̌^���擾
	switch (type) {
	case 2:								// �p�����[�^�[�������񂾂�����
	{
		char* str = (char*)mpval->pt;
		*ff0 = float(atof(str));
		break;
	}
	case 3:									// �p�����[�^�[��������������
	{
		double* ptr = (double*)mpval->pt;
		*ff0 = float(*ptr);
		break;
	}
	case 4:									// �p�����[�^�[��������������
	{
		int* ptr = (int*)mpval->pt;
		*ff0 = float(*ptr);
		break;
	}
	case 8:									// �p�����[�^�[��float��������
	{
		float* ptr = (float*)mpval->pt;
		*ff0 = float(*ptr);
		break;
	}
	default:
		puterror(HSPERR_TYPE_MISMATCH);			// �T�|�[�g���Ă��Ȃ��^�Ȃ�΃G���[
	}
}


void gtint(int* ii0)
{
	int chk = code_getprm();							// �p�����[�^�[���擾(�^�͖��Ȃ�)
	if (chk <= PARAM_END) {
		return;										// �p�����[�^�[�ȗ����̏���
	}
	int type = mpval->flag;							// �p�����[�^�[�̌^���擾
	switch (type) {
	case 2:								// �p�����[�^�[�������񂾂�����
	{
		char* str = (char*)mpval->pt;
		*ii0 = atoi(str);
		break;
	}
	case 3:									// �p�����[�^�[��������������
	{
		double* ptr = (double*)mpval->pt;
		*ii0 = int(*ptr);
		break;
	}
	case 4:									// �p�����[�^�[��������������
	{
		int* ptr = (int*)mpval->pt;
		*ii0 = int(*ptr);
		break;
	}
	case 8:									// �p�����[�^�[��float��������
	{
		float* ptr = (float*)mpval->pt;
		*ii0 = int(*ptr);
		break;
	}
	default:
		puterror(HSPERR_TYPE_MISMATCH);			// �T�|�[�g���Ă��Ȃ��^�Ȃ�΃G���[
	}
}


void gtdbl(double* dd0)
{
	int chk = code_getprm();							// �p�����[�^�[���擾(�^�͖��Ȃ�)
	if (chk <= PARAM_END) {
		return;										// �p�����[�^�[�ȗ����̏���
	}
	int type = mpval->flag;							// �p�����[�^�[�̌^���擾
	switch (type) {
	case 2:								// �p�����[�^�[�������񂾂�����
	{
		char* str = (char*)mpval->pt;
		*dd0 = atof(str);
		break;
	}
	case 3:									// �p�����[�^�[��������������
	{
		double* ptr = (double*)mpval->pt;
		*dd0 = double(*ptr);
		break;
	}
	case 4:									// �p�����[�^�[��������������
	{
		int* ptr = (int*)mpval->pt;
		*dd0 = double(*ptr);
		break;
	}
	case 8:									// �p�����[�^�[��float��������
	{
		float* ptr = (float*)mpval->pt;
		*dd0 = double(*ptr);
		break;
	}
	default:
		puterror(HSPERR_TYPE_MISMATCH);			// �T�|�[�g���Ă��Ȃ��^�Ȃ�΃G���[
	}
}

*/
/*
void retmeserr(cl_int ret)
{
	switch (ret) {							//����
	case CL_INVALID_PROGRAM_EXECUTABLE:
		MessageBox(NULL, "�f�o�C�X��Ŏ��s�\�ȁA����Ƀr���h���ꂽ�v���O�������������܂���", "�G���[", 0);

		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_COMMAND_QUEUE:
		MessageBox(NULL, "�f�o�C�Xid�������ȃf�o�C�X�ɂȂ��Ă��܂�", "�G���[", 0);

		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_KERNEL:
		MessageBox(NULL, "�J�[�l��id���Ԉ���Ă��܂�", "�G���[", 0);

		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_CONTEXT:
		MessageBox(NULL, "�J�[�l��id���Ⴄ�f�o�C�Xid�œo�^����Ă��܂��A���邢�͑������� event_wait_list ���̃C�x���g�Ɗ֘A�t����ꂽ�f�o�C�X�������łȂ�", "�G���[", 0);

		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_KERNEL_ARGS:
		MessageBox(NULL, "�J�[�l���������w�肳��Ă��܂���", "�G���[", 0);

		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_GLOBAL_WORK_SIZE:
		MessageBox(NULL, "global_work_size �� NULL �ł��B���邢�́Aglobal_work_size�̔z��̂ǂꂩ��0�ł��B�������̓J�[�l�������s����f�o�C�X��ł�global_work_size������l�𒴂��Ă���", "�G���[", 0);

		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_GLOBAL_OFFSET:
		MessageBox(NULL, "CL_INVALID_GLOBAL_OFFSET - global_work_offset �� NULL �łȂ�", "�G���[", 0);

		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_WORK_DIMENSION:
		MessageBox(NULL, "work_dim ���K�؂Ȓl�łȂ�", "�G���[", 0);

		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_WORK_GROUP_SIZE:
		MessageBox(NULL, "global_work_size��local_work_size �Ő����ł��Ȃ��A�܂���local_work_size[0]*local_work_size[1]*local_work_size[2]���A��̃��[�N�O���[�v���̃��[�N�A�C�e�����̍ő�l�𒴂���", "�G���[", 0);

		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_WORK_ITEM_SIZE:
		MessageBox(NULL, "local_work_size[0], ... local_work_size[work_dim - 1] �Ŏw�肵�����[�N�A�C�e�������Ή����� CL_DEVICE_MAX_WORK_ITEM_SIZES[0], ... CL_DEVICE_MAX_WORK_ITEM_SIZES[work_dim - 1] �̒l�����Ă���A�܂���0���w�肵��", "�G���[", 0);

		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_MEM_OBJECT_ALLOCATION_FAILURE:
		MessageBox(NULL, "kernel �̈����Ɏw�肳�ꂽ�o�b�t�@/�C���[�W�I�u�W�F�N�g�Ɋ֘A�t����ꂽ�f�[�^�ۑ��̂��߂̃������̈�̊m�ۂɎ��s����", "�G���[", 0);

		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_EVENT_WAIT_LIST:
		MessageBox(NULL, "event_wait_list �� NULL �� num_events_in_wait_list �� 0 ���傫���Ƃ��B���邢�� event_wait_list �� NULL �łȂ� num_events_in_wait_list �� 0 �̂Ƃ��B���邢�� event_wait_list ���̃C�x���g�I�u�W�F�N�g���L���Ȃ��̂łȂ�", "�G���[", 0);

		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_OUT_OF_RESOURCES:
		MessageBox(NULL, "�f�o�C�X��ł̃��\�[�X�m�ۂɎ��s����", "�G���[", 0);

		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_OUT_OF_HOST_MEMORY:
		MessageBox(NULL, "�z�X�g��ł̃��\�[�X�m�ۂɎ��s����", "�G���[", 0);

		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	}
	//��̂ǂ�ł��Ȃ����
	MessageBox(NULL, "�����s���̃G���[�ł�", "�G���[", 0);
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
	}//////////�f�o�C�X�����擾���܂����Ă���Ƃ���



	cl_device_id cdDeviceID[32];
	size_t size3 = 0;
	int count = 0;
	int countd = 0;
	context = new cl_context[gkw];
	command_queue = new cl_command_queue[gkw];


	char str1[1024];
	char str2[1024];


	for (int k = 0; k < gkw; k++) {//�R���e�L�X�g�ƃR�}���h���[�����
		context[k] = clCreateContext(NULL, 1, &gpus[k].dev, NULL, NULL, &ret);

		sprintf(str2, "�����s��");
		switch (ret) {							//����
		case CL_INVALID_PLATFORM:
			sprintf(str2, "CL_INVALID_PLATFORM\n�R���e�L�X�g�쐬�G���[�B\nproperties �� NULL �ł��v���b�g�t�H�[�����ЂƂ��I������Ȃ������Ƃ��B�܂��́Aproperties ���Ŏw�肵���v���b�g�t�H�[�����L���ȃv���b�g�t�H�[���łȂ��Ƃ��B");
			break;

		case CL_INVALID_PROPERTY:
			sprintf(str2, "CL_INVALID_PROPERTY\n�l���x�����ꂽ�v���p�e�B���ɑ΂��Ďw�肵���ꍇ�A�������̃R���e�L�X�g�E�v���p�e�B�����x�����ꂽ�v���p�e�B���łȂ��ꍇ�A�L���łȂ��A���邢�́A�����v���p�e�B������x�ȏ�w�肳���ꍇ�B�������Ȃ���A�g��cl_khr_gl_sharing���\�ɂȂ�ꍇ�A��L�̓����p�̃e�[�u���Ƀ��X�g���ꂽ���̈ȊO�̑��������A�����̒��Ŏw�肳���ꍇ�ACL_INVALID_PROPERTY���Ԃ���܂��B");
			break;

		case CL_INVALID_VALUE:
			sprintf(str2, "CL_INVALID_VALUE\n�R���e�L�X�g�쐬�G���[�B\ndevices �� NULL �̂Ƃ� num_devices ��0�̂Ƃ��B  pfn_notify �� NULL �� user_data �� NULL �łȂ��Ƃ��B");
			break;

		case CL_INVALID_DEVICE:
			sprintf(str2, "CL_INVALID_DEVICE\n�R���e�L�X�g�쐬�G���[�B\ndevices �ɗL���łȂ��f�o�C�X���܂܂�Ă���Ƃ�");
			break;

		case CL_DEVICE_NOT_AVAILABLE:
			sprintf(str2, "CL_DEVICE_NOT_AVAILABLE\n�R���e�L�X�g�쐬�G���[�B\ndevices ���̃f�o�C�X���AclGetDeviceIDs �ŕԂ��ꂽ�f�o�C�X�ł�����̂̌��ݗ��p�\�łȂ��Ƃ��B");
			break;

		case CL_OUT_OF_HOST_MEMORY:
			sprintf(str2, "CL_OUT_OF_HOST_MEMORY\n�R���e�L�X�g�쐬�G���[�B\n�z�X�g��ł̃��\�[�X�m�ۂɎ��s�����Ƃ��B");
			break;
		}


		if (ret != CL_SUCCESS) {
			sprintf(str1, "%s\n\n%s\n", gpus[k].name, str2);

			context[k] = clCreateContext(NULL, 1, &gpus[k].dev, NULL, NULL, &ret);
			sprintf(str2, "�����s��");
			switch (ret) {							//����
			case CL_INVALID_PLATFORM:
				sprintf(str2, "CL_INVALID_PLATFORM\n�R���e�L�X�g�쐬�G���[�B\nproperties �� NULL �ł��v���b�g�t�H�[�����ЂƂ��I������Ȃ������Ƃ��B�܂��́Aproperties ���Ŏw�肵���v���b�g�t�H�[�����L���ȃv���b�g�t�H�[���łȂ��Ƃ��B");
				break;

			case CL_INVALID_PROPERTY:
				sprintf(str2, "CL_INVALID_PROPERTY\n�l���x�����ꂽ�v���p�e�B���ɑ΂��Ďw�肵���ꍇ�A�������̃R���e�L�X�g�E�v���p�e�B�����x�����ꂽ�v���p�e�B���łȂ��ꍇ�A�L���łȂ��A���邢�́A�����v���p�e�B������x�ȏ�w�肳���ꍇ�B�������Ȃ���A�g��cl_khr_gl_sharing���\�ɂȂ�ꍇ�A��L�̓����p�̃e�[�u���Ƀ��X�g���ꂽ���̈ȊO�̑��������A�����̒��Ŏw�肳���ꍇ�ACL_INVALID_PROPERTY���Ԃ���܂��B");
				break;

			case CL_INVALID_VALUE:
				sprintf(str2, "CL_INVALID_VALUE\n�R���e�L�X�g�쐬�G���[�B\ndevices �� NULL �̂Ƃ� num_devices ��0�̂Ƃ��B  pfn_notify �� NULL �� user_data �� NULL �łȂ��Ƃ��B");
				break;

			case CL_INVALID_DEVICE:
				sprintf(str2, "CL_INVALID_DEVICE\n�R���e�L�X�g�쐬�G���[�B\ndevices �ɗL���łȂ��f�o�C�X���܂܂�Ă���Ƃ�");
				break;

			case CL_DEVICE_NOT_AVAILABLE:
				sprintf(str2, "CL_DEVICE_NOT_AVAILABLE\n�R���e�L�X�g�쐬�G���[�B\ndevices ���̃f�o�C�X���AclGetDeviceIDs �ŕԂ��ꂽ�f�o�C�X�ł�����̂̌��ݗ��p�\�łȂ��Ƃ��B");
				break;

			case CL_OUT_OF_HOST_MEMORY:
				sprintf(str2, "CL_OUT_OF_HOST_MEMORY\n�R���e�L�X�g�쐬�G���[�B\n�z�X�g��ł̃��\�[�X�m�ۂɎ��s�����Ƃ��B");
				break;
			}
			if (ret != CL_SUCCESS)
			{
				sprintf(str1, "%s\n\n%s\n", gpus[k].name, str2);
			}
		}




		command_queue[k] = clCreateCommandQueue(context[k], gpus[k].dev, CL_QUEUE_PROFILING_ENABLE, &ret);

		switch (ret) {							//����
		case CL_INVALID_CONTEXT:
			sprintf(str2, "�R�}���h�L���[�����G���[\ncontext ���L����OpenCL�R���e�L�X�g�łȂ��Ƃ��B");
			sprintf(str1, "%s\n\n%s", gpus[k].name, str2);
			break;

		case CL_INVALID_DEVICE:
			sprintf(str2, "�R�}���h�L���[�����G���[\ndevice ���L����OpenCL�f�o�C�X�łȂ��Ƃ��B���邢�́Acontext �Ɗ֘A�t�����Ă��Ȃ��Ƃ��B");
			sprintf(str1, "%s\n\n%s", gpus[k].name, str2);
			break;

		case CL_INVALID_VALUE:
			sprintf(str2, "�R�}���h�L���[�����G���[\nproperties �Ɏw�肵���l���L���Ȃ��̂łȂ��Ƃ��B");
			sprintf(str1, "%s\n\n%s", gpus[k].name, str2);

			sprintf(str1, "properties�ɃA�E�g�I�u�I�[�_�[���w�肵���Ƃ���G���[�ɐ������̂ŁA�C���I�[�_�[���s�Ƃ��čăR�}���h�L���[�������܂�");

			command_queue[k] = clCreateCommandQueue(context[k], gpus[k].dev, 0, &ret);
			break;

		case CL_INVALID_QUEUE_PROPERTIES:
			sprintf(str2, "�R�}���h�L���[�����G���[\nproperties �Ɏw�肵���l���A�L���ł�����̂̃f�o�C�X���T�|�[�g���Ă��Ȃ��Ƃ��B");
			sprintf(str1, "%s\n\n%s", gpus[k].name, str2);

			sprintf(str1, "properties�ɃA�E�g�I�u�I�[�_�[���w�肵���Ƃ���G���[�ɐ������̂ŁA�C���I�[�_�[���s�Ƃ��čăR�}���h�L���[�������܂�");

			command_queue[k] = clCreateCommandQueue(context[k], gpus[k].dev, 0, &ret);
			break;

		case CL_OUT_OF_HOST_MEMORY:
			sprintf(str2, "�R�}���h�L���[�����G���[\n�z�X�g��ł̃��\�[�X�m�ۂɎ��s�����Ƃ��B");
			sprintf(str1, "%s\n\n%s", gpus[k].name, str2);

			break;
		}


		if (ret == CL_SUCCESS) {
			char str3[4];
			gpus[k].CLyukflg = 1;
			sprintf(str2, "�R�}���h�L���[��������");
			sprintf(str1, "�f�o�C�X��\n%s\n\n%s\nOpenGL�A�g %s", gpus[k].name, str2, str3);
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
	sprintf(out, "�v���b�g�t�H�[��id=�J��%d�J�ƃf�o�C�X��=�J��%s�J�ƃf�o�C�Xid=�J��%d�J�ƃf�o�C�X���(DEFAULT 1,CPU 2,GPU 4,ACCELERATOR 8)=�J��%d�J��OpenCL�\�t�g�E�F�A�h���C�o�̃o�[�W����=�J��%s�J�ƃf�o�C�X���T�|�[�g����OpenCL�̃o�[�W����=�J��%s�J�ƃf�o�C�X�̃x���_�[=�J��%s�J�ƃ��[�J��������(���L������)�̃T�C�Y(byte)=�J��%d�J�ƃf�o�C�X�ő���g��(MHz)=�J��%f�J�ƕ��񉉎Z�R�A�̐�=�J��%d�J�ƃO���[�o���������T�C�Y(byte)=�J��%f�J�ƃ������I�u�W�F�N�g�Ƃ��Ċm�ۂł���ő�̃������T�C�Y(byte)=�J��%f�J�ƂЂƂ̃��[�N�O���[�v���̃��[�N�A�C�e�����̍ő�l=�J��%d�J�ƃ��[�N�O���[�v���Ƃ̃��[�N�A�C�e�����̂��ꂼ��̎����ɂ��Ă̍ő�l=�J��%d,%d,%d�J��2D�C���[�W�̕��̍ő�T�C�Y=�J��%d�J��2D�C���[�W�̍����̍ő�T�C�Y=�J��%d�J��3D�C���[�W�̕��̍ő�T�C�Y=�J��%d�J��3D�C���[�W�̍����̍ő�T�C�Y=�J��%d�J��3D�C���[�W�̐[���̍ő�T�C�Y=�J��%d�J�ƃ��[�J���������̃^�C�v�i��p�̃����� �L��1 ����0�j=�J��%d�J�ƃv���O�����\�[�X���R���p�C�����邽�߂ɗ��p�ł���R���p�C���̗L��(�L1 ��0)=�J��%d�J��%d�J��OpenCL�L��(�L1 ��0)=�J��%d�J�ƃf�o�C�X���T�|�[�g����g���@�\�̖��̂��X�y�[�X�ŋ�؂������X�g�̌`���̕�����ŕԂ��܂��J��%s�J��"
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



//prm3�͎Q�Ɠn���ł��邱�Ƃɒ���
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
			ss += "�R�s�[�T�C�Y>HSP�z��ϐ��T�C�Y �ł��B\n�R�s�[�T�C�Y=";
			ss += std::to_string(prm3);
			ss += "\nHSP�z��ϐ��T�C�Y=";
			ss += std::to_string(host_sz);
			MessageBox(NULL, ss.c_str(), "�G���[", 0);
			puterror(HSPERR_UNSUPPORTED_FUNCTION);
		}
		if (prm3 > dev_sz)
		{
			std::string ss = "";
			ss += "�R�s�[�T�C�Y>�f�o�C�X�������T�C�Y �ł��B\n�R�s�[�T�C�Y=";
			ss += std::to_string(prm3);
			ss += "\n�f�o�C�X�������T�C�Y=";
			ss += std::to_string(dev_sz);
			MessageBox(NULL, ss.c_str(), "�G���[", 0);
			puterror(HSPERR_UNSUPPORTED_FUNCTION);
		}
	}
	return;
}

//Dokrn��ReadBuffer�n�ŃC�x���g�L�^���邩
//���̊֐����� code_getdi�����Ă��邱�Ƃɒ���
cl_event* EventOutChk(int k)
{
	//outevent�֘A
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
	//wait event list�֘A
	cl_int ret = clEnqueueWriteBuffer(cmd, mem, CL_FALSE, ofst,
		size, vptr, num_event_wait_list__, ev_, outevent);
	thread_start--;
	if (ret != CL_SUCCESS) { retmeserr2(ret); }
	return;
}

void Thread_ReadBuffer(cl_command_queue cmd, cl_mem mem, int ofst, int size,
	void* vptr, int num_event_wait_list__, cl_event* ev_, cl_event* outevent)
{
	//wait event list�֘A
	cl_int ret = clEnqueueReadBuffer(cmd, mem, CL_FALSE, ofst,
		size, vptr, num_event_wait_list__, ev_, outevent);
	thread_start--;
	if (ret != CL_SUCCESS) { retmeserr2(ret); }
	return;
}






std::string readFileIntoString(const std::string& path) {
	std::ifstream input_file(path);
	if (!input_file.is_open()) {
		MessageBox(NULL, "�t�@�C�������݂��܂���", "�G���[", 0);
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
	//		���s���� (���ߎ��s���ɌĂ΂�܂�)
	//
	code_next();							// ���̃R�[�h���擾(�ŏ��ɕK���K�v�ł�)

	switch (cmd) {							// �T�u�R�}���h���Ƃ̕���

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
		p = code_gets();								// ��������擾
		strncpy(pathname, p, _MAX_PATH - 1);			// �擾������������R�s�[

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
		p1 = code_getdi(0);		// �p�����[�^1:���l
		char* ps;
		char pathname[_MAX_PATH];
		ps = code_gets();								// ��������擾
		strncpy(pathname, ps, _MAX_PATH - 1);			// �擾������������R�s�[
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
		p3 = code_getdi(0);		// �p�����[�^1:���l
		cl_int errcode_ret;
		if (p3 > 1024 * 1024 * 512) {
			MessageBox(NULL, "512M�𒴂���T�C�Y�͎w��ł��܂���", "�G���[", 0);
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










































	//////////////////////��������64�ڐA/////////////////
	//////////////////////��������64�ڐA/////////////////
	//////////////////////��������64�ڐA/////////////////
	//////////////////////��������64�ڐA/////////////////
	//////////////////////��������64�ڐA/////////////////
	//////////////////////��������64�ڐA/////////////////
	//////////////////////��������64�ڐA/////////////////
	//////////////////////��������64�ڐA/////////////////
	//////////////////////��������64�ڐA/////////////////


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
			MessageBox(NULL, "No OpenCL Devices\nHSPCL32N�͎g���܂���", "error", 0);
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
		//////////�f�o�C�X�����擾���܂����Ă���Ƃ���

		//�����ŃR���e�L�X�g�ƃR�}���h�L���[����
		context = new cl_context[dev_num];
		command_queue = new cl_command_queue[dev_num * COMMANDQUEUE_PER_DEVICE];

		for (int k = 0; k < dev_num; k++)
		{//�R���e�L�X�g�ƃR�}���h���[�����
			context[k] = clCreateContext(NULL, 1, &device_id[k], NULL, NULL, &errcode_ret);
			if (errcode_ret != CL_SUCCESS) retmeserr4(errcode_ret);

			for (int i = 0; i < COMMANDQUEUE_PER_DEVICE; i++)
			{
				command_queue[k * COMMANDQUEUE_PER_DEVICE + i] =
					clCreateCommandQueue(context[k], device_id[k], cmd_properties, &errcode_ret);
				if (errcode_ret != CL_SUCCESS) retmeserr3(errcode_ret);
			}
		}

		//�Ō��event�ϐ�����

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
		//����1
		PVal* pval1;
		APTR aptr1;
		aptr1 = code_getva(&pval1);
		//����2
		int prm2 = code_geti();
		int sz = GetMemSize((cl_mem)prm2);//�T�C�Y
		int n = min((1 << 30), sz);
		//HSP�ϐ�������
		exinfo->HspFunc_dim(pval1, HSPVAR_FLAG_INT, 0, (n + 3) / 4 * 4, 0, 0, 0);
		//�]��
		cl_int ret = clEnqueueReadBuffer(command_queue[clsetdev * COMMANDQUEUE_PER_DEVICE + clsetque], (cl_mem)prm2, CL_TRUE, 0,
			n, pval1->pt, 0, NULL, NULL);
		if (ret != CL_SUCCESS) { retmeserr2(ret); }
		break;
	}

	case 0x8D://HCLdim_fpFromBuffer
	{
		//����1
		PVal* pval1;
		APTR aptr1;
		aptr1 = code_getva(&pval1);
		//����2
		int prm2 = code_geti();
		int sz = GetMemSize((cl_mem)prm2);//�T�C�Y
		int n = min((1 << 30), sz);
		//HSP�ϐ�������
		exinfo->HspFunc_dim(pval1, 8, 0, (n + 7) / 8 * 8, 0, 0, 0);
		//�]��
		cl_int ret = clEnqueueReadBuffer(command_queue[clsetdev * COMMANDQUEUE_PER_DEVICE + clsetque], (cl_mem)prm2, CL_TRUE, 0,
			n, pval1->pt, 0, NULL, NULL);
		if (ret != CL_SUCCESS) { retmeserr2(ret); }
		break;
	}

	case 0x8E://HCLdim_dpFromBuffer
	{
		//����1
		PVal* pval1;
		APTR aptr1;
		aptr1 = code_getva(&pval1);
		//����2
		int prm2 = code_geti();
		int sz = GetMemSize((cl_mem)prm2);//�T�C�Y
		int n = min((1 << 30), sz);
		//HSP�ϐ�������
		exinfo->HspFunc_dim(pval1, HSPVAR_FLAG_DOUBLE, 0, (n + 7) / 8 * 8, 0, 0, 0);
		//�]��
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
		//����1 kernel
		int prm1 = code_geti();
		clReleaseProgram((cl_program)prm1);
		break;
	}

	case 0x5B:	// HCLSetKernel
	{
		int prm1 = code_geti();
		p2 = code_getdi(0);		// �p�����[�^2:���l�A����

		int chk = code_getprm();							// �p�����[�^�[���擾(�^�͖��Ȃ�)

		int type = mpval->flag;							// �p�����[�^�[�̌^���擾
		void* ppttr;
		int sizeofff = -1;
		switch (type) {
		case HSPVAR_FLAG_STR:								// �p�����[�^�[�������񂾂�����
		{
			ppttr = (char*)mpval->pt;
			sizeofff = 1;
			break;
		}
		case HSPVAR_FLAG_DOUBLE:									// �p�����[�^�[��������������
		{
			ppttr = (double*)mpval->pt;
			sizeofff = 8;
			break;
		}
		case 8:								// �p�����[�^�[��float��������
		{
			ppttr = (float*)mpval->pt;
			sizeofff = 4;
			break;
		}
		case HSPVAR_FLAG_INT:									// �p�����[�^�[��������������
		{
			ppttr = (int*)mpval->pt;
			sizeofff = 4;
			break;
		}
		default:
			puterror(HSPERR_TYPE_MISMATCH);			// �T�|�[�g���Ă��Ȃ��^�Ȃ�΃G���[
			sizeofff = -1;
			break;
		}

		int p4 = code_getdi(0);		// �p�����[�^4:���[�J���������[�t���O
		int clret = 0;

		if (prm1 == 0)
		{
			MessageBox(NULL, "�J�[�l��id��0�ł�", "�G���[", 0);
			puterror(HSPERR_UNSUPPORTED_FUNCTION);
		}
		else
		{
			//prm1�J�[�l��id�Ap2�͈����ʒu�Ap3�ɂ͎��ԁAp4�̓��[�J���������t���O
			if (p4 == 0) { clret = clSetKernelArg((cl_kernel)prm1, p2, sizeofff, ppttr); }
			if (p4 != 0) { clret = clSetKernelArg((cl_kernel)prm1, p2, p4, NULL); }
		}

		break;
	}

	case 0x5C://HCLSetKrns
	{
		int prm1 = code_geti();		// �p�����[�^1:�J�[�l��
		if (prm1 == 0) {
			MessageBox(NULL, "�J�[�l��id��0�ł�", "�G���[", 0);
			puterror(HSPERR_UNSUPPORTED_FUNCTION);
		}

		void* ppttr;
		int sizeofff;
		int chk;
		int type;

		for (int i = 0; i < 32; i++) {
			chk = code_getprm();							// �p�����[�^�[���擾(�^�͖��Ȃ�)
			if (chk <= PARAM_END) {
				break;										// �p�����[�^�[�ȗ����̏���
			}
			type = mpval->flag;							// �p�����[�^�[�̌^���擾
			switch (type) {
			case HSPVAR_FLAG_STR:								// �p�����[�^�[�������񂾂�����
			{
				ppttr = (char*)mpval->pt;
				sizeofff = 1;
				break;
			}
			case HSPVAR_FLAG_DOUBLE:									// �p�����[�^�[��������������
			{
				ppttr = (double*)mpval->pt;
				sizeofff = 8;
				break;
			}

			case 8:								// �p�����[�^�[��float��������
			{
				ppttr = (float*)mpval->pt;
				sizeofff = 4;
				break;
			}
			case HSPVAR_FLAG_INT:									// �p�����[�^�[��������������
			{
				ppttr = (int*)mpval->pt;
				sizeofff = 4;
				break;
			}
			default:
				puterror(HSPERR_TYPE_MISMATCH);			// �T�|�[�g���Ă��Ȃ��^�Ȃ�΃G���[
			}

			//p1�J�[�l��id�Ap2�͈����ʒu�Ap3�ɂ͎��ԁAp4�̓��[�J���������t���O
			clSetKernelArg((cl_kernel)prm1, i, sizeofff, ppttr);
		}
		break;
	}

	case 0x5D://HCLReleaseKernel
	{
		//����1 kernel
		int prm1 = code_geti();
		clReleaseKernel((cl_kernel)prm1);
		break;
	}

	case 0x60:	// HCLWriteBuffer
	{
		//����1
		int prm1 = code_geti();
		//����2�BHSP���̔z��ϐ�
		PVal* pval = code_getpval();
		//����3�A�R�s�[�T�C�Y
		int prm3 = code_getdi(-1);
		//����4�A�R�s�[���ofset
		int prm4 = code_getdi(0);
		//����5�A�R�s�[����ofset
		int prm5 = code_getdi(0);//�p�����[�^5
		int p7 = code_getdi(1);		//�u���b�L���O���[�h
		cl_bool TorF = ((p7 == 0) ? CL_FALSE : CL_TRUE);
		//�����ȗ��Ȃ�T�C�Y�͎���
		AutoReadWriteCopySize(prm3, pval, (cl_mem)prm1);////prm3�͎Q�Ɠn���ł��邱�Ƃɒ���

		//outevent�֘A
		cl_event* outevent = EventOutChk(prm3);
		//Intel CPU�ł͐������u���b�L���O�ł��Ȃ��̂ŁAevent�쐬���Ă������wait���Ă��炤
		cl_event tmpev;

		//wait event list�֘A
		cl_event* ev_ = GetWaitEvlist();

		cl_int ret = clEnqueueWriteBuffer(command_queue[clsetdev * COMMANDQUEUE_PER_DEVICE + clsetque], (cl_mem)prm1, TorF, prm4,
			prm3, (char*)((pval->pt) + prm5), num_event_wait_list, ev_, &tmpev);
		if (ret != CL_SUCCESS) { retmeserr2(ret); }

		//Intel CPU�ł͐������u���b�L���O�ł��Ȃ��̂ŁAevent�쐬���Ă������wait���Ă��炤
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
		//����1
		int prm1 = code_geti();
		//����2�BHSP���̔z��ϐ�
		PVal* pval = code_getpval();
		//����3�A�R�s�[�T�C�Y
		int prm3 = code_getdi(-1);
		//����4�A�R�s�[���ofset
		int prm4 = code_getdi(0);
		//����5�A�R�s�[����ofset
		int prm5 = code_getdi(0);//�p�����[�^5
		int p7 = code_getdi(1);		//�u���b�L���O���[�h
		cl_bool TorF = ((p7 == 0) ? CL_FALSE : CL_TRUE);
		//�����ȗ��Ȃ�T�C�Y�͎���
		AutoReadWriteCopySize(prm3, pval, (cl_mem)prm1);////prm3�͎Q�Ɠn���ł��邱�Ƃɒ���

		//outevent�֘A
		cl_event* outevent = EventOutChk(prm3);
		//wait event list�֘A
		cl_event* ev_ = GetWaitEvlist();
		cl_int ret = clEnqueueReadBuffer(command_queue[clsetdev * COMMANDQUEUE_PER_DEVICE + clsetque], (cl_mem)prm1, TorF, prm4,
			prm3, (char*)((pval->pt) + prm5), num_event_wait_list, ev_, outevent);
		if (ret != CL_SUCCESS) { retmeserr2(ret); }

		num_event_wait_list = 0;
		break;
	}

	//�R���p�C������-pthread ���K�v
	case 0x86:	// HCLWriteBuffer_NonBlocking
	{
		//����1
		int prm1 = code_geti();
		//����2�BHSP���̔z��ϐ�
		PVal* pval = code_getpval();
		//����3�A�R�s�[�T�C�Y
		int prm3 = code_getdi(-1);
		//����4�A�R�s�[���ofset
		int prm4 = code_getdi(0);
		//����5�A�R�s�[����ofset
		int prm5 = code_getdi(0);//�p�����[�^5
		cl_bool p7 = code_getdi(1);		//�u���b�L���O���[�h
		//�����ȗ��Ȃ�T�C�Y�͎���
		AutoReadWriteCopySize(prm3, pval, (cl_mem)prm1);////prm3�͎Q�Ɠn���ł��邱�Ƃɒ���

		//outevent�֘A
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
		//�����ŕʃX���b�h�ɂȂ���
		thread_start++;
		std::thread th(Thread_WriteBuffer, cmd, mem_obj, prm4, prm3, vptr, num_event_wait_list, ev_, outevent);
		th.detach();
		num_event_wait_list = 0;
		break;
	}

	case 0x87:	// HCLReadBuffer_NonBlocking
	{
		//����1
		int prm1 = code_geti();
		//����2�BHSP���̔z��ϐ�
		PVal* pval = code_getpval();
		//����3�A�R�s�[�T�C�Y
		int prm3 = code_getdi(-1);
		//����4�A�R�s�[���ofset
		int prm4 = code_getdi(0);
		//����5�A�R�s�[����ofset
		int prm5 = code_getdi(0);//�p�����[�^5
		cl_bool p7 = code_getdi(1);		//�u���b�L���O���[�h
		//�����ȗ��Ȃ�T�C�Y�͎���
		AutoReadWriteCopySize(prm3, pval, (cl_mem)prm1);////prm3�͎Q�Ɠn���ł��邱�Ƃɒ���

		//outevent�֘A
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
		//�����ŕʃX���b�h�ɂȂ���
		thread_start++;
		std::thread th(Thread_ReadBuffer, cmd, mem_obj, prm4, prm3, vptr, num_event_wait_list, ev_, outevent);
		th.detach();
		break;
	}

	case 0x62:	// HCLCopyBuffer
	{
		int prm2 = code_geti();//�R�s�[�惁�����I�u�W�F�N�gid
		int prm3 = code_geti();//�R�s�[���������I�u�W�F�N�gid
		int prm4 = code_getdi(-1);// �R�s�[�T�C�Y
		int prm5 = code_getdi(0);// �R�s�[��I�t�Z�b�g
		int prm6 = code_getdi(0);// �R�s�[���I�t�Z�b�g
		//�����ȗ��Ȃ�T�C�Y�͎���
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
				ss += "�R�s�[�T�C�Y>�R�s�[�惁�����T�C�Y �ł��B\n�R�s�[�T�C�Y=";
				ss += std::to_string(prm4);
				ss += "\n�R�s�[��T�C�Y=";
				ss += std::to_string(sz2);
				MessageBox(NULL, ss.c_str(), "�G���[", 0);
				puterror(HSPERR_UNSUPPORTED_FUNCTION);
			}
			if (prm4 > sz3)
			{
				std::string ss = "";
				ss += "�R�s�[�T�C�Y>�R�s�[���������T�C�Y �ł��B\n�R�s�[�T�C�Y=";
				ss += std::to_string(prm4);
				ss += "\n�R�s�[���T�C�Y=";
				ss += std::to_string(sz3);
				MessageBox(NULL, ss.c_str(), "�G���[", 0);
				puterror(HSPERR_UNSUPPORTED_FUNCTION);
			}
		}

		//outevent�֘A
		cl_event* outevent = EventOutChk(prm4);
		//wait event list�֘A
		cl_event* ev_ = GetWaitEvlist();
		cl_int ret = clEnqueueCopyBuffer(command_queue[clsetdev * COMMANDQUEUE_PER_DEVICE + clsetque],
			(cl_mem)prm3, (cl_mem)prm2, prm6, prm5, prm4, num_event_wait_list, ev_, outevent);
		num_event_wait_list = 0;
		if (ret != CL_SUCCESS)retmeserr2(ret);
		break;
	}

	//GPU��Ŏ��s
	case 0x63://HCLFillBuffer_i32
	{
		//����1 buffer
		int prm1 = code_geti();
		//����2 pattern
		int pattern = code_getdi(0);
		//����3�Aoffset(byte)
		int prm4 = code_getdi(0);
		//����4�Asize(byte)
		int prm5 = code_getdi(-1);//�p�����[�^5
		if (prm5 == -1)prm5 = GetMemSize((cl_mem)prm1);

		//outevent�֘A
		cl_event* outevent = EventOutChk(prm5);
		//wait event list�֘A
		cl_event* ev_ = GetWaitEvlist();

		cl_int ret = clEnqueueFillBuffer(command_queue[clsetdev * COMMANDQUEUE_PER_DEVICE + clsetque],
			(cl_mem)prm1, &pattern, 4, prm4, prm5, num_event_wait_list, ev_, outevent);
		if (ret != CL_SUCCESS) { retmeserr(ret); }
		num_event_wait_list = 0;
		break;
	}

	case 0x64://HCLFillBuffer_fp
	{
		//����1 buffer
		int prm1 = code_geti();
		//����2 pattern
		float pattern = code_getd();
		//����3�Aoffset(byte)
		int prm4 = code_getdi(0);
		//����4�Asize(byte)
		int prm5 = code_getdi(-1);//�p�����[�^5
		if (prm5 == -1)prm5 = GetMemSize((cl_mem)prm1);

		//outevent�֘A
		cl_event* outevent = EventOutChk(prm5);
		//wait event list�֘A
		cl_event* ev_ = GetWaitEvlist();

		cl_int ret = clEnqueueFillBuffer(command_queue[clsetdev * COMMANDQUEUE_PER_DEVICE + clsetque],
			(cl_mem)prm1, &pattern, 4, prm4, prm5, num_event_wait_list, ev_, outevent);
		if (ret != CL_SUCCESS) { retmeserr(ret); }
		num_event_wait_list = 0;
		break;
	}

	//GPU��Ŏ��s
	case 0x8B://HCLFillBuffer_dp
	{
		//����1 buffer
		int prm1 = code_geti();
		//����2 pattern
		double pattern = code_getdd(0.0);
		//����3�Aoffset(byte)
		int prm4 = code_getdi(0);
		//����4�Asize(byte)
		int prm5 = code_getdi(-1);//�p�����[�^5
		if (prm5 == -1)prm5 = GetMemSize((cl_mem)prm1);

		//outevent�֘A
		cl_event* outevent = EventOutChk(prm5);
		//wait event list�֘A
		cl_event* ev_ = GetWaitEvlist();

		cl_int ret = clEnqueueFillBuffer(command_queue[clsetdev * COMMANDQUEUE_PER_DEVICE + clsetque],
			(cl_mem)prm1, &pattern, 8, prm4, prm5, num_event_wait_list, ev_, outevent);
		if (ret != CL_SUCCESS) { retmeserr(ret); }
		num_event_wait_list = 0;
		break;
	}

	case 0x65://HCLReleaseBuffer
	{
		int prm2 = code_geti();		// �p�����[�^1:memobj
		cl_int ret = clReleaseMemObject((cl_mem)prm2);
		if (ret != CL_SUCCESS) {
			MessageBox(NULL, "�������J�����ł��܂���ł���", "�G���[", 0);
			puterror(HSPERR_UNSUPPORTED_FUNCTION);
		}
		break;
	}

	case 0x69://HCLWriteIndex_i32
	{
		int memid = code_geti();
		int b = code_geti();//idx
		int data = code_geti();//���e
		cl_int ret = clEnqueueWriteBuffer(command_queue[clsetdev * COMMANDQUEUE_PER_DEVICE + clsetque], (cl_mem)memid, CL_TRUE, b * 4, 4, &data, 0, NULL, NULL);
		if (ret != CL_SUCCESS) { retmeserr2(ret); }
		break;
	}

	case 0x6A://HCLWriteIndex_fp
	{
		int memid = code_geti();
		int b = code_geti();//idx
		float data = code_getd();//���e
		cl_int ret = clEnqueueReadBuffer(command_queue[clsetdev * COMMANDQUEUE_PER_DEVICE + clsetque], (cl_mem)memid, CL_TRUE, b * 4, 4, &data, 0, NULL, NULL);
		if (ret != CL_SUCCESS) { retmeserr2(ret); }
		break;
	}

	case 0x6B://HCLWriteIndex_dp
	{
		int memid = code_geti();
		int b = code_geti();//idx
		double data = code_getd();//���e
		cl_int ret = clEnqueueReadBuffer(command_queue[clsetdev * COMMANDQUEUE_PER_DEVICE + clsetque], (cl_mem)memid, CL_TRUE, b * 8, 8, &data, 0, NULL, NULL);
		if (ret != CL_SUCCESS) { retmeserr2(ret); }
		break;
	}

	// HCLCall
	// str source,int global_size,int local_size,array a,array b,array c�E�E�E�E�E
	case 0x6C:
	{
		cl_int ret;
		char* c_source;
		c_source = code_gets();								// ��������擾
		std::string s_sourse = std::string(c_source);
		program = WithSource_func(context[clsetdev], s_sourse, "");
		ret = clCreateKernelsInProgram(program, 1, &kernel, NULL);//�v���O�����̒��̍ŏ��ɂłĂ���J�[�l�����擾
		if (ret != CL_SUCCESS)retmeserr8(ret);
		//����global_size��local_size
		size_t global_size = code_getdi(1);	//����
		size_t local_size = code_getdi(1);
		//���Ɉ����擾��������������������������������������������������������������������������������������
		cl_mem clm[CLCALL_LOOP];
		void* host_ptr[CLCALL_LOOP];
		int copysize[CLCALL_LOOP];
		for (int i = 0; i < CLCALL_LOOP; i++) { copysize[i] = 0; clm[i] = NULL; host_ptr[i] = NULL; }
		//�ꎞ�z��ݒ肨�����


		for (int i = 0; i < CLCALL_LOOP; i++) {
			//�܂�cl_mem�^������ȊO���𔻒肵����
			bool memorval = false;//false��val�Ƃ����Ӗ��Afalse�Ȃ�cl_mem�����Ȃ�
			bool trygetva = true;//getva�����܂�������true

			void* ppttr;//�@
			int sizeofff;//�A
			//���̇@�A������Γ]���ł���
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

				//�z�񂪂��肻���Ȃ�
				if (asz > 1)
				{
					if (pval->offset == 0)//�Y������������0�Ȃ�
					{
						memorval = true;
						sizeofff = pval->size;
					}
					else//�Y�����ɈӖ������肻���Ȃ�
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
					break;										// �p�����[�^�[�ȗ����̏���
				}
				type = mpval->flag;							// �p�����[�^�[�̌^���擾
				switch (type) {
				case HSPVAR_FLAG_STR:								// �p�����[�^�[�������񂾂�����
				{
					ppttr = (char*)mpval->pt;
					sizeofff = 1;
					break;
				}
				case HSPVAR_FLAG_DOUBLE:									// �p�����[�^�[��������������
				{
					ppttr = (double*)mpval->pt;
					sizeofff = 8;
					break;
				}

				case 8:								// �p�����[�^�[��float��������
				{
					ppttr = (float*)mpval->pt;
					sizeofff = 4;
					break;
				}
				case HSPVAR_FLAG_INT:									// �p�����[�^�[��������������
				{
					ppttr = (int*)mpval->pt;
					sizeofff = 4;
					break;
				}
				default:
					puterror(HSPERR_TYPE_MISMATCH);			// �T�|�[�g���Ă��Ȃ��^�Ȃ�΃G���[
				}
			}




			if (memorval == false) //���������ʂ̃X�J���[�Ȃ�
			{
				//p1�J�[�l��id�Ap2�͈����ʒu�Ap3�ɂ̓T�C�Y�Ap4�͎���
				clSetKernelArg(kernel, i, sizeofff, ppttr);
			}
			else //������cl_mem�Ȃ�
			{
				clm[i] = clCreateBuffer(context[clsetdev], CL_MEM_READ_WRITE, pval->size, NULL, &ret);
				copysize[i] = pval->size;
				host_ptr[i] = ppttr;
				if (ret != CL_SUCCESS) retmeserr9(ret);
				//vram���m�ۂ��]��
				ret = clEnqueueWriteBuffer(command_queue[clsetdev * COMMANDQUEUE_PER_DEVICE + clsetque], clm[i], CL_TRUE, 0,
					copysize[i], host_ptr[i], 0, NULL, NULL);
				if (ret != CL_SUCCESS) { retmeserr2(ret); }
				//�u���b�L���O����œ]��
				//p1�J�[�l��id�Ap2�͈����ʒu�Ap3�ɂ̓T�C�Y�Ap4�͎���
				clSetKernelArg(kernel, i, 4, &clm[i]);
			}
		}

		//����ƈ����ݒ肪�I�����������������������������������������������������������������������������������������������������

		//���Ɋ֐��̎��s
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

		//GPU��CPU�ɕϐ���߂��Ă���
		for (int i = 0; i < CLCALL_LOOP; i++)
		{
			if (copysize[i] != 0)
			{
				ret = clEnqueueReadBuffer(command_queue[clsetdev * COMMANDQUEUE_PER_DEVICE + clsetque], clm[i], CL_TRUE, 0,
					copysize[i], host_ptr[i], 0, NULL, NULL);
				if (ret != CL_SUCCESS) { retmeserr2(ret); }
				//vram�̉��
				ret = clReleaseMemObject(clm[i]);
				if (ret != CL_SUCCESS) {
					MessageBox(NULL, "�������J�����ł��܂���ł���", "�G���[", 0);
					puterror(HSPERR_UNSUPPORTED_FUNCTION);
				}
				clm[i] = NULL; host_ptr[i] = NULL;
			}
		}

		//��Еt��
		clReleaseKernel(kernel);
		clReleaseProgram(program);

		//����ɂđS�s���I���̂͂��I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I
		break;
	}

	case 0x6D:	// HCLDoKrn1 int p1,int p2,int p3,int p4
	{
		int prm1 = code_geti();//�J�|�l��id
		size_t globalsize = code_getdi(1);
		size_t localsize = code_getdi(0);

		cl_int ret;
		//outevent�֘A
		cl_event* outevent = EventOutChk(prm1);
		//wait event list�֘A
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
		int prm1 = code_geti();//�J�|�l��id
		size_t globalsize[2];
		size_t localsize[2];
		globalsize[0] = code_geti();
		globalsize[1] = code_geti();
		localsize[0] = code_geti();
		localsize[1] = code_geti();

		cl_int ret;
		//outevent�֘A
		cl_event* outevent = EventOutChk(prm1);
		//wait event list�֘A
		cl_event* ev_ = GetWaitEvlist();
		ret = clEnqueueNDRangeKernel(command_queue[clsetdev * COMMANDQUEUE_PER_DEVICE + clsetque],
			(cl_kernel)prm1, 2, NULL, globalsize, localsize, num_event_wait_list, ev_, outevent);
		if (ret != CL_SUCCESS) { retmeserr(ret); }
		num_event_wait_list = 0;
		break;
	}

	case 0x7E:	// HCLDoKrn3 int p1,int p2,int p3,int p4,int p5,int p6,int p7,int p8
	{
		int prm1 = code_geti();//�J�|�l��id
		size_t globalsize[3];
		size_t localsize[3];
		globalsize[0] = code_geti();
		globalsize[1] = code_geti();
		globalsize[2] = code_geti();
		localsize[0] = code_geti();
		localsize[1] = code_geti();
		localsize[2] = code_geti();

		cl_int ret;
		//outevent�֘A
		cl_event* outevent = EventOutChk(prm1);
		//wait event list�֘A
		cl_event* ev_ = GetWaitEvlist();
		ret = clEnqueueNDRangeKernel(command_queue[clsetdev * COMMANDQUEUE_PER_DEVICE + clsetque],
			(cl_kernel)prm1, 3, NULL, globalsize, localsize, num_event_wait_list, ev_, outevent);
		if (ret != CL_SUCCESS) { retmeserr(ret); }
		num_event_wait_list = 0;
		break;
	}

	case 0x6E:	// HCLDoKernel int p1,int p2,array p3,array p4
	{
		int prm2 = code_geti();		// �J�[�l��
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
				APTR aptr1;	//�z��ϐ��̎擾
				aptr1 = code_getva(&pval1);//	���͕ϐ��̌^�Ǝ��̂̃|�C���^���擾
				HspVarProc* phvp1;
				void* ptr1;
				phvp1 = exinfo->HspFunc_getproc(pval1->flag);	//�^����������HspVarProc�\���̂ւ̃|�C���^
				ptr1 = phvp1->GetPtr(pval1);					//�f�[�^�ipval1�j�̎��Ԃ�����擪�|�C���^���擾�B

				p4[0] = *(int*)ptr1;
				p4[1] = *((int*)ptr1 + 1);
				if (p3 == 3)
					p4[2] = *((int*)ptr1 + 2);

				PVal* pval2;
				APTR aptr2;	//�z��ϐ��̎擾
				aptr2 = code_getva(&pval2);//	���͕ϐ��̌^�Ǝ��̂̃|�C���^���擾
				HspVarProc* phvp2;
				void* ptr2;
				phvp2 = exinfo->HspFunc_getproc(pval2->flag);	//�^����������HspVarProc�\���̂ւ̃|�C���^
				ptr2 = phvp2->GetPtr(pval2);					//�f�[�^�ipval1�j�̎��Ԃ�����擪�|�C���^���擾�B

				ptryes[0] = *(int*)ptr2;
				ptryes[1] = *((int*)ptr2 + 1);
				if (p3 == 3)
					ptryes[2] = *((int*)ptr2 + 2);
			}
		}

		cl_int ret;

		//outevent�֘A
		cl_event* outevent = EventOutChk(prm2);
		//wait event list�֘A
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
		int prm2 = code_geti();		// �p�����[�^1:�J�[�l��
		size_t p4 = code_getdi(1);	//����
		size_t p5 = code_getdi(1);//���[�J���T�C�Y
		if (p5 == 0) { p5 = 64; }
		size_t p4_1 = (p4 / p5) * p5;//p5�Ŋ���؂�鐔����
		size_t p4_2 = p4 - p4_1;//���̒[��
		cl_int ret;

		//outevent�֘A
		cl_event* outevent = EventOutChk(prm2);
		//wait event list�֘A
		cl_event* ev_ = GetWaitEvlist();

		if (p4_1 != 0) {
			ret = clEnqueueNDRangeKernel(command_queue[clsetdev * COMMANDQUEUE_PER_DEVICE + clsetque],
				(cl_kernel)prm2, 1, NULL, &p4_1, &p5, num_event_wait_list, ev_, outevent);//1��ڂ͖����I���
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

	case 0x70:	// HCLFinish//���݂̃f�o�C�X�̌��݂̃R�}���h�L���[�̒��ɂ���^�X�N��S���҂�
	{
		cl_int ret = clFinish(command_queue[clsetdev * COMMANDQUEUE_PER_DEVICE + clsetque]);
		if (ret != CL_SUCCESS) { retmeserr10(ret); }
		break;
	}

	case 0x71:	// HCLFlush//�R�}���h�L���[
	{
		cl_int ret = clFlush(command_queue[clsetdev * COMMANDQUEUE_PER_DEVICE + clsetque]);
		if (ret != CL_SUCCESS) retmeserr11(ret);
		break;
	}

	case 0x72:	// _ExHCLSetCommandQueueMax
	{
		if (dev_num != 0)
		{
			MessageBox(NULL, "���̖��߂�clinit�̑O�Ɏ��s���Ă�������", "�G���[", 0);
			puterror(HSPERR_UNSUPPORTED_FUNCTION);
		}
		COMMANDQUEUE_PER_DEVICE = code_geti();
		break;
	}

	case 0x73:	// _ExHCLSetCommandQueueProperties
	{
		if (dev_num != 0)
		{
			MessageBox(NULL, "���̖��߂�clinit�̑O�Ɏ��s���Ă�������", "�G���[", 0);
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
			MessageBox(NULL, "�w�肵���R�}���h�L���[��id���傫�����܂��B\n_ExHCLSetCommandQueueMax�ōő�l��ύX���Ă��������B", "�G���[", 0);
			puterror(HSPERR_UNSUPPORTED_FUNCTION);
		}
		break;
	}

	case 0x76:	// _ExHCLSetEventMax
	{
		if (dev_num != 0)
		{
			MessageBox(NULL, "���̖��߂�clinit�̑O�Ɏ��s���Ă�������", "�G���[", 0);
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
		APTR aptr1;	//�z��ϐ��̎擾
		aptr1 = code_getva(&pval1);//	���͕ϐ��̌^�Ǝ��̂̃|�C���^���擾

		num_event_wait_list = pval1->len[1];//�S�̗̂v�f��

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
		APTR aptr1;	//�z��ϐ��̎擾
		aptr1 = code_getva(&pval1);//	���͕ϐ��̌^�Ǝ��̂̃|�C���^����
		int n = pval1->len[1];//�S�̗̂v�f��
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

	////////////////////////�����܂�/////////////////////
	////////////////////////�����܂�/////////////////////
	////////////////////////�����܂�/////////////////////
	////////////////////////�����܂�/////////////////////
	////////////////////////�����܂�/////////////////////
	////////////////////////�����܂�/////////////////////
	////////////////////////�����܂�/////////////////////
	////////////////////////�����܂�/////////////////////
	////////////////////////�����܂�/////////////////////
	////////////////////////�����܂�/////////////////////
	////////////////////////�����܂�/////////////////////

	default:
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
	}
	return RUNMODE_RUN;
}






























































static double ref_doubleval;					// �Ԓl�̂��߂̕ϐ�
static float ref_floatval;						// �Ԓl�̂��߂̕ϐ�
static int ref_int32val;						// �Ԓl�̂��߂̕ϐ�
static char* cptr;								// �Ԓl�̂��߂̕ϐ�



static void* reffunc(int* type_res, int cmd)
{

	//		�֐��E�V�X�e���ϐ��̎��s���� (�l�̎Q�Ǝ��ɌĂ΂�܂�)
	//
	//			'('�Ŏn�܂邩�𒲂ׂ�
	//
	if (*type != TYPE_MARK) puterror(HSPERR_INVALID_FUNCPARAM);
	if (*val != '(') puterror(HSPERR_INVALID_FUNCPARAM);
	code_next();

	bool fDouble = false;
	bool fFloat = false;
	bool fInt = false;
	bool fStr = false;

	switch (cmd) {							// �T�u�R�}���h���Ƃ̕���

	case 0xFF:								// float�֐�
	{
		ref_floatval = 0.0f;
		double dp1 = code_getd();					// �����l���擾(�f�t�H���g�Ȃ�)
		ref_floatval = (float)dp1;				// �Ԓl��ݒ�
		*type_res = HspVarFloat_typeid();		// �Ԓl�̃^�C�v���w�肷��
		break;
	}

	//////////////////////////////////////////��������
	//////////////////////////////////////////��������
	//////////////////////////////////////////��������
	//////////////////////////////////////////��������
	//////////////////////////////////////////��������
	//////////////////////////////////////////��������
	//////////////////////////////////////////��������
	//////////////////////////////////////////��������
	//////////////////////////////////////////��������
	//////////////////////////////////////////��������
	//////////////////////////////////////////��������
	//////////////////////////////////////////��������
	//////////////////////////////////////////��������

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
		pathname = code_gets();								// ��������擾
		std::string s_sourse = readFileIntoString(std::string(pathname));

		char* c_option;
		c_option = code_getds("");								// ��������擾
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
		c_source = code_gets();								// �\�[�X�R�[�h
		std::string s_sourse = std::string(c_source);

		char* c_option;
		c_option = code_getds("");								// �r���h�I�v�V������������擾
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
		p = code_gets();								// ��������擾
		//strncpy(pathname, p, _MAX_PATH - 1);			// �擾������������R�s�[
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

	//�����֐� 64bit����*8�p�^�[�����o��
	case 0x7A:// HCLGetEventLogs
	{
		//0:���̃C�x���g��CL_COMMAND_NDRANGE_KERNEL���������ACL_COMMAND_WRITE_BUFFER��������
		//1:kernel_id���R�s�[�T�C�Y
		//2:����event�����s����device no
		//3:����event�����s����que no
		//4:CL_PROFILING_COMMAND_QUEUED�̎���                 0x1280
		//5:CL_PROFILING_COMMAND_SUBMIT�̎���                 0x1281
		//6:CL_PROFILING_COMMAND_START�̎���                  0x1282
		//7:CL_PROFILING_COMMAND_END�̎���                    0x1283
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

	////////////////////////////////////////////////////////////�����܂�
	////////////////////////////////////////////////////////////�����܂�
	////////////////////////////////////////////////////////////�����܂�
	////////////////////////////////////////////////////////////�����܂�
	////////////////////////////////////////////////////////////�����܂�
	////////////////////////////////////////////////////////////�����܂�
	////////////////////////////////////////////////////////////�����܂�
	////////////////////////////////////////////////////////////�����܂�
	////////////////////////////////////////////////////////////�����܂�
	////////////////////////////////////////////////////////////�����܂�
	////////////////////////////////////////////////////////////�����܂�
	////////////////////////////////////////////////////////////�����܂�
	////////////////////////////////////////////////////////////�����܂�
	////////////////////////////////////////////////////////////�����܂�
	////////////////////////////////////////////////////////////�����܂�
	////////////////////////////////////////////////////////////�����܂�
	////////////////////////////////////////////////////////////�����܂�
	////////////////////////////////////////////////////////////�����܂�
	////////////////////////////////////////////////////////////�����܂�
	////////////////////////////////////////////////////////////�����܂�


	default:
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
	}

	//			'('�ŏI��邩�𒲂ׂ�
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
	//		�I������ (�A�v���P�[�V�����I�����ɌĂ΂�܂�)
	//
	return 0;
}


int WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, PVOID pvReserved)
{
	//		DLL�G���g���[ (��������K�v�͂���܂���)
	//
	return TRUE;
}

EXPORT void WINAPI hsp3cmdinit(HSP3TYPEINFO* info)
{
	//		�v���O�C�������� (���s�E�I��������o�^���܂�)
	//
	hsp3sdk_init(info);		// SDK�̏�����(�ŏ��ɍs�Ȃ��ĉ�����)

	info->cmdfunc = cmdfunc;		// ���s�֐�(cmdfunc)�̓o�^
	info->reffunc = reffunc;		// �Q�Ɗ֐�(reffunc)�̓o�^
	info->termfunc = termfunc;		// �I���֐�(termfunc)�̓o�^

	registvar(-1, HspVarFloat_Init);		// �V�����^�̒ǉ�
}