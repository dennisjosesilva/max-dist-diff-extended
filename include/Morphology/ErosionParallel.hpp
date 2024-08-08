#pragma once 

#include <morphotree/core/box.hpp>
#include <vector>
#include <functional>

#include <Morphology/Adjacency.hpp>

namespace SE
{
  const std::vector<morphotree::I32Point> UnitCross = {
    {-1, 0}, {0, -1}, {1, 0}, {0, 1}
  };

  const std::vector<morphotree::I32Point> UnitBox = {
    {-1, 0}, {-1, -1}, {0, -1}, {1, -1},
    { 1, 0}, { 1,  1}, {0,  1}, {-1, 1}
  };
}

class ErosionParallel
{
public:  
  using Box = morphotree::Box;
  using uint8 = morphotree::uint8;
  using uint32 = morphotree::uint32;
  using StructuringElement = std::vector<morphotree::I32Point>;

  static const int OUT_OF_DOMAIN_VALUE;

  ErosionParallel(const Box& domain, const std::vector<uint8> &f);
  std::vector<int> compute(const StructuringElement &se) const;

private:
  const std::vector<uint8> &f_;
  const Box &domain_;
};

std::vector<int> erodeParallel(
  const morphotree::Box &domain,
  const std::vector<morphotree::uint8> &f,
  const ErosionParallel::StructuringElement &se);