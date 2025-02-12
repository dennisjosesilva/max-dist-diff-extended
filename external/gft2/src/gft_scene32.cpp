
#include "gft2/gft_scene32.h"

extern "C" {
  #include "gft2/gft_bzlib.h"
  #include "gft2/nifti1_io.h"
}
#include "gft2/gft_dicom.h"


namespace gft{
  namespace Scene32{

  sScene32 *Create(int xsize,int ysize,int zsize){
    sScene32 *scn=NULL;
    int **tmp=NULL;
    int xysize;
    int i,j,p,N;

    scn = (sScene32 *) calloc(1,sizeof(sScene32));
    if(scn == NULL) 
      gft::Error((char *)MSG1,(char *)"Scene32::Create");
 
    scn->xsize  = xsize;
    scn->ysize  = ysize;
    scn->zsize  = zsize;
    scn->dx     = 1.0;
    scn->dy     = 1.0;
    scn->dz     = 1.0;
    scn->maxval = 0;
    scn->minval = 0;
    scn->n      = xsize*ysize*zsize;
    scn->nii_hdr = NULL;

    //For SSE optimization we allocate a multiple of 4.
    N = scn->n;
    if(N%4!=0) N += (4-N%4);

    scn->data   = gft::AllocIntArray(N);
    if(scn->data==NULL) 
      gft::Error((char *)MSG1,(char *)"Scene32::Create");

    scn->array = (int ***) calloc(zsize, sizeof(int **));
    if(scn->array==NULL) 
      gft::Error((char *)MSG1,(char *)"Scene32::Create");

    tmp = (int **) calloc(zsize*ysize, sizeof(int *));
    if(tmp==NULL) 
      gft::Error((char *)MSG1,(char *)"Scene32::Create");

    scn->array[0] = tmp;
    for(i=1; i<zsize; i++)
      scn->array[i] = scn->array[i-1] + ysize;

    xysize = xsize*ysize;
    for(i=0; i<zsize; i++){
      for(j=0; j<ysize; j++){
	p = j + i*ysize;
	tmp[p] = scn->data + xysize*i + xsize*j;
      }}
    return(scn);
  }


  sScene32 *Create(sScene32 *scn){
    sScene32 *new_scn=NULL;
    new_scn = Create(scn->xsize,
		     scn->ysize,
		     scn->zsize);
    new_scn->dx = scn->dx;
    new_scn->dy = scn->dy;
    new_scn->dz = scn->dz;
    CloneNiftiHeader(scn, new_scn);
    return new_scn;
  }


  void     Destroy(sScene32 **scn){
    sScene32 *aux;
    
    aux = *scn;
    if(aux != NULL){
      if(aux->data     != NULL) gft::FreeIntArray(&aux->data);
      if(aux->array[0] != NULL) free(aux->array[0]);
      if(aux->array    != NULL) free(aux->array);
      if(aux->nii_hdr != NULL ) nifti_image_free(aux->nii_hdr);
      free(aux);
      *scn = NULL;
    }
  }

  sScene32 *Clone(sScene32 *scn){
    sScene32 *aux = Create(scn->xsize,scn->ysize,scn->zsize);
    aux->dx     = scn->dx;
    aux->dy     = scn->dy;
    aux->dz     = scn->dz;
    aux->maxval = scn->maxval;
    aux->minval = scn->minval;
    memcpy(aux->data,scn->data,sizeof(int)*aux->n);
    CloneNiftiHeader(scn, aux);
    return aux;
  }

  sScene32 *SubScene(sScene32 *scn, Voxel l, Voxel h){
    return SubScene(scn,
		    l.c.x, l.c.y, l.c.z,
		    h.c.x, h.c.y, h.c.z);
  }

  sScene32 *SubScene(sScene32 *scn,
		     int xl, int yl, int zl,
		     int xh, int yh, int zh){
    sScene32 *sub=NULL;
    Voxel v;
    int i,j;
    //v4si *p1,*p2;
  
    if(!IsValidVoxel(scn,xl,yl,zl)||
       !IsValidVoxel(scn,xh,yh,zh)||
       (xl > xh)||(yl>yh)||(zl>zh))
      return NULL;

    sub = Create(xh-xl+1,yh-yl+1,zh-zl+1);
    sub->dx = scn->dx;
    sub->dy = scn->dy;
    sub->dz = scn->dz;
    j = 0;
    for(v.c.z=zl; v.c.z<=zh; v.c.z++){
      for(v.c.y=yl; v.c.y<=yh; v.c.y++){
	/*
	for(v.c.x=xl; v.c.x<=xh-3; v.c.x+=4){
	  i = GetVoxelAddress(scn, v);
	  p1 = (v4si *)(&sub->data[j]);
	  p2 = (v4si *)(&scn->data[i]);
	  *p1 = *p2;
	  j+=4;
	  }
	for(; v.c.x <= xh; v.c.x++){
	  i = GetVoxelAddress(scn, v);
	  sub->data[j] = scn->data[i];
	  j++;
	}
	*/
	v.c.x=xl;
	i = GetVoxelAddress(scn, v);
	memcpy(&sub->data[j],&scn->data[i],sizeof(int)*sub->xsize);
	j += sub->xsize;
      }
    }
    return(sub);
  }


  void     Copy(sScene32 *dest, sScene32 *src){
    if(dest->xsize!=src->xsize ||
       dest->ysize!=src->ysize ||
       dest->zsize!=src->zsize)
      gft::Error((char *)"Incompatible data",(char *)"Scene32::Copy");
    dest->dx     = src->dx;
    dest->dy     = src->dy;
    dest->dz     = src->dz;
    dest->maxval = src->maxval;
    dest->minval = src->minval;
    memcpy(dest->data,src->data,sizeof(int)*src->n);
    CloneNiftiHeader(src, dest);
  }


  void     Copy(sScene32 *dest, sScene32 *src, Voxel v){
    int x1,y1,z1,x2,y2,z2,dx;
    Voxel t,u;
    int p,q;
    x1 = MAX(0, -v.c.x);
    x2 = MIN(src->xsize-1, dest->xsize-1 - v.c.x);
    dx = x2 - x1 + 1;
    if(dx<=0) return;

    y1 = MAX(0, -v.c.y);
    y2 = MIN(src->ysize-1, dest->ysize-1 - v.c.y);

    z1 = MAX(0, -v.c.z);
    z2 = MIN(src->zsize-1, dest->zsize-1 - v.c.z);
    
    for(t.c.z=z1; t.c.z<=z2; t.c.z++){
      for(t.c.y=y1; t.c.y<=y2; t.c.y++){
	t.c.x=x1;
	p = GetVoxelAddress(src, t);

	u.v = v.v + t.v;
	q = GetVoxelAddress(dest, u);

	memcpy(&dest->data[q],&src->data[p],sizeof(int)*dx);
      }
    }
  }


  void     Fill(sScene32 *scn, int value){
    if(value==0)
      memset((void *)scn->data, 0, sizeof(int)*scn->n);
    else{
      int p;
      v4si v;
      v4si *ptr;
      ((int *)(&v))[0] = value;
      ((int *)(&v))[1] = value;
      ((int *)(&v))[2] = value;
      ((int *)(&v))[3] = value;
      for(p=0; p<scn->n; p+=4){
	ptr = (v4si *)(&scn->data[p]);
	*ptr = v;
      }
    }
    scn->maxval = value;
    scn->minval = value;
  }


