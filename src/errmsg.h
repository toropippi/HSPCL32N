char bugchar[1024 * 1024];
void retmeserr(cl_int ret);//clEnqueueNDRangeKernel �Ŏ��s�������o���G���[���b�Z�[�W���܂Ƃ߂��֐�
void retmeserr2(cl_int ret);//clRead�Ŏ��s�������o���G���[���b�Z�[�W���܂Ƃ߂��֐�
void retmeserr3(cl_int ret);//clCreateCommandQueue�Ŏ��s�������o���G���[���b�Z�[�W���܂Ƃ߂��֐�
void retmeserr4(cl_int ret);//clCreateContext�Ŏ��s�������o���G���[���b�Z�[�W���܂Ƃ߂��֐�
void retmeserr5(cl_int ret);//HCLGetEventStartTime�Ŏ��s�������o���G���[���b�Z�[�W���܂Ƃ߂��֐�
void retmeserr6(cl_int ret); //clWaitForEvents�Ŏ��s�������o���G���[���b�Z�[�W���܂Ƃ߂��֐�
void retmeserr7(cl_device_id d_id, cl_program program);//HCLCreateProgram�Ŏ��s�������o���G���[���b�Z�[�W���܂Ƃ߂��֐�
void retmeserr8(cl_int ret);//clCreateKernel�Ŏ��s�������o���G���[���b�Z�[�W���܂Ƃ߂��֐�
void retmeserr9(cl_int ret);//clCreateBuffer�Ŏ��s�������o���G���[���b�Z�[�W���܂Ƃ߂��֐�
void retmeserr10(cl_int ret);//clFinish�Ŏ��s�������o���G���[���b�Z�[�W���܂Ƃ߂��֐�
void retmeserr11(cl_int ret);//clFlush�Ŏ��s�������o���G���[���b�Z�[�W���܂Ƃ߂��֐�
void retmeserr12(cl_int ret);//clGetMemObjectInfo�Ŏ��s�������o���G���[���b�Z�[�W���܂Ƃ߂��֐�
void retmeserr13(cl_int ret);//clCreateUserEvent�Ŏ��s�������o���G���[���b�Z�[�W���܂Ƃ߂��֐�
void retmeserr14(cl_int ret);//clSetUserEventStatus�Ŏ��s�������o���G���[���b�Z�[�W���܂Ƃ߂��֐�
void retmeserr15(cl_int ret);//HCLCreateProgramWithBinary�Ŏ��s�������o���G���[���b�Z�[�W���܂Ƃ߂��֐�
void retmeserr16(cl_int ret);//HCLCreateProgramWithBinary�Ŏ��s�������o���G���[���b�Z�[�W���܂Ƃ߂��֐�
void mes(const char* strc, int val);







