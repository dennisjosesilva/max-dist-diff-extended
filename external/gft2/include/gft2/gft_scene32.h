
#ifndef _GFT_SCENE32_H_
#define _GFT_SCENE32_H_

#include "gft2/gft_common.h"
#include "gft2/gft_image32.h"

extern "C" {
#include "gft2/nifti1_io.h"
}

namespace gft{
  namespace Scene32{
    
    /**
     * It supports both linear and three-dimensional access 
     * (i.e., scn->data[p] or scn->array[z][y][x] for a voxel
     * (x,y,z) at address p=x+y*xsize+z*xsize*ysize).
     */
    struct sScene32 {
      int *data;
      int ***array;
      int xsize,ysize,zsize;
      float dx,dy,dz;
      int maxval, minval, n;
      nifti_image *nii_hdr;
    };
    
    /**
     * \brief A constructor.
     */
    sScene32 *Create(int xsize,int ysize,int zsize);
    /**
     * \brief A constructor taking a reference scene as template.
     */
    sScene32 *Create(sScene32 *scn);
    /**
     * \brief A destructor.
     */
    void     Destroy(sScene32 **scn);
    void     Copy(sScene32 *dest, sScene32 *src);
    void     Copy(sScene32 *dest, sScene32 *src, Voxel v);
    /**
     * \brief A copy constructor.
     */
    sScene32 *Clone(sScene32 *scn);
    sScene32 *SubScene(sScene32 *scn, Voxel l, Voxel h);
    sScene32 *SubScene(sScene32 *scn,
		       int xl, int yl, int zl,
		       int xh, int yh, int zh);
    void     Fill(sScene32 *scn, int value);
    
    sScene32 *Read(char *filename);
    sScene32 *ReadCompressed(char *filename);
    sScene32 *ReadNifti1(char *filename);
    void      Write(sScene32 *scn, char *filename);
    void      WriteCompressed(sScene32 *scn, char *filename);
    void      WriteNifti1(sScene32 *scn, char *filename);
    
    void CloneNiftiHeader(sScene32 *src, sScene32 *dst);

    sScene32 *ChangeOrientationToLPS(sScene32 *scn, char *ori);
    
    sScene32 *LinearInterp(sScene32 *scn, float dx,float dy,float dz);
    /**
     * \brief Scales the image to match the dimensions of a target volume.
     *
     * Scales the image by interpolation to match the dimensions
     * and resolution of a target volume.
     *
     * @param scn The input image.
     * @param ref The reference volume from which the desired dimensions and resolution are obtained.
     * @param interpolation The interpolation type.
     */
    sScene32 *Scale(sScene32 *scn, sScene32 *ref,
		    InterpolationType interpolation);
    /**
     * \brief Scales the label image to match the dimensions of a target volume.
     *
     * Scales the image of labels by interpolation to match the dimensions
     * and resolution of a target volume.
     *
     * @param label The input label image.
     * @param ref The reference volume from which the desired dimensions and resolution are obtained.
     * @param interpolation The interpolation type.
     */
    sScene32 *ScaleLabel(sScene32 *label, sScene32 *ref,
			 InterpolationType interpolation);

    
    inline int   GetValue(sScene32 *scn, Voxel v);
    inline int   GetValue(sScene32 *scn, int p);
    inline int   GetValue(sScene32 *scn, int x, int y, int z);
    float GetValue_trilinear(sScene32 *scn, float x, float y, float z);
    /**
     * \brief Nearest-neighbor interpolation.
     */
    int   GetValue_nn(sScene32 *scn, float x, float y, float z);
    
    inline int GetAddressX(sScene32 *scn, int p);
    inline int GetAddressY(sScene32 *scn, int p);
    inline int GetAddressZ(sScene32 *scn, int p);
    inline int GetVoxelAddress(sScene32 *scn, Voxel v);
    inline int GetVoxelAddress(sScene32 *scn, int x, int y, int z);
    
    inline bool IsValidVoxel(sScene32 *scn, int x, int y, int z);
    inline bool IsValidVoxel(sScene32 *scn, Voxel v);
    