  sScene32 *Read(char *filename){
    sScene32  *scn=NULL;	
    FILE   *fp=NULL;
    uchar  *data8=NULL;
#if _WIN32 || __BYTE_ORDER==__LITTLE_ENDIAN 
    ushort *data16=NULL;
#endif
    char    type[10];
    int     i,n,v,xsize,ysize,zsize;
    long    pos;
    int     null;
    
    // Checking file type
    int len = strlen(filename);
    if ( (len>=4) && ((strcasecmp(filename + len - 4, ".hdr")==0) || (strcasecmp(filename + len - 4, ".img")==0))) 
      /*return ReadScene_Analyze(filename); */
      return ReadNifti1(filename);
    if ( (len>=8) && (strcasecmp(filename + len - 8, ".scn.bz2")==0))
      return ReadCompressed(filename);
    if ( (len>=4) && (strcasecmp(filename + len - 4, ".nii")==0))
      return ReadNifti1(filename);
    if ( (len>=7) && (strcasecmp(filename + len - 7, ".nii.gz")==0))
      return ReadNifti1(filename);
    if ( (len>=4) && (strcasecmp(filename + len - 4, ".dcm")==0))
      return DicomDirectoryConvert(filename, 0);
    if ( (len<=4) || (strcasecmp(filename + len - 4, ".scn")!=0)) {
      gft::Error(MSG2,"Scene32::Read: Invalid file name or extension.");
      return NULL;
    }
    
    // Read the scn file
    fp = fopen(filename,"rb");
    if (fp == NULL){
      gft::Error(MSG2,"Scene32::Read");
    }
    null = fscanf(fp,"%s\n",type);
    if((strcmp(type,"SCN")==0)){
      null = fscanf(fp,"%d %d %d\n",&xsize,&ysize,&zsize);
      scn = Create(xsize,ysize,zsize);
      n = xsize*ysize*zsize;
      null = fscanf(fp,"%f %f %f\n",&scn->dx,&scn->dy,&scn->dz);
      null = fscanf(fp,"%d",&v);
      pos = ftell(fp);
      //printf(" current relative position in file: %ld\n",pos);///	
      fseek(fp,(pos+1)*sizeof(char),SEEK_SET); // +1 for the EOL \n character not included in last fscanf() call
      if (v==8){
	data8  = gft::AllocUCharArray(n);			
	null = fread(data8,sizeof(uchar),n,fp);
	for (i=0; i < n; i++) 
	  scn->data[i] = (int) data8[i];
	gft::FreeUCharArray(&data8);
      } else if (v==16) {
	// assuming that data was written with LITTLE-ENDIAN Byte ordering, i.e. LSB...MSB
#if _WIN32 || __BYTE_ORDER==__LITTLE_ENDIAN
	// for PCs, Intel microprocessors (LITTLE-ENDIAN too) -> no change
	data16 = gft::AllocUShortArray(n);
	null = fread(data16,sizeof(ushort),n,fp);
	for (i=0; i < n; i++)
	  scn->data[i] = (int) data16[i];
	gft::FreeUShortArray(&data16);
#else
	// for Motorola, IBM, SUN (BIG-ENDIAN) -> SWAp Bytes!
	gft::Warning("Data is converted from LITTLE to BIG-ENDIAN","Scene32::Read");
	data8 = gft::AllocUCharArray(2*n);
	null = fread(data8,sizeof(uchar),2*n,fp);
	j=0;
	for (i=0; i < 2*n; i+=2) {
	  scn->data[j] = (int) data8[i] + 256 * (int) data8[i+1];
	  j++;
	}
	gft::FreeUCharArray(&data8);
#endif
      } else { /* n = 32 */
	//Warning("32-bit data. Values may be wrong (little/big endian byte ordering)","ReadScene");
	n = xsize*ysize*zsize;
	null = fread(scn->data,sizeof(int),n,fp);      
      }
      fclose(fp);
    } else {
      fprintf(stderr,"Input scene must be SCN\n");
      exit(-1);
    }
    scn->maxval = GetMaximumValue(scn);
    scn->minval = GetMinimumValue(scn);
    return(scn);
  }


  sScene32 *ReadCompressed(char *filename) {
    FILE   *f;
    gft_BZFILE *b;
    int     nBuf;
    char   *buf;
    int     bzerror;
    sScene32 *scn = NULL;
    int     xsize, ysize, zsize, pos;
    int     nbits, hdr_size, i, p;
    float   dx, dy, dz;
    char    s[ 2000 ];
    
    f = fopen ( filename, "r" );
    buf = ( char* ) calloc( 1025, sizeof( char ) );
    if ( !f ) {
      printf("Erro ao abrir arquivos!\n");
      return NULL;
    }
    b = gft_BZ2_bzReadOpen ( &bzerror, f, 0, 0, NULL, 0 );
    if ( bzerror != gft_BZ_OK ) {
      gft_BZ2_bzReadClose ( &bzerror, b );
      fclose(f);
      printf("Erro ao abrir cena compactada!\n");
      return NULL;
    }
    
    nBuf = gft_BZ2_bzRead ( &bzerror, b, buf, 1024 );
    hdr_size = 0;
    if ( ( ( bzerror == gft_BZ_OK ) || ( bzerror == gft_BZ_STREAM_END ) ) && (nBuf > 0) ) {
      sscanf( buf, "%[^\n]\n%d %d %d\n%f %f %f\n%d\n", s, &xsize, &ysize, &zsize, &dx, &dy, &dz, &nbits );
      //printf( "s:%s, xsize:%d, ysize:%d, zsize:%d, dx:%f, dy:%f, dz:%f, bits:%d\n", s, xsize, ysize, zsize, dx, dy, dz, nbits );
      if ( strcmp( s, "SCN" ) != 0 ) {
	printf( "Format must by a compressed scene.\nFound %s\n", s );
	return 0;
      }
      scn = Create( xsize, ysize, zsize );
      scn->dx = dx;
      scn->dy = dy;
      scn->dz = dz;
      
      hdr_size = strlen( s ) + strlen( "\n" );
      for( i = 0; i < 3; i++ ) {
	sscanf( &buf[ hdr_size ], "%[^\n]\n", s );
	hdr_size += strlen( s ) + strlen( "\n" );
      }
    }
    else {
      printf("Erro ao ler cena compactada!\n");
      return 0;
    }
    
    gft_BZ2_bzReadClose ( &bzerror, b );
    rewind( f );
    b = gft_BZ2_bzReadOpen ( &bzerror, f, 0, 0, NULL, 0 );
    if ( bzerror != gft_BZ_OK ) {
      gft_BZ2_bzReadClose ( &bzerror, b );
      fclose(f);
      printf("Erro ao abrir cena compactada!\n");
      return NULL;
    }
    
    nBuf = gft_BZ2_bzRead ( &bzerror, b, buf, hdr_size );
    p = 0;
    do {
      nBuf = gft_BZ2_bzRead ( &bzerror, b, buf, 1024 );
      if ( ( ( bzerror == gft_BZ_OK ) || ( bzerror == gft_BZ_STREAM_END ) ) && ( nBuf > 0 ) ) {
	pos = 0;
	while( pos < nBuf - ( nbits / 8 ) + 1 ) {
	  if( nbits == 8 ) {
	    scn->data[ p ] = ( int ) ( ( uchar ) buf[ pos ] );
	    pos++;
	  }
	  else if( nbits == 16 ) {
	    scn->data[ p ] = ( int ) ( ( ( uchar ) buf[ pos + 1 ] ) * 256 + ( ( uchar ) buf[ pos ] ) );
	    if ( scn->data[ p ] < 0 ) printf("dado negativo.\n");
	    pos += 2;
	  }
	  else {
	    scn->data[ p ] = ( int ) ( ( uchar ) buf[ pos + 3 ] );
	    for( i = 2; i >= 0; i-- ) {
	      scn->data[ p ] = scn->data[ p ] * 256 + ( ( uchar ) buf[ pos + i ] );
	    }
	    pos +=4;
	  }
	  p++;
	}
      }
    } while( bzerror == gft_BZ_OK );
    //printf( "scn->n:%d, p:%d\n", scn->n, p );
    
    GetMaximumValue( scn );
    //printf( "max=%d\n", scn->maxval );
    gft_BZ2_bzReadClose ( &bzerror, b );
    fclose(f);
    free(buf);
    
    //printf( "fim!\n" );
    scn->maxval = GetMaximumValue(scn);
    scn->minval = GetMinimumValue(scn);    
    return( scn );
  }


