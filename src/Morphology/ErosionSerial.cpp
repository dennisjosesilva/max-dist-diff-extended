#include <Morphology/ErosionSerial.hpp>

#include <omp.h>

const int ErosionSerial::OUT_OF_DOMAIN_VALUE = -1;

ErosionSerial::ErosionSerial(const Box& domain, const std::vector<uint8> &f)
  :domain_{domain}, f_{f}
{}

std::vector<int> ErosionSerial::compute(Adj adj) const
{
  using morphotree::I32Point;
  std::vector<int> f_eroded(domain_.numberOfPoints());
    
  for (uint32 pidx = 0; pidx < domain_.numberOfPoints(); pidx++) {
    int minValue = f_[pidx];

    for (uint32 qidx : adj(pidx)) {
      if (qidx == Box::UndefinedIndex) {
        minValue = OUT_OF_DOMAIN_VALUE;
        break;
      }
      else {
        int fvalue = static_cast<int>(f_[qidx]);
        if (fvalue < minValue)
          minValue = fvalue;
      }
    }

    f_eroded[pidx] = minValue;
  }

  return f_eroded;
}

std::vector<int> erodeSerial(
  const morphotree::Box &domain,
  const std::vector<morphotree::uint8> &f,
  Adj adj)
{
  ErosionSerial erosion(domain, f);
  return erosion.compute(adj);
}