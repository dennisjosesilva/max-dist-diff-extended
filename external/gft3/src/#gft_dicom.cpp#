
/* 

   dicom2scn
   Felipe Bergo - bergo@seul.org - June 2003

   limitations (mostly hardcoded defaults, easy to change on
                source-code level):

   slice images widths cannot be >4096 (8 bit data)
                                 >2048 (16 bit data)
                                 >1024 (32 bit data)

   individual DICOM file names cannot be longer than 512 characters
   maximum number of slices per volume: 1024
*/


#include "gft_dicom.h"

//char *argv_prog, *argv_src;


namespace gft{

  /* tipos de dados dos campos aceitos pelo DICOM 3 */
  VR DicomVR[26] = {
    { "AE",    16, 0 , 0 }, // 0
    { "AS",     4, 1 , 0 },
    { "AT",     4, 1 , 0 }, // 2
    { "CS",    16, 0 , 0 },
    { "DA",     8, 1 , 0 }, // 4
    { "DS",    16, 0 , 0 },
    { "DT",    26, 0 , 0 }, // 6
    { "FL",     4, 1 , 0 },
    { "FD",     8, 1 , 0 }, // 8
    { "IS",    12, 0 , 0 },
    { "LO",    64, 0 , 0 }, // 10
    { "LT", 10240, 0 , 0 },
    { "OB",     0, 0 , 1 }, // 12
    { "OW",     0, 0 , 1 },
    { "PN",    64, 0 , 0 }, // 14
    { "SH",    16, 0 , 0 },
    { "SL",     4, 1 , 0 }, // 16
    { "SQ",     0, 0 , 1 },
    { "SS",     2, 1 , 0 }, // 18
    { "ST",  1024, 0 , 0 },
    { "TM",    16, 0 , 0 }, // 20
    { "UI",    64, 0 , 0 },
    { "UL",     4, 1 , 0 }, // 22
    { "UN",     0, 0 , 1 },
    { "US",     2, 1 , 0 }, // 24
    { "UT",     0, 0 , 1 },
  };
  

/* ==================================================== */


DicomField * DFNew() {
  DicomField *df;
  df = (DicomField *) malloc(sizeof(DicomField));
  df->group = df->key = 0;
  df->data[0] = 0;
  df->len = 0;
  df->next = 0;
  return df;
}

void DFKill(DicomField *df) {
  if (df->next != 0) DFKill(df->next);
  free(df);
}


DicomSlice *DicomSliceNew(int w, int h){
  DicomSlice *i;
  i = (DicomSlice *)calloc(1, sizeof(DicomSlice));
  if(i == NULL)
    gft::Error((char *)MSG1,(char *)"DicomSliceNew");
  i->img = gft::Image32::Create(w, h);
  i->img->dx = i->img->dy = 1.0;
  i->zslice = 0.0;
  i->equip[0] = 0;
  i->modality[0] = 0;
  return i;
}



void DicomSliceKill(DicomSlice *i){
  if(i != NULL){
    gft::Image32::Destroy(&(i->img));
    free(i);
  }
}



int DicomSliceCmp(DicomSlice *a, DicomSlice *b){
  if (a->img->ncols != b->img->ncols ||
      a->img->nrows != b->img->nrows)
    return 1;
  if (a->img->dx != b->img->dx ||
      a->img->dy != b->img->dy)
    return 2;
  return(memcmp(a->img->data,b->img->data,
		sizeof(int) * a->img->ncols * a->img->nrows));
}


double robust_atof(char *p) {
  while( (*p) < 33)
    ++p;
  return(atof(p));
}


DicomSlice *ReadDicomSlice(char *filename, FILE *rf, int report){
  FILE *f;
  DicomSlice *I;
  char buf[4096], *p, q;
  DicomField *df, *col, *row, *zpos, *pixeldata, *bits, *spacing;
  int i,j,n,dataoffset, bpp;
  short int *sbuf;
  int       *ibuf;
  DicomField *thickness, *manuf, *model, *institution, *modality, *seqname;

  union {
    short int i16;
    char  i8[2];
  } endiantest;

  //  printf("reading %s\n",filename);

  f = fopen(filename,"r");
  if (!f) {
    fprintf(stderr,"dicom2scn: unable to open file %s\n",filename);
    exit(1);
  }

  if (rf && report)
    fprintf(rf,"reading %s\n",filename);

  /* check if NEMA header is present (DICM signature at offset 128) */
  fseek(f,128,SEEK_SET);
  fread(buf,1,4,f);
  buf[4]=0;
  if (strcmp(buf,"DICM")!=0) {
    fseek(f,0,SEEK_SET); /* no signature */
    if (rf && report) fprintf(rf,"NEMA=n ");
  } else {
    if (rf && report) fprintf(rf,"NEMA=y ");
  }

  /* read all fields */
  df = 0;
  while( ReadDicomField(f, &df) != 0 ) ;

  //  printf("done A\n");

  col         = DicomGetField(df, 0x0028, 0x0011);
  row         = DicomGetField(df, 0x0028, 0x0010);
  zpos        = DicomGetField(df, 0x0020, 0x1041);
  pixeldata   = DicomGetField(df, 0x7fe0, 0x0010);
  bits        = DicomGetField(df, 0x0028, 0x0100);
  spacing     = DicomGetField(df, 0x0028, 0x0030);
  thickness   = DicomGetField(df, 0x0018, 0x0050);
  manuf       = DicomGetField(df, 0x0008, 0x0070);
  model       = DicomGetField(df, 0x0008, 0x1090);
  institution = DicomGetField(df, 0x0008, 0x0080);
  modality    = DicomGetField(df, 0x0008, 0x0060);
  seqname     = DicomGetField(df, 0x0018, 0x0024);

  if (rf && report) 
    fprintf(rf,"W=%d\tH=%d\tZ=%.4f\tbits=%d\tdZ=%.4f",
	    col?atoi(col->data):-999,
	    row?atoi(row->data):-999,
	    zpos?atof(zpos->data):-999,
	    bits?atoi(bits->data):-999,
	    spacing?atof(spacing->data):-999);

  if (!col || !row || !pixeldata) {
    fprintf(stderr,"dicom2scn: at least one required field is missing.\n");
    exit(2);
  }

  I=DicomSliceNew(atoi(col->data),atoi(row->data));
  if (zpos) {
    //    printf("[%s]\n",zpos->data);
    I->zslice = atof(zpos->data);
  }

  I->thickness = 0.0;
  if (thickness)
    I->thickness = atof(thickness->data);

  strcpy(I->equip,manuf?(manuf->data):"?");
  strcat(I->equip,"/");
  strcat(I->equip,model?(model->data):"?");
  strcat(I->equip,"/");
  strcat(I->equip,institution?(institution->data):"?");

  strcpy(I->modality,modality?(modality->data):"?");
  strcat(I->modality,"/");
  strcat(I->modality,seqname?(seqname->data):"?");

  if (spacing) {
    strcpy(buf,spacing->data);
    p=strtok(buf,"\\");
    if (p) {
      I->img->dx = robust_atof(p);
      p = strtok(0,"\\");
      if (p) I->img->dy = robust_atof(p);
    }    
  }

  dataoffset = atoi(pixeldata->data);
  fseek(f,dataoffset,SEEK_SET);

  bpp = 16;
  if (bits)
    bpp = atoi(bits->data);

  n = I->img->ncols * I->img->nrows;
  switch(bpp) {

  case 8: /* ??? */
    for(i=0;i<n;i+=I->img->ncols) {
      fread(buf, 1, I->img->ncols, f);
      for(j=0;j<I->img->ncols;j++)
	I->img->data[i+j] = (int) ((unsigned char) (buf[j]));
    }
    break;
  case 16: /* the usual case */
    endiantest.i16 = 0x100;
    sbuf = (short int *) buf;
    for(i=0;i<n;i+=I->img->ncols) {
      fread(buf, 2, I->img->ncols, f);

      if (endiantest.i8[0]) {
	for(j=0;j<I->img->ncols;j++) {
	  q = buf[2*j];
	  buf[2*j] = buf[2*j+1];
	  buf[2*j+1] = q;
	}
      }      

      for(j=0;j<I->img->ncols;j++)
	I->img->data[i+j] = (int) (sbuf[j]);
	
    }
    break;
  case 32: /* ??? */
    endiantest.i16 = 0x100;
    ibuf = (int *) buf;
    for(i=0;i<n;i+=I->img->ncols) {
      fread(buf, 4, I->img->ncols, f);

      if (endiantest.i8[0]) {
	for(j=0;j<I->img->ncols;j++) {
	  q = buf[4*j];
	  buf[4*j] = buf[4*j+3];
	  buf[4*j+3] = q;
	  q = buf[4*j+1];
	  buf[4*j+1] = buf[4*j+2];
	  buf[4*j+2] = q;
	}
      }

      for(j=0;j<I->img->ncols;j++)
	I->img->data[i+j] = ibuf[j];
		
    }
    break;
  }

  fclose(f);
  DFKill(df);

  if (rf && report) fprintf(rf,"\n");
  return(I);
}


int ConvInt32(unsigned char *x) {
  int a,b,c,d;
  a = (int) (x[0]);
  b = (int) (x[1]);
  c = (int) (x[2]);
  d = (int) (x[3]);
  a = a + 256*b + 65536*c + 16777216*d;
  return a;
}

int ConvInt16(unsigned char *x) {
  int a,b;
  a = (int) (x[0]);
  b = (int) (x[1]);
  a = a + 256*b;
  return a;
}

DicomField * DicomGetField(DicomField *H, int group, int key) {
  for(;H;H=H->next)
    if (H->group == group && H->key == key)
      return H;
  return 0;
}

int ReadDicomField(FILE *f, DicomField **df) {
  DicomField *x;
  unsigned char b[64];
  int i;

  //  printf("fpos=%d\n",(int)(ftell(f)));

  x = DFNew();
  if (fread(b,1,4,f)!=4) {
    DFKill(x);
    return 0;
  }

  x->group = (int) b[0];
  x->group += 256 * ((int) b[1]);
  x->key   = (int) b[2];
  x->key   += 256 * ((int) b[3]);

  // printf("g:k = %.4x:%.4x\n",x->group,x->key);

  if (fread(b,1,4,f)!=4) {
    DFKill(x);
    return 0;
  }

  //  printf("hit 1 b=[%c%c]-%x:%x\n",b[0],b[1],b[2],b[3]);

  for(i=0;i<26;i++) {
    if (b[0] == DicomVR[i].id[0] && b[1] == DicomVR[i].id[1]) {

      //      printf("VR match: %c%c\n",b[0],b[1]);
      if (DicomVR[i].longlen) {
	if (fread(b,1,4,f)!=4) {
	  DFKill(x);
	  return 0;
	}
	x->len = ConvInt32(b);
	//	printf("longlen=%d (0x%.8x) %d %d %d %d\n",x->len, x->len, b[0],b[1],b[2],b[3]);
      } else {
	x->len = ConvInt16(&b[2]);
	//	printf("shortlen=%d (0x%.4x)\n",x->len,x->len);
      }
      if (x->len > 31) {
	sprintf(x->data,"%d",(int)(ftell(f)));
	if (fseek(f,x->len,SEEK_CUR)!=0) {
	  DFKill(x);
	  return 0;
	}
      } else {
	memset(b,0,64);
	if (fread(b,1,x->len,f)!=x->len) {
	  DFKill(x);
	  return 0;
	}
	if (i==16 || i==22) {
	  sprintf(x->data,"%d",(int)(ConvInt32(b)));
	} else if (i==18 || i==24) {
	  sprintf(x->data,"%d",(int)(ConvInt16(b)));
	} else {
	  memset(x->data,0,64);
	  memcpy(x->data,b,x->len);
	}
      }
      x->next = *df;
      *df     = x;
      return 1;
    }
  }

  //  printf("hit 3\n");

  // implicit VLs
  x->len = ConvInt32(&b[0]);
  //printf("LEN = %d\n",x->len);

  if (x->len > 63) {
    sprintf(x->data,"%d",(int)(ftell(f)));
    fseek(f, x->len, SEEK_CUR);
  } else {
    if (fread(b,1,x->len,f)!=x->len) {
      DFKill(x);
      return 0;
    }

    // slice thickness: always DS type
    // modality: always CS
    // seq name: always SH
    if ((x->group == 0x18 && x->key == 0x50) ||
	(x->group == 0x08 && x->key == 0x60) ||
	(x->group == 0x18 && x->key == 0x24) ||
	(x->group == 0x20 && x->key == 0x1041))
    {
      memset(x->data,0,64);
      memcpy(x->data,b,x->len);
    } else {

      switch(x->len) {
      case 2:
	sprintf(x->data,"%d",ConvInt16(b));
	break;
      case 4:
	sprintf(x->data,"%d",ConvInt32(b));
	break;
      default:
	memset(x->data,0,64);
	memcpy(x->data,b,x->len);
      }

    }
  }
  
  x->next = *df;
  *df     = x;

  //  printf("hit4\n");

  return 1;
}

int basenamelen = 0;

int slicecmp(const void *a, const void *b) {
  const char *sa, *sb;
  int na,nb;

  sa = (const char *) a;
  sb = (const char *) b;

  na = slicenumber(&sa[basenamelen]);
  nb = slicenumber(&sb[basenamelen]);
  if (na<nb) return -1;
  if (na>nb) return 1;
  return 0;
}

int zcmp(const void *a, const void *b) {
  const DicomSlice **sa, **sb;
  float na,nb;

  sa = (const DicomSlice **) a;
  sb = (const DicomSlice **) b;

  na = (*sa)->zslice;
  nb = (*sb)->zslice;
  if (na<nb) return -1;
  if (na>nb) return 1;
  return 0;
}


gft::sScene32 *DicomDirectoryConvert(char *inputsample,
				     int report){
  char dirpath[1024], basename[512], tmp[1024];
  char *farray;
  DIR *d;
  struct dirent *de;
  int i, j, k, nfiles = 0;
  DicomSlice **vol;
  float mz, nz, dx, dy, dz;
  FILE *f;
  FILE *rf=0;
  int minval, maxval, om;
  int fixneg=0, fixrange=0;
  int lenf;
  gft::sScene32 *scn;
  
  if (report)
    rf = fopen("dicom.txt", "w");

  DicomNameSplit(inputsample, dirpath, 1024, basename);

  printf("\r reading directory...         ");
  fflush(stdout);

  d = opendir(dirpath);
  if (d==NULL) {
    fprintf(stderr,"dicom2scn: cannot open directory %s\n",dirpath);
    exit(3);
  }

  farray = (char *) malloc(512 * 1024);
  if (!farray) exit(5);
  
  while(( de = readdir(d) ) != 0) {
    lenf = strlen(de->d_name);
    if(lenf < 4) continue;
    if(strcasecmp(&de->d_name[lenf-4],".dcm")==0){
      memset(&farray[512*nfiles],0,512);      
      strncpy(&farray[512*nfiles],de->d_name,511);
      ++nfiles;
    }
  }
  closedir(d);

  vol = (DicomSlice **) malloc(sizeof(DicomSlice *) * nfiles );

  /* read each slice's image */
  for(i=0;i<nfiles;i++) {
    sprintf(tmp,"%s/%s",dirpath,&farray[512*i]);

    printf("\r reading slice %d of %d            ",i+1,nfiles);
    fflush(stdout);

    vol[i] = ReadDicomSlice(tmp, rf, report);

    for(k=0; k<i; k++){
      if (vol[k]->zslice == vol[i]->zslice) {
	if (DicomSliceCmp(vol[k], vol[i])==0) {
	  printf("\nFound repeated slice, ignoring it and everything after it.\n");
	  nfiles = i+1;
	  break;
	} else {
	  printf("\nThere is Z overlapping, something is odd here...proceeding anyway.\n");
	}
      }
    }
    
    if (i>0) {
      if (vol[i]->img->ncols != vol[i-1]->img->ncols ||
	  vol[i]->img->nrows != vol[i-1]->img->nrows ||
	  vol[i]->img->dx != vol[i-1]->img->dx ||
	  vol[i]->img->dy != vol[i-1]->img->dy) {
	fprintf(stderr,"\n** Error: slice parameter (width, height, dx and/or dy) discontinuity from slice %s to slice %s. Stop.\n(try bounding the range with -b and -e, most likely this DICOM sequence contains several volumes at different sizes/modalities)\n\n", &farray[512*(i-1)], &farray[512*i]);
	exit(2);
      }
    }
    
  }

  printf("\r positionally sorting slices...           ");
  fflush(stdout);

  /* sort slices according to the z position */
  qsort(vol, nfiles, sizeof(DicomSlice *), zcmp);

  /* find out dz */
  mz = nz = 0.0;

  for(i=0;i<nfiles-1;i++) {
    mz += fabs(vol[i]->zslice - vol[i+1]->zslice);
    nz++;
  }

  dx = vol[0]->img->dx;
  dy = vol[0]->img->dy;
  dz = mz / nz;

  /*
  printf("\r checking bounds...               ");
  fflush(stdout);
  minval = INT_MAX; //100000;
  maxval = INT_MIN; //-100000;
  for(i=0;i<nfiles;i++) {
    k = vol[i]->img->ncols * vol[i]->img->nrows;
    for(j=0; j<k; j++){
      if (vol[i]->img->data[j] < minval) minval = vol[i]->img->data[j];
      if (vol[i]->img->data[j] > maxval) maxval = vol[i]->img->data[j];
    }
  }
  printf("\r checking bounds: %d to %d\n",minval,maxval);
  fflush(stdout);

  om = 0;
  if (minval < 0) {
    fixneg = 1;
    printf("\r fixing negative values...      ");
    fflush(stdout);

    for(i=0; i<nfiles; i++) {
      k = vol[i]->img->ncols * vol[i]->img->nrows;
      for(j=0; j<k; j++)
	vol[i]->img->data[j] += -minval;
    }
    om = -minval;
    minval = 0;
    maxval += -minval;
  }

  if (maxval - minval > 32767) {
    fixrange = 1;
    printf("\r fixing range...                ");
    fflush(stdout);
    for(i=0; i<nfiles; i++) {
      k = vol[i]->img->ncols * vol[i]->img->nrows;
      for(j=0; j<k; j++)
	vol[i]->img->data[j] = gft::LinearStretch(vol[i]->img->data[j],
						  minval,maxval,
						  0,32767);
    }
  }

  if (fixneg)
    printf("\r!! Negative values fixed, %d added to all intensities.\n",om);
  if (fixrange)
    printf("\r!! Dynamic range > 32767 fixed by normalization.\n");
  */

  /*
  printf("\r writing scn... 0%%               ");
  fflush(stdout);

  f = fopen(outputscn,"w");
  if (!f) {
    fprintf(stderr,"dicom2scn: cannot open %s for writing.\n",outputscn);
    exit(3);
  }

  fprintf(f,"SCN\n%d %d %d\n%.4f %.4f %.4f\n16\n",
	  vol[0]->img->ncols, vol[0]->img->nrows, nfiles,
	  dx, dy, dz);

  for(i=0; i<nfiles; i++) {
    printf("\r writing scn... %d%%              ",((i+1)*100)/nfiles);
    fflush(stdout);
    DicomSliceDump16(f, vol[i]);
  }

  printf("\r clean up                           ");
  fflush(stdout);

  fclose(f);
  */
  //--------
  scn = gft::Scene32::Create(vol[0]->img->ncols,
			     vol[0]->img->nrows, nfiles);
  scn->dx = dx;
  scn->dy = dy;
  scn->dz = dz;
  for(i=0; i<nfiles; i++){
    k = vol[i]->img->ncols * vol[i]->img->nrows;
    for(j=0; j<k; j++)
      scn->data[i*k + j] = vol[i]->img->data[j];
  }

  //--------
  for(i=0;i<nfiles;i++)
    DicomSliceKill(vol[i]);
  free(vol);

  if (rf && report)
    fclose(rf);

  printf("\r                             \r");
  fflush(stdout);

  scn->maxval = GetMaximumValue(scn);
  scn->minval = GetMinimumValue(scn);  
  return scn;
}





gft::sScene32 *DicomDirectoryConvert(char *inputsample, //char *outputscn, 
				     int first, int last, int report){
  char dirpath[1024], basename[512], tmp[1024];
  char *farray;
  DIR *d;
  struct dirent *de;
  int i, j, k, nfiles = 0;
  DicomSlice **vol;
  float mz, nz, dx, dy, dz;
  FILE *f;
  FILE *rf=0;
  int minval, maxval, om;
  int fixneg=0, fixrange=0;
  int lenf;
  gft::sScene32 *scn;
  
  if (report)
    rf = fopen("dicom.txt", "w");

  DicomNameSplit(inputsample, dirpath, 1024, basename);
  basenamelen = strlen(basename);

  printf("\r reading directory...         ");
  fflush(stdout);

  d = opendir(dirpath);
  if (d==NULL) {
    fprintf(stderr,"dicom2scn: cannot open directory %s\n",dirpath);
    exit(3);
  }

  farray = (char *) malloc(512 * 1024);
  if (!farray) exit(5);
  
  while(( de = readdir(d) ) != 0) {
    lenf = strlen(de->d_name);
    if(lenf < basenamelen+4) continue;
    if(strncmp(de->d_name, basename, basenamelen)==0 &&
       strcasecmp(&de->d_name[lenf-4],".dcm")==0){

      i = slicenumber(&(de->d_name[strlen(basename)]));
      if (i<first || i>last)
	continue;

      memset(&farray[512*nfiles],0,512);      
      strncpy(&farray[512*nfiles],de->d_name,511);
      ++nfiles;
    }
  }
  closedir(d);

  printf("\r numerically sorting slices...           ");
  fflush(stdout);

  /* sort slices according to the filesystem slice # */
  qsort(farray,nfiles,512,slicecmp);

  vol = (DicomSlice **) malloc(sizeof(DicomSlice *) * nfiles );

  /* read each slice's image */
  for(i=0;i<nfiles;i++) {
    sprintf(tmp,"%s/%s",dirpath,&farray[512*i]);

    printf("\r reading slice %d of %d            ",i+1,nfiles);
    fflush(stdout);

    j = atoi(&farray[512*i+strlen(basename)]);

    if (rf && report)
      fprintf(rf,"#=%d ",j);
    vol[i] = ReadDicomSlice(tmp, rf, report);

    //for(k=0;k<i-1;k++) {
    for(k=0; k<i; k++){
      if (vol[k]->zslice == vol[i]->zslice) {
	if (DicomSliceCmp(vol[k], vol[i])==0) {
	  printf("\nFound repeated slice, ignoring it and everything after it.\n");
	  nfiles = i+1;
	  break;
	} else {
	  printf("\nThere is Z overlapping, something is odd here...proceeding anyway.\n");
	}
      }
    }
    
    if (i>0) {
      if (vol[i]->img->ncols != vol[i-1]->img->ncols ||
	  vol[i]->img->nrows != vol[i-1]->img->nrows ||
	  vol[i]->img->dx != vol[i-1]->img->dx ||
	  vol[i]->img->dy != vol[i-1]->img->dy) {
	fprintf(stderr,"\n** Error: slice parameter (width, height, dx and/or dy) discontinuity from slice %d to slice %d. Stop.\n(try bounding the range with -b and -e, most likely this DICOM sequence contains several volumes at different sizes/modalities)\n\n", j-1, j);
	exit(2);
      }
    }
    
  }

  printf("\r positionally sorting slices...           ");
  fflush(stdout);

  /* sort slices according to the z position */
  qsort(vol, nfiles, sizeof(DicomSlice *), zcmp);

  /* find out dz */
  mz = nz = 0.0;

  for(i=0;i<nfiles-1;i++) {
    mz += fabs(vol[i]->zslice - vol[i+1]->zslice);
    nz++;
  }

  dx = vol[0]->img->dx;
  dy = vol[0]->img->dy;
  dz = mz / nz;

  /*
  printf("\r checking bounds...               ");
  fflush(stdout);
  minval = INT_MAX; //100000;
  maxval = INT_MIN; //-100000;
  for(i=0;i<nfiles;i++) {
    k = vol[i]->img->ncols * vol[i]->img->nrows;
    for(j=0; j<k; j++){
      if (vol[i]->img->data[j] < minval) minval = vol[i]->img->data[j];
      if (vol[i]->img->data[j] > maxval) maxval = vol[i]->img->data[j];
    }
  }
  printf("\r checking bounds: %d to %d\n",minval,maxval);
  fflush(stdout);

  om = 0;
  if (minval < 0) {
    fixneg = 1;
    printf("\r fixing negative values...      ");
    fflush(stdout);

    for(i=0; i<nfiles; i++) {
      k = vol[i]->img->ncols * vol[i]->img->nrows;
      for(j=0; j<k; j++)
	vol[i]->img->data[j] += -minval;
    }
    om = -minval;
    minval = 0;
    maxval += -minval;
  }

  if (maxval - minval > 32767) {
    fixrange = 1;
    printf("\r fixing range...                ");
    fflush(stdout);
    for(i=0; i<nfiles; i++) {
      k = vol[i]->img->ncols * vol[i]->img->nrows;
      for(j=0; j<k; j++)
	vol[i]->img->data[j] = gft::LinearStretch(vol[i]->img->data[j],
						  minval,maxval,
						  0,32767);
    }
  }

  if (fixneg)
    printf("\r!! Negative values fixed, %d added to all intensities.\n",om);
  if (fixrange)
    printf("\r!! Dynamic range > 32767 fixed by normalization.\n");
  */

  /*
  printf("\r writing scn... 0%%               ");
  fflush(stdout);

  f = fopen(outputscn,"w");
  if (!f) {
    fprintf(stderr,"dicom2scn: cannot open %s for writing.\n",outputscn);
    exit(3);
  }

  fprintf(f,"SCN\n%d %d %d\n%.4f %.4f %.4f\n16\n",
	  vol[0]->img->ncols, vol[0]->img->nrows, nfiles,
	  dx, dy, dz);

  for(i=0; i<nfiles; i++) {
    printf("\r writing scn... %d%%              ",((i+1)*100)/nfiles);
    fflush(stdout);
    DicomSliceDump16(f, vol[i]);
  }

  printf("\r clean up                           ");
  fflush(stdout);

  fclose(f);
  */
  //--------
  scn = gft::Scene32::Create(vol[0]->img->ncols,
			     vol[0]->img->nrows, nfiles);
  scn->dx = dx;
  scn->dy = dy;
  scn->dz = dz;
  for(i=0; i<nfiles; i++){
    k = vol[i]->img->ncols * vol[i]->img->nrows;
    for(j=0; j<k; j++)
      scn->data[i*k + j] = vol[i]->img->data[j];
  }

  //--------
  for(i=0;i<nfiles;i++)
    DicomSliceKill(vol[i]);
  free(vol);

  if (rf && report)
    fclose(rf);

  printf("\r                             \r");
  fflush(stdout);

  scn->maxval = GetMaximumValue(scn);
  scn->minval = GetMinimumValue(scn);  
  return scn;
}






/* split full name isample into its directory, basename,
   and disregard the slice number */
void DicomNameSplit(char *isample, 
		    char *odirpath, int odsz, 
		    char *obasename) {
  char *p;
  int j;

  /* if isample has no /, make odirpath the current dir, 
     and obasename <- isample */
  if (!strchr(isample, '/')) {
    getcwd(odirpath, odsz);
    strcpy(obasename, isample);
  } else {
    /* otherwise, copy path to odirpath, rest to obasename */
    strcpy(odirpath, isample);
    p = strrchr(odirpath, '/');
    *p = 0;
    strcpy(obasename, p+1);
  } 

  j = strlen(obasename);

  /* .dcm files */
  /* remove slice#.dcm from obasename */
  if (j >= 4) {
    if (strcasecmp(&obasename[j-4],".dcm")==0) {
      p = &obasename[j-4];
      *p = 0;
    }
  }

  /* non-.dcm */
  /* remove slice number from obasename */
  j = strlen(obasename);

  if (j>0) {
    p = &obasename[j-1];
    while(*p >= '0' && *p <= '9'){
      *p = 0;
      if(p == obasename)
	break;
      --p;
    }
  }
  return;
}

/* read all dicoms and find out the sequences in there */
/*
void DicomDirectoryInfo(char *inputsample, int extract) {
  char dirpath[1024], basename[512], tmp[1024];
  char *farray;
  DIR *d;
  struct dirent *de;
  int i,j,k, nfiles = 0;
  DicomSlice **vol;
  int heads[1024], nheads=0;
  int index;

  DicomNameSplit(inputsample, dirpath, 1024, basename);
  basenamelen = strlen(basename);

  printf("\r reading directory...          ");
  fflush(stdout);

  d = opendir(dirpath);
  if (d==NULL) {
    fprintf(stderr,"dicom2scn: cannot open directory %s\n",dirpath);
    exit(3);
  }

  farray = (char *) malloc(512 * 1024);
  if (!farray) exit(5);

  while(( de = readdir(d) ) != 0) {
    if (strncmp(de->d_name, basename, basenamelen)==0) {
      memset(&farray[512*nfiles],0,512);
      strncpy(&farray[512*nfiles],de->d_name,511);
      ++nfiles;
    }
  }
  closedir(d);

  printf("\r numerically sorting slices...           ");
  fflush(stdout);

  // sort slices
  qsort(farray,nfiles,512,slicecmp);

  // read slices
  heads[0] = 0;
  nheads = 1;
  vol = (DicomSlice **) malloc(sizeof(DicomSlice *) * nfiles );

  for(i=0;i<nfiles;i++) {
    sprintf(tmp,"%s/%s",dirpath,&farray[512*i]);

    printf("\r reading slice %d of %d            ",i+1,nfiles);
    fflush(stdout);

    j = slicenumber(&farray[512*i+basenamelen]);
    
    vol[i] = ReadDicomSlice(tmp, 0, 0);
    vol[i]->seqnum = j;

    for(k=heads[nheads-1];k<i-1;k++) {
      if (vol[k]->zslice == vol[i]->zslice) {
	heads[nheads++] = i;
      }
    }

    if (i>0) {
      if (vol[i]->W != vol[i-1]->W ||
          vol[i]->H != vol[i-1]->H ||
          vol[i]->dx != vol[i-1]->dx ||
          vol[i]->dy != vol[i-1]->dy ||
	  vol[i]->zslice == vol[i-1]->zslice) {
	heads[nheads++] = i;
      }
    }
  }
  heads[nheads] = nfiles;
  
  printf("\r                                   \n");

  index = 0;
  for(i=0;i<nheads;i++)
    index += DicomSeqInfo(index, vol, heads[i], heads[i+1]-1,extract);

  for(i=0;i<nfiles;i++)
    DicomSliceKill(vol[i]);
  free(vol);
}
*/



/*
int DicomSeqInfo(int index, DicomSlice **vol, int first, int last,int extract) {
  int i;
  double mdz, cdz, dz, thickness;
  int b,e;
  int minval, maxval;

  b = vol[first]->seqnum;
  e = vol[last]->seqnum;

  if (e < b) return 0;

  // sort slices according to the z position
  qsort(&vol[first], last-first+1, sizeof(DicomSlice *), zcmp);

  mdz = 0.0;
  cdz = 0.0;
  for(i=first;i<last;i++) {
    mdz += fabs(vol[i+1]->zslice - vol[i]->zslice);
    cdz += 1.0;
  }

  dz = 1.0;
  if (cdz > 0.0)
    dz = mdz / cdz;

  mdz = 0.0;
  cdz = 0.0;
  for(i=first;i<=last;i++) {
    mdz += vol[i]->thickness;
    cdz += 1.0;
  }
  thickness = 0.0;
  if (cdz > 0.0)
    thickness = mdz /cdz;

  minval =  2000000;
  maxval = -2000000;
  for(i=first;i<=last;i++) { 
    minval = MIN(minval, gft::Image32::GetMinVal(vol[i]->img));
    maxval = MAX(maxval, gft::Image32::GetMaxVal(vol[i]->img));
  }

  printf("[#%d] %dx%dx%d (%.2fx%.2fx%.2f) -- to extract: -b %d -e %d\n",
	 index+1, vol[first]->W, vol[first]->H, last-first+1, 
	 vol[first]->dx,vol[first]->dy,dz,b,e);
  printf("      range: [%d,%d] thickness: %.2f\n",minval,maxval,thickness);
  printf("      modality: [%s] equipment: [%s]\n",
	 vol[first]->modality,
	 vol[first]->equip);

  if (extract)
    DicomExtractSeq(index,b,e);
  printf("\n");

  return 1;
}


void DicomExtractSeq(int index,int b,int e) {
  int devnull,pid;

  char out[128];
  char bv[10],ev[10];

  sprintf(out,"dicom%.4d.scn",index+1);
  printf("      extracting as %s...",out); 
  fflush(stdout);

  devnull = open("/dev/null",O_WRONLY);

  pid = fork();

  if (!pid) {
    close(1);
    close(2);
    dup2(devnull, 1);
    dup2(devnull, 2);
    
    sprintf(bv,"%d",b);
    sprintf(ev,"%d",e);
    execlp(argv_prog,argv_prog,"-b",bv,"-e",ev,argv_src,out,(char *)NULL);
    return;

  } else {
    waitpid(pid,0,0);
    printf(" done\n");
  }

  close(devnull);
}
*/


void DicomSliceDump16(FILE *f, DicomSlice *S) {
  short int *tmp;
  char *ctmp;
  int i,n,left;

  union {
    short int i16;
    char  i8[2];
  } endiantest;
  
  endiantest.i16 = 0x0100;

  n = (S->img->ncols) * (S->img->nrows);
  tmp = (short int *) malloc(2*n);
  
  for(i=0;i<n;i++)
    tmp[i] = (short int) S->img->data[i];

  /* flip endianness to little-endian if needed */
  if (endiantest.i8[0]) {
    for(i=0;i<n;i++)
      tmp[i] = (tmp[i] >>8 ) | ((tmp[i]&0xff)<<8);
  }

  /* write out */
  left = n * 2;
  ctmp = (char *) tmp;
  while(left) {
    i = fwrite(ctmp, 1, left > 16384 ? 16384 : left, f);
    if (i<=0) {
      fprintf(stderr,"dicom2scn: write error.\n");
      exit(4);
    }
    ctmp += i;
    left -= i;
  }

  free(tmp);
}


void  FindSampleFileIfDirectory(char *sample) {
  DIR *d;
  struct dirent *de;
  struct stat s;
  int i,j,k,m,z;
  char *x, emsg[128];
  char tmp[1024],xtmp[1024];

  if (stat(sample, &s)!=0) {
    switch(errno) {
    case ENOENT:
    case ENOTDIR:
      strcpy(emsg, "path or file not found.");
      break;
    case EACCES:
      strcpy(emsg, "permission denied.");
      break;
    default:
      sprintf(emsg, "errno=%d.",errno);
      break;
    }
    fprintf(stderr," unable to stat %s: %s\n",sample,emsg);
    exit(2);
  }
  if (!S_ISDIR(s.st_mode)) return;
  d = opendir(sample);
  if (!d) {
    fprintf(stderr," unable to read directory %s, errno=%d.\n",sample,errno);
    exit(2);
  }

  strcpy(tmp, sample);
  if (tmp[strlen(tmp)-1] != '/')
    strcat(tmp,"/");
  z = 0;

  while(( de = readdir(d) ) != 0) {
    x = de->d_name;
    j = strlen(x);
    k = 0;
    m = 1;

    sprintf(xtmp,"%s%s",tmp,x);
    if (stat(xtmp,&s)!=0) continue;
    if (!S_ISREG(s.st_mode)) continue;

    /* pattern 1: *.dcm */
    if (j>4) {
      if (strcasecmp(&x[j-4],".dcm")==0) {
	printf("[directory given, %s matches naming convention]\n",x);
	strcpy(sample, xtmp);
	z = 1;
	break;
      }
    }

    /* pattern 2: [A-Z]*[0-9]+ */
    for(i=0;i<j;i++) {
      if (k==0) {
	if (x[i] >= '0' && x[i] <= '9') { k=1; continue; }
	if (x[i] >= 'A' && x[i] <= 'z') { continue; }
	m = 0; break;
      }
      if (k==1) {
	if (x[i] < '0' || x[i] > '9') { m=0; break; }
      }
    }
    if (k==1 && m==1) {
      printf("[directory given, %s matches naming convention]\n",x);
      strcpy(sample, xtmp);
      z = 1;
      break;
    }
  }
  closedir(d);
  if (!z) {
    fprintf(stderr,"[directory given, but no suitable DICOM file found in it]\n");
    exit(4);
  }
}



/*
void usage() {
    fprintf(stderr,"\nusage: dicom2scn [options] inputsample [outfile.scn]\n\n");
    fprintf(stderr,"  inputsample is ANY DICOM slice file, the other\n");
    fprintf(stderr,"  slices will be found automagically in the same\n");
    fprintf(stderr,"  directory (proper naming required). If you have a\n");
    fprintf(stderr,"  directory with ONLY DICOM files in it, you can\n");
    fprintf(stderr,"  provide just the directory name too.\n\n");
    fprintf(stderr,"options:\n");
    fprintf(stderr," -b n           start conversion at frame n [1]\n");
    fprintf(stderr," -e n           end conversion at frame n [last]\n");
    fprintf(stderr," -r             generate a dicom.txt report\n");
    fprintf(stderr," -i             print a description of the DICOM\n");
    fprintf(stderr,"                sequence(s) and exit.\n");
    fprintf(stderr," -xa            extract all volumes in this DICOM\n");
    fprintf(stderr," -h , --help    print this help\n\n");
    exit(1);
}


int main(int argc, char **argv) {
  char dicom[1024], scn[256];
  int first, last, i, j, wantreport, infomode, extract=0;

  first  = 1; 
  last   = 10000;
  wantreport = 0;
  infomode   = 0;
  j=0;
  strcpy(scn, "dicom.scn");

  argv_prog = argv[0];

  printf("dicom2scn - (c) 2002-2004 Felipe Bergo - bergo@seul.org\n");

  for(i=1;i<argc;i++) {
    if (strcmp(argv[i], "-b")==0) {
      if (i==argc-1) usage();
      first = atoi(argv[++i]);
      continue;
    }
    if (strcmp(argv[i], "-e")==0) {
      if (i==argc-1) usage();
      last = atoi(argv[++i]);
      continue;
    }
    if (strcmp(argv[i], "-r")==0) {
      wantreport = 1;
      continue;
    }
    if (strcmp(argv[i], "-i")==0) {
      infomode = 1;
      extract = 0;
      continue;
    }
    if (strcmp(argv[i], "-xa")==0) {
      infomode = 1;
      extract = 1;
      continue;
    }
    if (strcmp(argv[i], "-h")==0)
      usage();
    if (strcmp(argv[i], "--help")==0)
      usage();

    switch(j) {
    case 0:  strcpy(dicom, argv[i]); ++j; argv_src = argv[i]; break;
    case 1:  strcpy(scn, argv[i]); ++j; break;
    default: usage();
    }
  }

  if (j==0) usage();

  FindSampleFileIfDirectory(dicom);

  if (!infomode)
    DicomDirectoryConvert(dicom,scn,first,last,wantreport);
  else
    DicomDirectoryInfo(dicom,extract);
  
  return 0;
}
*/


int slicenumber(const char *s) {
  int value = 0;
  const char *p;
  for(p=s;(*p)!=0;p++)
    if ((*p)>='0' && (*p)<='9')
      value = (10*value) + ((*p)-'0');
    else
      break;
  return value;
}



} //end gft namespace

