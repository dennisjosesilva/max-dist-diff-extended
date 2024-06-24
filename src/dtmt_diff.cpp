
// #define APPDEBUG  1

#include "gft.h"

#include "edt_diff.hpp"


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



//Pega o valor do menor vizinho de cada pixel:
gft::sImage32 *get_min_neighbor(gft::sImage32 *img, gft::sAdjRel *A){
  gft::sImage32 *min_neighbor=NULL;
  gft::Pixel u,v;
  int i, p, q, min_val;
  min_neighbor = gft::Image32::Create(img);
  for(p = 0; p < img->n; p++){
    u.x = p % img->ncols;
    u.y = p / img->ncols;
    
    min_val = INT_MAX;
    for (i=1; i < A->n; i++){
      v.x = u.x + A->dx[i];
      v.y = u.y + A->dy[i];
      if(v.x >= 0 && v.x < img->ncols && v.y >= 0 && v.y < img->nrows){
	q = v.x + v.y * img->ncols;
	min_val = MIN(min_val, img->data[q]);
      }
      else
	min_val = INT_MIN;
    }
    min_neighbor->data[p] = min_val;
  }
  return min_neighbor;
}



void write_boundary(int *boundary, int nboundary,
		    gft::sImage32 *img, int T){
  gft::sImage32 *border=NULL;
  char filename[512];
  int i;
  border = gft::Image32::Create(img);
  for(i = 0; i < nboundary; i++)
    border->data[boundary[i]] = 1;
  sprintf(filename, (char *)"../out/border%03d_diff.pgm", T);
  gft::Image32::Write(border, (char *)filename);
  gft::Image32::Destroy(&border);
}


int get_boundary_max(int *boundary, int nboundary,
		     gft::sImage32 *Bedt){
  int i, Dmax = 0;
  for(i = 0; i < nboundary; i++)
    Dmax = MAX(Dmax, Bedt->data[boundary[i]]);
  return Dmax;
}


void insert_neighbors_pqueue(int p,
			     gft::sAdjRel *A,
			     gft::sImage32 *bin,
			     gft::sImage32 *cost,
			     gft::sPQueue32 *Q){
  gft::Pixel u,v;
  int i,q;
  u.x = p % bin->ncols;
  u.y = p / bin->ncols;
  
  for (i=1; i < A->n; i++){
    v.x = u.x + A->dx[i];
    v.y = u.y + A->dy[i];
    
    if(v.x >= 0 && v.x < bin->ncols &&
       v.y >= 0 && v.y < bin->nrows){
      q = v.x + v.y * bin->ncols;

      if(bin->data[q] > 0 &&
	 cost->data[q] != INT_MAX &&
	 Q->L.elem[q].color != GRAY){
	Q->L.elem[q].color = WHITE;
	//gft::PQueue32::InsertElem(&Q, q);
	gft::PQueue32::FastInsertElem(Q, q);
      }
    }
  }
}



