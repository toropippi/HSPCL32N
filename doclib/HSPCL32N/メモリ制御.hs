;------------------------
;   �������֘A
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
hspcl32n.as ���C���N���[�h���Ă��������B

%type
32bit�����^�C��GPGPU�p�v���O�C��

%group
OpenCL����������

%port
Win

;-------- ref --------

%index
clCreateBuffer
VRAM�쐬

%prm
(int p1)
int p1 : �m�ۂ���byte��	[in]

%inst
clSetDevice�Ŏw�肳�ꂽ�O���t�B�b�N�{�[�h�Ȃǂ̃f�o�C�X��Ƀ��������m�ۂ��܂��B
cl_mem_id���Ԃ�܂��B
�����32bit int�^�ł��B
���GDDR5,6�Ȃǂ̃������̂���O���{��ɁA�w�肵���T�C�Y�̃��������m�ۂ���邱�ƂɂȂ�܂��B
clReleaseBuffer�ŉ�����邱�Ƃ��ł��܂��B

����cl_mem_id������O�Ƀ��X�g����ƁAGPU���̃������ɃA�N�Z�X�ł��Ȃ����ƂɂȂ�A���������[�N�ɋ߂���ԂɂȂ��Ă��܂��܂��B
cl_mem_id��ϐ��ŕێ����Ă���ꍇ�㏑���ɂ͂����Ӊ������B


%href
clSetDevice
clReleaseBuffer
;--------

%index
clReleaseBuffer
VRAM�j��

%prm
int p1
int p1 : cl_mem_id			[in]
%inst
�f�o�C�X��̃�������������܂��B

%href
clCreateBuffer
;--------

%index
clWriteBuffer
HSP�z�����VRAM�ɏ���

%prm
int p1,array p2,int p3,int p4,int p5,int p6,int p7
int p1 : �R�s�[��cl_mem_id		[in]
array p2:�R�s�[���z��ϐ�			[in]
int p3 : �R�s�[�T�C�Ybyte,�ȗ���		[in]
int p4 : �R�s�[��̃I�t�Z�b�g,�ȗ���		[in]
int p5 : �R�s�[���̃I�t�Z�b�g,�ȗ���		[in]
int p6 : �u���b�L���O���[�hoff,�ȗ���		[in]
int p7 : event_id,�ȗ���			[in]

%inst
�z�X�g(CPU)����clSetDevice�Ŏw�肵���f�o�C�X(GPU)���Ƀf�[�^��]�����܂��B
p3�͏ȗ����A�R�s�[��ƃR�s�[���̔z��̂��������������̗p����܂��B
p4,p5�͏ȗ���0�ł��B
p3,p4,p5�̒P�ʂ�byte�ł��B
p6�̃u���b�L���O���[�h��0���w�肷���off�ɂȂ�A�]�����������Ȃ������Ɏ��̖��߂Ɉڂ�܂��B
p6�͏ȗ����f�t�H���g��1�ł��B�܂�u���b�L���O���[�h��on�ɂȂ��Ă���A�]�����I���܂ł��̖��߂̎��s���I���܂���B


%href
clCreateBuffer
clReadBuffer
clFillBuffer
;--------

%index
clReadBuffer
VRAM����HSP�z����ɓǍ�

%prm
int p1,array p2,int p3,int p4,int p5,int p6,int p7
int p1 : �R�s�[��cl_mem_id		[in]
array p2:�R�s�[��z��ϐ�			[out]
int p3 : �R�s�[�T�C�Ybyte,�ȗ���		[in]
int p4 : �R�s�[���̃I�t�Z�b�g,�ȗ���		[in]
int p5 : �R�s�[��̃I�t�Z�b�g,�ȗ���		[in]
int p6 : �u���b�L���O���[�hoff,�ȗ���		[in]
int p7 : event_id,�ȗ���			[in]

%inst
clSetDevice�Ŏw�肵���f�o�C�X(GPU)����z�X�g(CPU)���Ƀf�[�^��]�����܂��B
p3�͏ȗ����A�R�s�[��ƃR�s�[���̔z��̂��������������̗p����܂��B
p4,p5�͏ȗ���0�ł��B
p3,p4,p5�̒P�ʂ�byte�ł��B
p6�̃u���b�L���O���[�h��0���w�肷���off�ɂȂ�A�]�����������Ȃ������Ɏ��̖��߂Ɉڂ�܂��B
p6�͏ȗ����f�t�H���g��1�ł��B�܂�u���b�L���O���[�h��on�ɂȂ��Ă���A�]�����I���܂ł��̖��߂̎��s���I���܂���B


%href
clCreateBuffer
clWriteBuffer
clFillBuffer
;--------

%index
clCopyBuffer
VRAM���m�R�s�[

%prm
int p1,int p2,int p3,int p4,int p5,int p6
int p1 : �R�s�[��cl_mem_id		[in]
int p2 : �R�s�[��cl_mem_id		[in]
int p3 : �R�s�[�T�C�Ybyte,�ȗ���		[in]
int p4 : �R�s�[��̃I�t�Z�b�g,�ȗ���		[in]
int p5 : �R�s�[���̃I�t�Z�b�g,�ȗ���		[in]
int p6 : event_id,�ȗ���			[in]

%inst
clSetDevice�Ŏw�肵���f�o�C�X��̃������ԂŃR�s�[�����܂��B
p3�͏ȗ����A�R�s�[��ƃR�s�[���̔z��̂��������������̗p����܂��B
p4,p5�͏ȗ���0�ł��B
p3,p4,p5�̒P�ʂ�byte�ł��B


%href
clCreateBuffer
clWriteBuffer
clReadBuffer

;--------
%index
clCreateBufferFrom
VRAM�쐬(HSP�z��ϐ�����)

%prm
(array p1)
array p1 : HSP���̔z��ϐ�		[in]

%inst
dim���߂ȂǂŊm�ۂ���HSP�̔z��ϐ����A���̂܂܃R�s�[����VRAM�̍쐬�����܂��B
cl_mem_id���Ԃ�܂��B
�����32bit int�^�ł��B

%href
clCreateBuffer
;--------

%index
clWriteBuffer_NonBlocking
HSP�z�����VRAM�ɏ����A�����m���u���b�L���O���[�h

%prm
int p1,array p2,int p3,int p4,int p5,int p6,int p7
int p1 : �R�s�[��cl_mem_id		[in]
array p2:�R�s�[���z��ϐ�			[in]
int p3 : �R�s�[�T�C�Ybyte,�ȗ���		[in]
int p4 : �R�s�[��̃I�t�Z�b�g,�ȗ���		[in]
int p5 : �R�s�[���̃I�t�Z�b�g,�ȗ���		[in]
int p6 : �u���b�L���O���[�hoff,�ȗ���		[in]
int p7 : event_id,�ȗ���			[in]

%inst

�z�X�g(CPU)����clSetDevice�Ŏw�肵���f�o�C�X(GPU)���Ƀf�[�^��]�����܂��B

��clWriteBuffer�Ƃ̈Ⴂ
clWriteBuffer�̃m���u���b�L���O���[�h�w���NVIDIA GPU�ł͖����Ȃ悤�ŁA�K���u���b�L���O��on�ɂȂ��Ă��܂��܂��B
�]�����I���܂ő҂������̂��ǂ����Ă�����ꍇ�A����clWriteBuffer_NonBlocking���߂��g�p���Ă��������B
�v���O�C�������ł�
std::thread
�ŕʃX���b�h�𗧂��グ�A���̒���clEnqueueWriteBuffer�����s���Ă��܂��B
���̂Ƃ��A�ʃX���b�h�����ǂ̃^�C�~���O�Ŏ��s����邩�͂킩��Ȃ����Ƃɒ��ӂ��Ă��������B

�Ⴆ��clWriteBuffer_NonBlocking�̓]�����I���܂ő҂������ꍇclFinish�����s���Ă��AclEnqueueWriteBuffer���̂̎��s���܂��̉\��������܂��B
���̏ꍇ�]�����I���O��clFinish�̎��s���������Ă��܂��Ƃ������Ƃ��N����܂��B

���̂��߂�clGet_NonBlocking_Status�Ƃ������߂�����A���ꂪ0�ɂȂ��Ă���Ίm����clWriteBuffer_NonBlocking�̏������I����Ă��邱�Ƃ��ۏ؂���܂��B


%href
clWriteBuffer
clReadBuffer_NonBlocking
clGet_NonBlocking_Status
;--------

%index
clReadBuffer_NonBlocking
VRAM����HSP�z����ɓǍ��A�����m���u���b�L���O���[�h

%prm
int p1,array p2,int p3,int p4,int p5,int p6,int p7
int p1 : �R�s�[��cl_mem_id		[in]
array p2:�R�s�[��z��ϐ�			[out]
int p3 : �R�s�[�T�C�Ybyte,�ȗ���		[in]
int p4 : �R�s�[���̃I�t�Z�b�g,�ȗ���		[in]
int p5 : �R�s�[��̃I�t�Z�b�g,�ȗ���		[in]
int p6 : �u���b�L���O���[�hoff,�ȗ���		[in]
int p7 : event_id,�ȗ���			[in]

%inst

clSetDevice�Ŏw�肵���f�o�C�X(GPU)����z�X�g(CPU)���Ƀf�[�^��]�����܂��B

��clReadBuffer�Ƃ̈Ⴂ
clReadBuffer�̃m���u���b�L���O���[�h�w���NVIDIA GPU�ł͖����Ȃ悤�ŁA�K���u���b�L���O��on�ɂȂ��Ă��܂��܂��B
�]�����I���܂ő҂������̂��ǂ����Ă�����ꍇ�A����clReadBuffer_NonBlocking���߂��g�p���Ă��������B
�v���O�C�������ł�
std::thread
�ŕʃX���b�h�𗧂��グ�A���̒���clEnqueueReadBuffer�����s���Ă��܂��B
���̂Ƃ��A�ʃX���b�h�����ǂ̃^�C�~���O�Ŏ��s����邩�͂킩��Ȃ����Ƃɒ��ӂ��Ă��������B

�Ⴆ��clReadBuffer_NonBlocking�̓]�����I���܂ő҂������ꍇclFinish�����s���Ă��AclEnqueueReadBuffer���̂̎��s���܂��̉\��������܂��B
���̏ꍇ�]�����I���O��clFinish�̎��s���������Ă��܂��Ƃ������Ƃ��N����܂��B

���̂��߂�clGet_NonBlocking_Status�Ƃ������߂�����A���ꂪ0�ɂȂ��Ă���Ίm����clReadBuffer_NonBlocking�̏������I����Ă��邱�Ƃ��ۏ؂���܂��B

%href
clReadBuffer
clWriteBuffer_NonBlocking
clGet_NonBlocking_Status
;--------


%index
clGet_NonBlocking_Status
NonBlocking���s�󋵎擾

%prm
()

%inst

clReadBuffer_NonBlocking��clWriteBuffer_NonBlocking�̎��s�󋵂��ǂ��Ȃ��Ă��邩���擾���܂��B
�ȍ~����2��NonBlocking���߂ƌ����܂��B
�܂�NonBlocking���߂��Ăяo�������Ƃ̓v���O�C��������thread_start�ϐ����C���N�������g����܂��B
���̌�ʃX���b�h�������オ��ANonBlocking���ߓ�����clEnqueueReadBuffer,clEnqueueWriteBuffer�����s����A���̖��߂�ʂ�߂������thread_start�ϐ����f�N�������g����܂��B
������clEnqueueReadBuffer,clEnqueueWriteBuffer���͓̂]�����L���[�ɓ���邾���̖��߂ł��邱�Ƃɒ��ӂ��K�v�ŁA(�u���b�L���O���[�hoff)����(NVIDIA��GPU�łȂ�)�ꍇ�ɓ]�����I���O��thread_start���f�N�������g����Ă��邱�Ƃ����邱�Ƃ��l�����Ă��������B

����clGet_NonBlocking_Status���߂�thread_start�̐��l���擾���閽�߂ł��B

NVIDIA GPU�̏ꍇ�ANonBlocking���߂ɂ��f�[�^�]����������Ɏg�����Ƃ��ł��܂��B
�܂�NonBlocking���߂�event id���w�肵���ꍇ�A�v���O�C��������clEnqueueReadBuffer,clEnqueueWriteBuffer�̍s��ʂ肷���Ȃ���event���̎擾�ł��Ȃ�����(�G���[�ɂȂ�)�Aevent�擾�\����ɂ��g�����Ƃ��ł��܂��B

%href
clWriteBuffer_NonBlocking
clReadBuffer_NonBlocking
;--------

%index
clReadIndex_i32
VRAM����int�^��1�v�f�ǂݍ���

%prm
(int p1,int p2)

int p1 : cl_mem_id		[in]
int p2 : �z��̗v�f(index)		[in]

%inst
GPU��VRAM���璼��1�̒l���Ƃ肾���Ԃ��܂��B
p1��CL mem obj id
p2�͓ǂݏo���C���f�b�N�X���w�肵�ĉ������B
�u���b�L���O���[�h�̓I��(�]�������܂ő҂�)�ł��B

VRAM�̓��e��int�^4�̔z��ϐ��@(100,400,500,700)
�������ꍇ
clReadIndex_i32(memid,3)�@�́@700
��Ԃ��܂��B

%href
clReadIndex_dp
clReadIndex_fp
clReadIndex_i64
;--------

%index
clReadIndex_i64
VRAM����64bit int�^��1�v�f�ǂݍ���

%prm
(int p1,int p2)

int p1 : cl_mem_id		[in]
int p2 : �z��̗v�f(index)		[in]

%inst
GPU��VRAM���璼��1�̒l���Ƃ肾���Ԃ��܂��B
p1��CL mem obj id
p2�͓ǂݏo���C���f�b�N�X���w�肵�ĉ������B
�u���b�L���O���[�h�̓I��(�]�������܂ő҂�)�ł��B

VRAM�̓��e��64bit int�^4�̔z��ϐ��@(10000000000,40000000000,50000000000,70000000000)
�������ꍇ
clReadIndex_i64(memid,3)�@�́@70000000000
��Ԃ��܂��B

%href
clReadIndex_i32
clReadIndex_dp
clReadIndex_fp
;--------

%index
clReadIndex_dp
VRAM����double�^��1�v�f�ǂݍ���

%prm
(int p1,int p2)

int p1 : cl_mem_id		[in]
int p2 : �z��̗v�f(index)		[in]

%inst
GPU��VRAM���璼��1�̒l���Ƃ肾���Ԃ��܂��B
p1��CL mem obj id
p2�͓ǂݏo���C���f�b�N�X���w�肵�ĉ������B
�u���b�L���O���[�h�̓I��(�]�������܂ő҂�)�ł��B

VRAM�̓��e��double�^4�̔z��ϐ��@(100.0,400.0,500.0,700.0)
�������ꍇ
clReadIndex_dp(memid,3)�@�́@700.0
��Ԃ��܂��B

%href
clReadIndex_i32
clReadIndex_i64
clReadIndex_fp
;--------

%index
clReadIndex_fp
VRAM����float�^��1�v�f�ǂݍ���

%prm
(int p1,int p2)

int p1 : cl_mem_id		[in]
int p2 : �z��̗v�f(index)		[in]

%inst
GPU��VRAM���璼��1�̒l���Ƃ肾���Ԃ��܂��B
p1��CL mem obj id
p2�͓ǂݏo���C���f�b�N�X���w�肵�ĉ������B
�u���b�L���O���[�h�̓I��(�]�������܂ő҂�)�ł��B

VRAM�̓��e��float�^4�̔z��ϐ��@(100.0,400.0,500.0,700.0)
�������ꍇ
clReadIndex_dp(memid,3)�@�́@700.0
��Ԃ��܂��B

%href
clReadIndex_i32
clReadIndex_i64
clReadIndex_dp
;--------

%index
clWriteIndex_i32
VRAM��1�v�f��������

%prm
int p1,int p2,int p3

int p1 : cl_mem_id		[in]
int p2 : �z��̗v�f(index)		[in]
int p3 : ���e				[in]

%inst
p1�Ŏw�肵��VRAM(CL mem obj id)�̓��e��p3�l���������݂܂��B
�������݃C���f�b�N�X��p2�Ŏw�肵�܂��B
���̂Ƃ�VRAM��int�^�̔z��ϐ��Ƃ��čl���܂��B

�u���b�L���O���[�h�̓I��(�]�������܂ő҂�)�ł��B

%href
clWriteIndex_i64
clWriteIndex_dp
clWriteIndex_fp
;--------

%index
clWriteIndex_i64
VRAM��1�v�f��������

%prm
int p1,int p2,int64 p3

int p1 : cl_mem_id		[in]
int p2 : �z��̗v�f(index)		[in]
int64 p3 : ���e				[in]

%inst
p1�Ŏw�肵��VRAM(CL mem obj id)�̓��e��p3�l���������݂܂��B
�������݃C���f�b�N�X��p2�Ŏw�肵�܂��B
���̂Ƃ�VRAM��64bit int�^�̔z��ϐ��Ƃ��čl���܂��B

�u���b�L���O���[�h�̓I��(�]�������܂ő҂�)�ł��B

%href
clWriteIndex_i32
clWriteIndex_dp
clWriteIndex_fp
;--------

%index
clWriteIndex_dp
VRAM��1�v�f��������

%prm
int p1,int p2,double p3

int p1 : cl_mem_id		[in]
int p2 : �z��̗v�f(index)		[in]
double p3: ���e				[in]

%inst
p1�Ŏw�肵��VRAM(CL mem obj id)�̓��e��p3�l���������݂܂��B
�������݃C���f�b�N�X��p2�Ŏw�肵�܂��B
���̂Ƃ�VRAM��double�^�̔z��ϐ��Ƃ��čl���܂��B

�u���b�L���O���[�h�̓I��(�]�������܂ő҂�)�ł��B


%href
clWriteIndex_i32
clWriteIndex_i64
clWriteIndex_fp
;--------
%index
clWriteIndex_fp
VRAM��1�v�f��������

%prm
int p1,int p2,float p3

int p1 : cl_mem_id		[in]
int p2 : �z��̗v�f(index)		[in]
float p3: ���e				[in]

%inst
p1�Ŏw�肵��VRAM(CL mem obj id)�̓��e��p3�l���������݂܂��B
�������݃C���f�b�N�X��p2�Ŏw�肵�܂��B
���̂Ƃ�VRAM��float�^�̔z��ϐ��Ƃ��čl���܂��B

�u���b�L���O���[�h�̓I��(�]�������܂ő҂�)�ł��B


%href
clWriteIndex_i32
clWriteIndex_i64
clWriteIndex_dp
;--------
%index
clFillBuffer
VRAM���w��̌^�Ɛ��l�Ŗ��߂�

%prm
int p1,var p2,int p3,int p4,int p5

int p1 : cl_mem_id		[in]
var p2 : pattern			[in]
int p3 : offset,�ȗ���		[in]
int p4 : size,�ȗ���			[in]
int p5 : event_id,�ȗ���		[in]

%inst
p1�Ŏw�肵��VRAM(CL mem obj id)��p2�̒l���������݂܂��B
p2�̌^��int�^�Ȃ�4byte�����Afloat�^�ł�4byte�����Adouble�^��int64�^�Ȃ�8byte�u���ɒl���������܂�܂��B
�ȗ���p3=0,p4=�������T�C�Y
�ƂȂ�܂��B

���̖��ߎ��͎̂��s����������܂ő҂��߂ł͂Ȃ��AOpenCL�R�}���h���L���[�ɓ���邾���ł���A���ۂ̃J�[�l���̎��s�I����҂ɂ�event���g����clFinish���ő҂��ƂɂȂ�܂��B

%href
clReadBuffer
clWriteBuffer
;--------

%index
cldim_i32FromBuffer
HSP�z��ϐ��m�ۂ�VRAM����R�s�[

%prm
array p1,int p2
array p1 : HSP���̔z��ϐ�			[out]
int p2 : �R�s�[��cl_mem_id		[in]

%inst
p1�Ŏw�肵���ϐ���int�^�z��ϐ��Ƃ��ď��������A���e��p2����R�s�[���܂��B
�T�C�Y�͎����Ō��肳��܂��B
�Ȃ�HSP�̎d�l��A�m�ۂł���T�C�Y�̏����1GB�܂łł��B


%href
cldim_i64FromBuffer
cldim_dpFromBuffer
cldim_fpFromBuffer
;--------
%index
cldim_i64FromBuffer
HSP�z��ϐ��m�ۂ�VRAM����R�s�[

%prm
array p1,int p2
array p1 : HSP���̔z��ϐ�			[out]
int p2 : �R�s�[��cl_mem_id		[in]

%inst
p1�Ŏw�肵���ϐ���64bit int�^�z��ϐ��Ƃ��ď��������A���e��p2����R�s�[���܂��B
�T�C�Y�͎����Ō��肳��܂��B
�Ȃ�HSP�̎d�l��A�m�ۂł���T�C�Y�̏����1GB�܂łł��B


%href
cldim_i32FromBuffer
cldim_dpFromBuffer
cldim_fpFromBuffer
;--------
%index
cldim_dpFromBuffer
HSP�z��ϐ��m�ۂ�VRAM����R�s�[

%prm
array p1,int p2
array p1 : HSP���̔z��ϐ�			[out]
int p2 : �R�s�[��cl_mem_id		[in]

%inst
p1�Ŏw�肵���ϐ���double�^�z��ϐ��Ƃ��ď��������A���e��p2����R�s�[���܂��B
�T�C�Y�͎����Ō��肳��܂��B
�Ȃ�HSP�̎d�l��A�m�ۂł���T�C�Y�̏����1GB�܂łł��B


%href
cldim_i64FromBuffer
cldim_i32FromBuffer
cldim_fpFromBuffer
;--------
%index
cldim_fpFromBuffer
HSP�z��ϐ��m�ۂ�VRAM����R�s�[

%prm
array p1,int p2
array p1 : HSP���̔z��ϐ�			[out]
int p2 : �R�s�[��cl_mem_id		[in]

%inst
p1�Ŏw�肵���ϐ���float�^�z��ϐ��Ƃ��ď��������A���e��p2����R�s�[���܂��B
�T�C�Y�͎����Ō��肳��܂��B
�Ȃ�HSP�̎d�l��A�m�ۂł���T�C�Y�̏����1GB�܂łł��B


%href
cldim_i64FromBuffer
cldim_i32FromBuffer
cldim_dpFromBuffer

;----------------
%index
clGetAllBufferSize
VRAM���T�C�Y�擾

%prm
()

%inst
clCreateBuffer���ō쐬����buffer�̑��T�C�Y���擾���܂��B
���̖��߂�GPU��CPU�ł̃������g�p�ʂ��擾���閽�߂ł͂Ȃ��A�����܂�HSPCL32N�̃v���O�C�����g�����v���Z�X���̃f�o�C�Xbuffer�m�ۃT�C�Y�ł��邱�Ƃɒ��ӂ��Ă��������B


%href
clCreateBuffer
clCreateBufferFrom
clGetSize
;----------------
%index
clGetSize
�������T�C�Y�擾

%prm
(int p1)
int p1 : cl_mem_id		[in]

%inst
clCreateBuffer���ō쐬����buffer�̃T�C�Y��byte�ŕԂ��܂��B


%href
clCreateBuffer
clCreateBufferFrom
clGetAllBufferSize

;----------------
%index
clGarbageCollectionNow
VRAM�ꊇ���

%prm
int p1,int p2,int p3�E�E�E�E
int p1 : ���Ocl_mem_id,�ȗ���	[in]
int p2 : ���Ocl_mem_id,�ȗ���	[in]
int p3 : ���Ocl_mem_id,�ȗ���	[in]
int p4 : �E�E�E�E

%inst
clCreateBuffer���ō쐬����buffer��������܂��B

clCreateBuffer��clCreateBufferFrom�Ȃ�cl mem object�𐶐������
�����I��clReleaseBuffer�ŉ�����Ȃ�����v���Z�X���I������܂Ŏc�葱���܂��B
�����R�[�h����cl mem id�����X�g���Ă��܂��Ɖ�����邱�Ƃ��ł��Ȃ��Ȃ��Ă��܂��܂��B

�����Ńv���O�C�����őS�č쐬����cl mem id���o���Ă����A�[���I��GC���s�Ȃ����ƂŁA����Ȃ��Ȃ���cl mem������ł���悤�ɂ����̂�clGarbageCollectionNow���߂ł��B
�������������̎Q�ƃJ�E���^�������ł��Ȃ��̂Ŏ蓮�ŃJ�E���^���񂵂Ē����K�v������܂��B

clIncRefcntCLBufferId
clDecRefcntCLBufferId

���߂ŎQ�ƃJ�E���^�𑝌��ł��܂��B
�f�t�H���g�ŎQ�ƃJ�E���^��0�ł��B
�Q�ƃJ�E���^��1�ȏ�̂��̂͂��̖��߂Ŕj������܂���B

�܂��Q�ƃJ�E���^��0�ȉ��̂��̂ł�
p1,p2�E�E�E��mem id���w�肷�邱�ƂŁA���̖��ߎ��s���Ƀ�������������Ȃ��悤�ɂ��邱�Ƃ��ł��܂��B

%href
clIncRefcntCLBufferId
clDecRefcntCLBufferId

;----------------
%index
clIncRefcntCLBufferId
VRAM�Q�ƃJ�E���^�C���N�������g

%prm
int p1
int p1 : cl_mem_id	[in]

%inst
������id�̎Q�ƃJ�E���^��1���₵�܂��B
����͊�{�I��
clGarbageCollectionNow
���߂ƃZ�b�g�Ŏg���܂��B
�֋X��Q�ƃJ�E���^�Ɠǂ�ł��܂����A����HSP������ǂꂾ���Q�Ƃ���Ă��悤���A���̃J�E���^�͂����̃J�E���^�Ƃ��Ă����@�\���Ă��炸�A
���ۂ̎Q�Ə󋵂𑪂�Ă�����̂ł͂���܂���B

%href
clGarbageCollectionNow
clDecRefcntCLBufferId

;----------------
%index
clDecRefcntCLBufferId
VRAM�Q�ƃJ�E���^�f�N�������g

%prm
int p1
int p1 : cl_mem_id	[in]

%inst
������id�̎Q�ƃJ�E���^��1���炵�܂��B
����͊�{�I��
clGarbageCollectionNow
���߂ƃZ�b�g�Ŏg���܂��B
�֋X��Q�ƃJ�E���^�Ɠǂ�ł��܂����A����HSP������ǂꂾ���Q�Ƃ���Ă��悤���A���̃J�E���^�͂����̃J�E���^�Ƃ��Ă����@�\���Ă��炸�A
���ۂ̎Q�Ə󋵂𑪂�Ă�����̂ł͂���܂���B

%href
clGarbageCollectionNow
clIncRefcntCLBufferId