  sScene32 *ReadNifti1(char *filename){
    nifti_image *nii;
    sScene32 *scn;
    float max;
    int p;
  
    nii = nifti_image_read( filename , 1 );
    scn = Create( nii->nx, nii->ny, nii->nz );
    scn->dx = nii->dx;
    scn->dy = nii->dy;
    scn->dz = nii->dz;
    scn->nii_hdr = nii;
    
    if( ( nii->datatype == NIFTI_TYPE_COMPLEX64 ) ||
	( nii->datatype == NIFTI_TYPE_FLOAT64 ) ||
	( nii->datatype == NIFTI_TYPE_RGB24 ) ||
	( nii->datatype == NIFTI_TYPE_RGB24 ) ||
	( nii->datatype >= NIFTI_TYPE_UINT32 ) ||
	( nii->dim[ 0 ] < 3 ) || ( nii->dim[ 0 ] > 4 ) ) {
      printf( "Error: Data format not supported, or header is corrupted.\n" );
      printf( "Data that is NOT supported: complex, double, RGB, unsigned integer of 32 bits, temporal series and statistical images.\n" );
      exit( -1 );
    }
  
    if( nii->datatype == NIFTI_TYPE_INT32 ) {
      //printf( "Integer src image.\n" );
      memcpy( scn->data, nii->data, nii->nvox * nii->nbyper );
    }
    else if( nii->datatype == NIFTI_TYPE_UINT16 ) {
      //printf( "Unsigned short src image.\n" );
      for( p = 0; p < scn->n; p++ ) {
	scn->data[ p ] = ( ( unsigned short* ) nii->data )[ p ];
      }
    }
    else if( nii->datatype == NIFTI_TYPE_INT16 ) {
      //printf( "Short src image.\n" );
      for( p = 0; p < scn->n; p++ ) {
	scn->data[ p ] = ( (short* ) nii->data )[ p ];
      }
    }
    else if( nii->datatype == NIFTI_TYPE_UINT8 ) {
      //printf( "Unsigned char src image.\n" );
      for( p = 0; p < scn->n; p++ ) {
	scn->data[ p ] = ( ( unsigned char* ) nii->data )[ p ];
      }
    }
    else if( nii->datatype == NIFTI_TYPE_INT8 ) {
      //printf( "Char src image.\n" );
      for( p = 0; p < scn->n; p++ ) {
	scn->data[ p ] = ( (char* ) nii->data )[ p ];
      }
    }
    else if( nii->datatype == NIFTI_TYPE_FLOAT32 ){
      //printf( "Float src image.\n" );
      // max used to set data to integer range without loosing its precision.
      max = 0.0;
      for( p = 0; p < scn->n; p++ ) {
	if( max < ( ( float* ) nii->data )[ p ] ) {
	  max = ( ( float* ) nii->data )[ p ];
	}
      }
      nii->flt_conv_factor = 10000.0 / max;
      for( p = 0; p < scn->n; p++ ) {
	scn->data[ p ] = ROUND( ( ( float* ) nii->data )[ p ] * nii->flt_conv_factor );
      }
    }
    
    free( nii->data );
    nii->data = NULL;
    scn->maxval = GetMaximumValue(scn);
    scn->minval = GetMinimumValue(scn);
    return( scn );
  }




  void     Write(sScene32 *scn, char *filename){
    FILE *fp=NULL;
    int Imax;
    int i,n;
    uchar  *data8 =NULL;
    ushort *data16=NULL;

    // Checking file type
    int len = strlen(filename);
    if ( (len>=4) && ((strcasecmp(filename + len - 4, ".hdr")==0) || (strcasecmp(filename + len - 4, ".img")==0))) {
      //WriteScene_Analyze(scn, filename);
      WriteNifti1(scn, filename);
      return;
    }
    if ( (len>=8) && (strcasecmp(filename + len - 8, ".scn.bz2")==0)) {
      WriteCompressed(scn, filename);
      return;
    }
    if ( (len>=7) && (strcasecmp(filename + len - 7, ".nii.gz")==0)) {
      WriteNifti1( scn, filename );
      return;
    }
    if ( (len>=4) && (strcasecmp(filename + len - 4, ".nii")==0)) {
      WriteNifti1( scn, filename );
      return;
    }
    if ( (len<=4) || (strcasecmp(filename + len - 4, ".scn")!=0)) {
      gft::Error(MSG2,"Scene32::Write: Invalid file name or extension.");
    }


    // Writing the scn file
    fp = fopen(filename,"wb"); 
    if(fp == NULL) 
      gft::Error((char *)MSG2,(char *)"Scene32::Write");

    fprintf(fp,"SCN\n");
    fprintf(fp,"%d %d %d\n",scn->xsize,scn->ysize,scn->zsize);
    fprintf(fp,"%f %f %f\n",scn->dx,scn->dy,scn->dz);
  
    Imax = GetMaximumValue(scn);
    
    n = scn->n;
    if(Imax < 256) {
      fprintf(fp,"%d\n",8);
      data8 = gft::AllocUCharArray(n);
      for(i=0; i<n; i++) 
	data8[i] = (uchar) scn->data[i];
      fwrite(data8,sizeof(uchar),n,fp);
      gft::FreeUCharArray(&data8);
    } else if(Imax < 65536) {
      fprintf(fp,"%d\n",16);
      data16 = gft::AllocUShortArray(n);
      for(i=0; i<n; i++)
	data16[i] = (ushort) scn->data[i];
      fwrite(data16,sizeof(ushort),n,fp);
      gft::FreeUShortArray(&data16);
    } else {
      fprintf(fp,"%d\n",32);
      fwrite(scn->data,sizeof(int),n,fp);
    }
    fclose(fp);
  }


