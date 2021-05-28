#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "hsp3plugin.h"
#include "hspvar_float.h"

#include <CL/cl_gl.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/glu.h>
#include <CL/opencl.h>
#define MAX_SOURCE_SIZE 0x200000
#define MAX_PLATFORM_IDS 16 //platform_id�̍ő�l
#define MAX_DEVICE_IDS 32 //devices�̍ő�l

int gplatkz=0;//�v���b�g�t�H�[���̐�  2
int gdevkz=0;//�f�o�C�X�̐�  4

cl_platform_id platform_id[MAX_PLATFORM_IDS];
cl_context *context;
cl_command_queue *command_queue;
cl_mem mem_obj;//���4096�P�p
cl_program program;
cl_kernel kernel;
cl_event dmyevent;
cl_event *ddmyevent=&dmyevent;
int clsetdev=0;
void gtflt(float *ff0);
void gtint(int *ii0);
void gtdbl(double *dd0);
void retmeserr(cl_int ret);//clEnqueueNDRangeKernel �Ŏ��s�������o���G���[���b�Z�[�W���܂Ƃ߂��֐�
void retmeserr2(cl_int ret);//clRead�Ŏ��s�������o���G���[���b�Z�[�W���܂Ƃ߂��֐�


struct nandemooiiii{
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
	bool GLrenkei;
	bool CLyukflg;
};
nandemooiiii *gpus;


// 


