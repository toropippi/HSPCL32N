//event�̃^�C���X�^���v��\�����郂�W���[��
//���̃R�[�h����Q�Ƃ����
#ifndef clinit

#include "hspcl32n.as"
	clinit
	
	source={"
__kernel void vecAdd(__global int* a,__global int* b,__global int* c)
{
    uint gid = get_global_id(0);
    c[gid] = a[gid] + b[gid];
}
	"}
	prg=clCreateProgramWithSource(source)//OpenCL�̃R�[�h�������ꂽstr�𒼐ړ���邱�Ƃ��ł���
	krn=clCreateKernel(prg,"vecAdd")
	
	n=65536
	
	dim host_A,n
	dim host_B,n
	dim host_C,n
		repeat n
		host_A.cnt=cnt
		host_B.cnt=cnt
		loop

	clmem_A=clCreateBufferFrom(host_A)
	clmem_B=clCreateBufferFrom(host_B)
	clmem_C=clCreateBuffer(n*4)
	
	clSetKrns krn,clmem_A,clmem_B,clmem_C
	
	clDoKrn1 krn,n,64,0//event_id 0
	
	clFinish
	clReadBuffer clmem_C,host_C,n*4,,,,1//event_id 1
	
	//event���玞�ԏ����擾�B�Ԃ�l��64bit int
	screen 0,640,480,0
	ViewEvents1 2
	screen 1,640,480,0
	ViewEvents2 2
	screen 2,640,480,0
	ViewEvents3 2
	stop

#endif






#module ViewTimeStamp




#defcfunc IntStattoDouble int a
	retd=1.0*a
	if retd<0.0:retd+=4294967296.0
	d1=1.0*stat
	if d1<0.0:d1+=4294967296.0
	retd+=4294967296.0*d1
	return retd


#deffunc ViewEvents1 int event_num
	if event_num<=0:return
	//�܂��͂��ׂĂ̌v�Z���Ԃ��擾
	dim kinfo,event_num
	sdim kinfos,128,event_num
	ddim start_time,event_num
	ddim end_time,event_num
		repeat event_num
		start_time.cnt=IntStattoDouble(clGetEventLogs(cnt,6))//6��CL_PROFILING_COMMAND_START��time
		end_time.cnt=IntStattoDouble(clGetEventLogs(cnt,7))//7��CL_PROFILING_COMMAND_END��time
		kinfo.cnt=clGetEventLogs(cnt,0)//0��event���Ȃ�̃R�}���h���������擾
		if kinfo.cnt==CL_COMMAND_WRITE_BUFFER  :kinfos.cnt="WRITE_BUFFER"
		if kinfo.cnt==CL_COMMAND_READ_BUFFER   :kinfos.cnt="READ_BUFFER"
		if kinfo.cnt==CL_COMMAND_NDRANGE_KERNEL:kinfos.cnt="KERNEL"
		loop
		
	offset=start_time.0
	
		repeat event_num
		start_time.cnt-=offset
		end_time.cnt-=offset
		loop

	scalex=end_time.(event_num-1)

	//�v�Z���Ԃ��O���t�ŕ\��
	//��
	line 30,30,30,450
	pos 3,240

	line 30,450,620,450
	line 620,450,615,447
	line 620,450,615,453
	pos 270,460
	font msgothic,12
	mes "����(��s)"
		repeat 9
		line 30+cnt*580/8,450,30+cnt*580/8,445
		pos 30+cnt*580/8,450
		mes int(scalex*cnt/8000)
		loop

	//���s�P�ʂŕ\�����Ă���
		repeat event_num
			hsvcolor rnd(192),200,200
			lx=start_time.cnt*580/scalex+30
			rx=end_time.cnt*580/scalex+30
			boxf int(lx),200,int(rx),240
			pos int(lx),190
			color 0,0,0
			mes kinfos.cnt
		loop
	return







#deffunc ViewEvents2 int event_num
	if event_num<=0:return
	//�܂��͂��ׂĂ̌v�Z���Ԃ��擾
	dim kinfo,event_num
	sdim kinfos,128,event_num
	ddim qued_time,event_num
	ddim subd_time,event_num
	ddim start_time,event_num
	ddim end_time,event_num
		repeat event_num
		qued_time.cnt=IntStattoDouble(clGetEventLogs(cnt,4))//4��CL_PROFILING_COMMAND_QUEUED��time
		subd_time.cnt=IntStattoDouble(clGetEventLogs(cnt,5))//5��CL_PROFILING_COMMAND_SUBMIT��time
		start_time.cnt=IntStattoDouble(clGetEventLogs(cnt,6))//6��CL_PROFILING_COMMAND_START��time
		end_time.cnt=IntStattoDouble(clGetEventLogs(cnt,7))//7��CL_PROFILING_COMMAND_END��time
		
		kinfo.cnt=clGetEventLogs(cnt,0)//��event���Ȃ�̃R�}���h���������擾
		if kinfo.cnt==CL_COMMAND_WRITE_BUFFER  :kinfos.cnt="WRITE_BUFFER"
		if kinfo.cnt==CL_COMMAND_READ_BUFFER   :kinfos.cnt="READ_BUFFER"
		if kinfo.cnt==CL_COMMAND_NDRANGE_KERNEL:kinfos.cnt="KERNEL"
		loop
		
	offset=qued_time.0
	
		repeat event_num
		start_time.cnt-=offset
		end_time.cnt-=offset
		qued_time.cnt-=offset
		subd_time.cnt-=offset
		loop

	scalex=end_time.(event_num-1)
	
	//�v�Z���Ԃ��O���t�ŕ\��
	//��
	line 30,30,30,450
	pos 3,240

	line 30,450,620,450
	line 620,450,615,447
	line 620,450,615,453
	pos 270,460
	font msgothic,12
	mes "����(��s)"
		repeat 9
		line 30+cnt*580/8,450,30+cnt*580/8,445
		pos 30+cnt*580/8,450
		mes int(scalex*cnt/8000)
		loop
		

	//���s�P�ʂŕ\�����Ă���
		repeat event_num
			hsvcolor rnd(192),200,200
			//�܂����s���Ԃ�\��
			lx=start_time.cnt*580/scalex+30
			rx=end_time.cnt*580/scalex+30
			boxf int(lx),100+cnt*50,int(rx),130+cnt*50
			
			//�����\��
			color 0,0,0
			pos int(lx),90+cnt*50
			mes kinfos.cnt
			
			//�L���[�ɓ����ꂽ���Ԃ�submit���ꂽ���ԕ\��
			color 100,200,0
			lx=qued_time.cnt*580/scalex+30
			line int(lx),100+cnt*50,int(lx),130+cnt*50
			color 100,100,0
			lx=subd_time.cnt*580/scalex+30
			line int(lx),100+cnt*50,int(lx),130+cnt*50
		loop
	return





#deffunc ViewEvents3 int event_num
	if event_num<=0:return
	//�܂��͂��ׂĂ̌v�Z���Ԃ��擾
	dim kinfo,event_num
	sdim kinfos,128,event_num
	ddim start_time,event_num
	ddim end_time,event_num
	dim cmdq_no,event_num
	offset=18446744073709551616.0
		repeat event_num
		start_time.cnt=IntStattoDouble(clGetEventLogs(cnt,6))//6��CL_PROFILING_COMMAND_START��time
		end_time.cnt=IntStattoDouble(clGetEventLogs(cnt,7))//7��CL_PROFILING_COMMAND_END��time
		cmdq_no.cnt=clGetEventLogs(cnt,3)//3�͉��Ԃ�command que�Ŏ��s���ꂽ���擾
		kinfo.cnt=clGetEventLogs(cnt,0)//0��event���Ȃ�̃R�}���h���������擾
		if kinfo.cnt==CL_COMMAND_WRITE_BUFFER  :kinfos.cnt="WRITE_BUFFER"
		if kinfo.cnt==CL_COMMAND_READ_BUFFER   :kinfos.cnt="READ_BUFFER"
		if kinfo.cnt==CL_COMMAND_NDRANGE_KERNEL:kinfos.cnt="KERNEL"
		//MIN
		if offset>start_time.cnt:offset=start_time.cnt
		loop

	scalex=-18446744073709551616.0
		repeat event_num
		start_time.cnt-=offset
		end_time.cnt-=offset
		//MAX
		if scalex<end_time.cnt:scalex=end_time.cnt
		loop
	
	//�v�Z���Ԃ��O���t�ŕ\��
	//��
	line 30,30,30,450
	line 30,30,27,35
	line 30,30,33,35
	pos 0,240
	font msgothic,12
	mes "Queue\n�ԍ�"

	line 30,450,620,450
	line 620,450,615,447
	line 620,450,615,453
	pos 270,460
	mes "����(��s)"
		repeat 9
		line 30+cnt*580/8,450,30+cnt*580/8,445
		pos 30+cnt*580/8,450
		mes int(scalex*cnt/8000)
		loop

	//���s�P�ʂŕ\�����Ă���
		repeat event_num
			hsvcolor rnd(192),200,200
			//�܂����s���Ԃ�\��
			lx=start_time.cnt*580/scalex+30
			rx=end_time.cnt*580/scalex+30
			yy=270-cmdq_no.cnt*50
			boxf int(lx),yy,int(rx),yy+30
			
			//�����\��
			color 0,0,0
			pos int(lx),yy-10
			mes kinfos.cnt
		loop
	return




#global