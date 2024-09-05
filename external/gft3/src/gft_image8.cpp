
#include "gft3/gft_image8.h"

namespace gft{
  namespace Image8{

    sImage8 *Create(int ncols, int nrows){
      sImage8 *img = NULL;
      int i;
      
      img = (sImage8 *)malloc(sizeof(sImage8));
      if(img == NULL){
	printf("Error: Insufficient memory.\n");
	exit(1);
      }
      img->nrows = nrows;
      img->ncols = ncols;
      img->n = nrows*ncols;
      
      img->data = (uchar *)calloc(ncols*nrows, sizeof(uchar));
      if(img->data == NULL){
	printf("Error: Insufficient memory.\n");
	exit(1);
      }

      img->array = (uchar**)malloc(nrows*sizeof(uchar *));
      if(img->array == NULL){
	printf("Error: Insufficient memory.\n");
	exit(1);
      }
      for(i = 0; i < nrows; i++){
	img->array[i] = (img->data + i*ncols);
      }
      return img;
    }
    

    void Destroy(sImage8 **img){
      sImage8 *tmp;
      if(img!=NULL){
	tmp = *img;
	if(tmp != NULL){
	  if(tmp->data  != NULL) free(tmp->data);
	  if(tmp->array != NULL) free(tmp->array);
	  free(tmp);
	  *img = NULL;
	}
      }
    }

    


  } /*end Image8 namespace*/
} /*end gft namespace*/

