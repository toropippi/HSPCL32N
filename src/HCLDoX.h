void HCLDoXc(void);
void HCLDoXi(void);
void HCLDoXl(void);
void HCLDoXf(void);
void HCLDoXd(void);


int isupper_lower(unsigned char c)
{
	if ((c >= 65) & (c <= 90)) return 1;
	if ((c >= 97) & (c <= 122)) return 2;
	if (c == 95) return 3;
	if ((c >= 48) & (c <= 57)) return 4;
	return 0;
}

std::string CodeRefine(std::string sor, int typeflg,int* argt,int argcnt,int &_OUT_)
{
	std::string stype[8];
	stype[0] = "char";
	stype[1] = "int";
	stype[2] = "long";
	stype[3] = "float";
	stype[4] = "double";
	stype[5] = "uchar";
	stype[6] = "uint";
	stype[7] = "ulong";
	std::string Aa = "A";

	sor = " " + sor + "     ";
	size_t len = sor.size();

	int argcnt2 = 0;
	int globalnum[26];
	int privatenum[26];
	int C[10];
	int UC[10];
	int I[10];
	int UI[10];
	int L[10];
	int UL[10];
	int F[10];
	int D[10];
	int S[10];
	for (int i = 0; i < 26; i++) { globalnum[i] = 0; privatenum[i] = 0;}
	for (int i = 0; i < 10; i++) { C[i] = UC[i] = I[i] = UI[i] = L[i] = UL[i] = F[i] = D[i] = S[i] = 0; }

	for (size_t i = 1; i < len - 5; i++)
	{
		unsigned char c = sor[i];
		if (c >= 128) {
			i++;
			continue;
		}

		int ci = isupper_lower(c);
		if (ci == 1)
		{
			if (isupper_lower(sor[i - 1]) == 0)
			{
				if (isupper_lower(sor[i + 1]) == 0)
				{
					globalnum[c - 65]++;
				}
				else
				{


					switch (sor[i]) 
					{
					case 'C':
					{
						if ((isupper_lower(sor[i + 1]) == 4) & (isupper_lower(sor[i + 2]) == 0))
						{
							C[sor[i + 1] - 48]++;
						}
						break;
					}

					case 'I':
					{
						if ((isupper_lower(sor[i + 1]) == 4) & (isupper_lower(sor[i + 2]) == 0))
						{
							I[sor[i + 1] - 48]++;
						}
						break;
					}

					case 'L':
					{
						if ((isupper_lower(sor[i + 1]) == 4) & (isupper_lower(sor[i + 2]) == 0))
						{
							L[sor[i + 1] - 48]++;
						}
						break;
					}

					case 'F':
					{
						if ((isupper_lower(sor[i + 1]) == 4) & (isupper_lower(sor[i + 2]) == 0))
						{
							F[sor[i + 1] - 48]++;
						}
						break;
					}

					case 'D':
					{
						if ((isupper_lower(sor[i + 1]) == 4) & (isupper_lower(sor[i + 2]) == 0))
						{
							D[sor[i + 1] - 48]++;
						}
						break;
					}

					case 'S':
					{
						if ((isupper_lower(sor[i + 1]) == 4) & (isupper_lower(sor[i + 2]) == 0))
						{
							S[sor[i + 1] - 48]++;
						}
						break;
					}
					case 'U':
					{
						// UC??
						if (sor[i + 1] == 'C')
						{
							if ((isupper_lower(sor[i + 2]) == 4) & (isupper_lower(sor[i + 3]) == 0))
							{
								UC[sor[i + 2] - 48]++;
							}
						}
						// UI??
						if (sor[i + 1] == 'I')
						{
							if ((isupper_lower(sor[i + 2]) == 4) & (isupper_lower(sor[i + 3]) == 0))
							{
								UI[sor[i + 2] - 48]++;
							}
						}
						// UL??
						if (sor[i + 1] == 'L')
						{
							if ((isupper_lower(sor[i + 2]) == 4) & (isupper_lower(sor[i + 3]) == 0))
							{
								UL[sor[i + 2] - 48]++;
							}
						}
						break;
					}

					case 'O':
					{
						if ((sor[i + 1] == 'U') & (sor[i + 2] == 'T'))
						{
							_OUT_++;
						}
						break;
					}

					default:
						break;
					}
					

				}
			}
		}
		if (ci == 2)
		{
			if ((isupper_lower(sor[i - 1]) == 0) & (isupper_lower(sor[i + 1]) == 0))
			{
				privatenum[c - 97]++;
			}
		}
	}


	std::string header = "#define REP(j, n) for(int j = 0; j < (int)(n); j++)\n";
	header += "#define BARRIER barrier(CLK_LOCAL_MEM_FENCE);\n";
	header += "uint RND(uint s){s*=1847483629;s=(s^61)^(s>>16);s*=9;s=s^(s>>4);s*=0x27d4eb2d;s=s^(s>>15);return s;}\n";
	header += "__kernel void GenCode(";

	//??????1????????
	for (int i = 0; i < 26; i++)
	{
		if (globalnum[i] != 0)
		{
			Aa[0] = 65 + i;
			header += "__global " + stype[typeflg] + "* " + Aa + " ,";
			argcnt2++;
		}
	}

	if (argcnt2 == 0)
	{
		MessageBox(NULL, "?uA?v??????1????????????????????????????????", "?G???[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
	}

	//??????2???????????? C D F I L UC UI UL 
	for (int i = 0; i < 10; i++)
	{
		if (C[i] != 0)
		{
			header += "__global char* C" + std::to_string(i) + " ,";
			argcnt2++;
		}
	}
	for (int i = 0; i < 10; i++)
	{
		if (D[i] != 0)
		{
			header += "__global double* D" + std::to_string(i) + " ,";
			argcnt2++;
		}
	}
	for (int i = 0; i < 10; i++)
	{
		if (F[i] != 0)
		{
			header += "__global float* F" + std::to_string(i) + " ,";
			argcnt2++;
		}
	}
	for (int i = 0; i < 10; i++)
	{
		if (I[i] != 0)
		{
			header += "__global int* I" + std::to_string(i) + " ,";
			argcnt2++;
		}
	}
	for (int i = 0; i < 10; i++)
	{
		if (L[i] != 0)
		{
			header += "__global long long* L" + std::to_string(i) + " ,";
			argcnt2++;
		}
	}

	for (int i = 0; i < 10; i++)
	{
		if (UC[i] != 0)
		{
			header += "__global unssigned char* UC" + std::to_string(i) + " ,";
			argcnt2++;
		}
	}
	for (int i = 0; i < 10; i++)
	{
		if (UI[i] != 0)
		{
			header += "__global unsigned int* UI" + std::to_string(i) + " ,";
			argcnt2++;
		}
	}
	for (int i = 0; i < 10; i++)
	{
		if (UL[i] != 0)
		{
			header += "__global unsigned long long* UL" + std::to_string(i) + " ,";
			argcnt2++;
		}
	}


	//??????????
	privatenum[8] = 0;//i
	privatenum[9] = 0;//j
	privatenum[10] = 0;//k
	privatenum[23] = 0;//x
	privatenum[24] = 0;//y
	privatenum[25] = 0;//z
	for (int i = 0; i < 26; i++)
	{
		if (privatenum[i] != 0)
		{
			Aa[0] = 97 + i;
			if (argcnt2 >= 32) { argcnt2++; break; }
			header += "" + stype[argt[argcnt2]] + " " + Aa + " ,";
			argcnt2++;
		}
	}

	//?O?????G???[????
	if ((argcnt2 > 32) | (argcnt2 != argcnt))
	{
		if (argcnt2 > 32) MessageBox(NULL, "?J?[?l???R?[?h????????????????????????32????????????", "?G???[", 0);
		if (argcnt2 != argcnt) 
		{
			std::string errs = "?J?[?l???R?[?h??????????????????????(" + std::to_string(argcnt2) + ")??\nHSP?????w??????????????????(" + std::to_string(argcnt) + ")?????????????B";
			MessageBox(NULL, errs.c_str(), "?G???[", 0);
		}
		std::string srrs = "";
		for (int i = 0; i < 26; i++) if (globalnum[i] != 0) { Aa[0] = 65 + i; srrs += Aa + ","; }
		for (int i = 0; i < 10; i++) if (C[i] != 0) { srrs += "C" + std::to_string(i) + ","; }
		for (int i = 0; i < 10; i++) if (D[i] != 0) { srrs += "D" + std::to_string(i) + ","; }
		for (int i = 0; i < 10; i++) if (F[i] != 0) { srrs += "F" + std::to_string(i) + ","; }
		for (int i = 0; i < 10; i++) if (I[i] != 0) { srrs += "I" + std::to_string(i) + ","; }
		for (int i = 0; i < 10; i++) if (L[i] != 0) { srrs += "L" + std::to_string(i) + ","; }
		for (int i = 0; i < 10; i++) if (UC[i] != 0) { srrs += "UC" + std::to_string(i) + ","; }
		for (int i = 0; i < 10; i++) if (UI[i] != 0) { srrs += "UI" + std::to_string(i) + ","; }
		for (int i = 0; i < 10; i++) if (UL[i] != 0) { srrs += "UL" + std::to_string(i) + ","; }
		for (int i = 0; i < 26; i++) if (privatenum[i] != 0) { Aa[0] = 97 + i; srrs += Aa + ","; }

		MessageBox(NULL, srrs.c_str(), "?G???[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
	}

	//OUT????
	if (_OUT_ > 0) 
	{
		header += "__global " + stype[typeflg] + "* OUT ,";
	}

	//global id????????????
	if (header[header.size() - 1] == ',')header[header.size() - 1] = ' ';
	header += ")\n{\nint i = get_global_id(0);\nint li = get_local_id(0);\n";

	//shared memory????????
	for (int i = 0; i < 10; i++) 
	{
		if (S[i] != 0) 
		{
			header += "__local " + stype[typeflg] + " S" + std::to_string(i) + "[" + std::to_string(1 << i) + "];\n";
		}
	}

	return header + sor + "\n}";
}

//cl_mem???Q???n??
void HCLDoX(int typeflg,cl_mem &newmem)
{
	int sizelist[8];
	sizelist[0] = 1;
	sizelist[1] = 4;
	sizelist[2] = 8;
	sizelist[3] = 4;
	sizelist[4] = 8;
	sizelist[5] = 1;
	sizelist[6] = 4;
	sizelist[7] = 8;
	//???????????S??????
	cl_int ret;
	char* c_source;
	c_source = code_gets();								// ????????????
	std::string s_sourse = std::string(c_source);

	void* ppttr;
	int sizeofff;
	int chk;
	int type;

	int argt[32];
	int argcnt = 0;
	int argi32[32];
	INT64 argi64[32];
	double argdp[32];
	float argfp[32];
	for (int i = 0; i < 32; i++) 
	{
		argt[i] = 0; argi32[i] = 0; argi64[i] = 0; argdp[i] = 0.0; argfp[i] = 0.0;
	}

	for (int i = 0; i < 32; i++) {
		chk = code_getprm();							// ?p?????[?^?[??????(?^??????????)
		if (chk <= PARAM_END) break;				// ?p?????[?^?[????????????
		type = mpval->flag;							// ?p?????[?^?[???^??????
		switch (type) {
		case HSPVAR_FLAG_DOUBLE:									// ?p?????[?^?[??????????????
		{
			ppttr = (double*)mpval->pt;
			sizeofff = 8;
			argt[i] = 4;
			argcnt++;
			argdp[i] = *((double*)ppttr);
			break;
		}
		case HSPVAR_FLAG_INT:									// ?p?????[?^?[??????????????
		{
			ppttr = (int*)mpval->pt;
			sizeofff = 4;
			argt[i] = 1;
			argcnt++;
			argi32[i] = *((int*)ppttr);
			break;
		}
		default:
			if (type == HspVarInt64_typeid()) // ?p?????[?^?[??int64????????
			{
				ppttr = (INT64*)mpval->pt;
				sizeofff = 8;
				argt[i] = 2;
				argcnt++;
				argi64[i] = *((INT64*)ppttr);
				break;
			}
			if (type == HspVarFloat_typeid()) // ?p?????[?^?[??float????????
			{
				ppttr = (float*)mpval->pt;
				sizeofff = 4;
				argt[i] = 3;
				argcnt++;
				argfp[i] = *((float*)ppttr);
				break;
			}
			puterror(HSPERR_TYPE_MISMATCH);			// ?T?|?[?g???????????^???????G???[
			sizeofff = -1;
			break;
		}
	}
	//????????????????
	

	//???U?G???[????
	cl_mem baseMem;
	if (sizeof(size_t) == 8)baseMem = (cl_mem)argi64[0];
	if (sizeof(size_t) == 4)baseMem = (cl_mem)argi32[0];
	if (baseMem == 0)
	{
		MessageBox(NULL, "???????????L????cl_mem id??????????????", "?G???[", 0);
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
	}


	//?R?[?h????????
	int _OUT_ = 0;
	s_sourse = CodeRefine(s_sourse, typeflg, argt, argcnt, _OUT_);
	size_t h = KrnToHash(s_sourse);
	cl_kernel kernel = StrHashToKernel(s_sourse, h);

	//??????kernel?????????????B???????????w??
	//global?????v?Z?BA??????
	size_t global_size = GetMemSize(baseMem) / sizelist[typeflg];

	//????
	for (int i = 0; i < argcnt; i++)
	{
		if (argt[i] == 1)ppttr = (int*)&argi32[i];
		if (argt[i] == 2)ppttr = (INT64*)&argi64[i];
		if (argt[i] == 3)ppttr = (float*)&argfp[i];
		if (argt[i] == 4)ppttr = (double*)&argdp[i];
		clSetKernelArg(kernel, i, sizelist[argt[i]], ppttr);
	}
	//OUT???@?\??????????
	if (_OUT_ > 0) 
	{
		newmem = MyCreateBuffer(CL_MEM_READ_WRITE, GetMemSize(baseMem), NULL);
		clSetKernelArg(kernel, argcnt, sizeof(size_t), &newmem);
	}
	
	//?????????s????????
	//outevent???A
	cl_event* outevent = EventOutChk2((size_t)kernel);
	//wait event list???A
	cl_event* ev_ = GetWaitEvlist();
	size_t local_size = 64;
	size_t p4_1 = (global_size / local_size) * local_size;//local_size??????????????????
	size_t p4_2 = global_size - p4_1;//???????[??

	if (p4_1 != 0) {
		ret = clEnqueueNDRangeKernel(command_queue[clsetdev * COMMANDQUEUE_PER_DEVICE + clsetque],
			kernel, 1, NULL, &p4_1, &local_size, num_event_wait_list, ev_, outevent);//1???????????I????
		if (ret != CL_SUCCESS) { retmeserr(ret); }
	}

	if (p4_2 != 0) {
		local_size = p4_2;
		ret = clEnqueueNDRangeKernel(command_queue[clsetdev * COMMANDQUEUE_PER_DEVICE + clsetque],
			kernel, 1, &p4_1, &p4_2, &local_size, num_event_wait_list, ev_, NULL);
		if (ret != CL_SUCCESS) { retmeserr(ret); }
	}
	num_event_wait_list = 0;
	return;
}


void HCLDoXc(cl_mem& m)
{
	HCLDoX(0, m);
}

void HCLDoXi(cl_mem& m)
{
	HCLDoX(1, m);
}

void HCLDoXl(cl_mem& m)
{
	HCLDoX(2, m);
}

void HCLDoXf(cl_mem& m)
{
	HCLDoX(3, m);
}

void HCLDoXd(cl_mem& m)
{
	HCLDoX(4, m);
}

void HCLDoXuc(cl_mem& m)
{
	HCLDoX(5, m);
}

void HCLDoXui(cl_mem& m)
{
	HCLDoX(6, m);
}

void HCLDoXul(cl_mem &m)
{
	HCLDoX(7, m);
}