#include <Morphology/ErosionParallel.hpp>
#include <omp.h>


const int ErosionParallel::OUT_OF_DOMAIN_VALUE = -1;

ErosionParallel::ErosionParallel(const Box &domain, const std::vector<uint8> &f)
  :domain_{domain}, f_{f}
{}

std::vector<int> ErosionParallel::compute(const StructuringElement &se) const
{
  using morphotree::I32Point;
  std::vector<int> f_eroded(domain_.numberOfPoints());

  #pragma omp parallel for 
  for (uint32 pdix = 0; pdix < domain_.numberOfPoints(); pdix++) {
    int minValue = f_[pdix];
    I32Point p = domain_.indexToPoint(pdix);

    for (const I32Point &offset : se) {
      I32Point q = p + offset;
      if (!domain_.contains(q)) {
        minValue = OUT_OF_DOMAIN_VALUE;
        break;
      }
      else {
        uint32 qidx = domain_.pointToIndex(q);
        int fvalue = static_cast<int>(f_[qidx]);
        if (fvalue < minValue)
          minValue = fvalue;
      }
    }

    f_eroded[pdix] = minValue;
  }

  return f_eroded;
}

std::vector<int> erodeParallel(const morphotree::Box &domain,
  const std::vector<morphotree::uint8> &f,  
  const ErosionParallel::StructuringElement &se)
{
  ErosionParallel erosion{domain, f};
  return erosion.compute(se);
}