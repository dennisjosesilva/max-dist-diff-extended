#pragma once 

#include <functional>
#include <morphotree/core/alias.hpp>
#include <morphotree/core/point.hpp>
#include <morphotree/core/box.hpp>
#include <vector>
#include <array>

using Adj = std::function<const std::vector<morphotree::uint32> &(morphotree::uint32)>;

class Adj4
{
public:
  using I32Point = morphotree::I32Point;
  using uint32 = morphotree::uint32;
  using Box = morphotree::Box;

  Adj4(const Box &domain);

  const std::vector<uint32>& neighbours(const I32Point &p);
  const std::vector<uint32>& neighbours(uint32 pidx);

  const std::vector<uint32>& operator() (const I32Point &p);
  const std::vector<uint32>& operator() (uint32 pidx);

private:
  std::array<I32Point, 4> offsets_;
  std::vector<uint32> neighbours_;
  const Box &domain_;
};

class Adj8
{
public:
  using I32Point = morphotree::I32Point;
  using uint32 = morphotree::uint32;
  using Box = morphotree::Box;

  Adj8(const Box &domain);

  const std::vector<uint32>& neighbours(const I32Point &p);
  const std::vector<uint32>& neighbours(uint32 pidx);

  const std::vector<uint32>& operator() (const I32Point &p);
  const std::vector<uint32>& operator() (uint32 pidx);

private:
  std::array<I32Point, 8> offsets_;
  std::vector<uint32> neighbours_;
  const Box &domain_;
};