  void WriteCompressed(sScene32 *scn, char *filename) {
    FILE   *f;
    //FILE   *in;
    gft_BZFILE *b;
    //int     nBuf = 1024;
    //char    buf[1024];
    int    Imax, n, i;
    int    bzerror;
    char   *data;
    uchar  *data8;
    ushort *data16;
    
    f = fopen( filename, "wb" );
    
    //if ( ( !f ) || ( !in ) ) {
    if ( !f ) {
      printf("Error on opening the compressed file %s\n", filename);
      return;
    }
    
    b = gft_BZ2_bzWriteOpen( &bzerror, f, 9, 0, 30 );
    if (bzerror != gft_BZ_OK) {
      gft_BZ2_bzWriteClose ( &bzerror, b, 0, 0, 0 );
      fclose(f);
      printf("Error on opening the file %s\n", filename);
    }
    
    //while ( (bzerror == gft_BZ_OK ) && (nBuf > 0) ) {
    if (bzerror == gft_BZ_OK) {
      /* get data to write into buf, and set nBuf appropriately */
      data = (char*) calloc(200, sizeof(int));
      n = scn->xsize*scn->ysize*scn->zsize;
      sprintf(data,"SCN\n");
      sprintf(data,"%s%d %d %d\n",data,scn->xsize,scn->ysize,scn->zsize);
      sprintf(data,"%s%f %f %f\n",data,scn->dx,scn->dy,scn->dz);
      Imax = GetMaximumValue(scn);
      if (Imax < 256) {
	//printf("8bits\n");
	sprintf(data,"%s%d\n",data,8);
	gft_BZ2_bzWrite ( &bzerror, b, data, strlen( data ) );
	data8 = gft::AllocUCharArray(n);
	for (i=0; i < n; i++) 
	  data8[i] = (uchar) scn->data[i];
	gft_BZ2_bzWrite ( &bzerror, b, data8, n );
	gft::FreeUCharArray(&data8);
      } else if (Imax < 65536) {
	//printf("16bits\n");
	sprintf(data,"%s%d\n",data,16);
	gft_BZ2_bzWrite ( &bzerror, b, data, strlen( data ) );
	data16 = gft::AllocUShortArray(n);
	for (i=0; i < n; i++)
	  data16[i] = (ushort) scn->data[i];
	gft_BZ2_bzWrite ( &bzerror, b, data16, 2 * n );
	gft::FreeUShortArray(&data16);
      } else {
	//printf("32bits\n");
	sprintf(data,"%s%d\n",data,32);
	gft_BZ2_bzWrite ( &bzerror, b, data, strlen( data ) );
	gft_BZ2_bzWrite ( &bzerror, b, scn->data, 4 * n );
      }
      free(data);
	/*
	  nBuf = fread(buf, sizeof(char), 1024, in);
	  if (nBuf > 0)
	  gft_BZ2_bzWrite ( &bzerror, b, buf, nBuf );
	  
	  if (bzerror == gft_BZ_IO_ERROR) { 
	  break;
	  }
	*/
    }
    
    gft_BZ2_bzWriteClose ( &bzerror, b, 0, 0, 0 );
    fclose(f);
    
    if (bzerror == gft_BZ_IO_ERROR) {
      printf("Error on writing to %s\n", filename);
    }
  }


  void WriteNifti1(sScene32 *scn, char *filename){
    const int dims[8] = { 3, scn->xsize, scn->ysize, scn->zsize, 0, 0, 0, 0 };
    nifti_image *nii;
    int len;
    int p;
    
    nii = scn->nii_hdr;
    if( nii == NULL ){
      nii = scn->nii_hdr = nifti_make_new_nim(dims,
					      NIFTI_TYPE_INT32,
					      0);
      nii->pixdim[1] = nii->dx = scn->dx;
      nii->pixdim[2] = nii->dy = scn->dy;
      nii->pixdim[3] = nii->dz = scn->dz;
    }
    //------
    nii->datatype = NIFTI_TYPE_INT32;
    nii->nbyper = 4;
    
    if( nii->data != NULL )
      free( nii->data );
    nii->data = (void *)scn->data;
    //------    
    /*   
    if( nii->data != NULL )
      free( nii->data );
    nii->data = calloc( nii->nbyper, nii->nvox );
    
    if( nii->datatype == NIFTI_TYPE_INT32 ) {
      //printf( "Integer src image.\n" );
      memcpy( nii->data, scn->data, nii->nvox * nii->nbyper );
    }
    else if( ( nii->datatype == NIFTI_TYPE_INT16 ) || ( nii->datatype == NIFTI_TYPE_UINT16 ) ) {
      //printf( "Short src image.\n" );
      for( p = 0; p < scn->n; p++ ) {
	( ( unsigned short* ) nii->data )[ p ] = scn->data[ p ];
      }
    }
    else if( ( nii->datatype == NIFTI_TYPE_INT8 ) || ( nii->datatype == NIFTI_TYPE_INT8 ) || ( nii->datatype == NIFTI_TYPE_UINT8 ) ) {
      //printf( "Char src image.\n" );
      for( p = 0; p < scn->n; p++ ) {
	( ( unsigned char* ) nii->data )[ p ] = scn->data[ p ];
      }
    }
    else if( nii->datatype == NIFTI_TYPE_FLOAT32 ) {
      //printf( "Float src image.\n" );
      // max used to set data to integer range without loosing its precision.
      for( p = 0; p < scn->n; p++ ) {
	( ( float* ) nii->data )[ p ] = scn->data[ p ] / nii->flt_conv_factor;
      }
    }
    else 
      gft::Error( MSG5, "Scene32::WriteNifti1" );
    */
    
    len = strlen( filename );
    if( ( strcasecmp( filename + len - 4, ".hdr" ) == 0 ) || ( strcasecmp( filename + len - 4, ".img" ) == 0 ) )
      nii->nifti_type = NIFTI_FTYPE_NIFTI1_2;
    else
      nii->nifti_type = NIFTI_FTYPE_NIFTI1_1;
    nifti_set_filenames( nii, filename, 0, nii->byteorder );
    
    nifti_image_write( nii );
    //free( nii->data );
    nii->data = NULL;
  }


  void CloneNiftiHeader(sScene32 *src, sScene32 *dst){
    if( ( src != NULL ) && ( dst != NULL ) && ( src->nii_hdr != NULL ) ) {
      if( dst->nii_hdr != NULL ) {
	nifti_image_free( dst->nii_hdr );
	dst->nii_hdr = NULL;
      }
      dst->nii_hdr = nifti_copy_nim_info( src->nii_hdr );
    }
  }