    int GetMaximumValue(sScene32 *scn); /*deprecated function*/
    int GetMinimumValue(sScene32 *scn); /*deprecated function*/
    
    int GetMaxVal(sScene32 *scn);
    int GetMinVal(sScene32 *scn);
    
    sImage32 *GetSliceX(sScene32 *scn, int x);
    sImage32 *GetSliceY(sScene32 *scn, int y);
    sImage32 *GetSliceZ(sScene32 *scn, int z);
    
    void PutSliceX(sScene32 *scn, sImage32 *img, int x);
    void PutSliceY(sScene32 *scn, sImage32 *img, int y);
    void PutSliceZ(sScene32 *scn, sImage32 *img, int z);
    
    sScene32 *MBB(sScene32 *scn);
    void      MBB(sScene32 *scn, Voxel *l, Voxel *h);
    
    sScene32 *AddFrame(sScene32 *scn,  int sz, int value);
    sScene32 *RemFrame(sScene32 *fscn, int sz);
    
    
    //---------inline definitions------------------
    inline bool  IsValidVoxel(sScene32 *scn, int x, int y, int z){
      if((x >= 0)&&(x < scn->xsize)&&
	 (y >= 0)&&(y < scn->ysize)&&
	 (z >= 0)&&(z < scn->zsize))
	return(true);
      else
	return(false);
    }
    
    inline bool  IsValidVoxel(sScene32 *scn, Voxel v){
      if((v.c.x >= 0)&&(v.c.x < scn->xsize)&&
	 (v.c.y >= 0)&&(v.c.y < scn->ysize)&&
	 (v.c.z >= 0)&&(v.c.z < scn->zsize))
	return(true);
      else
	return(false);
    }
    
    inline int   GetValue(sScene32 *scn, Voxel v){
      return (scn->array[v.c.z][v.c.y][v.c.x]);
    }
    inline int   GetValue(sScene32 *scn, int p){
      return (scn->data[p]);
    }
    inline int   GetValue(sScene32 *scn, int x, int y, int z){
      return (scn->array[z][y][x]);
    }
    
    inline int GetAddressX(sScene32 *scn, int p){
      return ((p%(scn->xsize*scn->ysize))%scn->xsize);
    }
    inline int GetAddressY(sScene32 *scn, int p){
      return ((p%(scn->xsize*scn->ysize))/scn->xsize);
    }
    inline int GetAddressZ(sScene32 *scn, int p){
      return (p/(scn->xsize*scn->ysize));
    }
    inline int GetVoxelAddress(sScene32 *scn, Voxel v){
      return (v.c.x + v.c.y*scn->xsize + 
	      v.c.z*scn->xsize*scn->ysize);
    }
    inline int GetVoxelAddress(sScene32 *scn, int x, int y, int z){
      return (x + y*scn->xsize + z*scn->xsize*scn->ysize);
    }

  
  } //end Scene32 namespace

  typedef Scene32::sScene32 sScene32;
  
} //end gft namespace


#include "gft_scene16.h"
#include "gft_scene8.h"

namespace gft{
  namespace Scene32{
    
    sScene16 *ConvertTo16(sScene32 *scn);
    sScene8  *ConvertTo8(sScene32 *scn);

  } //end Scene32 namespace
} //end gft namespace


#include "gft_scene64.h"

namespace gft{
  namespace Scene32{

    sScene64 *ComputeIntegralScene(sScene32 *scn);

    //The dimensions of the window (i.e., xsize,ysize,zsize) 
    //should be given in millimeters.
    void DrawWindow(sScene32 *scn,
		    int val,
		    float xsize, 
		    float ysize, 
		    float zsize,
		    Voxel u);

    //The dimensions of the window (i.e., xsize,ysize,zsize) 
    //should be given in millimeters.
    int WindowNvoxels(sScene32 *scn,
		      float xsize,
		      float ysize,
		      float zsize);
    
  } //end Scene32 namespace
} //end gft namespace


#endif