int main(int argc, char **argv){
  gft::sImage32 *img=NULL, *bin=NULL, *edt=NULL;
  gft::sImage32 *min_neighbor=NULL;
  gft::sAdjRel *A4, *A8;
  gft::sPQueue32 *Q=NULL, *Q_edt=NULL;
  gft::sImage32 *root;
  gft::sImage32 *pred;
  gft::sImage32 *cost;
  gft::sImage32 *Bedt;
  //gft::sImage32 *Dx;
  //gft::sImage32 *Dy;
  char filename[512];
  int Imin, Imax, T, p, i, val, tmp, nRemoved, Dmax;
  int *boundary = NULL; //*start_new_seeds;
  int nboundary; //nSeeds;
  //clock_t start, end;
  struct timeval tic,toc;
  double totaltime;

  if(argc < 2){
    fprintf(stdout,"usage:\n");
    fprintf(stdout,"dtmt_diff <image>\n");
    exit(0);
  }
  strcpy(filename, argv[1]);
  img = ReadAnyImage(filename);
  bin = gft::Image32::Create(img);
  A4 = gft::AdjRel::Neighborhood_4();
  A8 = gft::AdjRel::Neighborhood_8();  
  
  Imin = gft::Image32::GetMinVal(img);
  Imax = gft::Image32::GetMaxVal(img);
  printf("Imin: %d, Imax: %d\n", Imin, Imax);

  gettimeofday(&tic,NULL);
  //start = clock();
  //------------------------------------------
  boundary = (int *)malloc((img->n+1)*sizeof(int));
  nboundary = 0;

  min_neighbor = get_min_neighbor(img, A4);

  Bedt = gft::Image32::Create(img);
  cost = gft::Image32::Create(img);
  pred = gft::Image32::Create(img);
  root = gft::Image32::Create(img);
  //Dx = gft::Image32::Create(img);
  //Dy = gft::Image32::Create(img);
  for(p = 0; p < img->n; p++){
    pred->data[p] = NIL;
    root->data[p] = p;
  }

  int nb = SQUARE(MIN(img->ncols, img->nrows)/2.0+1);
  //printf("nbuckets: %d\n", nb);
  
  Q_edt = gft::PQueue32::Create(nb, //img->n,
				img->n, cost->data);
  Q     = gft::PQueue32::Create(Imax+2,
				img->n, img->data);

  for(p = 0; p < img->n; p++)
    gft::PQueue32::FastInsertElem(Q, p);

  //nSeeds = 0;
  //start_new_seeds = boundary;
  T = Imax;
  while(!gft::PQueue32::IsEmpty(Q)){
    val = gft::PQueue32::FastGetMaxVal(Q);

    if(val == T){
      p = gft::PQueue32::FastRemoveMaxFIFO(Q);
      
      bin->data[p] = 1;
      //Boundary detection:
      if(min_neighbor->data[p] < T){
        //if(min_neighbor->data[p] == INT_MIN) printf("Seed= x: %d, y: %d\n", p%img->ncols, p/img->ncols);

        // insere elemento na borda.

        boundary[nboundary] = p;
        nboundary++;
        //nSeeds++;

        root->data[p] = p;
        pred->data[p] = NIL;
        cost->data[p] = 0;
        //gft::PQueue32::InsertElem(&Q_edt, p);
        gft::PQueue32::FastInsertElem(Q_edt, p);
      }
      else{
        cost->data[p] = INT_MAX;
        insert_neighbors_pqueue(p, A4, bin, cost, Q_edt);
      }
      //printf(" val: %d ", val);
    }
    else{
      // Finaliza de processar o level-set e computa a EDT incremental

      //printf("T: %d\n", T);
      EDT_DIFF(Q_edt, A8, bin, root, pred, cost, Bedt); //Dx, Dy);
               //start_new_seeds, nSeeds);
      //printf("\n=======================\n");
#ifdef APPDEBUG

      // Pega o valor máximo da distancia 	
      Dmax = get_boundary_max(boundary, nboundary, Bedt);
      //printf("edt_max: %d, ",Dmax);
      //printf("edt_max: %d\n",gft::Image32::GetMaxVal(cost));
      if(Dmax != gft::Image32::GetMaxVal(cost))
	printf("Falha\n");
      sprintf(filename, (char *)"../out/bin%03d_diff.pgm", T);
      gft::Image32::Write(bin, (char *)filename);
      sprintf(filename, (char *)"../out/edt%03d_diff.pgm", T);
      gft::Image32::Write(cost, (char *)filename);      
      write_boundary(boundary, nboundary, img, T);
#endif
      nRemoved = 0;
      T = val;
      for(i = 0; i < nboundary; i++){
	// coleta os pixels de borda que serão removidos
	p = boundary[i];
	//Remove from boundary:
	if(min_neighbor->data[p] >= T){
	  nboundary--;
	  //Faz troca para mover o pixel removido para o final do vetor:
	  //printf("Rem: %d\n", boundary[i]);
	  
	  tmp = boundary[i];
	  boundary[i] = boundary[nboundary];
	  boundary[nboundary] = tmp;
	  nRemoved++;
	  i--;
	}
      }

      // Remove os pixels de borda
      //printf("nRemoved: %d\n", nRemoved);
      treeRemoval(&boundary[nboundary], nRemoved,
		  bin, Q_edt, root, pred, cost, A8);
      
      //start_new_seeds = &boundary[nboundary];
      //nSeeds = 0;
    }
  }
  //Roda para a ultima iteracao que faltou.
  /*  
  EDT_DIFF(Q_edt, A8, bin, root, pred, cost); //Dx, Dy);
#ifdef APPDEBUG
  //printf("edt_max: %d\n",gft::Image32::GetMaxVal(cost));
  sprintf(filename, (char *)"./out/bin%03d_diff.pgm", T);
  gft::Image32::Write(bin, (char *)filename);
  sprintf(filename, (char *)"./out/edt%03d_diff.pgm", T);
  gft::Image32::Write(cost, (char *)filename);
  write_boundary(boundary, nboundary, img, T);
#endif
  */
  //------------------------------------------
  //end = clock();
  gettimeofday(&toc,NULL);

  //totaltime = 1000.0*(((double)(end - start))/CLOCKS_PER_SEC);
  totaltime = ((toc.tv_sec-tic.tv_sec)*1000.0 + 
	       (toc.tv_usec-tic.tv_usec)*0.001);

  printf("Total time: %f ms\n",totaltime);
  
  free(boundary);
  gft::Image32::Destroy(&cost);
  gft::Image32::Destroy(&pred);
  gft::Image32::Destroy(&root);
  //gft::Image32::Destroy(&Dx);
  //gft::Image32::Destroy(&Dy);
  gft::PQueue32::Destroy(&Q_edt);
  gft::PQueue32::Destroy(&Q);
  gft::Image32::Destroy(&img);
  gft::Image32::Destroy(&bin);
  gft::Image32::Destroy(&min_neighbor);
  gft::AdjRel::Destroy(&A4);
  gft::AdjRel::Destroy(&A8);
  return 0;
}
  