  sScene32 *ChangeOrientationToLPS(sScene32 *scn, char *ori){
    int Xsrc=0,Ysrc=0,Zsrc=0;
    int xsize=0,ysize=0,zsize=0;
    float dx=1.0,dy=1.0,dz=1.0;
    int p,q,oldx=0,oldy=0,oldz=0;
    sScene32 *res=NULL;
    gft::Voxel v;
    if      (ori[0]=='L') { Xsrc=+1; xsize=scn->xsize; dx=scn->dx; }
    else if (ori[0]=='R') { Xsrc=-1; xsize=scn->xsize; dx=scn->dx; }
    else if (ori[0]=='P') { Ysrc=+1; ysize=scn->xsize; dy=scn->dx; }
    else if (ori[0]=='A') { Ysrc=-1; ysize=scn->xsize; dy=scn->dx; }
    else if (ori[0]=='S') { Zsrc=+1; zsize=scn->xsize; dz=scn->dx; }
    else if (ori[0]=='I') { Zsrc=-1; zsize=scn->xsize; dz=scn->dx; }
    else{ gft::Error("Invalid orientation","ChangeOrientationToLPS"); }
    
    if      (ori[1]=='L') { Xsrc=+2; xsize=scn->ysize; dx=scn->dy; }
    else if (ori[1]=='R') { Xsrc=-2; xsize=scn->ysize; dx=scn->dy; }
    else if (ori[1]=='P') { Ysrc=+2; ysize=scn->ysize; dy=scn->dy; }
    else if (ori[1]=='A') { Ysrc=-2; ysize=scn->ysize; dy=scn->dy; }
    else if (ori[1]=='S') { Zsrc=+2; zsize=scn->ysize; dz=scn->dy; }
    else if (ori[1]=='I') { Zsrc=-2; zsize=scn->ysize; dz=scn->dy; }
    else{ gft::Error("Invalid orientation","ChangeOrientationToLPS"); }
    
    if      (ori[2]=='L') { Xsrc=+3; xsize=scn->zsize; dx=scn->dz; }
    else if (ori[2]=='R') { Xsrc=-3; xsize=scn->zsize; dx=scn->dz; }
    else if (ori[2]=='P') { Ysrc=+3; ysize=scn->zsize; dy=scn->dz; }
    else if (ori[2]=='A') { Ysrc=-3; ysize=scn->zsize; dy=scn->dz; }
    else if (ori[2]=='S') { Zsrc=+3; zsize=scn->zsize; dz=scn->dz; }
    else if (ori[2]=='I') { Zsrc=-3; zsize=scn->zsize; dz=scn->dz; }
    else{ gft::Error("Invalid orientation","ChangeOrientationToLPS"); }
    
    res= gft::Scene32::Create(xsize,ysize,zsize);
    res->dx = dx;
    res->dy = dy;
    res->dz = dz;
    
    if( scn->nii_hdr != NULL ) {
      res->nii_hdr = nifti_copy_nim_info( scn->nii_hdr );
    }
    
    v.c.z=0; v.c.y=0;
    for (v.c.z=0; v.c.z < zsize; v.c.z++)
      for (v.c.y=0; v.c.y < ysize; v.c.y++)
	for (v.c.x=0; v.c.x < xsize; v.c.x++) {
	  p = gft::Scene32::GetVoxelAddress(res, v);
	    
	  switch (Xsrc) {
	  case +1: oldx=v.c.x; break;
	  case -1: oldx=xsize-v.c.x-1; break;
	  case +2: oldy=v.c.x; break;
	  case -2: oldy=xsize-v.c.x-1; break;
	  case +3: oldz=v.c.x; break;
	  case -3: oldz=xsize-v.c.x-1; break;
	  }
	  switch (Ysrc) {
	  case +1: oldx=v.c.y; break;
	  case -1: oldx=ysize-v.c.y-1; break;
	  case +2: oldy=v.c.y; break;
	  case -2: oldy=ysize-v.c.y-1; break;
	  case +3: oldz=v.c.y; break;
	  case -3: oldz=ysize-v.c.y-1; break;
	  }
	  switch (Zsrc) {
	  case +1: oldx=v.c.z; break;
	  case -1: oldx=zsize-v.c.z-1; break;
	  case +2: oldy=v.c.z; break;
	  case -2: oldy=zsize-v.c.z-1; break;
	  case +3: oldz=v.c.z; break;
	  case -3: oldz=zsize-v.c.z-1; break;
	  }
	  q = gft::Scene32::GetVoxelAddress(scn, oldx, oldy, oldz);   
	  res->data[p]=scn->data[q];
	}
    
    return res;
  }
    
    

  sScene32 *LinearInterp(sScene32 *scn,float dx,float dy,float dz){
    int value;
    sScene32 *scene,*tmp;
    gft::Voxel P,Q,R; /* previous, current, and next voxel */
    float min=FLT_MAX;
    float walked_dist,dist_PQ;
    
    /* The default voxel sizes of the input scene should be dx=dy=dz=1.0 */
    
    if(scn->dx == 0.0 && scn->dy == 0.0 && scn->dz == 0.0){
      scn->dx = 1.0;
      scn->dy = 1.0;
      scn->dz = 1.0;
    }
    
    /* The default voxel sizes of the output scene should be dx=dy=dz=min(dx,dy,dz) */
    
    if ((dx == 0.0) || (dy == 0.0) || (dz == 0.0)) {
      if (scn->dx < min)
	min = scn->dx;
      if (scn->dy < min)
	min = scn->dy;
      if (scn->dz < min)
	min = scn->dz;
      dx = min; dy = min; dz = min;
      if (min <= 0) {
	fprintf(stderr,"Voxel distance can not be negative.\n");
	exit(-1);
      }
    }
    
    /* If there is no need for resampling then returns input scene */
    
    if ((dx == scn->dx) && (dy == scn->dy) && (dz == scn->dz)) {
      scene = Clone(scn);
      return (scene);
    } else {
      /* Else the working image is the input image */
      scene = scn;
    }
    
    /* Resample in x */
    
    if (dx != scn->dx) {
      tmp = Create(ROUND((float)(scene->xsize)*scene->dx/dx),
		   scene->ysize,
		   scene->zsize);
      for(Q.c.x=0; Q.c.x < tmp->xsize; Q.c.x++)
	for(Q.c.z=0; Q.c.z < tmp->zsize; Q.c.z++)
	  for(Q.c.y=0; Q.c.y < tmp->ysize; Q.c.y++) {
	    
	    
	    walked_dist = (float)Q.c.x * dx; /* the walked distance so far */
	    
	    P.c.x = (int)(walked_dist/scn->dx); /* P is the previous pixel in the
						   original scene */
	    P.c.y = Q.c.y;
	    P.c.z = Q.c.z;
	    
	    R.c.x = P.c.x + 1; /* R is the next pixel in the original
				  image. Observe that Q is in between P
				  and R. */
	    R.c.y = P.c.y;
	    R.c.z = P.c.z;
	    
	    dist_PQ =  walked_dist - (float)P.c.x * scn->dx;  /* the distance between P and Q */
	    
	    /* interpolation: P --- dPQ --- Q ---- dPR-dPQ ---- R
	       
	       I(Q) = (I(P)*(dPR-dPQ) + I(R)*dPQ) / dPR
	       
	    */

	    if(IsValidVoxel(scene, R))
	      value = ROUND(( (scn->dx - dist_PQ)*(float)GetValue(scene,P) +
			      dist_PQ * (float)GetValue(scene,R) )/scn->dx);
	    else
	      value = GetValue(scene,P);
	    //tmp->data[Q.c.x+tmp->tby[Q.c.y]+tmp->tbz[Q.c.z]]=value;
	    tmp->array[Q.c.z][Q.c.y][Q.c.x] = value;
	  }
      scene=tmp;
    }
    
    /* Resample in y */
    
    if (dy != scn->dy) {
      tmp = Create(scene->xsize,
		   ROUND((float)scene->ysize * scn->dy / dy),
		   scene->zsize);
      for(Q.c.y=0; Q.c.y < tmp->ysize; Q.c.y++)
	for(Q.c.z=0; Q.c.z < tmp->zsize; Q.c.z++)
	  for(Q.c.x=0; Q.c.x < tmp->xsize; Q.c.x++) {
	      
	    walked_dist = (float)Q.c.y * dy;
	    
	    P.c.x = Q.c.x;
	    P.c.y = (int)(walked_dist/scn->dy);
	    P.c.z = Q.c.z;
	    
	    R.c.x = P.c.x;
	    R.c.y = P.c.y + 1;
	    R.c.z = P.c.z;
	    
	    dist_PQ =  walked_dist - (float)P.c.y * scn->dy;
	    /* comecar a adaptar daqui !! */

	    if(IsValidVoxel(scene, R))
	      value = ROUND(( ((scn->dy - dist_PQ)*(float)GetValue(scene,P)) +
			      dist_PQ * (float)GetValue(scene,R)) / scn->dy) ;
	    else
	      value = GetValue(scene,P);

	    //tmp->data[Q.x+tmp->tby[Q.y]+tmp->tbz[Q.z]]=value;
	    tmp->array[Q.c.z][Q.c.y][Q.c.x] = value;
	  }
      if (scene != scn) {
	Destroy(&scene);
      }
      scene=tmp;
    }
    
    /* Resample in z */
    
    if (dz != scn->dz) {
      tmp = Create(scene->xsize,
		   scene->ysize,
		   ROUND((float)scene->zsize * scn->dz / dz));
      for(Q.c.z=0; Q.c.z < tmp->zsize; Q.c.z++)
	for(Q.c.y=0; Q.c.y < tmp->ysize; Q.c.y++)
	  for(Q.c.x=0; Q.c.x < tmp->xsize; Q.c.x++) {
	    
	    walked_dist = (float)Q.c.z * dz;
	    
	    P.c.x = Q.c.x;
	    P.c.y = Q.c.y;
	    P.c.z = (int)(walked_dist/scn->dz);
	    
	    R.c.x = P.c.x;
	    R.c.y = P.c.y;
	    R.c.z = P.c.z + 1;
	    
	    dist_PQ =  walked_dist - (float)P.c.z * scn->dz;

	    if(IsValidVoxel(scene, R))
	      value = ROUND((( (scn->dz - dist_PQ)*(float)GetValue(scene,P)) +
			     dist_PQ * (float)GetValue(scene,R)) / scn->dz) ;
	    else
	      value = GetValue(scene,P);

	    //tmp->data[Q.x+tmp->tby[Q.y]+tmp->tbz[Q.z]]=value;
	    tmp->array[Q.c.z][Q.c.y][Q.c.x] = value;
	  }
      if (scene != scn) {
	Destroy(&scene);
      }
      scene=tmp;
    }
    
    scene->dx=dx;
    scene->dy=dy;
    scene->dz=dz;
    scene->maxval=scn->maxval;
    scene->minval=scn->minval;
    return(scene);
  }



