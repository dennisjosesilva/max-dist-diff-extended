
#define APPDEBUG  1

#include "gft.h"

#include "edt.hpp"


gft::sImage32 *ReadAnyImage(char *file){
  gft::sImage32 *img;
  char command[512];
  int s;

  s = strlen(file);
  if(strcasecmp(&file[s-3], (char *)"pgm") == 0){
    img = gft::Image32::Read(file);
  }
  else{
    sprintf(command, (char *)"convert %s image_tmp.pgm", file);
    system(command);
    img = gft::Image32::Read((char *)"image_tmp.pgm");
    system((char *)"rm image_tmp.pgm");
  }
  return img;
}



int main(int argc, char **argv){
  gft::sImage32 *img=NULL, *bin=NULL, *border=NULL, *edt=NULL;
  gft::sAdjRel *A4, *A8;
  char filename[512];
  int Imin, Imax, T;
  //clock_t start, end;
  struct timeval tic,toc;
  double totaltime;

  if(argc < 2){
    fprintf(stdout,"usage:\n");
    fprintf(stdout,"dtmt <image>\n");
    exit(0);
  }
  strcpy(filename, argv[1]);
  img = ReadAnyImage(filename);
  A4 = gft::AdjRel::Neighborhood_4();
  A8 = gft::AdjRel::Neighborhood_8();  
  
  Imin = gft::Image32::GetMinVal(img);
  Imax = gft::Image32::GetMaxVal(img);
  printf("Imin: %d, Imax: %d\n", Imin, Imax);

  gettimeofday(&tic,NULL);
  //start = clock();
  //------------------------------------------
  
  for(T=Imax; T > Imin; T--){
    bin = gft::Image32::Threshold(img, T, Imax);

    //edt = gft::Image32::Mask2EDT(bin, A, INTERIOR, INT_MAX, 0);
    edt = EDT(bin, A8, INTERIOR);

#ifdef APPDEBUG
    border = GetObjBorders(bin, A4);
    sprintf(filename, (char *)"../out/border%03d.pgm", T);
    gft::Image32::Write(border, (char *)filename);
    gft::Image32::Destroy(&border);
    
    sprintf(filename, (char *)"../out/edt%03d.pgm", T);
    gft::Image32::Write(edt, (char *)filename);
#endif
    
    gft::Image32::Destroy(&bin);
    gft::Image32::Destroy(&edt);
  }

  //------------------------------------------
  //end = clock();
  gettimeofday(&toc,NULL);

  //totaltime = 1000.0*(((double)(end - start))/CLOCKS_PER_SEC);
  totaltime = ((toc.tv_sec-tic.tv_sec)*1000.0 + 
	       (toc.tv_usec-tic.tv_usec)*0.001);

  printf("Total time: %f ms\n",totaltime);
 
  gft::Image32::Destroy(&img);
  gft::AdjRel::Destroy(&A8);
  gft::AdjRel::Destroy(&A4);
  return 0;
}
  
