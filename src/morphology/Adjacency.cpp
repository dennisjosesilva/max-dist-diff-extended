#include <morphology/Adjacency.hpp>


// -------------------------------------------------
// Adj4
// -------------------------------------------------
Adj4::Adj4(const Box &domain)
  : domain_{domain},
    offsets_{ I32Point{-1, 0}, I32Point{0, -1}, I32Point{1, 0}, I32Point{0, 1} }
{
  neighbours_.resize(4);
}

const std::vector<morphotree::uint32>& Adj4::neighbours(const I32Point &p)
{
  int i = 0;
  for (const I32Point &d : offsets_) {
    I32Point q = p + d;
    if (domain_.contains(q))
      neighbours_[i] = domain_.pointToIndex(q);
    else 
      neighbours_[i] = Box::UndefinedIndex;
    i++;
  }
  

  return neighbours_;
}

const std::vector<morphotree::uint32>& Adj4::neighbours(uint32 pidx) 
{
  return neighbours(domain_.indexToPoint(pidx));
}

const std::vector<morphotree::uint32>& Adj4::operator()(const I32Point &p) 
{
  return neighbours(p);
}

const std::vector<morphotree::uint32>& Adj4::operator()(uint32 pidx) 
{
  return neighbours(pidx);
}

// -------------------------------------------------
// Adj8
// -------------------------------------------------
Adj8::Adj8(const Box &domain)
  : domain_{domain},
    offsets_{ I32Point{-1, 0}, I32Point{-1, -1}, I32Point{0, -1}, I32Point{ 1, -1},
              I32Point{ 1, 0}, I32Point{ 1,  1}, I32Point{0 , 1}, I32Point{-1,  1}}
{
  neighbours_.resize(8);
}

const std::vector<morphotree::uint32>& Adj8::neighbours(const I32Point &p)
{
  int i = 0;
  for (const I32Point &d : offsets_) {
    I32Point q = p + d;
    if (domain_.contains(q))
      neighbours_[i] = domain_.pointToIndex(q);
    else 
      neighbours_[i] = Box::UndefinedIndex;
    i++;
  }

  return neighbours_;
}

const std::vector<morphotree::uint32>& Adj8::neighbours(uint32 pidx) 
{
  return neighbours(domain_.indexToPoint(pidx));
}

const std::vector<morphotree::uint32>& Adj8::operator()(const I32Point &p) 
{
  return neighbours(p);
}

const std::vector<morphotree::uint32>& Adj8::operator()(uint32 pidx) 
{
  return neighbours(pidx);
}
