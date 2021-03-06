typedef struct{
	char x;
	char y;
	char z;
} Vec3;


__kernel void fp_calc(__global Vec3 *mema,float gtx,float gty,float gtz,int winx) {
	int icx = get_global_id(0);
	int icy = get_global_id(1);
	Vec3 za;
	za.x=0;
	za.y=0;
	za.z=0;
	float cx=gtx+gtz*icx;
	float cy=gty+gtz*icy;
	float jx=cx;
	float jy=cy;
	float jz;
	cy*=0.5f;
	cx*=-1.0f;

	for(int i=0;i<256;i++){
		jz=-jy*jy-cx;
		jy=2.0f*(jx*jy+cy);
		jx=(jx*jx+jz);
		if (jx>65536){za.x=(i%128)*2;za.y=(i%8)*32;za.z=(i%64)*4;break;}
		if (jx<-65536){za.y=(i%128)*2;za.z=(i%8)*32;za.x=(i%64)*4;break;}
	}


	mema[icx+icy*winx]=za;
}




__kernel void dp_calc(__global Vec3 *mema,double gtx,double gty,double gtz,int winx) {
	int icx = get_global_id(0);
	int icy = get_global_id(1);
	Vec3 za;
	za.x=0;
	za.y=0;
	za.z=0;
	double cx=gtx+gtz*icx;
	double cy=gty+gtz*icy;
	double jx=cx;
	double jy=cy;
	double jz;
	cy*=0.5;
	cx*=-1.0;

	for(int i=0;i<256;i++){
		jz=-jy*jy-cx;
		jy=2.0*(jx*jy+cy);
		jx=(jx*jx+jz);
		if (jx>65536){za.x=(i%128)*2;za.y=(i%8)*32;za.z=(i%64)*4;break;}
		if (jx<-65536){za.y=(i%128)*2;za.z=(i%8)*32;za.x=(i%64)*4;break;}
	}

	mema[icx+icy*winx]=za;
}