HGLRC hrc;//GL�֘A
HWND hwndd;
HDC hdc_d;
static PIXELFORMATDESCRIPTOR pfd = {
		sizeof (PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL,
		PFD_TYPE_RGBA,
		24,
		0, 0, 0,
		0, 0, 0,
		0, 0,
		0, 0, 0, 0, 0,
		32,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0,
		0,
		0
	};
int onexitflg=0;
int dbgflg=0;



static void newcmd1( void )
{
	cl_device_id device_id[MAX_DEVICE_IDS];
    cl_uint ret_num_devices;
    cl_uint ret_num_platforms;
    cl_int ret = clGetPlatformIDs(MAX_PLATFORM_IDS, platform_id, &ret_num_platforms);
	gplatkz=ret_num_platforms;
	int gkw=0;

	if (gplatkz!=0){
		for(int i=0;i<gplatkz;i++){
		ret = clGetDeviceIDs( platform_id[i], CL_DEVICE_TYPE_ALL,MAX_DEVICE_IDS, device_id, &ret_num_devices);
		gkw+=ret_num_devices;
		}
		gpus=new struct nandemooiiii[gkw];
		int w=0;
		for(int i=0;i<gplatkz;i++){
		ret = clGetDeviceIDs( platform_id[i], CL_DEVICE_TYPE_ALL,MAX_DEVICE_IDS, device_id, &ret_num_devices);
			for(int j=0;j<int(ret_num_devices);j++)
			{
				clGetDeviceInfo(device_id[j], CL_DEVICE_NAME, 1024, &gpus[w].name, NULL);
				clGetDeviceInfo(device_id[j], CL_DEVICE_TYPE,sizeof(cl_device_type),&gpus[w].coretype, NULL);
				clGetDeviceInfo(device_id[j], CL_DEVICE_VENDOR, 1024, &gpus[w].vendor, NULL);
				clGetDeviceInfo(device_id[j], CL_DEVICE_EXTENSIONS, 1024, &gpus[w].EXTENSIONS, NULL);
				clGetDeviceInfo(device_id[j], CL_DRIVER_VERSION, 1024, &gpus[w].version, NULL);
				clGetDeviceInfo(device_id[j], CL_DEVICE_VERSION, 1024, &gpus[w].cversion, NULL);
				clGetDeviceInfo(device_id[j], CL_DEVICE_LOCAL_MEM_SIZE,sizeof(cl_ulong), &gpus[w].localmemsize, NULL);
				clGetDeviceInfo(device_id[j], CL_DEVICE_GLOBAL_MEM_SIZE,sizeof(cl_ulong), &gpus[w].globalmemsize, NULL);
				clGetDeviceInfo(device_id[j], CL_DEVICE_MAX_MEM_ALLOC_SIZE,sizeof(cl_ulong), &gpus[w].maxmemalloc, NULL);
				clGetDeviceInfo(device_id[j], CL_DEVICE_MAX_CLOCK_FREQUENCY,sizeof(cl_uint), &gpus[w].maxfrq, NULL);
				clGetDeviceInfo(device_id[j], CL_DEVICE_MAX_WORK_GROUP_SIZE,sizeof(size_t), &gpus[w].maxworkgsz, NULL);

				size_t itj[3];
				clGetDeviceInfo(device_id[j], CL_DEVICE_MAX_WORK_ITEM_SIZES,sizeof(size_t)*3, &itj, NULL);
				gpus[w].maxworkisz0=itj[0];gpus[w].maxworkisz1=itj[1];gpus[w].maxworkisz2=itj[2];
				clGetDeviceInfo(device_id[j], CL_DEVICE_MAX_COMPUTE_UNITS,sizeof(cl_uint), &gpus[w].maxcompunit, NULL);
				cl_device_type itj3;
				clGetDeviceInfo(device_id[j], CL_DEVICE_TYPE,sizeof(cl_device_type), &itj3, NULL);
				gpus[w].devtype=int(itj3);
				gpus[w].plat=i;
				gpus[w].dev=device_id[j];

				clGetDeviceInfo(device_id[j], CL_DEVICE_IMAGE2D_MAX_WIDTH,sizeof(size_t), &gpus[w].m2dx, NULL);
				clGetDeviceInfo(device_id[j], CL_DEVICE_IMAGE2D_MAX_HEIGHT,sizeof(size_t), &gpus[w].m2dy, NULL);
				clGetDeviceInfo(device_id[j], CL_DEVICE_IMAGE3D_MAX_WIDTH,sizeof(size_t), &gpus[w].m3dx, NULL);
				clGetDeviceInfo(device_id[j], CL_DEVICE_IMAGE3D_MAX_HEIGHT,sizeof(size_t), &gpus[w].m3dy, NULL);
				clGetDeviceInfo(device_id[j], CL_DEVICE_IMAGE3D_MAX_DEPTH,sizeof(size_t), &gpus[w].m3dz, NULL);
				cl_device_local_mem_type itj4;
				clGetDeviceInfo(device_id[j], CL_DEVICE_LOCAL_MEM_TYPE,sizeof(cl_device_local_mem_type), &itj4, NULL);
				if (itj4==CL_GLOBAL){gpus[w].localmemtype=0;}
				if (itj4==CL_LOCAL){gpus[w].localmemtype=1;}
				cl_bool itj6;
				clGetDeviceInfo(device_id[j], CL_DEVICE_COMPILER_AVAILABLE,sizeof(cl_bool), &itj6, NULL);
				if (itj6==CL_FALSE){gpus[w].compile=0;}
				if (itj6==CL_TRUE){gpus[w].compile=1;}
				w++;
			}
		}
	}//////////�f�o�C�X�����擾���܂����Ă���Ƃ���


	
	cl_context_properties props[] =//GL�Ɗ֘A�t���v���p�e�B�t�@�C���쐬
	{
		CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
		CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
		CL_CONTEXT_PLATFORM, (cl_context_properties)platform_id[0],
		0
	};
	clGetGLContextInfoKHR_fn _clGetGLContextInfoKHR =(clGetGLContextInfoKHR_fn)clGetExtensionFunctionAddress("clGetGLContextInfoKHR");
	cl_device_id cdDeviceID[32];
	size_t size3=0;
	int count =0;
	int countd =0;
	context=new cl_context[gkw];
	command_queue=new cl_command_queue[gkw];


	//���������GL�Ƃ̘A�g
	for(int i=0;i<gplatkz;i++){
		props[5] =(cl_context_properties)platform_id[i];
		if (dbgflg!=0){
			_clGetGLContextInfoKHR(props,CL_DEVICES_FOR_GL_CONTEXT_KHR, sizeof(cl_device_id), cdDeviceID, &size3);
		}else{
			_clGetGLContextInfoKHR(props,CL_CURRENT_DEVICE_FOR_GL_CONTEXT_KHR, sizeof(cl_device_id), cdDeviceID, &size3);
		}
		count=size3/sizeof(cl_device_id);
		countd+=count;

		for(int j=0;j<count;j++){// Create an OpenCL context������CL��GL���т�
			for(int k=0;k<gkw;k++){
				if (cdDeviceID[j]==gpus[k].dev){
					gpus[k].GLrenkei=1;
				}
			}
		}
	}//gpus[k].GLrenkei��openGL�L�����ǂ������ꂽ
	if (countd==0){if (dbgflg==1){MessageBox(NULL, "���ׂĂ�CPU/GPU��ŁAOpenGL��OpenCL���A�g�ł��Ȃ����ł��BOpenGL�����ŋN�����܂�", "�x��", 0); }}


	
	char str1[1024];
	char str2[1024];




	for(int k=0;k<gkw;k++){//�R���e�L�X�g�ƃR�}���h���[�����
		if (gpus[k].GLrenkei==1){
			props[5] =(cl_context_properties)platform_id[gpus[k].plat];
			context[k] = clCreateContext( props, 1, &gpus[k].dev, NULL, NULL, &ret);	
		}else{
			context[k] = clCreateContext( NULL, 1, &gpus[k].dev, NULL, NULL, &ret);
		}
		
		
		sprintf(str2, "�����s��");
		switch( ret ) {							//����
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

		case CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR:
			sprintf(str2, "CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR\nwhen an invalid OpenGL context or share group object handle is specified in properties (only if the cl_khr_gl_sharing extension is enabled).\n\n�����̃I�[�v��GL�R���e�L�X�g���邢�̓V�F�A�E�O���[�v�E�I�u�W�F�N�g�E�n���h��������(cl_khr_gl_sharing����g�����\�ɂȂ�ꍇ�̂�)�̒��Ŏw�肳���ꍇ�B�V�F�A�E�O���[�v������CL_CGL_SHAREGROUP_KHR�̃Z�b�g�ɂ��CGL�Ɋ�Â����I�[�v��GL�C���v�������e�[�V�����ɑ΂��Ďw�肳��A�w�肳�ꂽ�V�F�A�E�O���[�v���L����CGL�����ʂ��Ȃ��ꍇ�A�O���[�v�E�I�u�W�F�N�g(cl_khr_gl_sharing����g�����\�ɂȂ�ꍇ�̂�)�����L���܂��B");
			break;
		}


		if (ret!=CL_SUCCESS){
			sprintf(str1, "%s\n\n%s\n",gpus[k].name,str2);
			if (dbgflg==1){MessageBox(NULL, str1, "�R���e�L�X�g�����G���[", 0);MessageBox(NULL,"opengl�A�g�����ŃR���e�L�X�g�Đ������܂�", "", 0);}
			gpus[k].GLrenkei=0;
			context[k] = clCreateContext( NULL, 1, &gpus[k].dev, NULL, NULL, &ret);
			sprintf(str2, "�����s��");
			switch( ret ) {							//����
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

			case CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR:
				sprintf(str2, "CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR\nwhen an invalid OpenGL context or share group object handle is specified in properties (only if the cl_khr_gl_sharing extension is enabled).\n\n�����̃I�[�v��GL�R���e�L�X�g���邢�̓V�F�A�E�O���[�v�E�I�u�W�F�N�g�E�n���h��������(cl_khr_gl_sharing����g�����\�ɂȂ�ꍇ�̂�)�̒��Ŏw�肳���ꍇ�B�V�F�A�E�O���[�v������CL_CGL_SHAREGROUP_KHR�̃Z�b�g�ɂ��CGL�Ɋ�Â����I�[�v��GL�C���v�������e�[�V�����ɑ΂��Ďw�肳��A�w�肳�ꂽ�V�F�A�E�O���[�v���L����CGL�����ʂ��Ȃ��ꍇ�A�O���[�v�E�I�u�W�F�N�g(cl_khr_gl_sharing����g�����\�ɂȂ�ꍇ�̂�)�����L���܂��B");
				break;
			}
			if (ret!=CL_SUCCESS){sprintf(str1, "%s\n\n%s\n",gpus[k].name,str2);if (dbgflg==1){MessageBox(NULL, str1, "�R���e�L�X�g�����G���[", 0);}}
		}

		if (ret==CL_SUCCESS){if (dbgflg==1){MessageBox(NULL,"�R���e�L�X�g��������", "�R���e�L�X�g��������", 0);}}







		command_queue[k] = clCreateCommandQueue(context[k],gpus[k].dev, CL_QUEUE_PROFILING_ENABLE, &ret);
		
		switch( ret ) {							//����
		case CL_INVALID_CONTEXT:
			sprintf(str2, "�R�}���h�L���[�����G���[\ncontext ���L����OpenCL�R���e�L�X�g�łȂ��Ƃ��B");
			sprintf(str1, "%s\n\n%s",gpus[k].name,str2);
			if (dbgflg==1){MessageBox(NULL, str1, "�R�}���h�L���[�����G���[", 0);}
			break;

		case CL_INVALID_DEVICE:
			sprintf(str2, "�R�}���h�L���[�����G���[\ndevice ���L����OpenCL�f�o�C�X�łȂ��Ƃ��B���邢�́Acontext �Ɗ֘A�t�����Ă��Ȃ��Ƃ��B");
			sprintf(str1, "%s\n\n%s",gpus[k].name,str2);
			if (dbgflg==1){MessageBox(NULL, str1, "�R�}���h�L���[�����G���[", 0);}
			break;

		case CL_INVALID_VALUE:
			sprintf(str2, "�R�}���h�L���[�����G���[\nproperties �Ɏw�肵���l���L���Ȃ��̂łȂ��Ƃ��B");
			sprintf(str1, "%s\n\n%s",gpus[k].name,str2);
			if (dbgflg==1){MessageBox(NULL, str1, "�R�}���h�L���[�����G���[", 0);}
			sprintf(str1, "properties�ɃA�E�g�I�u�I�[�_�[���w�肵���Ƃ���G���[�ɐ������̂ŁA�C���I�[�_�[���s�Ƃ��čăR�}���h�L���[�������܂�");
			if (dbgflg==1){MessageBox(NULL, str1, "", 0);}
			command_queue[k] = clCreateCommandQueue(context[k],gpus[k].dev,0, &ret);
			break;

		case CL_INVALID_QUEUE_PROPERTIES:
			sprintf(str2, "�R�}���h�L���[�����G���[\nproperties �Ɏw�肵���l���A�L���ł�����̂̃f�o�C�X���T�|�[�g���Ă��Ȃ��Ƃ��B");
			sprintf(str1, "%s\n\n%s",gpus[k].name,str2);
			if (dbgflg==1){MessageBox(NULL, str1, "�R�}���h�L���[�����G���[", 0);}
			sprintf(str1, "properties�ɃA�E�g�I�u�I�[�_�[���w�肵���Ƃ���G���[�ɐ������̂ŁA�C���I�[�_�[���s�Ƃ��čăR�}���h�L���[�������܂�");
			if (dbgflg==1){MessageBox(NULL, str1, "", 0);}
			command_queue[k] = clCreateCommandQueue(context[k],gpus[k].dev,0, &ret);
			break;

		case CL_OUT_OF_HOST_MEMORY:
			sprintf(str2, "�R�}���h�L���[�����G���[\n�z�X�g��ł̃��\�[�X�m�ۂɎ��s�����Ƃ��B");
			sprintf(str1, "%s\n\n%s",gpus[k].name,str2);
			if (dbgflg==1){MessageBox(NULL, str1, "�R�}���h�L���[�����G���[", 0);}
			break;
		}


		if (ret==CL_SUCCESS){
			char str3[4];
			gpus[k].CLyukflg=1;
			if (gpus[k].GLrenkei==1){sprintf(str3, "�L��");}
			if (gpus[k].GLrenkei==0){sprintf(str3, "����");}
			sprintf(str2, "�R�}���h�L���[��������");
			sprintf(str1, "�f�o�C�X��\n%s\n\n%s\nOpenGL�A�g %s",gpus[k].name,str2,str3);
			if (dbgflg==1){MessageBox(NULL, str1, "", 0);}
		}else{
			gpus[k].CLyukflg=0;
		}

	}



	gdevkz=gkw;
	PVal *pval;
	APTR aptr;
	aptr = code_getva( &pval );
	code_setva( pval, aptr, HSPVAR_FLAG_INT, &gkw );
}


static void newcmd2( void )
{
	int p1=clsetdev;
	char out[90000];
	sprintf( out,"�v���b�g�t�H�[��id=�J��%d�J�ƃf�o�C�X��=�J��%s�J�ƃf�o�C�Xid=�J��%d�J�ƃf�o�C�X���(DEFAULT 1,CPU 2,GPU 4,ACCELERATOR 8)=�J��%d�J��OpenCL�\�t�g�E�F�A�h���C�o�̃o�[�W����=�J��%s�J�ƃf�o�C�X���T�|�[�g����OpenCL�̃o�[�W����=�J��%s�J�ƃf�o�C�X�̃x���_�[=�J��%s�J�ƃ��[�J��������(���L������)�̃T�C�Y(byte)=�J��%d�J�ƃf�o�C�X�ő���g��(MHz)=�J��%f�J�ƕ��񉉎Z�R�A�̐�=�J��%d�J�ƃO���[�o���������T�C�Y(byte)=�J��%f�J�ƃ������I�u�W�F�N�g�Ƃ��Ċm�ۂł���ő�̃������T�C�Y(byte)=�J��%f�J�ƂЂƂ̃��[�N�O���[�v���̃��[�N�A�C�e�����̍ő�l=�J��%d�J�ƃ��[�N�O���[�v���Ƃ̃��[�N�A�C�e�����̂��ꂼ��̎����ɂ��Ă̍ő�l=�J��%d,%d,%d�J��2D�C���[�W�̕��̍ő�T�C�Y=�J��%d�J��2D�C���[�W�̍����̍ő�T�C�Y=�J��%d�J��3D�C���[�W�̕��̍ő�T�C�Y=�J��%d�J��3D�C���[�W�̍����̍ő�T�C�Y=�J��%d�J��3D�C���[�W�̐[���̍ő�T�C�Y=�J��%d�J�ƃ��[�J���������̃^�C�v�i��p�̃����� �L��1 ����0�j=�J��%d�J�ƃv���O�����\�[�X���R���p�C�����邽�߂ɗ��p�ł���R���p�C���̗L��(�L1 ��0)=�J��%d�J��OpenGL�A�g�L��(�L1 ��0)=�J��%d�J��OpenCL�L��(�L1 ��0)=�J��%d�J�ƃf�o�C�X���T�|�[�g����g���@�\�̖��̂��X�y�[�X�ŋ�؂������X�g�̌`���̕�����ŕԂ��܂��J��%s�J��"
		,gpus[p1].plat,gpus[p1].name,p1,int(gpus[p1].coretype),gpus[p1].version,gpus[p1].cversion,gpus[p1].vendor
		,int(gpus[p1].localmemsize),float(gpus[p1].maxfrq)
		,int(gpus[p1].maxcompunit),float(gpus[p1].globalmemsize),float(gpus[p1].maxmemalloc),int(gpus[p1].maxworkgsz)
		,int(gpus[p1].maxworkisz0),int(gpus[p1].maxworkisz1),int(gpus[p1].maxworkisz2)
		,int(gpus[p1].m2dx),int(gpus[p1].m2dy),int(gpus[p1].m3dx),int(gpus[p1].m3dy),int(gpus[p1].m3dz)
		,int(gpus[p1].localmemtype),int(gpus[p1].compile),int(gpus[p1].GLrenkei),int(gpus[p1].CLyukflg),gpus[p1].EXTENSIONS);
	PVal *pval;
	APTR aptr;
	aptr = code_getva( &pval );
	code_setva( pval, aptr, HSPVAR_FLAG_STR, &out );
}



static void newcmd3( void )
{
	if (onexitflg==0){
		cl_int ret;

		if (gdevkz!=0){
			for(int j=0;j<gdevkz;j++){
				ret = clFlush(command_queue[j]);
				ret = clFinish(command_queue[j]);
				ret = clReleaseCommandQueue(command_queue[j]);
				ret = clReleaseContext(context[j]);
			}
			delete [] context;
			delete [] command_queue;
			delete [] gpus;
		}
//		wglMakeCurrent(NULL,NULL);
//		wglDeleteContext(hrc);
	}
	onexitflg=1;
}


static void newcmd4( void )
{
	char *p;
	char pathname[_MAX_PATH];
	p = code_gets();								// ��������擾
	strncpy( pathname, p, _MAX_PATH-1 );			// �擾������������R�s�[
	
    FILE *fp;
    char *source_str;
    size_t source_size;
	if (( fp=fopen(pathname, "r")) == NULL) {
		puterror(HSPERR_FILE_IO);
	}else{
		source_str = (char*)malloc(MAX_SOURCE_SIZE);
		source_size = fread( source_str, 1, MAX_SOURCE_SIZE, fp);
		fclose( fp );
		// Build the program
		program = clCreateProgramWithSource(context[clsetdev], 1, (const char **)&source_str, (const size_t *)&source_size, NULL);
		cl_int err0=clBuildProgram(program, 1,&gpus[clsetdev].dev, NULL, NULL, NULL);
		if (err0!=CL_SUCCESS){
		size_t len;
		char buffer[2048*64];
		clGetProgramBuildInfo(program,gpus[clsetdev].dev, 
				CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
		MessageBox(NULL, (LPCSTR)buffer, "Error on OpenCL code", MB_OK);
		puterror(HSPERR_UNKNOWN_CODE);
		}

		PVal *pval;
		APTR aptr;
		aptr = code_getva( &pval );
		code_setva( pval, aptr, HSPVAR_FLAG_INT, &program );
	}
}

static void newcmd5( void )
{
	p1 = code_getdi(0);		// �p�����[�^1:���l
	char *p;
	char pathname[_MAX_PATH];
	p = code_gets();								// ��������擾
	strncpy( pathname, p, _MAX_PATH-1 );			// �擾������������R�s�[
	cl_int ret;

    kernel= clCreateKernel((cl_program)p1, pathname,&ret);

	PVal *pval;
	APTR aptr;
	aptr = code_getva( &pval );
	code_setva( pval, aptr, HSPVAR_FLAG_INT, &kernel );
	
	switch( ret ) {							//����
		
	case CL_INVALID_PROGRAM:
		MessageBox(NULL, "program ���L���ȃv���O�����I�u�W�F�N�g�łȂ�", "�G���[", 0);
		newcmd3();
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
		break;

	case CL_INVALID_PROGRAM_EXECUTABLE:
		MessageBox(NULL, "program �ɐ���Ƀr���h���ꂽ���s�\�v���O�������Ȃ�", "�G���[", 0);
		newcmd3();
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
		break;

	case CL_INVALID_KERNEL_NAME:
		MessageBox(NULL, "kernel_name �� program ���Ɍ�����Ȃ�", "�G���[", 0);
		newcmd3();
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
		break;

	case CL_INVALID_KERNEL_DEFINITION:
		MessageBox(NULL, "kernel_name ���^����A����������̌^�Ƃ����� __kernel �֐��̊֐���`���Aprogram ���r���h���ꂽ���ׂẴf�o�C�X�œ����łȂ�", "�G���[", 0);
		newcmd3();
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
		break;

	case CL_INVALID_VALUE:
		MessageBox(NULL, "kernel_name �� NULL", "�G���[", 0);
		newcmd3();
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
		break;
		
	case CL_OUT_OF_RESOURCES:
		MessageBox(NULL, "�f�o�C�X��ł̃��\�[�X�m�ۂɎ��s����", "�G���[", 0);
		newcmd3();
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
		break;

	case CL_OUT_OF_HOST_MEMORY:
		MessageBox(NULL, "�z�X�g��ł̃��\�[�X�m�ۂɎ��s����", "�G���[", 0);
		newcmd3();
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
		break;
	}
}

static void newcmd6( void )
{
	p1 = code_getdi(0);		// �p�����[�^1:���l
	p2 = code_getdi(0);		// �p�����[�^1:���l

	int chk = code_getprm();							// �p�����[�^�[���擾(�^�͖��Ȃ�)
	if ( chk<=PARAM_END ) {
		return;										// �p�����[�^�[�ȗ����̏���
	}
	int type = mpval->flag;							// �p�����[�^�[�̌^���擾
	void* ppttr;
	int sizeofff;
	switch( type ) {
	case HSPVAR_FLAG_STR:								// �p�����[�^�[�������񂾂�����
		{
		ppttr = (char *)mpval->pt;
		sizeofff=1;
		break;
		}
	case HSPVAR_FLAG_DOUBLE:									// �p�����[�^�[��������������
		{
		ppttr = (double *)mpval->pt;
		sizeofff=8;
		break;
		}
	case 8:								// �p�����[�^�[�������񂾂�����
		{
		ppttr = (float *)mpval->pt;
		sizeofff=4;
		break;
		}
	case HSPVAR_FLAG_INT:									// �p�����[�^�[��������������
		{
		ppttr = (int *)mpval->pt;
		sizeofff=4;
		break;
		}
	default:
		puterror( HSPERR_TYPE_MISMATCH );			// �T�|�[�g���Ă��Ȃ��^�Ȃ�΃G���[
	}

	int p4 = code_getdi(0);		// �p�����[�^1:���l
	int clret=0;

	if (p1==0){
		MessageBox(NULL, "�J�[�l��id��0�ł�", "�G���[", 0);
		newcmd3();
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
	}else{
		//p1�J�[�l��id�Ap2�͈����ʒu�Ap3�ɂ͎��ԁAp4�̓��[�J���������t���O
		if (p4==0){clret=clSetKernelArg((cl_kernel)p1,p2,sizeofff, ppttr);}
		if (p4!=0){clret=clSetKernelArg((cl_kernel)p1,p2,p4,NULL);}
	}
}


static void newcmd7( void )
{
	PVal *pval;
	APTR aptr;
	aptr = code_getva( &pval );
	p3 = code_getdi(0);		// �p�����[�^1:���l
	cl_int errcode_ret;
	if (p3>1024*1024*512){
		MessageBox(NULL, "512M�𒴂���T�C�Y�͎w��ł��܂���", "�G���[", 0);
		newcmd3();
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
		}

	mem_obj= clCreateBuffer(context[clsetdev], CL_MEM_READ_WRITE, 
        p3, NULL,&errcode_ret);
	
	int dnryndtyd=0;
    cl_int ret=clEnqueueWriteBuffer(command_queue[clsetdev],mem_obj,0,1,
            1,&dnryndtyd, 0, NULL, NULL);


	switch( errcode_ret ) {							//����
		
	case CL_INVALID_CONTEXT:
		MessageBox(NULL, "context ���L����OpenCL�R���e�L�X�g�łȂ�", "�G���[", 0);
		newcmd3();
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
		break;
	case CL_INVALID_VALUE:
		MessageBox(NULL, "�ǂݏ�����p���������p�ӂł��܂���ł���", "�G���[", 0);
		newcmd3();
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
		break;
	case CL_INVALID_KERNEL:
		MessageBox(NULL, "size �� 0 �̂Ƃ��B�������� size �� context ���̃f�o�C�X�� CL_DEVICE_MAX_MEM_ALLOC_SIZE �̒l���傫��", "�G���[", 0);
		newcmd3();
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
		break;
	case CL_INVALID_HOST_PTR:
		MessageBox(NULL, "host_ptr �� NULL �� CL_MEM_USE_HOST_PTR �������� CL_MEM_COPY_HOST_PTR �� flags �Ɏw�肳��Ă���Ƃ��B�������́ACL_MEM_COPY_HOST_PTR �� CL_MEM_USE_HOST_PTR ���ݒ肳��Ă��Ȃ��̂� host_ptr �� NULL �łȂ�", "�G���[", 0);
		newcmd3();
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
		break;
	case CL_MEM_OBJECT_ALLOCATION_FAILURE:
		MessageBox(NULL, "�o�b�t�@�I�u�W�F�N�g�̃��������m�ۂ���̂Ɏ��s����", "�G���[", 0);
		newcmd3();
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
		break;
	case CL_OUT_OF_RESOURCES:
		MessageBox(NULL, "�f�o�C�X��ł̃��\�[�X�m�ۂɎ��s����", "�G���[", 0);
		newcmd3();
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
		break;
	case CL_OUT_OF_HOST_MEMORY:
		MessageBox(NULL, "�z�X�g��ł̃��\�[�X�m�ۂɎ��s����", "�G���[", 0);
		newcmd3();
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
		break;
	}

	if (ret!=CL_SUCCESS){retmeserr2(ret);}

	clFinish(command_queue[clsetdev]);
	code_setva( pval, aptr, HSPVAR_FLAG_INT, &mem_obj);
}

static void newcmd8( void )
{
	p2 = code_getdi(0);		// �p�����[�^1:���l

	PVal *pval1;
	APTR aptr1;	//�z��ϐ��̎擾
	aptr1 = code_getva( &pval1 );//	���͕ϐ��̌^�Ǝ��̂̃|�C���^���擾
	HspVarProc *phvp1;
	void *ptr1;
	phvp1 = exinfo->HspFunc_getproc( pval1->flag );	//�^����������HspVarProc�\���̂ւ̃|�C���^
	ptr1  = phvp1->GetPtr( pval1 );					//�f�[�^�ipval1�j�̎��Ԃ�����擪�|�C���^���擾�B

	p4 = code_getdi(0);		//�T�C�Y
	p5 = code_getdi(0);		//�R�s�[��I�t�Z�b�g�T�C�Y
	p6 = code_getdi(0);		//�R�s�[���I�t�Z�b�g�T�C�Y
	cl_bool p7 = code_getdi(1);		//�u���b�L���O���[�h

    cl_int ret=clEnqueueWriteBuffer(command_queue[clsetdev], (cl_mem)p2, p7, p5,
            p4,(char *)ptr1+p6, 0, NULL,    NULL    );
	
	if (ret!=CL_SUCCESS){retmeserr2(ret);}
}

static void newcmd9( void )
{
	p2 = code_getdi(0);		// �p�����[�^1:���l

	PVal *pval1;
	APTR aptr1;	//�z��ϐ��̎擾
	aptr1 = code_getva( &pval1 );//	���͕ϐ��̌^�Ǝ��̂̃|�C���^���擾
	HspVarProc *phvp1;
	void *ptr1;
	phvp1 = exinfo->HspFunc_getproc( pval1->flag );	//�^����������HspVarProc�\���̂ւ̃|�C���^
	ptr1  = phvp1->GetPtr( pval1 );					//�f�[�^�ipval1�j�̎��Ԃ�����擪�|�C���^���擾�B

	p4 = code_getdi(0);		//�T�C�Y
	p5 = code_getdi(0);		//�R�s�[���I�t�Z�b�g�T�C�Y
	p6 = code_getdi(0);		//�R�s�[��I�t�Z�b�g�T�C�Y
	cl_bool p7 = code_getdi(1);		//�u���b�L���O���[�h

    cl_int ret=clEnqueueReadBuffer(command_queue[clsetdev],(cl_mem)p2, p7, p5,
            p4,(char *)ptr1+p6, 0, NULL,    NULL    );
	
	if (ret!=CL_SUCCESS){retmeserr2(ret);}

}

static void newcmd10( void )
{
	p2= code_getdi(0);//�R�s�[�惁�����I�u�W�F�N�gid
	p3= code_getdi(0);//�R�s�[���������I�u�W�F�N�gid
	size_t p4 = code_getdi(0);		// �R�s�[�T�C�Y
	size_t p5 = code_getdi(0);		// �R�s�[��I�t�Z�b�g
	size_t p6 = code_getdi(0);		// �R�s�[���I�t�Z�b�g
	cl_bool p7 = code_getdi(1);		//�u���b�L���O���[�h
	cl_int ret=clEnqueueCopyBuffer (command_queue[clsetdev],(cl_mem)p3,(cl_mem)p2,p6,p5,p4, 0, NULL,NULL);


	switch(ret ) {							//����
	case CL_INVALID_COMMAND_QUEUE:
		MessageBox(NULL, "command_queue is not a valid command-queue", "�G���[", 0);
		newcmd3();
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
		break;
	case CL_INVALID_CONTEXT:
		MessageBox(NULL, "�������I�u�W�F�N�g���ʂ̃f�o�C�X�ō쐬���ꂽ�\��������܂�", "�G���[", 0);
		newcmd3();
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
		break;
	case CL_INVALID_MEM_OBJECT:
		MessageBox(NULL, "�������I�u�W�F�N�g�̎��̂�����܂���B�������I�u�W�F�N�g���ʂ̃f�o�C�X�ō쐬���ꂽ�\��������܂��B", "�G���[", 0);
		newcmd3();
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
		break;
	case CL_INVALID_VALUE:
		MessageBox(NULL, "�A�h���X�A�N�Z�X�ᔽ�ł��B�������ݗ̈�or�ǂݍ��ݗ̈悪�͂ݏo���Ă܂��B", "�G���[", 0);
		newcmd3();
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
		break;
	case CL_MEM_COPY_OVERLAP:
		MessageBox(NULL, "�A�h���X�A�N�Z�X�ᔽ�ł��B�������ݗ̈悩�ǂݍ��ݗ̈悪�͂ݏo���Ă܂��B", "�G���[", 0);
		newcmd3();
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
		break;
	case CL_MEM_OBJECT_ALLOCATION_FAILURE:
		MessageBox(NULL, "data store �̂��߂�allocate memory����̂����s���܂���", "�G���[", 0);
		newcmd3();
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
		break;
	case CL_OUT_OF_RESOURCES:
		MessageBox(NULL, "�f�o�C�X(GPU)��ł̃��\�[�X�m�ۂɎ��s����", "�G���[", 0);
		newcmd3();
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
		break;
	case CL_OUT_OF_HOST_MEMORY:
		MessageBox(NULL, "�z�X�g(CPU)��ł̃��\�[�X�m�ۂɎ��s����", "�G���[", 0);
		newcmd3();
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
		break;
	}

	if (p7){
		cl_int ret=clFinish(command_queue[clsetdev]);
		switch( ret ) {
		case CL_INVALID_COMMAND_QUEUE:
			MessageBox(NULL, "���������L���Ȓl�ł͂���܂���", "�G���[", 0);
			newcmd3();
			puterror( HSPERR_UNSUPPORTED_FUNCTION );
			break;
		case CL_INVALID_CONTEXT:
			MessageBox(NULL, "���邢�͑������� event_wait_list ���̃C�x���g�Ɗ֘A�t����ꂽ�f�o�C�X�������łȂ�", "�G���[", 0);
			newcmd3();
			puterror( HSPERR_UNSUPPORTED_FUNCTION );
			break;
		case CL_INVALID_EVENT:
			MessageBox(NULL, "event_list�̃C�x���g�I�u�W�F�N�g���s��", "�G���[", 0);
			newcmd3();
			puterror( HSPERR_UNSUPPORTED_FUNCTION );
			break;
		case CL_OUT_OF_HOST_MEMORY:
			MessageBox(NULL, "�z�X�g��ł̃��\�[�X�m�ۂɎ��s����", "�G���[", 0);
			newcmd3();
			puterror( HSPERR_UNSUPPORTED_FUNCTION );
			break;
		}
	}
}

static void newcmd11( void )
{
	p2 = code_getdi(0);		// �J�[�l��
	p3 = code_getdi(1);		// work_dim
	size_t p4[3];
	size_t ptryes[3];

	if (p3==1){
		p4[0]=code_getdi(1);
		ptryes[0]=code_getdi(0);
	}else{
		if ((p3==2)|(p3==3)){
			PVal *pval1;
			APTR aptr1;	//�z��ϐ��̎擾
			aptr1 = code_getva( &pval1 );//	���͕ϐ��̌^�Ǝ��̂̃|�C���^���擾
			HspVarProc *phvp1;
			void *ptr1;
			phvp1 = exinfo->HspFunc_getproc( pval1->flag );	//�^����������HspVarProc�\���̂ւ̃|�C���^
			ptr1  = phvp1->GetPtr( pval1 );					//�f�[�^�ipval1�j�̎��Ԃ�����擪�|�C���^���擾�B

			p4[0]=*(size_t *)ptr1;
			p4[1]=*((size_t *)ptr1+1);
			p4[2]=*((size_t *)ptr1+2);


			PVal *pval2;
			APTR aptr2;	//�z��ϐ��̎擾
			aptr2 = code_getva( &pval2 );//	���͕ϐ��̌^�Ǝ��̂̃|�C���^���擾
			HspVarProc *phvp2;
			void *ptr2;
			phvp2 = exinfo->HspFunc_getproc( pval2->flag );	//�^����������HspVarProc�\���̂ւ̃|�C���^
			ptr2  = phvp2->GetPtr( pval2 );					//�f�[�^�ipval1�j�̎��Ԃ�����擪�|�C���^���擾�B

			ptryes[0]=*(size_t *)ptr2;
			ptryes[1]=*((size_t *)ptr2+1);
			ptryes[2]=*((size_t *)ptr2+2);
		}
	}



	cl_int ret;
	if (ptryes[0]!=0){
		ret=clEnqueueNDRangeKernel(command_queue[clsetdev],(cl_kernel)p2,p3, NULL, p4, ptryes, 0, NULL,NULL);
	}else{
		ret=clEnqueueNDRangeKernel(command_queue[clsetdev],(cl_kernel)p2,p3, NULL, p4,NULL, 0, NULL,NULL);
	}
	if (ret!=CL_SUCCESS){retmeserr(ret);}

}

static void newcmd12( void )
{
	p2 = code_getdi(0);		// �p�����[�^1:���l

	cl_int ret=clReleaseKernel((cl_kernel)p2);
	if (ret!=CL_SUCCESS){
		MessageBox(NULL, "�J�[�l���J�����ł��܂���ł���", "�G���[", 0);
		newcmd3();
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
		}
}

static void newcmd13( void )
{
	p2 = code_getdi(0);		// �p�����[�^1:���l

	cl_int ret=clReleaseProgram((cl_program)p2);
	if (ret!=CL_SUCCESS){
		MessageBox(NULL, "�v���O�����f�[�^�J�����ł��܂���ł���", "�G���[", 0);
		newcmd3();
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
		}
}

static void newcmd14( void )
{
	p2 = code_getdi(0);		// �p�����[�^1:���l

	cl_int ret=0;
	ret=clReleaseMemObject((cl_mem)p2);

	if (ret!=CL_SUCCESS){
		MessageBox(NULL, "�������J�����ł��܂���ł���", "�G���[", 0);
		newcmd3();
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
		}
}

static void newcmd15( void )
{
	PVal *pval1;
	APTR aptr1;
	aptr1 = code_getva( &pval1 );
	exinfo->HspFunc_dim(pval1,8, 0,code_getdi(0), 0, 0, 0);
}

static void newcmd16( void )//�f�o�C�X�Z�b�g
{
	clsetdev= code_getdi(0);
}

static void newcmd17( void )//�f�o�C�X�ɂ���S���̃^�X�N�҂�
{
	cl_int ret=clFinish(command_queue[clsetdev]);

	switch( ret ) {							//����
		
	case CL_INVALID_COMMAND_QUEUE:
		MessageBox(NULL, "���������L���Ȓl�ł͂���܂���", "�G���[", 0);
		newcmd3();
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
		break;
	case CL_INVALID_CONTEXT:
		MessageBox(NULL, "���邢�͑������� event_wait_list ���̃C�x���g�Ɗ֘A�t����ꂽ�f�o�C�X�������łȂ�", "�G���[", 0);
		newcmd3();
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
		break;
	case CL_INVALID_EVENT:
		MessageBox(NULL, "event_list�̃C�x���g�I�u�W�F�N�g���s��", "�G���[", 0);
		newcmd3();
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
		break;
	case CL_OUT_OF_HOST_MEMORY:
		MessageBox(NULL, "�z�X�g��ł̃��\�[�X�m�ۂɎ��s����", "�G���[", 0);
		newcmd3();
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
		break;
	}

}

static void newcmd18( void )
{
	int p2 = code_getdi(0);		// �J�[�l��
	size_t p4=code_getdi(1);
	size_t ptryes=code_getdi(0);
	cl_int ret;
	if (ptryes!=0){
		ret=clEnqueueNDRangeKernel(command_queue[clsetdev],(cl_kernel)p2,1, NULL, &p4,&ptryes, 0, NULL,    NULL    );
	}else{
		ret=clEnqueueNDRangeKernel(command_queue[clsetdev],(cl_kernel)p2,1, NULL, &p4,NULL, 0, NULL,    NULL    );
	}
	if (ret!=CL_SUCCESS){retmeserr(ret);}
}


static void newcmd19( void )
{
	p2 = code_getdi(0);		// �J�[�l��
	size_t p4[2];
	p4[0]=code_getdi(1);
	p4[1]=code_getdi(1);
	size_t ptryes[2];
	ptryes[0]=code_getdi(0);
	ptryes[1]=code_getdi(0);
	
	cl_int ret;
	if (ptryes[0]!=0){
		ret=clEnqueueNDRangeKernel(command_queue[clsetdev],(cl_kernel)p2,2, NULL, p4,ptryes, 0, NULL,    NULL    );
	}else{
		ret=clEnqueueNDRangeKernel(command_queue[clsetdev],(cl_kernel)p2,2, NULL, p4,NULL, 0, NULL,    NULL    );
	}
	if (ret!=CL_SUCCESS){retmeserr(ret);}
}

static void newcmd20( void )
{
	p2 = code_getdi(0);		// �J�[�l��
	size_t p4[3];
	p4[0]=code_getdi(1);
	p4[1]=code_getdi(1);
	p4[2]=code_getdi(1);
	size_t ptryes[3];
	ptryes[0]=code_getdi(0);
	ptryes[1]=code_getdi(0);
	ptryes[2]=code_getdi(0);
	
	cl_int ret;
	if (ptryes[0]!=0){
		ret=clEnqueueNDRangeKernel(command_queue[clsetdev],(cl_kernel)p2,3, NULL, p4,ptryes, 0, NULL,    NULL    );
	}else{
		ret=clEnqueueNDRangeKernel(command_queue[clsetdev],(cl_kernel)p2,3, NULL, p4,NULL, 0, NULL,    NULL    );
	}
	if (ret!=CL_SUCCESS){retmeserr(ret);}
}

static void newcmd21( void )//������\�[�X�R���p�C��
{
    char *source_str;
	source_str = (char*)malloc(MAX_SOURCE_SIZE);
	source_str = code_gets();								// ��������擾
	
	int saizu=code_getdi(0);

	program = clCreateProgramWithSource(context[clsetdev], 1, (const char **)&source_str, (const size_t *)&saizu, NULL);
	cl_int err0=clBuildProgram(program, 1,&gpus[clsetdev].dev, NULL, NULL, NULL);
	if (err0!=CL_SUCCESS){
		size_t len;
		char buffer[2048*64];
		clGetProgramBuildInfo(program,gpus[clsetdev].dev, 
				CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
		MessageBox(NULL, (LPCSTR)buffer, "Error on OpenCL code", MB_OK);
		puterror(HSPERR_UNKNOWN_CODE);
	}
	
	PVal *pval;
	APTR aptr;
	aptr = code_getva( &pval );
	code_setva( pval, aptr, HSPVAR_FLAG_INT, &program );
}



static void newcmd24( void )
{
	dbgflg = code_getdi(0);		// �p�����[�^1:���l
}


static void newcmd51( void )
{
	cl_device_info devinfoi = code_getdi(0);
	size_t szt=0;
	int bufferout[1024*4];
	clGetDeviceInfo(gpus[clsetdev].dev,devinfoi,  sizeof(bufferout), &bufferout, &szt);

//	char bufferout2[1024*16];
//	memcpy(&bufferout2,&bufferout,szt);

	int *ptr0o;
	ptr0o=&bufferout[0];
	int tmp3=0;
	memcpy(&tmp3,&ptr0o,4);
	PVal *pval;
	APTR aptr;
	aptr = code_getva( &pval );
	code_setva( pval, aptr, HSPVAR_FLAG_INT, &tmp3 );
	
	PVal *pval2;
	APTR aptr2;
	aptr2 = code_getva( &pval2 );
	code_setva( pval2, aptr2, HSPVAR_FLAG_INT, &szt );
}




static void newcmd52()//HCLDoKrn1_sub
{
	int p2 = code_getdi(0);		// �J�[�l��
	size_t p4=code_getdi(1);	//����
	size_t p5=code_getdi(1);//���[�J���T�C�Y
	if (p5==0){p5=192;}
	size_t p4_1=(p4/p5)*p5;//p5�Ŋ���؂�鐔����
	size_t p4_2=p4-p4_1;//���̒[��

	cl_int ret;
	if (p4_1!=0){
		ret=clEnqueueNDRangeKernel(command_queue[clsetdev],(cl_kernel)p2,1, NULL, &p4_1,&p5, 0, NULL,    NULL    );//1��ڂ͖����I���
		if (ret!=CL_SUCCESS){retmeserr(ret);}
	}
	if (p4_2!=0){
		p5=p4_2;
		ret=clEnqueueNDRangeKernel(command_queue[clsetdev],(cl_kernel)p2,1, &p4_1, &p4_2,&p5, 0, NULL,    NULL    );
		if (ret!=CL_SUCCESS){retmeserr(ret);}
	}
}


static void newcmd53()//HCLcall
{
	int p2 = code_getdi(0);		// �J�[�l��
	size_t p4=code_getdi(1);	//����
	cl_int ret=clEnqueueNDRangeKernel(command_queue[clsetdev],(cl_kernel)p2,1, NULL, 
            &p4,NULL, 0, NULL,ddmyevent);
	if (ret!=CL_SUCCESS){retmeserr(ret);}
}


static void newcmd54()//HCLdBugPut_d
{
	p2 = code_getdi(0);		// �J�[�l��
	p3 = code_getdi(0);		// �C���f�b�N�X
	double p4 = code_getd();		//���g
    cl_int ret=clEnqueueWriteBuffer(command_queue[clsetdev], (cl_mem)p2,1,p3*sizeof(double),
            sizeof(double),&p4, 0, NULL,    NULL    );
	if (ret!=CL_SUCCESS){retmeserr2(ret);}
}


static void newcmd55()//HCLdBugPut_i
{
	p2 = code_getdi(0);		// �J�[�l��
	p3 = code_getdi(0);		// �C���f�b�N�X
	int p4 = code_getdi(0);		//���g
    cl_int ret=clEnqueueWriteBuffer(command_queue[clsetdev], (cl_mem)p2,1,p3*sizeof(int),
            sizeof(int),&p4, 0, NULL,    NULL    );
	if (ret!=CL_SUCCESS){retmeserr2(ret);}
}


static void newcmd56()//HCLdBugPut_f
{
	p2 = code_getdi(0);		// �J�[�l��
	p3 = code_getdi(0);		// �C���f�b�N�X


	int chk = code_getprm();							// �p�����[�^�[���擾(�^�͖��Ȃ�)
	if ( chk<=PARAM_END ) {
		return;										// �p�����[�^�[�ȗ����̏���
	}
	int type = mpval->flag;							// �p�����[�^�[�̌^���擾
	void* ppttr;
	int sizeofff;
	float p4;
	double s8;
	int p10;
	switch( type ) {
	case HSPVAR_FLAG_DOUBLE:									// �p�����[�^�[��������������
		{
		ppttr = (double *)mpval->pt;
		sizeofff=8;
		memcpy(&s8,ppttr,sizeofff);
		p4=float(s8);
		break;
		}
	case 8:								// �p�����[�^�[�������񂾂�����
		{
		ppttr = (float *)mpval->pt;
		sizeofff=4;
		memcpy(&p4,ppttr,sizeofff);
		break;
		}
	case HSPVAR_FLAG_INT:									// �p�����[�^�[��������������
		{
		ppttr = (int *)mpval->pt;
		sizeofff=4;
		memcpy(&p10,ppttr,sizeofff);
		p4=float(p10);
		break;
		}
	default:
		puterror( HSPERR_TYPE_MISMATCH );			// �T�|�[�g���Ă��Ȃ��^�Ȃ�΃G���[
	}

	//�����p4�ɐ�����float��������ꂽ




    cl_int ret=clEnqueueWriteBuffer(command_queue[clsetdev], (cl_mem)p2,1,p3*sizeof(float),
            sizeof(float),&p4, 0, NULL,    NULL    );
	if (ret!=CL_SUCCESS){retmeserr2(ret);}
}


static void newcmd57()//HCLdBugPut_s
{
	p2 = code_getdi(0);		// �J�[�l��
	int p3 = code_getdi(0);		// �C���f�b�N�X
	int p4 = code_getdi(0);		//���g
    cl_int ret=clEnqueueWriteBuffer(command_queue[clsetdev], (cl_mem)p2,1,p3*sizeof(char),
            sizeof(char),&p4, 0, NULL,    NULL    );
	if (ret!=CL_SUCCESS){retmeserr2(ret);}
}



static void newcmd58()//HCLGetDev
{
	PVal *pval;
	APTR aptr;
	aptr = code_getva( &pval );
	code_setva( pval, aptr, HSPVAR_FLAG_INT, &clsetdev );
}


static void newcmd59()//HCLSetKrns
{
	p1 = code_getdi(0);		// �p�����[�^1:���l
	if (p1==0){
		MessageBox(NULL, "�J�[�l��id��0�ł�", "�G���[", 0);
		newcmd3();
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
	}

	
	void* ppttr;
	int sizeofff;
	int chk;
	int type;


	for(int i=0;i<16;i++){
		chk = code_getprm();							// �p�����[�^�[���擾(�^�͖��Ȃ�)
		if ( chk<=PARAM_END ) {
			return;										// �p�����[�^�[�ȗ����̏���
		}
		type = mpval->flag;							// �p�����[�^�[�̌^���擾
		switch( type ) {
		case HSPVAR_FLAG_STR:								// �p�����[�^�[�������񂾂�����
			{
			ppttr = (char *)mpval->pt;
			sizeofff=1;
			break;
			}
		case HSPVAR_FLAG_DOUBLE:									// �p�����[�^�[��������������
			{
			ppttr = (double *)mpval->pt;
			sizeofff=8;
			break;
			}
		case 8:								// �p�����[�^�[�������񂾂�����
			{
			ppttr = (float *)mpval->pt;
			sizeofff=4;
			break;
			}
		case HSPVAR_FLAG_INT:									// �p�����[�^�[��������������
			{
			ppttr = (int *)mpval->pt;
			sizeofff=4;
			break;
			}
		default:
			puterror( HSPERR_TYPE_MISMATCH );			// �T�|�[�g���Ă��Ȃ��^�Ȃ�΃G���[
		}

		//p1�J�[�l��id�Ap2�͈����ʒu�Ap3�ɂ͎��ԁAp4�̓��[�J���������t���O
		clSetKernelArg((cl_kernel)p1,i,sizeofff, ppttr);
	}
}



static void newcmd60()//HCLGetEventTime
{
	cl_ulong dmyend;
	cl_ulong dmystart;

	clGetEventProfilingInfo(dmyevent,CL_PROFILING_COMMAND_START,sizeof(cl_ulong),&dmystart,NULL);
	clGetEventProfilingInfo(dmyevent,CL_PROFILING_COMMAND_END  ,sizeof(cl_ulong),&dmyend,NULL);
	
	int dmydt=dmyend-dmystart;
	PVal *pval;
	APTR aptr;
	aptr = code_getva( &pval );
	code_setva( pval, aptr, HSPVAR_FLAG_INT, &dmydt );
}


static void newcmd61()//��
{
	
}

static void newcmd62()//��
{
	
}

static void newcmd63()//��
{
	
}

static void newcmd64()//��
{
	
}

static void newcmd65()//��
{
	
}

static void newcmd66()//��
{
	
}

static void newcmd67()//��
{
	
}

static void newcmd68()//��
{
	
}




















































































//----------------------------------------------------------------------------------------�����ȉ���OpenGL--------------------------------
//----------------------------------------------------------------------------------------�����ȉ���OpenGL--------------------------------
//----------------------------------------------------------------------------------------�����ȉ���OpenGL--------------------------------
//----------------------------------------------------------------------------------------�����ȉ���OpenGL--------------------------------
//----------------------------------------------------------------------------------------�����ȉ���OpenGL--------------------------------
//----------------------------------------------------------------------------------------�����ȉ���OpenGL--------------------------------
//----------------------------------------------------------------------------------------�����ȉ���OpenGL--------------------------------
//----------------------------------------------------------------------------------------�����ȉ���OpenGL--------------------------------
//----------------------------------------------------------------------------------------�����ȉ���OpenGL--------------------------------
//----------------------------------------------------------------------------------------�����ȉ���OpenGL--------------------------------
//----------------------------------------------------------------------------------------�����ȉ���OpenGL--------------------------------
//----------------------------------------------------------------------------------------�����ȉ���OpenGL--------------------------------
//----------------------------------------------------------------------------------------�����ȉ���OpenGL--------------------------------
//----------------------------------------------------------------------------------------�����ȉ���OpenGL--------------------------------
//----------------------------------------------------------------------------------------�����ȉ���OpenGL--------------------------------
//----------------------------------------------------------------------------------------�����ȉ���OpenGL--------------------------------

static void newcmd23( void )//GL�����ݒ�
{
	int p1 = code_getdi(0);		// �p�����[�^1:���l
	memcpy(&hwndd,&p1,4);

	hdc_d = GetDC(hwndd);
	SetPixelFormat(hdc_d,ChoosePixelFormat(hdc_d, &pfd), &pfd);
	hrc = wglCreateContext(hdc_d);
	wglMakeCurrent(hdc_d, hrc);
	glewInit();
}


static void newcmd22( void )//hspcl32_glReadBuffer
{
	p2 = code_getdi(0);		// �p�����[�^1:���l

	PVal *pval1;
	APTR aptr1;	//�z��ϐ��̎擾
	aptr1 = code_getva( &pval1 );//	���͕ϐ��̌^�Ǝ��̂̃|�C���^���擾
	HspVarProc *phvp1;
	void *ptr1;
	phvp1 = exinfo->HspFunc_getproc( pval1->flag );	//�^����������HspVarProc�\���̂ւ̃|�C���^
	ptr1  = phvp1->GetPtr( pval1 );					//�f�[�^�ipval1�j�̎��Ԃ�����擪�|�C���^���擾�B

	p4 = code_getdi(0);		//�T�C�Y


	glBindBuffer(GL_ARRAY_BUFFER,p2);
	void *ptrdmm= glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
	memcpy(ptr1,ptrdmm,p4);
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER,0);
}



static void newcmd25( void )
{
	PVal *pval;
	APTR aptr;
	aptr = code_getva( &pval );
	p2 = code_getdi(0);		// �p�����[�^1:���l
	mem_obj= clCreateFromGLBuffer(context[clsetdev],CL_MEM_READ_WRITE,p2, NULL);
	code_setva( pval, aptr, HSPVAR_FLAG_INT, &mem_obj);
}


static void newcmd26( void )
{
	p1 = code_getdi(0);		// �p�����[�^1:���l
	cl_mem dm2;
	memcpy(&dm2,&p1,4);
	clEnqueueAcquireGLObjects(command_queue[clsetdev], 1,&dm2, 0,0,0);
}

static void newcmd27( void )
{
	p1 = code_getdi(0);		// �p�����[�^1:���l
	cl_mem dm2;
	memcpy(&dm2,&p1,4);
	clEnqueueReleaseGLObjects(command_queue[clsetdev], 1,&dm2, 0,0,0);
}






static void newcmd28( void )
{
	p1 = code_getdi(0);		// �p�����[�^1:���l
	p2 = code_getdi(0);		// �p�����[�^1:���l
	glBindBuffer(p1,p2);
}

static void newcmd29( void )
{
	p1 = code_getdi(0);		// �p�����[�^1:���l
	p2 = code_getdi(0);		// �p�����[�^1:���l
	p3 = code_getdi(0);		// �p�����[�^1:���l
	p4 = code_getdi(0);		// �p�����[�^1:���l
	void *p3tr;
	memcpy(&p3tr,&p3,4);
	glBufferData(p1,p2,p3tr,p4);
}

static void newcmd45( void )
{
	p1 = code_getdi(0);		// �p�����[�^1:���l
	p2 = code_getdi(0);		// �p�����[�^1:���l
	p3 = code_getdi(0);		// �p�����[�^1:���l
	p4 = code_getdi(0);		// �p�����[�^1:���l
	GLvoid *p4tr;
	memcpy(&p4tr,&p4,4);
	glBufferSubData(p1,p2,p3,p4tr);
}

static void newcmd30( void )
{
	p1 = code_getdi(0);		// �p�����[�^1:���l
	p2 = code_getdi(0);		// �p�����[�^1:���l
	GLuint *p2tr;
	memcpy(&p2tr,&p2,4);

	glDeleteBuffers(p1,p2tr);
}

static void newcmd31( void )
{
	p1 = code_getdi(0);		// �p�����[�^1:���l
	p2 = code_getdi(0);		// �p�����[�^1:���l
	GLuint *p2tr;
	memcpy(&p2tr,&p2,4);
	glGenBuffers(p1,p2tr);
}


static void newcmd32( void )
{
	p1 = code_getdi(0);		// �p�����[�^1:���l
	p2 = code_getdi(0);		// �p�����[�^1:���l
	GLuint *p2tr;
	memcpy(&p2tr,&p2,4);
	glGenRenderbuffers(p1,p2tr);
}


static void newcmd33( void )
{
	p1 = code_getdi(0);		// �p�����[�^1:���l
	p2 = code_getdi(0);		// �p�����[�^1:���l
	GLuint *p2tr;
	memcpy(&p2tr,&p2,4);
	glDeleteRenderbuffers(p1,p2tr);
}


static void newcmd34( void )
{
	p1 = code_getdi(0);		// �p�����[�^1:���l
	p2 = code_getdi(0);		// �p�����[�^1:���l
	glBindRenderbuffer(p1,p2);
}


static void newcmd35( void )
{
	p1 = code_getdi(0);		// �p�����[�^1:���l
	p2 = code_getdi(0);		// �p�����[�^1:���l
	p3 = code_getdi(0);		// �p�����[�^1:���l
	p4 = code_getdi(0);		// �p�����[�^1:���l
	glRenderbufferStorage(p1,p2,p3,p4);
}


static void newcmd36( void )
{
	p1 = code_getdi(0);		// �p�����[�^1:���l
	p2 = code_getdi(0);		// �p�����[�^1:���l
	GLuint *p2tr;
	memcpy(&p2tr,&p2,4);
	glGenVertexArrays(p1,p2tr);
}



static void newcmd37( void )
{
	p1 = code_getdi(0);		// �p�����[�^1:���l
	p2 = code_getdi(0);		// �p�����[�^1:���l
	GLuint *p2tr;
	memcpy(&p2tr,&p2,4);
	glDeleteVertexArrays(p1,p2tr);
}


static void newcmd38( void )
{
	p1 = code_getdi(0);		// �p�����[�^1:���l
	glBindVertexArray(p1);
}


static void newcmd39( void )
{
	p1 = code_getdi(0);		// �p�����[�^1:���l
	p2 = code_getdi(0);		// �p�����[�^1:���l
	GLuint *p2tr;
	memcpy(&p2tr,&p2,4);
	glGenFramebuffers(p1,p2tr);
}


static void newcmd40( void )
{
	p1 = code_getdi(0);		// �p�����[�^1:���l
	p2 = code_getdi(0);		// �p�����[�^1:���l
	GLuint *p2tr;
	memcpy(&p2tr,&p2,4);
	glDeleteFramebuffers(p1,p2tr);
}


static void newcmd41( void )
{
	p1 = code_getdi(0);		// �p�����[�^1:���l
	p2 = code_getdi(0);		// �p�����[�^1:���l
	glBindFramebuffer(p1,p2);
}


static void newcmd42( void )
{
	p1 = code_getdi(0);		// �p�����[�^1:���l
	p2 = code_getdi(0);		// �p�����[�^1:���l
	p3 = code_getdi(0);		// �p�����[�^1:���l
	p4 = code_getdi(0);		// �p�����[�^1:���l
	p5 = code_getdi(0);		// �p�����[�^1:���l
	glFramebufferTexture2D(p1,p2,p3,p4,p5);
}


static void newcmd43( void )
{
	p1 = code_getdi(0);		// �p�����[�^1:���l
	p2 = code_getdi(0);		// �p�����[�^1:���l
	p3 = code_getdi(0);		// �p�����[�^1:���l
	p4 = code_getdi(0);		// �p�����[�^1:���l
	glFramebufferRenderbuffer(p1,p2,p3,p4);
}

static void newcmd44( void )
{
	p1 = code_getdi(0);		// �p�����[�^1:���l
	glGenerateMipmap(p1);
}


static void newcmd46( void )
{
	PVal *pval;
	APTR aptr;
	aptr = code_getva( &pval );
	p2 = code_getdi(0);		// �p�����[�^1:���l
	mem_obj= clCreateFromGLTexture2D(context[clsetdev],CL_MEM_READ_WRITE,GL_TEXTURE_2D,0,p2,0);
	code_setva( pval, aptr, HSPVAR_FLAG_INT, &mem_obj);
}


static void newcmd47( void )
{
	p1 = code_getdi(0);		// �p�����[�^1:���l
	p2 = code_getdi(0);		// �p�����[�^1:���l
	GLuint *p2tr;
	memcpy(&p2tr,&p2,4);
	glDeleteTextures(1,p2tr);
}



static void newcmd48( void )
{
	PVal *pval1;
	APTR aptr1;	//�z��ϐ��̎擾
	aptr1 = code_getva( &pval1 );//	���͕ϐ��̌^�Ǝ��̂̃|�C���^���擾
	HspVarProc *phvp1;
	void *ptr1;
	phvp1 = exinfo->HspFunc_getproc( pval1->flag );	//�^����������HspVarProc�\���̂ւ̃|�C���^
	ptr1  = phvp1->GetPtr( pval1 );					//�f�[�^�ipval1�j�̎��Ԃ�����擪�|�C���^���擾�B
	
	
	PVal *pval2;
	APTR aptr2;	//�z��ϐ��̎擾
	aptr2 = code_getva( &pval2 );//	���͕ϐ��̌^�Ǝ��̂̃|�C���^���擾
	HspVarProc *phvp2;
	void *ptr2;
	phvp2 = exinfo->HspFunc_getproc( pval2->flag );	//�^����������HspVarProc�\���̂ւ̃|�C���^
	ptr2  = phvp2->GetPtr( pval2 );					//�f�[�^�ipval1�j�̎��Ԃ�����擪�|�C���^���擾�B
	
	p3=code_getdi(0);

	char *dmptr1=(char*)ptr1;
	char *dmptr2=(char*)ptr2;

	for(int i=0;i<p3;i+=3){
		dmptr2[i+2]=dmptr1[i  ];
		dmptr2[i+1]=dmptr1[i+1];
		dmptr2[i  ]=dmptr1[i+2];
	}
}
static void newcmd49( void )
{
	PVal *pval1;
	APTR aptr1;	//�z��ϐ��̎擾
	aptr1 = code_getva( &pval1 );//	���͕ϐ��̌^�Ǝ��̂̃|�C���^���擾
	HspVarProc *phvp1;
	void *ptr1;
	phvp1 = exinfo->HspFunc_getproc( pval1->flag );	//�^����������HspVarProc�\���̂ւ̃|�C���^
	ptr1  = phvp1->GetPtr( pval1 );					//�f�[�^�ipval1�j�̎��Ԃ�����擪�|�C���^���擾�B
		
	PVal *pval2;
	APTR aptr2;	//�z��ϐ��̎擾
	aptr2 = code_getva( &pval2 );//	���͕ϐ��̌^�Ǝ��̂̃|�C���^���擾
	HspVarProc *phvp2;
	void *ptr2;
	phvp2 = exinfo->HspFunc_getproc( pval2->flag );	//�^����������HspVarProc�\���̂ւ̃|�C���^
	ptr2  = phvp2->GetPtr( pval2 );					//�f�[�^�ipval1�j�̎��Ԃ�����擪�|�C���^���擾�B
	
	p3=code_getdi(0)/4;

	char *dmptr1=(char*)ptr1;
	char *dmptr2=(char*)ptr2;
	int idx1=0;
	int idx2=0;

	for(int i=0;i<p3;i++){
		idx1=i*3;
		idx2=i*4;
		dmptr2[idx1  ]=dmptr1[idx2  ];
		dmptr2[idx1+1]=dmptr1[idx2+1];
		dmptr2[idx1+2]=dmptr1[idx2+2];
	}
}
static void newcmd50( void )
{
	PVal *pval1;
	APTR aptr1;	//�z��ϐ��̎擾
	aptr1 = code_getva( &pval1 );//	���͕ϐ��̌^�Ǝ��̂̃|�C���^���擾
	HspVarProc *phvp1;
	void *ptr1;
	phvp1 = exinfo->HspFunc_getproc( pval1->flag );	//�^����������HspVarProc�\���̂ւ̃|�C���^
	ptr1  = phvp1->GetPtr( pval1 );					//�f�[�^�ipval1�j�̎��Ԃ�����擪�|�C���^���擾�B
		
	PVal *pval2;
	APTR aptr2;	//�z��ϐ��̎擾
	aptr2 = code_getva( &pval2 );//	���͕ϐ��̌^�Ǝ��̂̃|�C���^���擾
	HspVarProc *phvp2;
	void *ptr2;
	phvp2 = exinfo->HspFunc_getproc( pval2->flag );	//�^����������HspVarProc�\���̂ւ̃|�C���^
	ptr2  = phvp2->GetPtr( pval2 );					//�f�[�^�ipval1�j�̎��Ԃ�����擪�|�C���^���擾�B
	
	p3=code_getdi(0)/3;

	
	int toumeiflg=code_getdi(0);
	char toumei_r=code_getdi(0);
	char toumei_g=code_getdi(0);
	char toumei_b=code_getdi(0);


	char *dmptr1=(char*)ptr1;
	char *dmptr2=(char*)ptr2;
	int idx1=0;
	int idx2=0;
	char tmpr=0;
	char tmpg=0;
	char tmpb=0;

	if (toumeiflg==0){
		for(int i=0;i<p3;i++){
			idx1=i*4;
			idx2=i*3;
			tmpr=dmptr1[idx2  ];
			tmpg=dmptr1[idx2+1];
			tmpb=dmptr1[idx2+2];
			dmptr2[idx1  ]=tmpr;
			dmptr2[idx1+1]=tmpg;
			dmptr2[idx1+2]=tmpb;
			dmptr2[idx1+3]=255;
		}
	}else{
		for(int i=0;i<p3;i++){
			idx1=i*4;
			idx2=i*3;
			tmpr=dmptr1[idx2  ];
			tmpg=dmptr1[idx2+1];
			tmpb=dmptr1[idx2+2];
			dmptr2[idx1  ]=tmpr;
			dmptr2[idx1+1]=tmpg;
			dmptr2[idx1+2]=tmpb;
			dmptr2[idx1+3]=255;
			if (toumei_r==tmpr){if (toumei_g==tmpg){if (toumei_b==tmpb){dmptr2[idx1+3]=0;}}}
		}
	}
}


/*------------------------------------------------------------*/

static int cmdfunc( int cmd )
{
	//		���s���� (���ߎ��s���ɌĂ΂�܂�)
	//
	code_next();							// ���̃R�[�h���擾(�ŏ��ɕK���K�v�ł�)

	switch( cmd ) {							// �T�u�R�}���h���Ƃ̕���

	case 0x00:								// newcmd1 ������CLini  :newcmd1 int p1:::p1�ɂ̓f�o�C�X������������
		newcmd1();
		break;

	case 0x01:								// newcmd2 ���擾    :newcmd1 int p1,str p2:::p1�̓f�o�C�Xid�Ap2�ɂ͏�񂪑�������
		newcmd2();
		break;

	case 0x02:								// newcmd3 �I����    :newcmd3:::
		newcmd3();
		break;
		
	case 0x03:								// newcmd4 //�v���O�����r���h    :newcmd4 int p1,str p2,int p3:::p1�̓f�o�C�Xid�Ap2�̓J�[�l��.cl�\�[�X���Ap3�ɂ̓v���O����id����������
		newcmd4();
		break;

	case 0x04:								// newcmd5 //�v���O��������J�[�l���o�^    :newcmd5 int p1,int p2,int p3:::p1�v���O����id�Ap2�̓J�[�l���֐����Ap3�ɂ̓J�[�l��id����������
		newcmd5();
		break;

	case 0x05:								// newcmd6 //�Z�b�g�J�[�l��    :newcmd6 int p1,int p2,int p3,int p4:::p1�J�[�l��id�Ap2�͈����ʒu�Ap3�ɂ͎��ԁAp4�̓��[�J���������t���O
		newcmd6();
		break;

	case 0x06:								// newcmd7 //�������m��    :newcmd7 int p1,int p2,int p3:::p1�f�o�C�Xid�Ap2�ϐ�id����������Ap3��byte��
		newcmd7();
		break;

	case 0x07:								// newcmd8 //�z�X�g�f�o�C�X�]��    :newcmd8 int p1,int p2,array p3,int p4,int p5,int p6,int p7:::p1�f�o�C�Xid�Ap2�R�s�[��f�o�t�ϐ�id�A���ҁO��p3���ԁAp4�R�s�[�T�C�Y�Ap5�R�s�[��̃I�t�Z�b�g�Ap6�R�s�[���̃I�t�Z�b�g�Ap7�u���b�L���O���[�hoff
		newcmd8();
		break;

	case 0x08:								// newcmd9 //�f�o�C�X�z�X�g�]��    :newcmd9 int p1,int p2,array p3,int p4,int p5,int p6,int p7:::p1�f�o�C�Xid�Ap2�R�s�[���f�o�t�ϐ�id�A�R�s�[��p3���ԁAp4�R�s�[�T�C�Y�Ap5�R�s�[���̃I�t�Z�b�g�Ap6�R�s�[��̃I�t�Z�b�g�Ap7�u���b�L���O���[�hoff
		newcmd9();
		break;

	case 0x09:								// newcmd10 //�f�o�C�X�f�o�C�X�]��    :newcmd10 int p1,int p2,int p3,int p4,int p5,int p6:::p1�f�o�C�Xid�Ap2�R�s�[��f�o�t�ϐ�id�Ap3�R�s�[���f�o�t�ϐ�id�Ap4�R�s�[�T�C�Y�Ap5�R�s�[��̃I�t�Z�b�g�Ap6�R�s�[���̃I�t�Z�b�g
		newcmd10();
		break;

	case 0x0A:								// newcmd11 //�J�[�l�����s    :newcmd11 int p1,int p2,int p3,int p4,int p5:::p1�f�o�C�Xid�Ap2�J�[�l��id�Ap3work_dim�Ap4global_work_size�Ap5local_work_size
		newcmd11();
		break;

	case 0x0B:								// newcmd12 //clReleaseKernel(kernel)
		newcmd12();
		break;

	case 0x0C:								// newcmd13 //clReleaseProgram(program)
		newcmd13();
		break;

	case 0x0D:								// newcmd14 //clReleaseMemObject(a_mem_obj)
		newcmd14();
		break;

	case 0x0E:								// newcmd15 //fdim
		newcmd15();
		break;

	case 0x0F:								// newcmd16 //clsetdev
		newcmd16();
		break;

	case 0x10:								// newcmd17 //clWaitEvents
		newcmd17();
		break;

	case 0x11:								// newcmd18 // clDokrn1
		newcmd18();
		break;

	case 0x12:								// newcmd19 // clDokrn2
		newcmd19();
		break;

	case 0x13:								// newcmd20 // clDokrn3
		newcmd20();
		break;

	case 0x14:								// newcmd21 //
		newcmd21();
		break;

	case 0x15:								// newcmd22 //
		newcmd22();
		break;

	case 0x16:								// newcmd23 ////GL�����ݒ�
		newcmd23();
		break;
		
	case 0x17:								// newcmd24 //CL_err_mes_MODE
		newcmd24();
		break;

	case 0x18:								// newcmd25 //clCreateFromGLBuffer
		newcmd25();
		break;

	case 0x19:								// newcmd26 //clEnqueueAcquireGLObjects
		newcmd26();
		break;

	case 0x1A:								// newcmd27 //clEnqueueReleaseGLObjects
		newcmd27();
		break;

	case 0x1B:								// newcmd28 //glBindBuffer
		newcmd28();
		break;

	case 0x1C:								// newcmd29 //glBufferData
		newcmd29();
		break;

	case 0x1D:								// newcmd30 //glDeleteBuffers
		newcmd30();
		break;

	case 0x1E:								// newcmd31 //glGenBuffers
		newcmd31();
		break;

	case 0x1F:								// newcmd31 //glGenBuffers
		newcmd32();
		break;

	case 0x20:								// newcmd31 //glGenBuffers
		newcmd33();
		break;

	case 0x21:								// newcmd31 //glGenBuffers
		newcmd34();
		break;

	case 0x22:								// newcmd31 //glGenBuffers
		newcmd35();
		break;

	case 0x23:								// newcmd31 //glGenBuffers
		newcmd36();
		break;

	case 0x24:								// newcmd31 //glGenBuffers
		newcmd37();
		break;

	case 0x25:								// newcmd31 //glGenBuffers
		newcmd38();
		break;

	case 0x26:								// newcmd31 //glGenBuffers
		newcmd39();
		break;

	case 0x27:								// newcmd31 //glGenBuffers
		newcmd40();
		break;

	case 0x28:								// newcmd31 //glGenBuffers
		newcmd41();
		break;

	case 0x29:								// newcmd31 //glGenBuffers
		newcmd42();
		break;

	case 0x2A:								// newcmd31 //glGenBuffers
		newcmd43();
		break;

	case 0x2B:								// newcmd31 //glGenBuffers
		newcmd44();
		break;

	case 0x2C:								// newcmd31 //glbuffersubdata
		newcmd45();
		break;

	case 0x2D:								// newcmd31 //clCreateFromGLTexture2D
		newcmd46();
		break;

	case 0x2E:								// newcmd31 //gldeleteTexture
		newcmd47();
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
	case 0x32:								// newcmd51 //clGetDeviceInfo
		newcmd51();
		break;
	case 0x33:								//HCLDoKrn1_sub
		newcmd52();
		break;
	case 0x34:								//hspclnewcmd4
		newcmd53();
		break;
	case 0x35:								//put i
		newcmd54();
		break;
	case 0x36:								//put d 
		newcmd55();
		break;
	case 0x37:								//put f
		newcmd56();
		break;
	case 0x38:								//put s
		newcmd57();
		break;
	case 0x39:								//HCLGetDev
		newcmd58();
		break;
	case 0x3A:								//HCLSetKrns
		newcmd59();
		break;
	case 0x3B:								//��
		newcmd60();
		break;
	case 0x3C:								//��
		newcmd61();
		break;
	case 0x3D:								//��
		newcmd62();
		break;
	case 0x3E:								//��
		newcmd63();
		break;
	case 0x3F:								//��
		newcmd64();
		break;
	case 0x40:								//��
		newcmd65();
		break;
	case 0x41:								//��
		newcmd66();
		break;
	case 0x42:								//��
		newcmd67();
		break;
	case 0x43:								//��
		newcmd68();
		break;
	default:
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
	}
	return RUNMODE_RUN;
}










//----------------------------------------------------------------------------------------�����ȉ��͎����ō�����֐�--------------------------------
//----------------------------------------------------------------------------------------�����ȉ��͎����ō�����֐�--------------------------------
//----------------------------------------------------------------------------------------�����ȉ��͎����ō�����֐�--------------------------------
//----------------------------------------------------------------------------------------�����ȉ��͎����ō�����֐�--------------------------------
//----------------------------------------------------------------------------------------�����ȉ��͎����ō�����֐�--------------------------------
//----------------------------------------------------------------------------------------�����ȉ��͎����ō�����֐�--------------------------------
//----------------------------------------------------------------------------------------�����ȉ��͎����ō�����֐�--------------------------------
//----------------------------------------------------------------------------------------�����ȉ��͎����ō�����֐�--------------------------------
//----------------------------------------------------------------------------------------�����ȉ��͎����ō�����֐�--------------------------------
//----------------------------------------------------------------------------------------�����ȉ��͎����ō�����֐�--------------------------------
//----------------------------------------------------------------------------------------�����ȉ��͎����ō�����֐�--------------------------------
//----------------------------------------------------------------------------------------�����ȉ��͎����ō�����֐�--------------------------------
//----------------------------------------------------------------------------------------�����ȉ��͎����ō�����֐�--------------------------------

void gtflt(float *ff0)
{
	int chk = code_getprm();							// �p�����[�^�[���擾(�^�͖��Ȃ�)
	if ( chk<=PARAM_END ) {
		return;										// �p�����[�^�[�ȗ����̏���
	}
	int type = mpval->flag;							// �p�����[�^�[�̌^���擾
	switch( type ) {
	case 2:								// �p�����[�^�[�������񂾂�����
		{
		char *str = (char *)mpval->pt;
		*ff0=float(atof(str));
		break;
		}
	case 3:									// �p�����[�^�[��������������
		{
		double *ptr = (double *)mpval->pt;
		*ff0=float(*ptr);
		break;
		}
	case 4:									// �p�����[�^�[��������������
		{
		int *ptr = (int *)mpval->pt;
		*ff0=float(*ptr);
		break;
		}
	case 8:									// �p�����[�^�[��float��������
		{
		float *ptr = (float *)mpval->pt;
		*ff0=float(*ptr);
		break;
		}
	default:
		puterror( HSPERR_TYPE_MISMATCH );			// �T�|�[�g���Ă��Ȃ��^�Ȃ�΃G���[
	}
}


void gtint(int *ii0)
{
	int chk = code_getprm();							// �p�����[�^�[���擾(�^�͖��Ȃ�)
	if ( chk<=PARAM_END ) {
		return;										// �p�����[�^�[�ȗ����̏���
	}
	int type = mpval->flag;							// �p�����[�^�[�̌^���擾
	switch( type ) {
	case 2:								// �p�����[�^�[�������񂾂�����
		{
		char *str = (char *)mpval->pt;
		*ii0=atoi(str);
		break;
		}
	case 3:									// �p�����[�^�[��������������
		{
		double *ptr = (double *)mpval->pt;
		*ii0=int(*ptr);
		break;
		}
	case 4:									// �p�����[�^�[��������������
		{
		int *ptr = (int *)mpval->pt;
		*ii0=int(*ptr);
		break;
		}
	case 8:									// �p�����[�^�[��float��������
		{
		float *ptr = (float *)mpval->pt;
		*ii0=int(*ptr);
		break;
		}
	default:
		puterror( HSPERR_TYPE_MISMATCH );			// �T�|�[�g���Ă��Ȃ��^�Ȃ�΃G���[
	}
}


void gtdbl(double *dd0)
{
	int chk = code_getprm();							// �p�����[�^�[���擾(�^�͖��Ȃ�)
	if ( chk<=PARAM_END ) {
		return;										// �p�����[�^�[�ȗ����̏���
	}
	int type = mpval->flag;							// �p�����[�^�[�̌^���擾
	switch( type ) {
	case 2:								// �p�����[�^�[�������񂾂�����
		{
		char *str = (char *)mpval->pt;
		*dd0=atof(str);
		break;
		}
	case 3:									// �p�����[�^�[��������������
		{
		double *ptr = (double *)mpval->pt;
		*dd0=double(*ptr);
		break;
		}
	case 4:									// �p�����[�^�[��������������
		{
		int *ptr = (int *)mpval->pt;
		*dd0=double(*ptr);
		break;
		}
	case 8:									// �p�����[�^�[��float��������
		{
		float *ptr = (float *)mpval->pt;
		*dd0=double(*ptr);
		break;
		}
	default:
		puterror( HSPERR_TYPE_MISMATCH );			// �T�|�[�g���Ă��Ȃ��^�Ȃ�΃G���[
	}
}



void retmeserr(cl_int ret)
{
	switch( ret ) {							//����
	case CL_INVALID_PROGRAM_EXECUTABLE:
		MessageBox(NULL, "�f�o�C�X��Ŏ��s�\�ȁA����Ƀr���h���ꂽ�v���O�������������܂���", "�G���[", 0);
		newcmd3();
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
		break;
	case CL_INVALID_COMMAND_QUEUE:
		MessageBox(NULL, "�f�o�C�Xid�������ȃf�o�C�X�ɂȂ��Ă��܂�", "�G���[", 0);
		newcmd3();
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
		break;
	case CL_INVALID_KERNEL:
		MessageBox(NULL, "�J�[�l��id���Ԉ���Ă��܂�", "�G���[", 0);
		newcmd3();
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
		break;
	case CL_INVALID_CONTEXT:
		MessageBox(NULL, "�J�[�l��id���Ⴄ�f�o�C�Xid�œo�^����Ă��܂��A���邢�͑������� event_wait_list ���̃C�x���g�Ɗ֘A�t����ꂽ�f�o�C�X�������łȂ�", "�G���[", 0);
		newcmd3();
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
		break;
	case CL_INVALID_KERNEL_ARGS:
		MessageBox(NULL, "�J�[�l���������w�肳��Ă��܂���", "�G���[", 0);
		newcmd3();
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
		break;
	case CL_INVALID_GLOBAL_WORK_SIZE:
		MessageBox(NULL, "global_work_size �� NULL �ł��B���邢�́Aglobal_work_size�̔z��̂ǂꂩ��0�ł��B�������̓J�[�l�������s����f�o�C�X��ł�global_work_size������l�𒴂��Ă���", "�G���[", 0);
		newcmd3();
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
		break;
	case CL_INVALID_GLOBAL_OFFSET:
		MessageBox(NULL, "CL_INVALID_GLOBAL_OFFSET - global_work_offset �� NULL �łȂ�", "�G���[", 0);
		newcmd3();
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
		break;
	case CL_INVALID_WORK_DIMENSION:
		MessageBox(NULL, "work_dim ���K�؂Ȓl�łȂ�", "�G���[", 0);
		newcmd3();
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
		break;
	case CL_INVALID_WORK_GROUP_SIZE:
		MessageBox(NULL, "global_work_size��local_work_size �Ő����ł��Ȃ��A�܂���local_work_size[0]*local_work_size[1]*local_work_size[2]���A��̃��[�N�O���[�v���̃��[�N�A�C�e�����̍ő�l�𒴂���", "�G���[", 0);
		newcmd3();
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
		break;
	case CL_INVALID_WORK_ITEM_SIZE:
		MessageBox(NULL, "local_work_size[0], ... local_work_size[work_dim - 1] �Ŏw�肵�����[�N�A�C�e�������Ή����� CL_DEVICE_MAX_WORK_ITEM_SIZES[0], ... CL_DEVICE_MAX_WORK_ITEM_SIZES[work_dim - 1] �̒l�����Ă���A�܂���0���w�肵��", "�G���[", 0);
		newcmd3();
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
		break;
	case CL_MEM_OBJECT_ALLOCATION_FAILURE:
		MessageBox(NULL, "kernel �̈����Ɏw�肳�ꂽ�o�b�t�@/�C���[�W�I�u�W�F�N�g�Ɋ֘A�t����ꂽ�f�[�^�ۑ��̂��߂̃������̈�̊m�ۂɎ��s����", "�G���[", 0);
		newcmd3();
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
		break;
	case CL_INVALID_EVENT_WAIT_LIST:
		MessageBox(NULL, "event_wait_list �� NULL �� num_events_in_wait_list �� 0 ���傫���Ƃ��B���邢�� event_wait_list �� NULL �łȂ� num_events_in_wait_list �� 0 �̂Ƃ��B���邢�� event_wait_list ���̃C�x���g�I�u�W�F�N�g���L���Ȃ��̂łȂ�", "�G���[", 0);
		newcmd3();
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
		break;
	case CL_OUT_OF_RESOURCES:
		MessageBox(NULL, "�f�o�C�X��ł̃��\�[�X�m�ۂɎ��s����", "�G���[", 0);
		newcmd3();
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
		break;
	case CL_OUT_OF_HOST_MEMORY:
		MessageBox(NULL, "�z�X�g��ł̃��\�[�X�m�ۂɎ��s����", "�G���[", 0);
		newcmd3();
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
		break;
	}
	//��̂ǂ�ł��Ȃ����
	MessageBox(NULL, "�����s���̃G���[�ł�", "�G���[", 0);
	newcmd3();puterror( HSPERR_UNSUPPORTED_FUNCTION );
}




void retmeserr2(cl_int ret)
{
		switch(ret ) {							//����
	case CL_INVALID_COMMAND_QUEUE:
		MessageBox(NULL, "command_queue is not a valid command-queue", "�G���[", 0);
		newcmd3();
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
		break;
	case CL_INVALID_CONTEXT:
		MessageBox(NULL, "�������I�u�W�F�N�g���ʂ̃f�o�C�X�ō쐬���ꂽ�\��������܂�", "�G���[", 0);
		newcmd3();
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
		break;
	case CL_INVALID_MEM_OBJECT:
		MessageBox(NULL, "�������I�u�W�F�N�g�̎��̂�����܂���B�������I�u�W�F�N�g���ʂ̃f�o�C�X�ō쐬���ꂽ�\��������܂��B", "�G���[", 0);
		newcmd3();
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
		break;
	case CL_INVALID_VALUE:
		MessageBox(NULL, "�A�h���X�A�N�Z�X�ᔽ�ł��B�ǂݍ��ݗ̈悪�͂ݏo���Ă܂��B", "�G���[", 0);
		newcmd3();
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
		break;
	case CL_MEM_OBJECT_ALLOCATION_FAILURE:
		MessageBox(NULL, "data store �̂��߂�allocate memory����̂����s���܂���", "�G���[", 0);
		newcmd3();
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
		break;
	case CL_OUT_OF_RESOURCES:
		MessageBox(NULL, "�f�o�C�X(GPU)��ł̃��\�[�X�m�ۂɎ��s����", "�G���[", 0);
		newcmd3();
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
		break;
	case CL_OUT_OF_HOST_MEMORY:
		MessageBox(NULL, "�z�X�g(CPU)��ł̃��\�[�X�m�ۂɎ��s����", "�G���[", 0);
		newcmd3();
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
		break;
	}
	//��̂ǂ�ł��Ȃ����
	MessageBox(NULL, "�����s���̃G���[�ł�", "�G���[", 0);
	newcmd3();puterror( HSPERR_UNSUPPORTED_FUNCTION );

}



//----------------------------------------------------------------------------------------�����ȉ���HSP�̃T���v���R�s�[--------------------------------
//----------------------------------------------------------------------------------------�����ȉ���HSP�̃T���v���R�s�[--------------------------------
//----------------------------------------------------------------------------------------�����ȉ���HSP�̃T���v���R�s�[--------------------------------
//----------------------------------------------------------------------------------------�����ȉ���HSP�̃T���v���R�s�[--------------------------------
//----------------------------------------------------------------------------------------�����ȉ���HSP�̃T���v���R�s�[--------------------------------
//----------------------------------------------------------------------------------------�����ȉ���HSP�̃T���v���R�s�[--------------------------------
//----------------------------------------------------------------------------------------�����ȉ���HSP�̃T���v���R�s�[--------------------------------
//----------------------------------------------------------------------------------------�����ȉ���HSP�̃T���v���R�s�[--------------------------------
//----------------------------------------------------------------------------------------�����ȉ���HSP�̃T���v���R�s�[--------------------------------
//----------------------------------------------------------------------------------------�����ȉ���HSP�̃T���v���R�s�[--------------------------------
//----------------------------------------------------------------------------------------�����ȉ���HSP�̃T���v���R�s�[--------------------------------
//----------------------------------------------------------------------------------------�����ȉ���HSP�̃T���v���R�s�[--------------------------------
//----------------------------------------------------------------------------------------�����ȉ���HSP�̃T���v���R�s�[--------------------------------
//----------------------------------------------------------------------------------------�����ȉ���HSP�̃T���v���R�s�[--------------------------------
//----------------------------------------------------------------------------------------�����ȉ���HSP�̃T���v���R�s�[--------------------------------

static double ref_fval;						// �Ԓl�̂��߂̕ϐ�
static double dp1;

static void *reffunc( int *type_res, int cmd )
{
	//		�֐��E�V�X�e���ϐ��̎��s���� (�l�̎Q�Ǝ��ɌĂ΂�܂�)
	//
	//			'('�Ŏn�܂邩�𒲂ׂ�
	//
	if ( *type != TYPE_MARK ) puterror( HSPERR_INVALID_FUNCPARAM );
	if ( *val != '(' ) puterror( HSPERR_INVALID_FUNCPARAM );
	code_next();
	float p9;
	int  p10;
	double p8;
	char p7;
	cl_int ret;
	int p6=0;
	int p5=0;
	memcpy(&ref_fval,&p6,4);
	memcpy(&ref_fval,&p5,4);

	switch( cmd ) {							// �T�u�R�}���h���Ƃ̕���

	case 0xFF:								// float�֐�
		ref_fval=0.0;
		dp1 = code_getd();					// �����l���擾(�f�t�H���g�Ȃ�)
		p9=(float)dp1;				// �Ԓl��ݒ�
		memcpy(&ref_fval,&p9,4);
		*type_res = HspVarFloat_typeid();		// �Ԓl�̃^�C�v���w�肷��
		break;

	case 0xFE://hclget_d
		p2 = code_getdi(0);		// �J�[�l��
		p3 = code_getdi(0);		// �C���f�b�N�X
		//p7���g
		ref_fval=0.0;
		ret=clEnqueueReadBuffer(command_queue[clsetdev], (cl_mem)p2,1,p3*sizeof(double),sizeof(double),&p8, 0, NULL,    NULL    );
		if (ret!=CL_SUCCESS){retmeserr2(ret);}
		memcpy(&ref_fval,&p8,sizeof(double));
		*type_res = HSPVAR_FLAG_DOUBLE;			// �Ԓl�̃^�C�v�𐮐��Ɏw�肷��
		break;
		
	case 0xFD://hclget_i
		p2 = code_getdi(0);		// �J�[�l��
		p3 = code_getdi(0);		// �C���f�b�N�X
		//p7���g
		ref_fval=0;
		ret=clEnqueueReadBuffer(command_queue[clsetdev], (cl_mem)p2,1,p3*sizeof(int),sizeof(int),&p10, 0, NULL,    NULL    );
		if (ret!=CL_SUCCESS){retmeserr2(ret);}
		memcpy(&ref_fval,&p10,sizeof(int));
		*type_res = HSPVAR_FLAG_INT;			// �Ԓl�̃^�C�v�𐮐��Ɏw�肷��
		break;

	case 0xFC://hclget_f
		p2 = code_getdi(0);		// �J�[�l��
		p3 = code_getdi(0);		// �C���f�b�N�X
		//p7���g
		ref_fval=0.0;
		ret=clEnqueueReadBuffer(command_queue[clsetdev], (cl_mem)p2,1,p3*sizeof(float),sizeof(float),&p9, 0, NULL,    NULL    );
		if (ret!=CL_SUCCESS){retmeserr2(ret);}
		memcpy(&ref_fval,&p9,sizeof(float));
		*type_res = 8;			// �Ԓl�̃^�C�v�𐮐��Ɏw�肷��
		break;

	case 0xFB://hclget_s
		p2 = code_getdi(0);		// �J�[�l��
		p3 = code_getdi(0);		// �C���f�b�N�X
		//p7���g
		p7=0;
		ref_fval=0;
		ret=clEnqueueReadBuffer(command_queue[clsetdev], (cl_mem)p2,1,p3*sizeof(char),sizeof(char),&p7, 0, NULL,    NULL    );
		if (ret!=CL_SUCCESS){retmeserr2(ret);}
		memcpy(&ref_fval,&p7,sizeof(char));
		*type_res = HSPVAR_FLAG_INT;			// �Ԓl�̃^�C�v�𐮐��Ɏw�肷��
		break;

	default:
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
	}

	//			'('�ŏI��邩�𒲂ׂ�
	//
	if ( *type != TYPE_MARK ) puterror( HSPERR_INVALID_FUNCPARAM );
	if ( *val != ')' ) puterror( HSPERR_INVALID_FUNCPARAM );
	code_next();

	return (void *)&ref_fval;
}


static int termfunc( int option )
{
	//		�I������ (�A�v���P�[�V�����I�����ɌĂ΂�܂�)
	//
	return 0;
}


int WINAPI DllMain (HINSTANCE hInstance, DWORD fdwReason, PVOID pvReserved)
{
	//		DLL�G���g���[ (��������K�v�͂���܂���)
	//
	return TRUE;
}

EXPORT void WINAPI hsp3cmdinit( HSP3TYPEINFO *info )
{
	//		�v���O�C�������� (���s�E�I��������o�^���܂�)
	//
	hsp3sdk_init( info );		// SDK�̏�����(�ŏ��ɍs�Ȃ��ĉ�����)
	
	info->cmdfunc = cmdfunc;		// ���s�֐�(cmdfunc)�̓o�^
	info->reffunc = reffunc;		// �Q�Ɗ֐�(reffunc)�̓o�^
	info->termfunc = termfunc;		// �I���֐�(termfunc)�̓o�^

	registvar( -1, HspVarFloat_Init );		// �V�����^�̒ǉ�
}