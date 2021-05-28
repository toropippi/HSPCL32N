#regcmd "_hsp3cmdinit@4","HSPCL32N.dll",1
#cmd clfloat $FF

#regcmd "_hsp3cmdinit@4","HSPCL32N.dll"
#cmd clini	$000
#cmd clGetDevInfo_	$001
#cmd clbye				$002
#cmd clBuildProgram 		$003
#cmd clCreateKernel		$004
#cmd clSetKernel		$005
#cmd clCreateBuffer		$006
#cmd clWriteBuffer		$007
#cmd clReadBuffer		$008
#cmd clCopyBuffer		$009
#cmd clDoKernel			$00A
#cmd clReleaseKernel		$00B
#cmd clReleaseProgram		$00C
#cmd clReleaseMemObject		$00D
#cmd clfdim			$00E
#cmd clSetDev			$00F
#cmd clWaitTask			$010
#cmd clDoKrn1			$011
#cmd clDoKrn2			$012
#cmd clDoKrn3			$013



#module hspcl32n

#defcfunc clGetDevInfo
	sdim k,90000
	clGetDevInfo_ k
	strrep k,"ŠJ‹Æ","\n"
	return k


#defcfunc clGetDevName
	sdim k,90000
	clGetDevInfo_ k
	strrep k,"ŠJ‹Æ","\n"
	notesel k
	sdim asgrhset,256
	noteget asgrhset,3
	return asgrhset


#global
