#regcmd "_hsp3cmdinit@4","HSPCL32N.dll",1
#cmd clfloat $0FF



//#regcmd "_hsp3cmdinit@4","HSPCL32N.dll"

#cmd clGetDeviceCount $054
#cmd clGetSettingDevice $056
#cmd clCreateProgram $057
#cmd clCreateProgramWithSource $058
#cmd clCreateKernel $05A
#cmd clCreateBuffer $05E
#cmd clCreateBufferFrom $05F
#cmd clReadIndex_i32 $066
#cmd clReadIndex_fp $067
#cmd clReadIndex_dp $068
#cmd clGetSettingCommandQueue $075
#cmd clGetEventLogs $07A
#cmd clGetEventStatus $07B
#cmd clGetKernelName $079//A4
#cmd clGetDevName $0AF
#cmd clGet_NonBlocking_Status $088






#cmd clini				$050
#cmd clinit				$050
#cmd clGetDeviceInfo	$055
#cmd clbye				$0A2
#cmd clBuildProgram 	$0A3
//#cmd clCreateKernel		$079
//#cmd clCreateBuffer		$0A6
#cmd clfdim				$0AE
#cmd HCLdim_i32FromBuffer	$08C
#cmd HCLdim_fpFromBuffer	$08D
#cmd HCLdim_dpFromBuffer	$08E
#cmd clSetDev			$053
#cmd clSetDevice		$053
#cmd clReleaseProgram	$059
#cmd clSetKernel		$05B
#cmd clSetKrns			$05C
#cmd clReleaseKernel	$05D
#cmd clWriteBuffer		$060
#cmd clReadBuffer		$061
#cmd clWriteBuffer_NonBlocking	$086
#cmd clReadBuffer_NonBlocking	$087
#cmd clCopyBuffer		$062
#cmd clFillBuffer_i32	$063
#cmd clFillBuffer_fp	$064
#cmd clFillBuffer_dp	$08B
#cmd clReleaseMemObject	$065
#cmd clReleaseBuffer	$065
#cmd clWriteIndex_i32	$069
#cmd clWriteIndex_fp	$06A
#cmd clWriteIndex_dp	$06B
#cmd clCall				$06C
#cmd clDoKrn1			$06D
#cmd clDoKrn2			$052
#cmd clDoKrn3			$07E
#cmd clDoKernel			$06E
#cmd clDoKernel_sub		$06F
#cmd clWaitTask			$070
#cmd clFinish			$070
#cmd clFlush			$071
#cmd _ExclSetCommandQueueMax	$072
#cmd _ExclSetCommandQueueProperties $073
#cmd clSetCommandQueue	 $074
#cmd _ExclSetEventMax	$076
#cmd clSetWaitEvent 	$077
#cmd clSetWaitEvents	$078
#cmd clWaitForEvent	$07C
#cmd clWaitForEvents	$07D
#cmd clCreateUserEvent	$089
#cmd clSetUserEventStatus	$08A
#cmd _ConvRGBtoBGR	$083
#cmd _ConvRGBAtoRGB	$084
#cmd _ConvRGBtoRGBA	$085






#module hspcl32n

#defcfunc clGetDeviceInfo_s int a
	ptrk=0:szs=0
	clGetDeviceInfo a,ptrk,szs
	if szs!=0{
		dupptr out,ptrk,szs,2;2文字列型、ptrkは整数ポインタ、szsはサイズ、「out」はここで作成された新しい変数
		//この時点でoutは、C++側で定義している変数のポインタを参照している→今後内容が変わる可能性
		sdim retstr,szs
		retstr=out
		return retstr
	}else{
		return ""
	}
#defcfunc clGetDeviceInfo_i int a,int index
	ptrk=0:szs=0
	clGetDeviceInfo a,ptrk,szs
	if szs!=0{
		dupptr out,ptrk,szs,4;4整数型、ptrkは整数ポインタ、szsはサイズ、変数名はここで作成された新しい変数
		return out.index
	}else{
		return 0
	}

#deffunc convRGBtoBGR array a,array b
	sizea=varsize(a)
	sizeb=varsize(b)
	if sizea>sizeb:sizea=sizeb
	_convRGBtoBGR a,b,sizea
	return
#deffunc convRGBAtoRGB array a,array b
	sizea=varsize(a)
	sizeb=(varsize(b)/3)*4
	if sizea>sizeb:sizea=sizeb
	_convRGBAtoRGB a,b,sizea
	return
#deffunc convRGBtoRGBA array a,array b,int toumeiflg
	if toumeiflg:tmpr=ginfo_r:tmpg=ginfo_g:tmpb=ginfo_b
	sizea=varsize(a)
	sizeb=(varsize(b)/4)*3
	if sizea>sizeb:sizea=sizeb
	if toumeiflg=0:_convRGBtoRGBA a,b,sizea,0,0,0,0:else:_convRGBtoRGBA a,b,sizea,1,tmpr,tmpg,tmpb
	return
#global