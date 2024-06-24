#include <NonDiffMaxDistComputer.hpp>
#include <edt.hpp>

NonDiffMaxDistComputer::NonDiffMaxDistComputer(
  const Box &domain, const std::vector<uint8> &f)
  : domain_{domain}, f_{f}
{}

std::vector<NonDiffMaxDistComputer::uint32> 
  NonDiffMaxDistComputer::computeAttribute(const MTree &tree) const
{
  std::array<std::vector<NodePtr>, 256> levelToNodes = extractLevelMap(tree);
  std::vector<uint32> maxDistAttr(tree.numberOfNodes(), 0);

  gft::sImage32 *bin = gft::Image32::Create(domain_.width(), domain_.height());
  gft::sImage32 *edt = nullptr;
  gft::sAdjRel *A8 = gft::AdjRel::Neighborhood_8();

  for (uint32 pidx = 0; pidx < domain_.numberOfPoints(); pidx++)
    bin->data[pidx] = 0;

  for (int level = 255; level >= 0; level--) {
    const std::vector<NodePtr> &nodes = levelToNodes[level];
    
    if (nodes.empty())
      continue;

    for (NodePtr node : levelToNodes[level]) {
      for (uint32 pidx : node->cnps()) {
        bin->data[pidx] = 1;
      }      
    }

    edt = EDT(bin, A8, INTERIOR);

    for (NodePtr node : levelToNodes[level]) {
      std::vector<uint32> cc = node->reconstruct();
      int maxDist = 0;
      for (uint32 pidx : cc) {
        if (maxDist < edt->data[pidx]) 
          maxDist = edt->data[pidx];
      }
      maxDistAttr[node->id()] = maxDist;
    }
    gft::Image32::Destroy(&edt);
  }

  gft::Image32::Destroy(&bin);
  return maxDistAttr;
}

std::array<std::vector<NonDiffMaxDistComputer::NodePtr>, 256>
  NonDiffMaxDistComputer::extractLevelMap(const MTree &tree) const
{
  std::array<std::vector<NodePtr>, 256> levelToNodes;
  tree.tranverse([&levelToNodes](NodePtr node){
    levelToNodes[node->level()].push_back(node);
  });

  return levelToNodes;
}


std::vector<morphotree::uint32> computeNonDiffMaxDistanceAttribute(
  const morphotree::Box &domain,
  const std::vector<morphotree::uint8> &f,
  const NonDiffMaxDistComputer::MTree &tree)
{
  return NonDiffMaxDistComputer(domain, f).computeAttribute(tree);
}