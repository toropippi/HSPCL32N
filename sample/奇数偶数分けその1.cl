__kernel void wake(__global int *mema,__global char *memb,__global char *memc) {
	int ic = get_global_id(0)*64;
	int reg=0;
	char kisu=0;//0～255の整数変数
	char gusu=0;

	for(int i=0;i<64;i++){
	reg=mema[ic+i];
		if (reg%2==0){//ifの後はカッコで括る、「=」ではなく「==」にしないとエラー
			gusu++;
		}else{
			kisu++;
		}
	}


	memb[ic>>6]=kisu;//ビットシフト。HSPと同じ。memb[get_global_id(0)]と同意味
	memc[ic>>6]=gusu;
}
