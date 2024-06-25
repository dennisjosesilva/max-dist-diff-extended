#pragma once 

#include <morphotree/core/box.hpp>
#include <vector>
#include <functional>
#include <morphology/Adjacency.hpp>

class ErosionSerial 
{
public:  
  using Box = morphotree::Box;
  using uint8 = morphotree::uint8;
  using uint32 = morphotree::uint32;

  static const int OUT_OF_DOMAIN_VALUE;

  ErosionSerial(const Box& domain, const std::vector<uint8> &f);
  std::vector<int> compute(Adj adj) const;

private:
  const std::vector<uint8> &f_;
  const Box &domain_;
};


std::vector<int> erodeSerial(
  const morphotree::Box &domain,
  const std::vector<morphotree::uint8> &f,
  Adj adj);