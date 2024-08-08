#include <Morphology/ErosionGPU.hpp>
#include <cuda.h>
#include <cuda_runtime.h>

#include <iostream>


static void HandleError(cudaError_t err, const char *file, int line)
{
  if (err != cudaSuccess) {
    printf("%s in %s at line %d\n", cudaGetErrorString(err), file, line);
    exit(EXIT_FAILURE);
  }
}
#define HANDLE_ERROR(err) (HandleError(err, __FILE__, __LINE__))


__device__ int OUT_OF_DOMAIN_VALUE = -1;

__global__ void erode(
    morphotree::uint32 width,
    morphotree::uint32 height,
    morphotree::uint8 *f,
    int *dx, int *dy, 
    unsigned int se_size,    
    int *f_eroded)
{
  int px = blockIdx.x * blockDim.x + threadIdx.x;
  int py = blockIdx.y * blockDim.y + threadIdx.y;  


  if (0 <= px && px < width && 0 <= py && py < height) {      
    // If thread produces a valid pixel
    morphotree::uint32 pidx = width*py + px;
    int minValue = static_cast<int>(f[pidx]);

    for (int i = 0; i < se_size; i++) {
      int qx = px + dx[i];
      int qy = py + dy[i];
      
      if (0 <= qx && qx < width && 0 <= qy && qy < height) {
        morphotree::uint32 qidx = qy * width + qx;  
        if (static_cast<int>(f[qidx]) < minValue) {
          minValue = static_cast<int>(f[qidx]);
        }
      }
      else {
        minValue = OUT_OF_DOMAIN_VALUE;
        break;
      }    
    }
    
    f_eroded[pidx] = minValue;
  }  
}

StructuringElement::StructuringElement(const std::vector<int> &pdx,
  const std::vector<int> &pdy)
  : dx{pdx}, dy{pdy}
{}

StructuringElement::StructuringElement(std::vector<int> &&pdx, 
  std::vector<int> &&pdy)
  : dx{pdx}, dy{pdy}
{}

StructuringElement StructuringElement::UnitCross()
{
  return StructuringElement{
    {-1,  0,  1,  0}, // dx
    { 0, -1,  0,  1}  // dy
  };
}

StructuringElement StructuringElement::UnitBox()
{
  return StructuringElement{
    {-1, -1,  0,  1,  1,  1,  0, -1}, // dx
    { 0, -1, -1, -1,  0,  1,  1,  1}  // dy
  };
}


ErosionGPU::ErosionGPU(const Box& domain, const std::vector<uint8> &f)
  : domain_{domain}, f_{f}
{}

std::vector<int> ErosionGPU::compute(const StructuringElement &se) const
{  
  uint8 *f_dev;
  int *f_eroded_dev;
  int *dx_dev,  *dy_dev;
  
  // Allocate required memory in GPU
  HANDLE_ERROR(cudaMalloc((void**)&f_dev, domain_.numberOfPoints() * sizeof(uint8)));
  HANDLE_ERROR(cudaMalloc((void**)&f_eroded_dev, static_cast<size_t>(domain_.numberOfPoints()) * sizeof(int)));
  HANDLE_ERROR(cudaMalloc((void**)&dx_dev, se.dx.size() * sizeof(int)));
  HANDLE_ERROR(cudaMalloc((void**)&dy_dev, se.dy.size() * sizeof(int)));

  // copy CPU to GPU
  HANDLE_ERROR(cudaMemcpy(f_dev, f_.data(), static_cast<size_t>(domain_.numberOfPoints()) * sizeof(uint8), cudaMemcpyHostToDevice));
  HANDLE_ERROR(cudaMemcpy(dx_dev, se.dx.data(), se.dx.size() * sizeof(int), cudaMemcpyHostToDevice));
  HANDLE_ERROR(cudaMemcpy(dy_dev, se.dy.data(), se.dy.size() * sizeof(int), cudaMemcpyHostToDevice));

  dim3 threadDim{16, 16, 1};
  // dim3 blockDim{64, 64, 1};
  dim3 blockDim{ (15 + domain_.width()) / (threadDim.x), 
     (15 + domain_.height()) / (threadDim.y) };


  std::cout << "blockDim: " << blockDim.x << ", " << blockDim.y << std::endl;

  // Compute Block size and thread size
  erode<<<blockDim, threadDim>>>(
    domain_.width(), domain_.height(),  // domain
    f_dev,                              // f
    dx_dev,          dy_dev,            // dx, dy
    se.dx.size(),                       // se width
    f_eroded_dev);                      // output

  
  // copy back data from GPU to CPU  
  std::vector<int> f_eroded(domain_.numberOfPoints());
  HANDLE_ERROR(cudaMemcpy(f_eroded.data(), f_eroded_dev, 
    domain_.numberOfPoints() * sizeof(int), cudaMemcpyDeviceToHost));
  
  return f_eroded;
}

std::vector<int> erodeGPU(const morphotree::Box &domain,
  const std::vector<morphotree::uint8> &f,
  const StructuringElement &se)
{
  ErosionGPU erosion{domain, f};
  return erosion.compute(se);
}