  sScene32 *Scale(sScene32 *scn, sScene32 *ref,
		  InterpolationType interpolation){
    sScene32 *scl;
    gft::Voxel u;
    float x,y,z;
    if(scn->dx == ref->dx &&
       scn->dy == ref->dy &&
       scn->dz == ref->dz &&
       scn->xsize == ref->xsize &&
       scn->ysize == ref->ysize &&
       scn->zsize == ref->zsize){
      return Clone(scn);
    }

    scl = Create(ref);
    if(interpolation == none){ //nearest-neighbor.

      for(u.c.x = 0; u.c.x < scl->xsize; u.c.x++){
	for(u.c.y = 0; u.c.y < scl->ysize; u.c.y++){
	  for(u.c.z = 0; u.c.z < scl->zsize; u.c.z++){
	    x = (u.c.x - scl->xsize/2.)*(ref->dx/scn->dx) + scn->xsize/2.;
	    y = (u.c.y - scl->ysize/2.)*(ref->dy/scn->dy) + scn->ysize/2.;
	    z = (u.c.z - scl->zsize/2.)*(ref->dz/scn->dz) + scn->zsize/2.;
	    scl->array[u.c.z][u.c.y][u.c.x] = GetValue_nn(scn,x,y,z);
	  }
	}
      }

    }
    else if(interpolation == linear){

      for(u.c.x = 0; u.c.x < scl->xsize; u.c.x++){
	for(u.c.y = 0; u.c.y < scl->ysize; u.c.y++){
	  for(u.c.z = 0; u.c.z < scl->zsize; u.c.z++){
	    x = (u.c.x - scl->xsize/2.)*(ref->dx/scn->dx) + scn->xsize/2.;
	    y = (u.c.y - scl->ysize/2.)*(ref->dy/scn->dy) + scn->ysize/2.;
	    z = (u.c.z - scl->zsize/2.)*(ref->dz/scn->dz) + scn->zsize/2.;
	    scl->array[u.c.z][u.c.y][u.c.x] = GetValue_trilinear(scn,x,y,z);
	  }
	}
      }

    }
    else{
      gft::Warning((char *)"This feature has not yet been implemented",
		   (char *)"Scene32::Scale");
    }
    return scl;
  }



  sScene32 *ScaleLabel(sScene32 *label, sScene32 *ref,
		       InterpolationType interpolation){
    sScene32 *scl, *bin, *tmp;
    int p,l,Lmax,Lmin;
    if(label->dx == ref->dx &&
       label->dy == ref->dy &&
       label->dz == ref->dz &&
       label->xsize == ref->xsize &&
       label->ysize == ref->ysize &&
       label->zsize == ref->zsize){
      return Clone(label);
    }
    else if(interpolation == none){ //nearest-neighbor.
      return Scale(label, ref, interpolation);
    }
    else if(interpolation == linear){
      scl = Create(ref);
      bin = Create(label);
      Lmax = GetMaxVal(label);
      Lmin = GetMinVal(label);
      Fill(scl, Lmin);
      for(l = Lmin+1; l <= Lmax; l++){
	for(p = 0; p < bin->n; p++){
	  if(label->data[p] == l)
	    bin->data[p] = 255;
	  else
	    bin->data[p] = 0;
	}
	tmp = Scale(bin, ref, interpolation);
	for(p = 0; p < tmp->n; p++){
	  if(tmp->data[p] > 127)
	    scl->data[p] = l;
	}
	Destroy(&tmp);
      }
      Destroy(&bin);
      return scl;
    }
    else{
      gft::Warning((char *)"This feature has not yet been implemented",
		   (char *)"Scene32::Scale");
      scl = Create(ref);
      return scl;
    }
  }
    