void retmeserr(cl_int ret)
{
	switch (ret) {							//����
	case CL_INVALID_PROGRAM_EXECUTABLE:
		MessageBox(NULL, "CL_INVALID_PROGRAM_EXECUTABLE\n�f�o�C�X��Ŏ��s�\�ȁA����Ƀr���h���ꂽ�v���O�������������܂���", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_COMMAND_QUEUE:
		MessageBox(NULL, "CL_INVALID_COMMAND_QUEUE\n�f�o�C�Xid�������ȃf�o�C�X�ɂȂ��Ă��܂�", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_KERNEL:
		MessageBox(NULL, "CL_INVALID_KERNEL\n�J�[�l��id���Ԉ���Ă��܂�", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_CONTEXT:
		MessageBox(NULL, "CL_INVALID_CONTEXT\n�J�[�l��id���Ⴄ�f�o�C�Xid�œo�^����Ă��܂��A���邢�͑������� event_wait_list ���̃C�x���g�Ɗ֘A�t����ꂽ�f�o�C�X�������łȂ�", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_KERNEL_ARGS:
		MessageBox(NULL, "CL_INVALID_KERNEL_ARGS\n�J�[�l���������w�肳��Ă��܂���", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_GLOBAL_WORK_SIZE:
		MessageBox(NULL, "CL_INVALID_GLOBAL_WORK_SIZE\nglobal_work_size �� NULL �ł��B���邢�́Aglobal_work_size�̔z��̂ǂꂩ��0�ł��B�������̓J�[�l�������s����f�o�C�X��ł�global_work_size������l�𒴂��Ă���", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_GLOBAL_OFFSET:
		MessageBox(NULL, "CL_INVALID_GLOBAL_OFFSET\nCL_INVALID_GLOBAL_OFFSET - global_work_offset �� NULL �łȂ�", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_WORK_DIMENSION:
		MessageBox(NULL, "CL_INVALID_WORK_DIMENSION\nwork_dim ���K�؂Ȓl�łȂ�", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_WORK_GROUP_SIZE:
		MessageBox(NULL, "CL_INVALID_WORK_GROUP_SIZE\nglobal_work_size��local_work_size �Ő����ł��Ȃ��A�܂���local_work_size[0]*local_work_size[1]*local_work_size[2]���A��̃��[�N�O���[�v���̃��[�N�A�C�e�����̍ő�l�𒴂���", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_WORK_ITEM_SIZE:
		MessageBox(NULL, "CL_INVALID_WORK_ITEM_SIZE\nlocal_work_size[0], ... local_work_size[work_dim - 1] �Ŏw�肵�����[�N�A�C�e�������Ή����� CL_DEVICE_MAX_WORK_ITEM_SIZES[0], ... CL_DEVICE_MAX_WORK_ITEM_SIZES[work_dim - 1] �̒l�����Ă���A�܂���0���w�肵��", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_MEM_OBJECT_ALLOCATION_FAILURE:
		MessageBox(NULL, "CL_MEM_OBJECT_ALLOCATION_FAILURE\nkernel �̈����Ɏw�肳�ꂽ�o�b�t�@/�C���[�W�I�u�W�F�N�g�Ɋ֘A�t����ꂽ�f�[�^�ۑ��̂��߂̃������̈�̊m�ۂɎ��s����", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_EVENT_WAIT_LIST:
		MessageBox(NULL, "CL_INVALID_EVENT_WAIT_LIST\nevent_wait_list �� NULL �� num_events_in_wait_list �� 0 ���傫���Ƃ��B���邢�� event_wait_list �� NULL �łȂ� num_events_in_wait_list �� 0 �̂Ƃ��B���邢�� event_wait_list ���̃C�x���g�I�u�W�F�N�g���L���Ȃ��̂łȂ�", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_OUT_OF_RESOURCES:
		MessageBox(NULL, "CL_OUT_OF_RESOURCES\n�f�o�C�X��ł̃��\�[�X�m�ۂɎ��s����", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_OUT_OF_HOST_MEMORY:
		MessageBox(NULL, "CL_OUT_OF_HOST_MEMORY\n�z�X�g��ł̃��\�[�X�m�ۂɎ��s����", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	}
	//��̂ǂ�ł��Ȃ����
	MessageBox(NULL, "�����s���̃G���[�ł�", "�G���[", 0);
	puterror(HSPERR_UNSUPPORTED_FUNCTION);
}




void retmeserr2(cl_int ret)
{
	switch (ret) {							//����
	case CL_INVALID_COMMAND_QUEUE:
		MessageBox(NULL, "CL_INVALID_COMMAND_QUEUE\ncommand_queue is not a valid command-queue", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_CONTEXT:
		MessageBox(NULL, "CL_INVALID_CONTEXT\n�������I�u�W�F�N�g���ʂ̃f�o�C�X�ō쐬���ꂽ�\��������܂�", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_MEM_OBJECT:
		MessageBox(NULL, "CL_INVALID_MEM_OBJECT\n�������I�u�W�F�N�g�̎��̂�����܂���B�������I�u�W�F�N�g���ʂ̃f�o�C�X�ō쐬���ꂽ�\��������܂��B", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_VALUE:
		MessageBox(NULL, "CL_INVALID_VALUE\n�A�h���X�A�N�Z�X�ᔽ�ł��B�ǂݍ��ݗ̈悪�͂ݏo���Ă܂��B", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_MEM_COPY_OVERLAP:
		MessageBox(NULL, "CL_MEM_COPY_OVERLAP\n�A�h���X�A�N�Z�X�ᔽ�ł��B�������ݗ̈悩�ǂݍ��ݗ̈悪�͂ݏo���Ă܂��B", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_MEM_OBJECT_ALLOCATION_FAILURE:
		MessageBox(NULL, "CL_MEM_OBJECT_ALLOCATION_FAILURE\ndata store �̂��߂�allocate memory����̂����s���܂���", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_OUT_OF_RESOURCES:
		MessageBox(NULL, "CL_OUT_OF_RESOURCES\n�f�o�C�X(GPU)��ł̃��\�[�X�m�ۂɎ��s����", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_OUT_OF_HOST_MEMORY:
		MessageBox(NULL, "CL_OUT_OF_HOST_MEMORY\n�z�X�g(CPU)��ł̃��\�[�X�m�ۂɎ��s����", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	}
	//��̂ǂ�ł��Ȃ����
	MessageBox(NULL, "�����s���̃G���[�ł�", "�G���[", 0);
	puterror(HSPERR_UNSUPPORTED_FUNCTION);

}

void retmeserr3(cl_int ret)
{
	switch (ret) {							//����
	case CL_INVALID_CONTEXT:
		MessageBox(NULL, "CL_INVALID_CONTEXT\nCL_INVALID_CONTEXT:if context is not a valid context.\\n�R���e�L�X�g���L���ȃR���e�L�X�g�łȂ��ꍇ", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_DEVICE:
		MessageBox(NULL, "CL_INVALID_DEVICE\nCL_INVALID_DEVICE:if device is not a valid device or is not associated with context\\n�f�o�C�X���L���ȃf�o�C�X�ł͂Ȃ��ꍇ�A�܂��̓R���e�L�X�g�Ɋ֘A�t�����Ă��Ȃ��ꍇ", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_VALUE:
		MessageBox(NULL, "CL_INVALID_VALUE\nCL_INVALID_VALUE: if values specified in properties are not valid.\\n�v���p�e�B�Ŏw�肳�ꂽ�l�������ȏꍇ�B", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_QUEUE_PROPERTIES:
		MessageBox(NULL, "CL_INVALID_QUEUE_PROPERTIES\nCL_INVALID_QUEUE_PROPERTIES:if values specified in properties are valid but are not supported by the device.�v���p�e�B�Ŏw�肳�ꂽ�l�͗L���ł��邪�A�f�o�C�X�ŃT�|�[�g����Ă��Ȃ��ꍇ�B", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_OUT_OF_HOST_MEMORY:
		MessageBox(NULL, "CL_OUT_OF_HOST_MEMORY\nCL_OUT_OF_HOST_MEMORY:if there is a failure to allocate resources required by the OpenCL implementation on the host.\\n�z�X�g���OpenCL�����ɕK�v�ȃ��\�[�X�̊��蓖�ĂɎ��s�����ꍇ�B", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	}
	//��̂ǂ�ł��Ȃ����
	MessageBox(NULL, "�����s���̃G���[�ł�", "�G���[", 0);
	puterror(HSPERR_UNSUPPORTED_FUNCTION);
}

void retmeserr4(cl_int ret)
{
	switch (ret) {							//����
	case CL_INVALID_PLATFORM:
		MessageBox(NULL, "CL_INVALID_PLATFORM\nCL_INVALID_PLATFORM:if properties is NULL and no platform could be selected or if platform value specified in properties is not a valid platform.", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_VALUE:
		MessageBox(NULL, "CL_INVALID_VALUE\nCL_INVALID_VALUE:CL_INVALID_VALUE if context property name in properties is not a supported property name; if devices is NULL; if num_devices is equal to zero; or if pfn_notify is NULL but user_data is not NULL.", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_DEVICE:
		MessageBox(NULL, "CL_INVALID_DEVICE\nCL_INVALID_DEVICE: if devices contains an invalid device or are not associated with the specified platform.", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_DEVICE_NOT_AVAILABLE:
		MessageBox(NULL, "CL_DEVICE_NOT_AVAILABLE\nCL_DEVICE_NOT_AVAILABLE if a device in devices is currently not available even though the device was returned by clGetDeviceIDs.", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_OUT_OF_HOST_MEMORY:
		MessageBox(NULL, "CL_OUT_OF_HOST_MEMORY\nCL_OUT_OF_HOST_MEMORY:if there is a failure to allocate resources required by the OpenCL implementation on the host.", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	}
	//��̂ǂ�ł��Ȃ����
	MessageBox(NULL, "�����s���̃G���[�ł�", "�G���[", 0);
	puterror(HSPERR_UNSUPPORTED_FUNCTION);
}


void retmeserr5(cl_int ret)
{
	switch (ret) {							//����
	case CL_PROFILING_INFO_NOT_AVAILABLE:
		MessageBox(NULL, "CL_PROFILING_INFO_NOT_AVAILABLE\n�R�}���h�L���[ �� CL_QUEUE_PROFILING_ENABLE �t���O���ݒ肳��Ă��Ȃ��Ƃ��B���邢�́Aevent �Ɗ֘A�t����ꂽ�R�}���h�̎��s��Ԃ� CL_COMPLETE �łȂ��Ƃ��B���邢�́Aevent �����[�U�C�x���g�I�u�W�F�N�g�̂Ƃ��B", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_EVENT:
		MessageBox(NULL, "CL_INVALID_EVENT\nevent ���L���ȃC�x���g�I�u�W�F�N�g�łȂ��Ƃ��B", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_VALUE:
		MessageBox(NULL, "CL_INVALID_VALUE\nparam_name ���T�|�[�g����Ă���l�łȂ��A���邢�́Aparam_value_size �Ŏw�肳�ꂽ�T�C�Y����L�̕\�Ŏw�肳��Ă���߂�l�^�̃T�C�Y��菬�������� param_value �� NULL �łȂ��Ƃ��B", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_OUT_OF_RESOURCES:
		MessageBox(NULL, "CL_OUT_OF_RESOURCES\n�f�o�C�X��ł̃��\�[�X�m�ۂɎ��s�����Ƃ��B", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_OUT_OF_HOST_MEMORY:
		MessageBox(NULL, "CL_OUT_OF_HOST_MEMORY\n�z�X�g��ł̃��\�[�X�m�ۂɎ��s�����Ƃ��B", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	}
	//��̂ǂ�ł��Ȃ����
	MessageBox(NULL, "�����s���̃G���[�ł�", "�G���[", 0);
	puterror(HSPERR_UNSUPPORTED_FUNCTION);
}



void retmeserr6(cl_int ret)
{
	switch (ret) {							//����
	case CL_INVALID_EVENT:
		MessageBox(NULL, "CL_INVALID_EVENT\nevent_list ���L���ȃC�x���g�I�u�W�F�N�g�łȂ��Ƃ��B", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_VALUE:
		MessageBox(NULL, "CL_INVALID_VALUE\nnum_events �� 0 ���邢�� event_list �� NULL �̂Ƃ��B", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_CONTEXT:
		MessageBox(NULL, "CL_INVALID_CONTEXT\nevent_list ���̃C�x���g���֘A�t�����Ă���OpenCL�R���e�L�X�g�������łȂ��Ƃ��B", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST:
		MessageBox(NULL, "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST\nevent_list ���̃C�x���g�̂��������ꂩ�̎��s��Ԃ����̒l�̂Ƃ��B", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_OUT_OF_RESOURCES:
		MessageBox(NULL, "CL_OUT_OF_RESOURCES\n�f�o�C�X��ł̃��\�[�X�m�ۂɎ��s�����Ƃ��B", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_OUT_OF_HOST_MEMORY:
		MessageBox(NULL, "CL_OUT_OF_HOST_MEMORY\n�z�X�g��ł̃��\�[�X�m�ۂɎ��s�����Ƃ��B", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	}
	//��̂ǂ�ł��Ȃ����
	MessageBox(NULL, "�����s���̃G���[�ł�", "�G���[", 0);
	puterror(HSPERR_UNSUPPORTED_FUNCTION);
}


void retmeserr7(cl_device_id d_id,cl_program program)
{
	size_t len;
	clGetProgramBuildInfo(program, d_id,CL_PROGRAM_BUILD_LOG, sizeof(bugchar), bugchar, &len);
	MessageBox(NULL, (LPCSTR)bugchar, "Error on OpenCL code", MB_OK);
	puterror(HSPERR_UNKNOWN_CODE);
}



void retmeserr8(cl_int ret)
{

	switch (ret) {							//����

	case CL_INVALID_PROGRAM:
		MessageBox(NULL, "CL_INVALID_PROGRAM\nprogram ���L���ȃv���O�����I�u�W�F�N�g�łȂ�", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;

	case CL_INVALID_PROGRAM_EXECUTABLE:
		MessageBox(NULL, "CL_INVALID_PROGRAM_EXECUTABLE\nprogram �ɐ���Ƀr���h���ꂽ���s�\�v���O�������Ȃ�", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;

	case CL_INVALID_KERNEL_NAME:
		MessageBox(NULL, "CL_INVALID_KERNEL_NAME\nkernel_name �� program ���Ɍ�����Ȃ�", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;

	case CL_INVALID_KERNEL_DEFINITION:
		MessageBox(NULL, "CL_INVALID_KERNEL_DEFINITION\nkernel_name ���^����A����������̌^�Ƃ����� __kernel �֐��̊֐���`���Aprogram ���r���h���ꂽ���ׂẴf�o�C�X�œ����łȂ�", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;

	case CL_INVALID_VALUE:
		MessageBox(NULL, "CL_INVALID_VALUE\nkernel_name �� NULL", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;

	case CL_OUT_OF_RESOURCES:
		MessageBox(NULL, "CL_OUT_OF_RESOURCES\n�f�o�C�X��ł̃��\�[�X�m�ۂɎ��s����", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;

	case CL_OUT_OF_HOST_MEMORY:
		MessageBox(NULL, "CL_OUT_OF_HOST_MEMORY\n�z�X�g��ł̃��\�[�X�m�ۂɎ��s����", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	}
	//��̂ǂ�ł��Ȃ����
	MessageBox(NULL, "�����s���̃G���[�ł�", "�G���[", 0);
	puterror(HSPERR_UNSUPPORTED_FUNCTION);
}



void retmeserr9(cl_int ret)
{
	switch (ret) {							//����
	case CL_INVALID_CONTEXT:
		MessageBox(NULL, "CL_INVALID_CONTEXT\ncontext ���L����OpenCL�R���e�L�X�g�łȂ�", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_VALUE:
		MessageBox(NULL, "CL_INVALID_VALUE\n�ǂݏ�����p���������p�ӂł��܂���ł���", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_KERNEL:
		MessageBox(NULL, "CL_INVALID_KERNEL\nsize �� 0 �̂Ƃ��B�������� size �� context ���̃f�o�C�X�� CL_DEVICE_MAX_MEM_ALLOC_SIZE �̒l���傫��", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_HOST_PTR:
		MessageBox(NULL, "CL_INVALID_HOST_PTR\nhost_ptr �� NULL �� CL_MEM_USE_HOST_PTR �������� CL_MEM_COPY_HOST_PTR �� flags �Ɏw�肳��Ă���Ƃ��B�������́ACL_MEM_COPY_HOST_PTR �� CL_MEM_USE_HOST_PTR ���ݒ肳��Ă��Ȃ��̂� host_ptr �� NULL �łȂ�", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_MEM_OBJECT_ALLOCATION_FAILURE:
		MessageBox(NULL, "CL_MEM_OBJECT_ALLOCATION_FAILURE\n�o�b�t�@�I�u�W�F�N�g�̃��������m�ۂ���̂Ɏ��s����", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_OUT_OF_RESOURCES:
		MessageBox(NULL, "CL_OUT_OF_RESOURCES\n�f�o�C�X��ł̃��\�[�X�m�ۂɎ��s����", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_OUT_OF_HOST_MEMORY:
		MessageBox(NULL, "CL_OUT_OF_HOST_MEMORY\n�z�X�g��ł̃��\�[�X�m�ۂɎ��s����", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	}
	//��̂ǂ�ł��Ȃ����
	MessageBox(NULL, "�����s���̃G���[�ł�", "�G���[", 0);
	puterror(HSPERR_UNSUPPORTED_FUNCTION);
}


void retmeserr10(cl_int ret) 
{

	switch (ret) {							//����

	case CL_INVALID_COMMAND_QUEUE:
		MessageBox(NULL, "CL_INVALID_COMMAND_QUEUE\nCL_INVALID_COMMAND_QUEUE�@���������L���Ȓl�ł͂���܂���", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_CONTEXT:
		MessageBox(NULL, "CL_INVALID_CONTEXT\n�������� event_wait_list ���̃C�x���g�Ɗ֘A�t����ꂽ�f�o�C�X�������łȂ�\ncontext ���L����OpenCL�R���e�L�X�g�łȂ�", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_EVENT:
		MessageBox(NULL, "CL_INVALID_EVENT\nevent_list�̃C�x���g�I�u�W�F�N�g���s��", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_OUT_OF_HOST_MEMORY:
		MessageBox(NULL, "CL_OUT_OF_HOST_MEMORY\n�z�X�g��ł̃��\�[�X�m�ۂɎ��s����", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_VALUE:
		MessageBox(NULL, "CL_INVALID_VALUE\n�ǂݏ�����p���������p�ӂł��܂���ł���", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_KERNEL:
		MessageBox(NULL, "CL_INVALID_KERNEL\nsize �� 0 �̂Ƃ��B�������� size �� context ���̃f�o�C�X�� CL_DEVICE_MAX_MEM_ALLOC_SIZE �̒l���傫��", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_HOST_PTR:
		MessageBox(NULL, "CL_INVALID_HOST_PTR\nhost_ptr �� NULL �� CL_MEM_USE_HOST_PTR �������� CL_MEM_COPY_HOST_PTR �� flags �Ɏw�肳��Ă���Ƃ��B�������́ACL_MEM_COPY_HOST_PTR �� CL_MEM_USE_HOST_PTR ���ݒ肳��Ă��Ȃ��̂� host_ptr �� NULL �łȂ�", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_MEM_OBJECT_ALLOCATION_FAILURE:
		MessageBox(NULL, "CL_MEM_OBJECT_ALLOCATION_FAILURE\n�o�b�t�@�I�u�W�F�N�g�̃��������m�ۂ���̂Ɏ��s����", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_OUT_OF_RESOURCES:
		MessageBox(NULL, "CL_OUT_OF_RESOURCES\n�f�o�C�X��ł̃��\�[�X�m�ۂɎ��s����", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	}
	//��̂ǂ�ł��Ȃ����
	MessageBox(NULL, "�����s���̃G���[�ł�", "�G���[", 0);
	puterror(HSPERR_UNSUPPORTED_FUNCTION);

}


void retmeserr11(cl_int ret)
{
	switch (ret) {							//����
	case CL_INVALID_COMMAND_QUEUE:
		MessageBox(NULL, "CL_INVALID_COMMAND_QUEUE\nCL_INVALID_COMMAND_QUEUE: if command_queue is not a valid command-queue", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_OUT_OF_HOST_MEMORY:
		MessageBox(NULL, "CL_OUT_OF_HOST_MEMORY\nCL_OUT_OF_HOST_MEMORY:if there is a failure to allocate resources required by the OpenCL implementation on the host.", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	}
	//��̂ǂ�ł��Ȃ����
	MessageBox(NULL, "�����s���̃G���[�ł�", "�G���[", 0);
	puterror(HSPERR_UNSUPPORTED_FUNCTION);

}


void retmeserr12(cl_int ret)
{
	switch (ret)
	{							//����
	case CL_INVALID_VALUE:
		MessageBox(NULL, "CL_INVALID_VALUE\nparam_name ���T�|�[�g����Ă���l�łȂ��A���邢�́Aparam_value_size �Ŏw�肳�ꂽ�T�C�Y����L�̕\�Ŏw�肳��Ă���߂�l�^�̃T�C�Y��菬�������� param_value �� NULL �łȂ��Ƃ��B", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_MEM_OBJECT:
		MessageBox(NULL, "CL_INVALID_MEM_OBJECT\nmemobj ���L���ȃ������I�u�W�F�N�g�łȂ��Ƃ�", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_OUT_OF_RESOURCES:
		MessageBox(NULL, "CL_OUT_OF_RESOURCES\n�f�o�C�X��ł̃��\�[�X�m�ۂɎ��s�����Ƃ�", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_OUT_OF_HOST_MEMORY:
		MessageBox(NULL, "CL_OUT_OF_HOST_MEMORY\n�z�X�g��ł̃��\�[�X�m�ۂɎ��s�����Ƃ�", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	}
	//��̂ǂ�ł��Ȃ����
	MessageBox(NULL, "�����s���̃G���[�ł�", "�G���[", 0);
	puterror(HSPERR_UNSUPPORTED_FUNCTION);
}


void retmeserr13(cl_int ret)
{
	switch (ret)
	{							//����
	case CL_INVALID_CONTEXT:
		MessageBox(NULL, "CL_INVALID_CONTEXT\nif context is not a valid context.\n�R���e�L�X�g���L���ȃR���e�L�X�g�łȂ��ꍇ�B", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_OUT_OF_RESOURCES:
		MessageBox(NULL, "CL_OUT_OF_RESOURCES\nif there is a failure to allocate resources required by the OpenCL implementation on the device.\n�f�o�C�X�ł�OpenCL�����ɕK�v�ȃ��\�[�X�̊��蓖�ĂɎ��s�����ꍇ�B", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_OUT_OF_HOST_MEMORY:
		MessageBox(NULL, "CL_OUT_OF_HOST_MEMORY\nif there is a failure to allocate resources required by the OpenCL implementation on the host\n�z�X�g�ł�OpenCL�����ɕK�v�ȃ��\�[�X�̊��蓖�ĂɎ��s�����ꍇ�B", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	}
	//��̂ǂ�ł��Ȃ����
	MessageBox(NULL, "�����s���̃G���[�ł�", "�G���[", 0);
	puterror(HSPERR_UNSUPPORTED_FUNCTION);
}



void retmeserr14(cl_int ret)
{
	switch (ret)
	{							//����
	case CL_INVALID_EVENT:
		MessageBox(NULL, "CL_INVALID_EVENT\nif event is not a valid user event.\n�C�x���g���L���ȃ��[�U�[�C�x���g�łȂ��ꍇ�B", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_VALUE:
		MessageBox(NULL, "CL_INVALID_VALUE\nif the execution_status is not CL_COMPLETE or a negative integer value.\nexecute_status��CL_COMPLETE�܂��͕��̐����l�łȂ��ꍇ�B", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_OPERATION:
		MessageBox(NULL, "CL_INVALID_OPERATION\nif the execution_status for event has already been changed by a previous call to clSetUserEventStatus.\n�C�x���g��execution_status���AclSetUserEventStatus�ւ̈ȑO�̌Ăяo���ɂ���Ă��łɕύX����Ă���ꍇ�B", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_OUT_OF_RESOURCES:
		MessageBox(NULL, "CL_OUT_OF_RESOURCES\nif there is a failure to allocate resources required by the OpenCL implementation on the device.\n�f�o�C�X�ł�OpenCL�����ɕK�v�ȃ��\�[�X�̊��蓖�ĂɎ��s�����ꍇ�B", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_OUT_OF_HOST_MEMORY:
		MessageBox(NULL, "CL_OUT_OF_HOST_MEMORY\nif there is a failure to allocate resources required by the OpenCL implementation on the host.\n�z�X�g�ł�OpenCL�����ɕK�v�ȃ��\�[�X�̊��蓖�ĂɎ��s�����ꍇ�B", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	}
	//��̂ǂ�ł��Ȃ����
	MessageBox(NULL, "�����s���̃G���[�ł�", "�G���[", 0);
	puterror(HSPERR_UNSUPPORTED_FUNCTION);
}


void retmeserr15(cl_int ret)
{
	switch (ret)
	{							//����
	case CL_INVALID_VALUE:
		MessageBox(NULL, "CL_INVALID_VALUE\nlengths[i] ��0��������\nbinaries[i] �� NULL �̂Ƃ�", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_BINARY:
		MessageBox(NULL, "CL_INVALID_BINARY\n�v���O�����o�C�i�����f�o�C�X�ɑ΂��ėL���Ȃ��̂łȂ��Ƃ�\nCL_INVALID_BINARY ���Ԃ���܂��B\nbinary_status �� NULL ���w�肷��Ɩ�������܂��B", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	}
	//��̂ǂ�ł��Ȃ����
	MessageBox(NULL, "�����s���̃G���[�ł�", "�G���[", 0);
	puterror(HSPERR_UNSUPPORTED_FUNCTION);
}


void retmeserr16(cl_int ret)
{
	switch (ret)
	{							//����
	case CL_INVALID_CONTEXT:
		MessageBox(NULL, "CL_INVALID_CONTEXT\ncontext ���L����OpenCL�R���e�L�X�g�łȂ��Ƃ��B", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_VALUE:
		MessageBox(NULL, "CL_INVALID_VALUE\ndevice_list �� NULL �̂Ƃ��B\n���邢�́Anum_devices ��0�ł���Ƃ��B\nlengths �������� binaries �� NULL �̂Ƃ��B\n���邢�́Alengths[i] �̂����ꂩ��0��������\nbinaries[i] �̂����ꂩ�� NULL �̂Ƃ��B", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_DEVICE:
		MessageBox(NULL, "CL_INVALID_DEVICE\ndevice_list �Ŏw�肳��Ă���OpenCL�f�o�C�X��\ncontext �Ɗ֘A�t����ꂽ�f�o�C�X�łȂ��Ƃ��B", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_INVALID_BINARY:
		MessageBox(NULL, "CL_INVALID_BINARY\n�����ꂩ�̃f�o�C�X�Ńv���O�����o�C�i�����L���Ȃ��̂łȂ��������Ƃ��B\nbinary_status�ɂ��ꂼ��̃f�o�C�X�ł̏󋵂��Ԃ���܂��B", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_OUT_OF_RESOURCES:
		MessageBox(NULL, "CL_OUT_OF_RESOURCES\n�f�o�C�X��ł̃��\�[�X�m�ۂɎ��s�����Ƃ��B", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	case CL_OUT_OF_HOST_MEMORY:
		MessageBox(NULL, "CL_OUT_OF_HOST_MEMORY\n�z�X�g��ł̃��\�[�X�m�ۂɎ��s�����Ƃ��B", "�G���[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
		break;
	}
	//��̂ǂ�ł��Ȃ����
	MessageBox(NULL, "�����s���̃G���[�ł�", "�G���[", 0);
	puterror(HSPERR_UNSUPPORTED_FUNCTION);
}








void mes(const char* strc, int val)
{
	char c[10];
	c[0] = val % 1000000000 / 100000000 + 48;
	c[1] = val % 100000000 / 10000000 + 48;
	c[2] = val % 10000000 / 1000000 + 48;
	c[3] = val % 1000000 / 100000 + 48;
	c[4] = val % 100000 / 10000 + 48;
	c[5] = val % 10000 / 1000 + 48;
	c[6] = val % 1000 / 100 + 48;
	c[7] = val % 100 / 10 + 48;
	c[8] = val % 10 + 48;
	c[9] = 0;
	MessageBox(NULL, c, strc, 0);
}





















































































































std::string SGEMM_SOURCE0 = R"EOB(
__kernel void floatGEMM_a(int M,int N,int K,__global float*A,__global float*B,__global float*C)
{
int threadIdxx=get_local_id(0);
int threadIdxy=get_local_id(1);
int blockIdxx=get_group_id(0);
int blockIdxy=get_group_id(1);
int tidn=threadIdxx;
int tidm=threadIdxy;
int offsetN=TSN*blockIdxx+tidn;
int offsetM=TSM*blockIdxy+tidm;
if (blockIdxy==M/128) offsetM-=128-M%128;
if (blockIdxx==N/128) offsetN-=128-N%128;
int Boffset=tidm/2*N+(tidm%2)*64+offsetN;
int Aoffset=tidn+offsetM*K;
__local float4 Asub[TSN/4][TSK];
__local float4 Bsub[TSM/4][TSK];
float4 Areg;
float4 Breg[2];
float acc[WPTN*WPTM];
for (int wn=0; wn < WPTN; wn++) {
for (int wm=0; wm < WPTM; wm++) {
acc[wn*8+wm]=0.0f;
}
}
int numTiles=K/TSK;
for (int t=0; t < numTiles; t++) {
float4 dt;
dt.x=A[Aoffset];Aoffset+=16*K;
dt.y=A[Aoffset];Aoffset+=16*K;
dt.z=A[Aoffset];Aoffset+=16*K;
dt.w=A[Aoffset];Aoffset+=16*K;
Asub[tidm][tidn]=dt;
dt.x=A[Aoffset];Aoffset+=16*K;
dt.y=A[Aoffset];Aoffset+=16*K;
dt.z=A[Aoffset];Aoffset+=16*K;
dt.w=A[Aoffset];Aoffset-=112*K-16;
Asub[tidm+16][tidn]=dt;
dt.x=B[Boffset];
dt.y=B[Boffset+16];
dt.z=B[Boffset+32];
dt.w=B[Boffset+48];Boffset+=8*N;
Bsub[tidm][tidn]=dt;
dt.x=B[Boffset];
dt.y=B[Boffset+16];
dt.z=B[Boffset+32];
dt.w=B[Boffset+48];
Bsub[tidm+16][tidn]=dt;
Boffset+=8*N;
barrier(CLK_LOCAL_MEM_FENCE);
for (int k=0; k < TSK; k++) {
Breg[0]=Bsub[k*2][tidn];
Areg=Asub[tidm][k];
Breg[1]=Bsub[k*2+1][tidn];
acc[0]+=Areg.x*Breg[0].x;
acc[1]+=Areg.x*Breg[0].y;
acc[2]+=Areg.x*Breg[0].z;
acc[3]+=Areg.x*Breg[0].w;
acc[4]+=Areg.x*Breg[1].x;
acc[5]+=Areg.x*Breg[1].y;
acc[6]+=Areg.x*Breg[1].z;
acc[7]+=Areg.x*Breg[1].w;
acc[8+0]+=Areg.y*Breg[0].x;
acc[8+1]+=Areg.y*Breg[0].y;
acc[8+2]+=Areg.y*Breg[0].z;
acc[8+3]+=Areg.y*Breg[0].w;
acc[8+4]+=Areg.y*Breg[1].x;
acc[8+5]+=Areg.y*Breg[1].y;
acc[8+6]+=Areg.y*Breg[1].z;
acc[8+7]+=Areg.y*Breg[1].w;
acc[16+0]+=Areg.z*Breg[0].x;
acc[16+1]+=Areg.z*Breg[0].y;
acc[16+2]+=Areg.z*Breg[0].z;
acc[16+3]+=Areg.z*Breg[0].w;
acc[16+4]+=A)EOB";


std::string SGEMM_SOURCE1 = R"EOB(reg.z*Breg[1].x;
acc[16+5]+=Areg.z*Breg[1].y;
acc[16+6]+=Areg.z*Breg[1].z;
acc[16+7]+=Areg.z*Breg[1].w;
acc[24+0]+=Areg.w*Breg[0].x;
acc[24+1]+=Areg.w*Breg[0].y;
acc[24+2]+=Areg.w*Breg[0].z;
acc[24+3]+=Areg.w*Breg[0].w;
acc[24+4]+=Areg.w*Breg[1].x;
acc[24+5]+=Areg.w*Breg[1].y;
acc[24+6]+=Areg.w*Breg[1].z;
acc[24+7]+=Areg.w*Breg[1].w;
Areg=Asub[tidm+16][k];
acc[32+0]+=Areg.x*Breg[0].x;
acc[32+1]+=Areg.x*Breg[0].y;
acc[32+2]+=Areg.x*Breg[0].z;
acc[32+3]+=Areg.x*Breg[0].w;
acc[32+4]+=Areg.x*Breg[1].x;
acc[32+5]+=Areg.x*Breg[1].y;
acc[32+6]+=Areg.x*Breg[1].z;
acc[32+7]+=Areg.x*Breg[1].w;
acc[40+0]+=Areg.y*Breg[0].x;
acc[40+1]+=Areg.y*Breg[0].y;
acc[40+2]+=Areg.y*Breg[0].z;
acc[40+3]+=Areg.y*Breg[0].w;
acc[40+4]+=Areg.y*Breg[1].x;
acc[40+5]+=Areg.y*Breg[1].y;
acc[40+6]+=Areg.y*Breg[1].z;
acc[40+7]+=Areg.y*Breg[1].w;
acc[48+0]+=Areg.z*Breg[0].x;
acc[48+1]+=Areg.z*Breg[0].y;
acc[48+2]+=Areg.z*Breg[0].z;
acc[48+3]+=Areg.z*Breg[0].w;
acc[48+4]+=Areg.z*Breg[1].x;
acc[48+5]+=Areg.z*Breg[1].y;
acc[48+6]+=Areg.z*Breg[1].z;
acc[48+7]+=Areg.z*Breg[1].w;
acc[56+0]+=Areg.w*Breg[0].x;
acc[56+1]+=Areg.w*Breg[0].y;
acc[56+2]+=Areg.w*Breg[0].z;
acc[56+3]+=Areg.w*Breg[0].w;
acc[56+4]+=Areg.w*Breg[1].x;
acc[56+5]+=Areg.w*Breg[1].y;
acc[56+6]+=Areg.w*Breg[1].z;
acc[56+7]+=Areg.w*Breg[1].w;
}
barrier(CLK_LOCAL_MEM_FENCE);
}
int km=K%16;
int maxAidx=M*K-1;
int maxBidx=N*K-1;
int BoffsetDmy=min(Boffset, maxBidx);
Areg.x=A[Aoffset]; Aoffset+=16*K;
Breg[0].x=B[BoffsetDmy]; BoffsetDmy=min(Boffset+16, maxBidx);
Areg.y=A[Aoffset]; Aoffset+=16*K;
Breg[0].y=B[BoffsetDmy]; BoffsetDmy=min(Boffset+32, maxBidx);
Areg.z=A[Aoffset]; Aoffset+=16*K;
Breg[0].z=B[BoffsetDmy]; BoffsetDmy=min(Boffset+48, maxBidx);
Areg.w=A[Aoffset]; Aoffset+=16*K;
Breg[0].w=B[BoffsetDmy]; Boffset=min(Boffset+8*N, maxBidx);
Asub[tidm][tidn]=Areg;
Bsub[tidm][tidn]=Breg[0];
Areg.x=A[Aoffset]; Aoffset+=16*K;
Breg[0].x=B[Boffset]; BoffsetDmy=min(Boffset+16, maxBidx);
Areg.y=A[Aoffset]; Aoffset+=16*K;
Breg[0].y=B[Bof)EOB";


std::string SGEMM_SOURCE2 = R"EOB(fsetDmy]; BoffsetDmy=min(Boffset+32, maxBidx);
Areg.z=A[Aoffset]; Aoffset=min(Aoffset+16*K, maxAidx);
Breg[0].z=B[BoffsetDmy]; BoffsetDmy=min(Boffset+48, maxBidx);
Areg.w=A[Aoffset];
Breg[0].w=B[BoffsetDmy];
Asub[tidm+16][tidn]=Areg;
Bsub[tidm+16][tidn]=Breg[0];
barrier(CLK_LOCAL_MEM_FENCE);
for (int k=0; k < km; k++) {
Breg[0]=Bsub[k*2][tidn];
Areg=Asub[tidm][k];
Breg[1]=Bsub[k*2+1][tidn];
acc[0]+=Areg.x*Breg[0].x;
acc[1]+=Areg.x*Breg[0].y;
acc[2]+=Areg.x*Breg[0].z;
acc[3]+=Areg.x*Breg[0].w;
acc[4]+=Areg.x*Breg[1].x;
acc[5]+=Areg.x*Breg[1].y;
acc[6]+=Areg.x*Breg[1].z;
acc[7]+=Areg.x*Breg[1].w;
acc[8+0]+=Areg.y*Breg[0].x;
acc[8+1]+=Areg.y*Breg[0].y;
acc[8+2]+=Areg.y*Breg[0].z;
acc[8+3]+=Areg.y*Breg[0].w;
acc[8+4]+=Areg.y*Breg[1].x;
acc[8+5]+=Areg.y*Breg[1].y;
acc[8+6]+=Areg.y*Breg[1].z;
acc[8+7]+=Areg.y*Breg[1].w;
acc[16+0]+=Areg.z*Breg[0].x;
acc[16+1]+=Areg.z*Breg[0].y;
acc[16+2]+=Areg.z*Breg[0].z;
acc[16+3]+=Areg.z*Breg[0].w;
acc[16+4]+=Areg.z*Breg[1].x;
acc[16+5]+=Areg.z*Breg[1].y;
acc[16+6]+=Areg.z*Breg[1].z;
acc[16+7]+=Areg.z*Breg[1].w;
acc[24+0]+=Areg.w*Breg[0].x;
acc[24+1]+=Areg.w*Breg[0].y;
acc[24+2]+=Areg.w*Breg[0].z;
acc[24+3]+=Areg.w*Breg[0].w;
acc[24+4]+=Areg.w*Breg[1].x;
acc[24+5]+=Areg.w*Breg[1].y;
acc[24+6]+=Areg.w*Breg[1].z;
acc[24+7]+=Areg.w*Breg[1].w;
Areg=Asub[tidm+16][k];
acc[32+0]+=Areg.x*Breg[0].x;
acc[32+1]+=Areg.x*Breg[0].y;
acc[32+2]+=Areg.x*Breg[0].z;
acc[32+3]+=Areg.x*Breg[0].w;
acc[32+4]+=Areg.x*Breg[1].x;
acc[32+5]+=Areg.x*Breg[1].y;
acc[32+6]+=Areg.x*Breg[1].z;
acc[32+7]+=Areg.x*Breg[1].w;
acc[40+0]+=Areg.y*Breg[0].x;
acc[40+1]+=Areg.y*Breg[0].y;
acc[40+2]+=Areg.y*Breg[0].z;
acc[40+3]+=Areg.y*Breg[0].w;
acc[40+4]+=Areg.y*Breg[1].x;
acc[40+5]+=Areg.y*Breg[1].y;
acc[40+6]+=Areg.y*Breg[1].z;
acc[40+7]+=Areg.y*Breg[1].w;
acc[48+0]+=Areg.z*Breg[0].x;
acc[48+1]+=Areg.z*Breg[0].y;
acc[48+2]+=Areg.z*Breg[0].z;
acc[48+3]+=Areg.z*Breg[0].w;
acc[48+4]+=Areg.z*Breg[1].x;
acc[48+5]+=Areg.z*Breg[1].y;
acc[48+6]+=Areg.z*Breg[1].z;
)EOB";


std::string SGEMM_SOURCE3 = R"EOB(acc[48+7]+=Areg.z*Breg[1].w;
acc[56+0]+=Areg.w*Breg[0].x;
acc[56+1]+=Areg.w*Breg[0].y;
acc[56+2]+=Areg.w*Breg[0].z;
acc[56+3]+=Areg.w*Breg[0].w;
acc[56+4]+=Areg.w*Breg[1].x;
acc[56+5]+=Areg.w*Breg[1].y;
acc[56+6]+=Areg.w*Breg[1].z;
acc[56+7]+=Areg.w*Breg[1].w;
}
for (int wn=0; wn < 8; wn++) {
int globalCol=offsetN+wn*RTSN;
for (int wm=0; wm < 8; wm++) {
int globalRow=offsetM+wm*RTSM;
C[globalRow*N+globalCol]=acc[wm*8+wn];
}
}
}
__kernel void floatGEMM_k(int M,int N,int K,__global float*A,__global float*B,__global float*C)
{
int threadIdxx=get_local_id(0);
int threadIdxy=get_local_id(1);
int blockIdxx=get_group_id(0);
int blockIdxy=get_group_id(1);
int tidn=threadIdxx;
int tidm=threadIdxy;
int offsetN=TSN*blockIdxx+tidn;
int offsetM=TSM*blockIdxy+tidm;
if (blockIdxy==M/128) offsetM-=128-M%128;
if (blockIdxx==N/128) offsetN-=128-N%128;
int Boffset=tidm/2*N+(tidm%2)*64+offsetN;
int Aoffset=tidn+offsetM*K;
__local float4 Asub[TSN/4][TSK];
__local float4 Bsub[TSM/4][TSK];
float4 Areg;
float4 Breg[2];
float acc[WPTN*WPTM];
for (int wn=0; wn<WPTN; wn++) {
for (int wm=0; wm<WPTM; wm++) {
acc[wn*8+wm]=0.0f;
}
}
int numTiles=K/TSK;
int tid=tidm*16+tidn;
for (int t=0; t<numTiles; t++) {
float4 dt;
dt.x=A[Aoffset];Aoffset+=16*K;
dt.y=A[Aoffset];Aoffset+=16*K;
dt.z=A[Aoffset];Aoffset+=16*K;
dt.w=A[Aoffset];Aoffset+=16*K;
Asub[tidm][tidn]=dt;
dt.x=A[Aoffset];Aoffset+=16*K;
dt.y=A[Aoffset];Aoffset+=16*K;
dt.z=A[Aoffset];Aoffset+=16*K;
dt.w=A[Aoffset];Aoffset-=112*K-16;
Asub[tidm+16][tidn]=dt;
dt.x=B[Boffset];
dt.y=B[Boffset+16];
dt.z=B[Boffset+32];
dt.w=B[Boffset+48];Boffset+=8*N;
Bsub[tidm][tidn]=dt;
dt.x=B[Boffset];
dt.y=B[Boffset+16];
dt.z=B[Boffset+32];
dt.w=B[Boffset+48];
Bsub[tidm+16][tidn]=dt;
Boffset+=8*N;
barrier(CLK_LOCAL_MEM_FENCE);
int tidnk=tidn;
for (int k=0; k<TSK; k++) {
Breg[0]=Bsub[k*2][tidn];
Areg=Asub[tidm][k];
Breg[1]=Bsub[k*2+1][tidn];
acc[0]+=Areg.x*Breg[0].x;
acc[1]+=Areg.x*Breg[0].y;
acc[2]+=Areg.x*Breg[0].z;
acc[3]+=Areg.x*Breg)EOB";


std::string SGEMM_SOURCE4 = R"EOB([0].w;
acc[4]+=Areg.x*Breg[1].x;
acc[5]+=Areg.x*Breg[1].y;
acc[6]+=Areg.x*Breg[1].z;
acc[7]+=Areg.x*Breg[1].w;
acc[8+0]+=Areg.y*Breg[0].x;
acc[8+1]+=Areg.y*Breg[0].y;
acc[8+2]+=Areg.y*Breg[0].z;
acc[8+3]+=Areg.y*Breg[0].w;
acc[8+4]+=Areg.y*Breg[1].x;
acc[8+5]+=Areg.y*Breg[1].y;
acc[8+6]+=Areg.y*Breg[1].z;
acc[8+7]+=Areg.y*Breg[1].w;
acc[16+0]+=Areg.z*Breg[0].x;
acc[16+1]+=Areg.z*Breg[0].y;
acc[16+2]+=Areg.z*Breg[0].z;
acc[16+3]+=Areg.z*Breg[0].w;
acc[16+4]+=Areg.z*Breg[1].x;
acc[16+5]+=Areg.z*Breg[1].y;
acc[16+6]+=Areg.z*Breg[1].z;
acc[16+7]+=Areg.z*Breg[1].w;
acc[24+0]+=Areg.w*Breg[0].x;
acc[24+1]+=Areg.w*Breg[0].y;
acc[24+2]+=Areg.w*Breg[0].z;
acc[24+3]+=Areg.w*Breg[0].w;
acc[24+4]+=Areg.w*Breg[1].x;
acc[24+5]+=Areg.w*Breg[1].y;
acc[24+6]+=Areg.w*Breg[1].z;
acc[24+7]+=Areg.w*Breg[1].w;
Areg=Asub[tidm+16][k];
acc[32+0]+=Areg.x*Breg[0].x;
acc[32+1]+=Areg.x*Breg[0].y;
acc[32+2]+=Areg.x*Breg[0].z;
acc[32+3]+=Areg.x*Breg[0].w;
acc[32+4]+=Areg.x*Breg[1].x;
acc[32+5]+=Areg.x*Breg[1].y;
acc[32+6]+=Areg.x*Breg[1].z;
acc[32+7]+=Areg.x*Breg[1].w;
acc[40+0]+=Areg.y*Breg[0].x;
acc[40+1]+=Areg.y*Breg[0].y;
acc[40+2]+=Areg.y*Breg[0].z;
acc[40+3]+=Areg.y*Breg[0].w;
acc[40+4]+=Areg.y*Breg[1].x;
acc[40+5]+=Areg.y*Breg[1].y;
acc[40+6]+=Areg.y*Breg[1].z;
acc[40+7]+=Areg.y*Breg[1].w;
acc[48+0]+=Areg.z*Breg[0].x;
acc[48+1]+=Areg.z*Breg[0].y;
acc[48+2]+=Areg.z*Breg[0].z;
acc[48+3]+=Areg.z*Breg[0].w;
acc[48+4]+=Areg.z*Breg[1].x;
acc[48+5]+=Areg.z*Breg[1].y;
acc[48+6]+=Areg.z*Breg[1].z;
acc[48+7]+=Areg.z*Breg[1].w;
acc[56+0]+=Areg.w*Breg[0].x;
acc[56+1]+=Areg.w*Breg[0].y;
acc[56+2]+=Areg.w*Breg[0].z;
acc[56+3]+=Areg.w*Breg[0].w;
acc[56+4]+=Areg.w*Breg[1].x;
acc[56+5]+=Areg.w*Breg[1].y;
acc[56+6]+=Areg.w*Breg[1].z;
acc[56+7]+=Areg.w*Breg[1].w;
}
barrier(CLK_LOCAL_MEM_FENCE);
}
for (int wn=0; wn<8; wn++) {
int globalCol=offsetN+wn*RTSN;
for (int wm=0; wm<8; wm++) {
int globalRow=offsetM+wm*RTSM;
C[globalRow*N+globalCol]=acc[wm*8+wn];
}
}
}
__kernel void floatGEMM_sm)EOB";


std::string SGEMM_SOURCE5 = R"EOB(all(int M,int N,int K,__global float*A,__global float*B,__global float*C)
{
int threadIdxx=get_local_id(0);
int threadIdxy=get_local_id(1);
int blockIdxx=get_group_id(0);
int blockIdxy=get_group_id(1);
int tidn=threadIdxx;
int tidm=threadIdxy;
int offsetN=TSN*blockIdxx+tidn;
int offsetM=TSM*blockIdxy+tidm;
int Boffset=tidm/2*N+(tidm%2)*64+offsetN;
int Aoffset=tidn+offsetM*K;
__local float4 Asub[TSN*TSK/4];
__local float4 Bsub[TSK*TSM/4];
float4 Areg;
float4 Breg[2];
float acc[WPTN*WPTM];
for (int wn=0; wn < WPTN; wn++) {
for (int wm=0; wm < WPTM; wm++) {
acc[wn*8+wm]=0.0f;
}
}
int tid=tidm*16+tidn;
int maxAidx=M*K-1;
int maxBidx=N*K-1;
int nowAoffset=min(Aoffset, maxAidx);
int nowBoffset=min(Boffset, maxBidx);
for (int t=0; t < K; t+=16) {
float4 dta;
float4 dtb;
dta.x=A[nowAoffset]; nowAoffset=min(Aoffset+16*K, maxAidx);
dtb.x=B[nowBoffset]; nowBoffset=min(Boffset+16, maxBidx);
dta.y=A[nowAoffset]; nowAoffset=min(Aoffset+32*K, maxAidx);
dtb.y=B[nowBoffset]; nowBoffset=min(Boffset+32, maxBidx);
dta.z=A[nowAoffset]; nowAoffset=min(Aoffset+48*K, maxAidx);
dtb.z=B[nowBoffset]; nowBoffset=min(Boffset+48, maxBidx);
dta.w=A[nowAoffset]; nowAoffset=min(Aoffset+64*K, maxAidx);
dtb.w=B[nowBoffset]; Boffset+=8*N; nowBoffset=min(Boffset, maxBidx);
Asub[tid]=dta;
Bsub[tid]=dtb;
dta.x=A[nowAoffset]; nowAoffset=min(Aoffset+80*K, maxAidx);
dtb.x=B[nowBoffset]; nowBoffset=min(Boffset+16, maxBidx);
dta.y=A[nowAoffset]; nowAoffset=min(Aoffset+96*K, maxAidx);
dtb.y=B[nowBoffset]; nowBoffset=min(Boffset+32, maxBidx);
dta.z=A[nowAoffset]; nowAoffset=min(Aoffset+112*K, maxAidx);
dtb.z=B[nowBoffset]; nowBoffset=min(Boffset+48, maxBidx);
dta.w=A[nowAoffset]; Aoffset+=16; nowAoffset=min(Aoffset, maxAidx);
dtb.w=B[nowBoffset]; Boffset+=8*N; nowBoffset=min(Boffset, maxBidx);
Asub[tid+256]=dta;
Bsub[tid+256]=dtb;
barrier(CLK_LOCAL_MEM_FENCE);
int tidnk=tidn;
int tidmk=tidm*16;
int kmin=min(K-t, 16);
for (int k=0; k < kmin; k++) {
Breg[0]=Bsub[tidnk]; tidnk+=16;
Areg=Asub[tidmk]; tidmk+)EOB";


std::string SGEMM_SOURCE6 = R"EOB(=256;
Breg[1]=Bsub[tidnk]; tidnk+=16;
acc[0]+=Areg.x*Breg[0].x;
acc[1]+=Areg.x*Breg[0].y;
acc[2]+=Areg.x*Breg[0].z;
acc[3]+=Areg.x*Breg[0].w;
acc[4]+=Areg.x*Breg[1].x;
acc[5]+=Areg.x*Breg[1].y;
acc[6]+=Areg.x*Breg[1].z;
acc[7]+=Areg.x*Breg[1].w;
acc[8+0]+=Areg.y*Breg[0].x;
acc[8+1]+=Areg.y*Breg[0].y;
acc[8+2]+=Areg.y*Breg[0].z;
acc[8+3]+=Areg.y*Breg[0].w;
acc[8+4]+=Areg.y*Breg[1].x;
acc[8+5]+=Areg.y*Breg[1].y;
acc[8+6]+=Areg.y*Breg[1].z;
acc[8+7]+=Areg.y*Breg[1].w;
acc[16+0]+=Areg.z*Breg[0].x;
acc[16+1]+=Areg.z*Breg[0].y;
acc[16+2]+=Areg.z*Breg[0].z;
acc[16+3]+=Areg.z*Breg[0].w;
acc[16+4]+=Areg.z*Breg[1].x;
acc[16+5]+=Areg.z*Breg[1].y;
acc[16+6]+=Areg.z*Breg[1].z;
acc[16+7]+=Areg.z*Breg[1].w;
acc[24+0]+=Areg.w*Breg[0].x;
acc[24+1]+=Areg.w*Breg[0].y;
acc[24+2]+=Areg.w*Breg[0].z;
acc[24+3]+=Areg.w*Breg[0].w;
acc[24+4]+=Areg.w*Breg[1].x;
acc[24+5]+=Areg.w*Breg[1].y;
acc[24+6]+=Areg.w*Breg[1].z;
acc[24+7]+=Areg.w*Breg[1].w;
Areg=Asub[tidmk]; tidmk-=255;
acc[32+0]+=Areg.x*Breg[0].x;
acc[32+1]+=Areg.x*Breg[0].y;
acc[32+2]+=Areg.x*Breg[0].z;
acc[32+3]+=Areg.x*Breg[0].w;
acc[32+4]+=Areg.x*Breg[1].x;
acc[32+5]+=Areg.x*Breg[1].y;
acc[32+6]+=Areg.x*Breg[1].z;
acc[32+7]+=Areg.x*Breg[1].w;
acc[40+0]+=Areg.y*Breg[0].x;
acc[40+1]+=Areg.y*Breg[0].y;
acc[40+2]+=Areg.y*Breg[0].z;
acc[40+3]+=Areg.y*Breg[0].w;
acc[40+4]+=Areg.y*Breg[1].x;
acc[40+5]+=Areg.y*Breg[1].y;
acc[40+6]+=Areg.y*Breg[1].z;
acc[40+7]+=Areg.y*Breg[1].w;
acc[48+0]+=Areg.z*Breg[0].x;
acc[48+1]+=Areg.z*Breg[0].y;
acc[48+2]+=Areg.z*Breg[0].z;
acc[48+3]+=Areg.z*Breg[0].w;
acc[48+4]+=Areg.z*Breg[1].x;
acc[48+5]+=Areg.z*Breg[1].y;
acc[48+6]+=Areg.z*Breg[1].z;
acc[48+7]+=Areg.z*Breg[1].w;
acc[56+0]+=Areg.w*Breg[0].x;
acc[56+1]+=Areg.w*Breg[0].y;
acc[56+2]+=Areg.w*Breg[0].z;
acc[56+3]+=Areg.w*Breg[0].w;
acc[56+4]+=Areg.w*Breg[1].x;
acc[56+5]+=Areg.w*Breg[1].y;
acc[56+6]+=Areg.w*Breg[1].z;
acc[56+7]+=Areg.w*Breg[1].w;
}
barrier(CLK_LOCAL_MEM_FENCE);
}
for (int wn=0; wn < 8; wn++) {
int globalRow=offset)EOB";


std::string SGEMM_SOURCE7 = R"EOB(N+wn*RTSN;
if (globalRow >=N) break;
for (int wm=0; wm < 8; wm++) {
int globalCol=offsetM+wm*RTSM;
if (globalCol >=M) break;
C[globalCol*N+globalRow]=acc[wm*8+wn];
}
}
}
__kernel void floatTrans(int M,int N,__global float*A,__global float*AT)
{
int threadIdxx=get_local_id(0);
int threadIdxy=get_local_id(1);
int blockIdxx=get_group_id(0);
int blockIdxy=get_group_id(1);
int tidn=threadIdxx;
int tidm=threadIdxy;
int tidoffset=(tidn+tidm)%16;
int offsetN=16*blockIdxx+tidn;
int offsetM=16*blockIdxy+tidm;
offsetN=min(offsetN, N-1);
offsetM=min(offsetM, M-1);
int woffsetN=16*blockIdxx+tidm;
int woffsetM=16*blockIdxy+tidn;
woffsetN=min(woffsetN, N-1);
woffsetM=min(woffsetM, M-1);
__local float sub[256];
sub[tidoffset+tidm*16]=A[offsetN+offsetM*N];
barrier(CLK_LOCAL_MEM_FENCE);
AT[woffsetN*M+woffsetM]=sub[tidoffset+tidn*16];
}
__kernel void floatGEMV(__global float*A,__global float*X,__global float*Y,int col,int sqrtflg){
int gi=get_group_id(0);
int i=get_local_id(0);
int li=i;
float r=0.0;
for(;i<col;i+=256)
r+=A[gi*col+i]*X[i];
__local float S[256];
S[li]=r;
i=128;
for(;i>0;i/=2)
{
barrier(CLK_LOCAL_MEM_FENCE);
if (li<i)S[li]+=S[li+i];
}
if (li==0){
if (sqrtflg==1)S[0]=sqrt(S[0]);
Y[gi]=S[0];
}
}
)EOB";

