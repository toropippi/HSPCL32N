;------------------------
;  �f�o�C�X���֘A
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
hspcl32n.as ���C���N���[�h���Ă��������B

%type
32bit�����^�C��GPGPU�p�v���O�C��

%group
OpenCL�f�o�C�X���Ǘ�

%port
Win

;-------- ref --------

%index
clinit
HSPCL32N��������

%prm

%inst
HSPCL32N�����������܂��B
hspcl32n.as���C���N���[�h�������ƂɎ��s���ĉ������B
�F������OpenCL�f�o�C�X����stat�ɑ������܂��B

%href
clini
clGetDeviceCount
;--------
%index
clini
HSPCL32N��������(������)

%prm

%inst
HSPCL32N�����������܂��B
hspcl32n.as���C���N���[�h�������ƂɎ��s���ĉ������B
����͋����߂ł���clinit�����g���������B

%href
clinit
clGetDeviceCount

;--------
%index
clbye
HSPCL32N���I��(������)

%prm

%inst
HSPCL32N���I�����܂��B
����͋����߂ł���݊����̂��ߎc���Ă��܂��B
HCPCL32N ver2.2�ȍ~�ł͂��̖��ߎ��̂͂Ȃɂ��s�Ȃ�Ȃ����߂ƂȂ�܂����B

%href
clini
clinit
;--------

%index
clGetDeviceCount
OpenCL�f�o�C�X���擾

%prm
()

%inst
OpenCL�f�o�C�X�Ƃ��ĔF�������f�o�C�X�����Ԃ�܂��B
clSetDevice�Ŏw��ł���ԍ���0�`clGetDeviceCount()-1�͈̔͂ƂȂ�܂��B

%href
clinit
clSetDevice

;--------

%index
clSetDevice
�f�o�C�X�Z�b�g

%prm
int p1
int p1	�f�o�C�Xid [in]

%inst

�J�[�l�����߂�J�[�l���o�^�A�������m�ۂ����s����f�o�C�X���w�肵�܂��B
clSetDevice�Ŏw��ł���ԍ���0�`clGetDeviceCount()-1�͈̔͂ƂȂ�܂��B

�ȉ��̖��߂͐ݒ肵���f�o�C�X�݂̂Ɏ��s����܂��B

clGetSettingDevice
clCreateProgram
clCreateProgramWithSource
clCreateKernel
clCreateBuffer
clCreateBufferFrom
clWriteBuffer
clWriteBuffer_NonBlocking
clReadBuffer
clReadBuffer_NonBlocking
clCopyBuffer
clFillBuffer_i32
clFillBuffer_i64
clFillBuffer_dp
cldim_i32FromBuffer
cldim_i64FromBuffer
cldim_dpFromBuffer
clReadIndex_i32
clReadIndex_i64
clReadIndex_dp
clWriteIndex_i32
clWriteIndex_i64
clWriteIndex_dp
clCall
clDoKrn1
clDoKrn2
clDoKrn3
clDoKernel
clDoKrn1_sub
clFinish
clFlush
clSetCommandQueue


clSetDevice�����s���Ă��Ȃ��ꍇ�́A�J�����g�f�o�C�X�̓f�o�C�X0�ł��B

%href
clGetSettingDevice
clCreateProgram
clCreateProgramWithSource
clCreateKernel
clCreateBuffer
clCreateBufferFrom
clWriteBuffer
clWriteBuffer_NonBlocking
clReadBuffer
clReadBuffer_NonBlocking
clCopyBuffer
clFillBuffer_i32
clFillBuffer_i64
clFillBuffer_dp
cldim_i32FromBuffer
cldim_i64FromBuffer
cldim_dpFromBuffer
clReadIndex_i32
clReadIndex_i64
clReadIndex_dp
clWriteIndex_i32
clWriteIndex_i64
clWriteIndex_dp
clCall
clDoKrn1
clDoKrn2
clDoKrn3
clDoKernel
clDoKrn1_sub
clFinish
clFlush
clSetCommandQueue
;--------

%index
clSetDev
�f�o�C�X�Z�b�g(������)

%prm
int p1
int p1	�f�o�C�Xid [in]

%inst

�J�[�l�����߂�J�[�l���o�^�A�������m�ۂ����s����f�o�C�X���w�肵�܂��B
clSetDev�Ŏw��ł���ԍ���0�`clGetDeviceCount()-1�͈̔͂ƂȂ�܂��B
����͋����߂ł���clSetDevice�����g���������B

;--------

%index
clGetDevName
�f�o�C�X���擾(������)

%prm
()

%inst

clSetDevice�ŃZ�b�g���ꂽ�f�o�C�X�̃f�o�C�X���𕶎���ŕԂ��܂��B
����͋����߂ł���clGetDeviceInfo_s�����g���������B
;--------


%index
clGetSettingDevice
�Z�b�g�f�o�C�Xno�擾

%prm
()

%inst
clSetDevice�Ŏw�肵���f�o�C�X��id���Ԃ�܂��B

%href
clSetDevice
;--------

%index
clGetDeviceInfo_s
�f�o�C�X���擾

%prm
(int p1)
int p1 �F param_name [in]

%inst
clSetDevice�ŃZ�b�g�����f�o�C�X�̏��𕶎���Ŏ擾���܂��B
p1��param_name���w�肵�ĉ������B�߂�l�͕�����ɂȂ�܂��B

p1�͉��L��URL�̕\�ɂ���l����ЂƂI��Ŏw��ł��܂��B
���{��T�C�g�Fhttp://wiki.tommy6.net/wiki/clGetDeviceInfo
�p��T�C�g�Fhttps://www.khronos.org/registry/cl/sdk/1.0/docs/man/xhtml/clGetDeviceInfo.html

%href
clGetDeviceInfo_i
clGetDeviceInfo_d
;--------

%index
clGetDeviceInfo_i
�f�o�C�X���擾

%prm
(int p1,int p2)
int p1 �F param_name [in]
int p2 �F index [in]

%inst
clSetDevice �ŃZ�b�g�����f�o�C�X�̏��𕶎���Ŏ擾���܂��B
p1��param_name���w�肵�ĉ������B�߂�l��int�^�̐����ɂȂ�܂��B
�߂�l�̐��l���z��̏ꍇ�́Ap2��index���w�肷�邱�ƂŁAp2�Ԗڂ̗v�f�̏��𓾂邱�Ƃ��ł��܂��B

p1�͉��L��URL�̕\�ɂ���l����ЂƂI��Ŏw��ł��܂��B
���{��T�C�g�Fhttp://wiki.tommy6.net/wiki/clGetDeviceInfo
�p��T�C�g�Fhttps://www.khronos.org/registry/cl/sdk/1.0/docs/man/xhtml/clGetDeviceInfo.html

%href
clGetDeviceInfo_d
clGetDeviceInfo_s
;--------

%index
clGetDeviceInfo_d
�f�o�C�X���擾

%prm
(int p1,int p2)
int p1 �F param_name [in]
int p2 �F index [in]

%inst
clSetDevice �ŃZ�b�g�����f�o�C�X�̏��𕶎���Ŏ擾���܂��B
p1��param_name���w�肵�ĉ������B�߂�l��double�^�̎����ɂȂ�܂��B
�߂�l�̐��l���z��̏ꍇ�́Ap2��index���w�肷�邱�ƂŁAp2�Ԗڂ̗v�f�̏��𓾂邱�Ƃ��ł��܂��B

p1�͉��L��URL�̕\�ɂ���l����ЂƂI��Ŏw��ł��܂��B
���{��T�C�g�Fhttp://wiki.tommy6.net/wiki/clGetDeviceInfo
�p��T�C�g�Fhttps://www.khronos.org/registry/cl/sdk/1.0/docs/man/xhtml/clGetDeviceInfo.html

%href
clGetDeviceInfo_i
clGetDeviceInfo_s
;--------

%index
_ExclSetCommandQueueMax
�R�}���h�L���[�̍ő吔���Z�b�g

%prm
int p1
int p1	�R�}���h�L���[�̍ő吔 [in]


%inst
clInit�̎��s�O�Ɏw�肷�邱�ƂŁA1�̃f�o�C�X������̃R�}���h�L���[�̍ő�𑝂₷���Ƃ��ł��܂��B
�f�t�H���g�ł�1�̃f�o�C�X������̃R�}���h�L���[��0�`3�܂ł����܂��B

%href
_ExclSetCommandQueueProperties
_ExclSetEventMax
;--------

%index
_ExclSetCommandQueueProperties
�R�}���h�L���[�v���p�e�B�̃Z�b�g

%prm
int p1
int p1	properties [in]

%inst
clInit�̎��s�O�Ɏw�肷�邱�ƂŁAOpenCL�̃R�}���h�L���[�̃v���p�e�B�̐ݒ��ς��邱�Ƃ��ł��܂��B
�R�}���h�L���[�v���p�e�B�̓f�t�H���g�ł�CL_QUEUE_PROFILING_ENABLE���w�肳��Ă��܂��B
http://wiki.tommy6.net/wiki/clCreateCommandQueue
���Q�l�ɂ��ĉ������B

%href
_ExclSetCommandQueueMax
_ExclSetEventMax
;--------

%index
_ExclSetEventMax
�C�x���g�̍ő吔���Z�b�g

%prm
int p1
int p1	�C�x���g�̍ő吔 [in]

%inst
clInit�̎��s�O�Ɏw�肷�邱�ƂŁA�L�^�\�C�x���g�̍ő�𑝂₷���Ƃ��ł��܂��B
�f�t�H���g�ŃC�x���gid��0�`65535�Ԃ܂ł����܂��B

%href
_ExclSetCommandQueueMax
_ExclSetCommandQueueProperties
;--------