  float GetValue_trilinear(sScene32 *scn, float x, float y, float z){
    int px,py,pz,i1,i2;
    float dx,dy,dz;
    int nbx=0,nby=0,nbz=0;
    float p1,p2,p3,p4,p5,p6,res;

    if(x>scn->xsize-1||y>scn->ysize-1||z>scn->zsize-1||x<0||y<0||z<0)
      return GetValue_nn(scn, x, y, z);
      /*
      gft::Error((char *)"Out-of-bounds",
		 (char *)"Scene32::GetValue_trilinear");
      */
      
    px = (int)x;  dx=x-px;
    py = (int)y;  dy=y-py;
    pz = (int)z;  dz=z-pz;

    // If it's not on the border, it has a neighour ahead.
    if(px<scn->xsize-1) nbx=1; 
    if(py<scn->ysize-1) nby=1;
    if(pz<scn->zsize-1) nbz=1;

    // 1st: Interpolate in Z
    i1 = scn->array[pz][py][px];     //i1 = scn->data[GetVoxelAddress(scn,px,py,pz)];
    i2 = scn->array[pz+nbz][py][px]; //i2 = scn->data[GetVoxelAddress(scn,px,py,pz+nbz)];
    p1 = i1 + dz*(i2-i1);
    i1 = scn->array[pz][py][px+nbx];     //i1 = scn->data[GetVoxelAddress(scn,px+nbx,py,pz)];
    i2 = scn->array[pz+nbz][py][px+nbx]; //i2 = scn->data[GetVoxelAddress(scn,px+nbx,py,pz+nbz)];
    p2 = i1 + dz*(i2-i1);
    i1 = scn->array[pz][py+nby][px];     //i1 = scn->data[GetVoxelAddress(scn,px,py+nby,pz)];
    i2 = scn->array[pz+nbz][py+nby][px]; //i2 = scn->data[GetVoxelAddress(scn,px,py+nby,pz+nbz)];
    p3 = i1 + dz*(i2-i1);
    i1 = scn->array[pz][py+nby][px+nbx];     //i1 = scn->data[GetVoxelAddress(scn,px+nbx,py+nby,pz)];
    i2 = scn->array[pz+nbz][py+nby][px+nbx]; //i2 = scn->data[GetVoxelAddress(scn,px+nbx,py+nby,pz+nbz)];
    p4 = i1 + dz*(i2-i1);
    // 2nd: Interpolate in X
    p5 = p1 + dx*(p2-p1);
    p6 = p3 + dx*(p4-p3);
    // 3rd: Interpolate in Y
    res = p5 + dy*(p6-p5);
    return res;
  }


  // return the nearest voxel.
  int   GetValue_nn(sScene32 *scn, float x, float y, float z){
    Voxel v;
    /*
    if(x>scn->xsize-1||y>scn->ysize-1||z>scn->zsize-1||
       x<(float)0||y<(float)0||z<(float)0)
      gft::Error((char *)"Out-of-bounds",
		 (char *)"Scene32::GetValue_nn");
    */
    v.c.x=(int)(x+0.5);
    v.c.y=(int)(y+0.5);
    v.c.z=(int)(z+0.5);
    if(v.c.x > scn->xsize-1) v.c.x = scn->xsize-1;
    else if(v.c.x < 0)       v.c.x = 0;

    if(v.c.y > scn->ysize-1) v.c.y = scn->ysize-1;
    else if(v.c.y < 0)       v.c.y = 0;

    if(v.c.z > scn->zsize-1) v.c.z = scn->zsize-1;
    else if(v.c.z < 0)       v.c.z = 0;
    
    return scn->array[v.c.z][v.c.y][v.c.x];
  }


  int          GetMaximumValue(sScene32 *scn){
    int p,Imax;
    
    Imax = INT_MIN;
    for(p=0; p<scn->n; p++) {
      if(scn->data[p] > Imax)
	Imax = scn->data[p];
    }
    scn->maxval = Imax;
    return(Imax); 
  }

  int          GetMinimumValue(sScene32 *scn){
    int p,Imin;
  
    Imin = INT_MAX; 
    for(p=0; p<scn->n; p++){
      if(scn->data[p] < Imin)
	Imin = scn->data[p];
    }
    scn->minval = Imin;
    return(Imin); 
  }


  int GetMaxVal(sScene32 *scn){
    return GetMaximumValue(scn);
  }


  int GetMinVal(sScene32 *scn){
    return GetMinimumValue(scn);
  }
    

  sImage32 *GetSliceX(sScene32 *scn, int x){
    sImage32 *img;
    int z,y;
    img = gft::Image32::Create(scn->ysize, scn->zsize);
    for(z = 0; z < scn->zsize; z++){
      for(y = 0; y < scn->ysize; y++){
	img->array[z][y] = scn->array[z][y][x];
      }
    }
    img->dx = scn->dy;
    img->dy = scn->dz;
    if(scn->maxval == 0)
      GetMaximumValue(scn);
    img->maxval = scn->maxval;
    img->minval = scn->minval;
    return img;
  }


  sImage32 *GetSliceY(sScene32 *scn, int y){
    sImage32 *img;
    int z,x;
    img = gft::Image32::Create(scn->xsize, scn->zsize);
    for(z = 0; z < scn->zsize; z++){
      for(x = 0; x < scn->xsize; x++){
	img->array[z][x] = scn->array[z][y][x];
      }
    }
    img->dx = scn->dx;
    img->dy = scn->dz;
    if(scn->maxval == 0)
      GetMaximumValue(scn);
    img->maxval = scn->maxval;
    img->minval = scn->minval;
    return img;
  }

    
  sImage32 *GetSliceZ(sScene32 *scn, int z){
    sImage32 *img;
    int *data;
    img = gft::Image32::Create(scn->xsize, scn->ysize);
    data = scn->data + z*img->n;
    memcpy(img->data, data, sizeof(int)*img->n);
    img->dx = scn->dx;
    img->dy = scn->dy;
    if(scn->maxval == 0)
      GetMaximumValue(scn);
    img->maxval = scn->maxval;
    img->minval = scn->minval;
    return img;
  }

    
  void PutSliceX(sScene32 *scn, sImage32 *img, int x){
    int z,y;
    for(z = 0; z < scn->zsize; z++){
      for(y = 0; y < scn->ysize; y++){
	scn->array[z][y][x] = img->array[z][y];
      }
    }
  }
    

  void PutSliceY(sScene32 *scn, sImage32 *img, int y){
    int z,x;
    for(z = 0; z < scn->zsize; z++){
      for(x = 0; x < scn->xsize; x++){
	scn->array[z][y][x] = img->array[z][x];
      }
    }
  }

  void PutSliceZ(sScene32 *scn, sImage32 *img, int z){
    int *data;
    data = scn->data + z*img->n;
    memcpy(data, img->data, sizeof(int)*img->n);
  }
    

  void     MBB(sScene32 *scn, Voxel *l, Voxel *h){
    Voxel v;
    
    l->c.x  = scn->xsize-1;
    l->c.y  = scn->ysize-1;
    l->c.z  = scn->zsize-1;
    h->c.x = 0;
    h->c.y = 0;
    h->c.z = 0;
  
    for(v.c.z=0; v.c.z<scn->zsize; v.c.z++)
      for(v.c.y=0; v.c.y<scn->ysize; v.c.y++)
	for(v.c.x=0; v.c.x<scn->xsize; v.c.x++)    
	  if(scn->data[GetVoxelAddress(scn,v)] > 0){
	    if(v.c.x < l->c.x)
	      l->c.x = v.c.x;
	    if(v.c.y < l->c.y)
	      l->c.y = v.c.y;
	    if(v.c.z < l->c.z)
	      l->c.z = v.c.z;
	    if(v.c.x > h->c.x)
	      h->c.x = v.c.x;
	    if(v.c.y > h->c.y)
	      h->c.y = v.c.y;	
	    if(v.c.z > h->c.z)
	      h->c.z = v.c.z;	
	  }
  }


