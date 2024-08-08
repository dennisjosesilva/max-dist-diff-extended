#pragma once 

#include <morphotree/core/box.hpp>
#include <vector>

struct StructuringElement 
{
  StructuringElement(const std::vector<int> &pdx,
    const std::vector<int> &pdy);
  
  StructuringElement(std::vector<int> &&pdx, 
    std::vector<int> &&pdy);


  std::vector<int> dx;
  std::vector<int> dy;

  static StructuringElement UnitCross();
  static StructuringElement UnitBox();
};

class ErosionGPU
{
public:
  using Box = morphotree::Box;
  using uint8 = morphotree::uint8;
  using uint32 = morphotree::uint32;
  
  ErosionGPU(const Box& domain, const std::vector<uint8> &f);
  std::vector<int> compute(const StructuringElement &se) const;

private:
  const std::vector<uint8> &f_;
  const Box &domain_;
};

std::vector<int> erodeGPU(const morphotree::Box &domain,
  const std::vector<morphotree::uint8> &f,
  const StructuringElement &se);
