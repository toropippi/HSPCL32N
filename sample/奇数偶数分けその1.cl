__kernel void wake(__global int *mema,__global char *memb,__global char *memc) {
	int ic = get_global_id(0)*64;
	int reg=0;
	char kisu=0;//0�`255�̐����ϐ�
	char gusu=0;

	for(int i=0;i<64;i++){
	reg=mema[ic+i];
		if (reg%2==0){//if�̌�̓J�b�R�Ŋ���A�u=�v�ł͂Ȃ��u==�v�ɂ��Ȃ��ƃG���[
			gusu++;
		}else{
			kisu++;
		}
	}


	memb[ic>>6]=kisu;//�r�b�g�V�t�g�BHSP�Ɠ����Bmemb[get_global_id(0)]�Ɠ��Ӗ�
	memc[ic>>6]=gusu;
}