  sScene32 *MBB(sScene32 *scn){
    Voxel lower,higher;
    sScene32 *mbb=NULL;
    MBB(scn, &lower, &higher);
    mbb = SubScene(scn,
		   lower.c.x,  lower.c.y,  lower.c.z,
		   higher.c.x, higher.c.y, higher.c.z);
    return(mbb);
  }


  sScene32 *AddFrame(sScene32 *scn, int sz, int value){
    sScene32 *fscn;
    int y, z,*dst,*src,nbytes,offset1, offset2;
    
    fscn = Create(scn->xsize+(2*sz),
		  scn->ysize+(2*sz), 
		  scn->zsize+(2*sz));
    fscn->dx = scn->dx;
    fscn->dy = scn->dy;
    fscn->dz = scn->dz;

    Fill(fscn,value);
    nbytes = sizeof(int)*scn->xsize;
  
    offset1 = 0;
    offset2 = GetVoxelAddress(fscn, sz, sz, sz);
    
    for(z=0; z<scn->zsize; z++){
      src = scn->data+offset1;
      dst = fscn->data+offset2;
      for(y=0; y<scn->ysize; y++){
	memcpy(dst,src,nbytes);
	src += scn->xsize;
	dst += fscn->xsize;
      }
      offset1 += scn->xsize*scn->ysize;
      offset2 += fscn->xsize*fscn->ysize;
    }
    return(fscn);
  }


  sScene32 *RemFrame(sScene32 *fscn, int sz){
    sScene32 *scn;
    int y,z,*dst,*src,nbytes,offset;
    
    scn = Create(fscn->xsize-(2*sz),
		 fscn->ysize-(2*sz),
		 fscn->zsize-(2*sz));
    scn->dx = fscn->dx;
    scn->dy = fscn->dy;
    scn->dz = fscn->dz;

    nbytes = sizeof(int)*scn->xsize;  
    offset = GetVoxelAddress(fscn, sz, sz, sz);

    src = fscn->data+offset;
    dst = scn->data;
    for(z=0; z<scn->zsize; z++,src+=2*sz*fscn->xsize) {
      for(y=0; y<scn->ysize; y++,src+=fscn->xsize,dst+=scn->xsize){
	memcpy(dst,src,nbytes);
      }
    }
    return(scn);
  }


  sScene16 *ConvertTo16(sScene32 *scn){
    sScene16 *scn16;
    int p;

    scn16 = Scene16::Create(scn->xsize,
			    scn->ysize,
			    scn->zsize);
    scn16->dx = scn->dx;
    scn16->dy = scn->dy;
    scn16->dz = scn->dz;
    for(p=0; p<scn->n; p++){
      scn16->data[p] = (ushort)scn->data[p];
    }
    return scn16;
  }

  sScene8  *ConvertTo8(sScene32 *scn){
    sScene8 *scn8;
    int p;

    scn8 = Scene8::Create(scn->xsize,
			  scn->ysize,
			  scn->zsize);
    scn8->dx = scn->dx;
    scn8->dy = scn->dy;
    scn8->dz = scn->dz;
    for(p=0; p<scn->n; p++){
      scn8->data[p] = (uchar)scn->data[p];
    }
    return scn8;
  }


  sScene64 *ComputeIntegralScene(sScene32 *scn){
    sScene64 *Iscn=NULL;
    int p,q,i,j,k;
    int xysize = scn->xsize*scn->ysize;
    long long sum;
    
    //iscn = CreateScene(scn->xsize, scn->ysize, scn->zsize);
    //SetVoxelSize(iscn, scn->dx, scn->dy, scn->dz);
    Iscn = Scene64::Create(scn->xsize, scn->ysize, scn->zsize);
    
    for(k = 0; k < scn->zsize; k++){
      for(i = 0; i < scn->ysize; i++){
	for(j = 0; j < scn->xsize; j++){
	  p = GetVoxelAddress(scn,j,i,k);
	  sum = scn->data[p];
	  
	  if(k-1>=0){
	    q = p-xysize;
	    sum += Iscn->data[q];
	  }
	  if(j-1>=0){
	    q = p-1;
	    sum += Iscn->data[q];
	  }
	  if(j-1>=0 && k-1>=0){
	    q = p-1-xysize;
	    sum -= Iscn->data[q];
	  }
	  if(i-1>=0){
	    q = p-scn->xsize;
	    sum += Iscn->data[q];
	  }
	  if(i-1>=0 && k-1>=0){
	    q = p-scn->xsize-xysize;
	    sum -= Iscn->data[q];
	  }
	  if(j-1>=0 && i-1>=0){
	    q = p-1-scn->xsize;
	    sum -= Iscn->data[q];
	  }
	  if(j-1>=0 && i-1>=0 && k-1>=0){
	    q = p-1-scn->xsize-xysize;
	    sum += Iscn->data[q];
	  }
	  Iscn->data[p] = sum;
	}
      }
    }
    return Iscn;
  }
    


  //The dimensions of the window (i.e., xsize,ysize,zsize) 
  //should be given in millimeters.
  void DrawWindow(sScene32 *scn,
		  int val,
		  float xsize, 
		  float ysize, 
		  float zsize,
		  Voxel u){
    Voxel v,lo,hi;
    int dx,dy,dz;
    int q;
    float sum = 0.0;
    float vol;
    
    dx = ROUND(xsize/(scn->dx*2.0));
    dy = ROUND(ysize/(scn->dy*2.0));
    dz = ROUND(zsize/(scn->dz*2.0));
    
    //---------------------
    hi.c.x = MIN(u.c.x+dx, scn->xsize-1);
    hi.c.y = MIN(u.c.y+dy, scn->ysize-1);
    hi.c.z = MIN(u.c.z+dz, scn->zsize-1);
    
    lo.c.x = MAX(u.c.x-dx-1, 0);
    lo.c.y = MAX(u.c.y-dy-1, 0);
    lo.c.z = MAX(u.c.z-dz-1, 0);
    
    for(v.c.x = lo.c.x+1; v.c.x <= hi.c.x; v.c.x++){
      for(v.c.y = lo.c.y+1; v.c.y <= hi.c.y; v.c.y++){
	for(v.c.z = lo.c.z+1; v.c.z <= hi.c.z; v.c.z++){
	  q = GetVoxelAddress(scn,v.c.x,v.c.y,v.c.z);
	  scn->data[q] = val;
	}
      }
    }
  }


    //The dimensions of the window (i.e., xsize,ysize,zsize) 
    //should be given in millimeters.
    int WindowNvoxels(sScene32 *scn,
		      float xsize,
		      float ysize,
		      float zsize){
      int nw,dx,dy,dz;
      dx = ROUND(xsize/(scn->dx*2.0));
      dy = ROUND(ysize/(scn->dy*2.0));
      dz = ROUND(zsize/(scn->dz*2.0));
      nw = (2*dx + 1)*(2*dy + 1)*(2*dz + 1);
      return nw;
    }   
    

    
  } //end Scene32 namespace
} //end gft namespace

