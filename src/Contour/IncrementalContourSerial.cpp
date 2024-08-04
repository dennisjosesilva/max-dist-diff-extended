#include <Contour/IncrementalContourSerial.hpp>


IncrementalContourSerial::IncrementalContourSerial(const Box &domain,
    const std::vector<uint8> &f, Adj adj)
    : domain_{domain}, f_{f}, adj_{adj}
{}
  
std::vector<IncrementalContourSerial::Contours> 
  IncrementalContourSerial::compute(const MTree &tree) const
{
  std::vector<Contours> contours(tree.numberOfNodes());
  std::vector<int> f_eroded = erodeSerial(domain_, f_, adj_);

  std::array<std::vector<NodePtr>, 256> levelToNodes = extractLevelMap(tree);

  // process the level sets from 255 down to 0
  for (int level=255; level >= 0; level--)
  {
    const std::vector<NodePtr> &nodes = levelToNodes[level];

    // skip level that does not contain nodes
    if (nodes.empty())
      continue;

    // There exist at least one node in "level"
    // So, we have to process them.
    for (NodePtr node : nodes) {
      // process node 
      // define Ncontour which will be processed
      std::unordered_set<uint32> &Ncontour = contours[node->id()];

      // reuse children contour pixels
      for (NodePtr c : node->children()) {
        for (uint32 pidx : contours[c->id()]) {          
          if (f_eroded[pidx] < static_cast<int>(node->level()))
            Ncontour.insert(pidx);
        }
      }
    

      // check CNPs
      for (uint32 pidx : node->cnps()) {
        if (f_eroded[pidx] < static_cast<int>(node->level()))
          Ncontour.insert(pidx);
      }

      contours[node->id()] = Ncontour;
    }
  }

  return contours;
}

std::array<std::vector<IncrementalContourSerial::NodePtr>, 256> 
  IncrementalContourSerial::extractLevelMap(const MTree &tree) const
{
  std::array<std::vector<NodePtr>, 256> levelToNodes;
  tree.tranverse([&levelToNodes](NodePtr node){
    levelToNodes[node->level()].push_back(node);
  });

  return levelToNodes;
}

std::vector<IncrementalContourSerial::Contours> extractContourSerial(
  const IncrementalContourSerial::Box &domain, 
  const std::vector<IncrementalContourSerial::uint8> &f,
  Adj adj,
  const IncrementalContourSerial::MTree &tree)
{
  return IncrementalContourSerial(domain, f, adj).compute(tree);
}