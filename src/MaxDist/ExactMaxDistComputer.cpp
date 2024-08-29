#include <MaxDist/ExactMaxDistComputer.hpp>
#include <Morphology/ErosionParallel.hpp>
#include <morphotree/core/io.hpp>

#include <unordered_map>

// Useful function
std::vector<morphotree::uint32> computeExactMaxDistanceAttribute(
  const morphotree::Box &domain,
  const std::vector<morphotree::uint8> &f,
  const ExactMaxDistComputer::MTree &tree)
{
  ExactMaxDistComputer comp{domain, f};
  return comp.computeAttribute(tree);
}


ExactMaxDistComputer::ExactMaxDistComputer(
  const Box &domain, const std::vector<uint8> &f)
  : domain_{domain}, f_{f}
{}

std::vector<morphotree::uint32>
  ExactMaxDistComputer::computeAttribute(const MTree &tree) const
{
  // define useful images
  std::vector<uint32> maxDist(tree.numberOfNodes(), 0);
  std::array<std::vector<NodePtr>, 256> levelToNodes = extractLevelMap(tree);

  std::vector<int> f_eroded = erodeParallel(domain_, f_, SE::UnitCross);
  std::vector<bool> bin(domain_.numberOfPoints(), false);
  std::vector<bool> contourImage(domain_.numberOfPoints(), false);

  std::unordered_map<uint32, std::vector<uint32>> contours;

  for (int level=255; level >= 0; level--) {
    // skip level that does not contain nodes
    const std::vector<NodePtr> nodes = levelToNodes[level];
    if (nodes.empty())
      continue;

    // There exist at least one node in "level"
    // So, we have to process them.
    for (NodePtr node : nodes) {
      // process node       
      contours.insert({node->id(), std::vector<uint32>()});

      std::vector<uint32> &Ncontour = contours[node->id()];

      // reuse children contour pixels and collect pixels which need
      // to be removed in the DIFT
      for (NodePtr c : node->children()) {
        for (uint32 pidx : contours[c->id()]) {
          if (f_eroded[pidx] < static_cast<int>(node->level())) {
            Ncontour.push_back(pidx);            
          }            
          else {
            contourImage[pidx] = false;
          }
        }
        
        contours.erase(c->id());
      }

      // Compute new contour points 
      for (uint32 pidx : node->cnps()) {
        // incrementally create level-set binary image
        bin[pidx] = true;

        // check if CNP is a contour pixel
        if (f_eroded[pidx] < static_cast<int>(node->level())) {
          // pidx is a contour pixel
          // add it to the contour
          Ncontour.push_back(pidx);
          contourImage[pidx] = true;
        }
      }
    }

    // compute EDT
    std::vector<int> edt(domain_.numberOfPoints(), 0);
    std::vector<int> g(domain_.numberOfPoints(), 0);    
    I32Point p(0, 0);  
    for (p.x() = domain_.left(); p.x() <= domain_.right(); p.x()++) {
      // scan 1
      p.y() = 0;
      uint32 pidx = domain_.pointToIndex(p);
      if (contourImage[pidx])
        g[pidx] = 0;
      else 
        g[pidx] = INT_MAX;

      for (p.y() = domain_.top()+1; p.y() <= domain_.bottom(); p.y()++) {
        uint32 pidx = domain_.pointToIndex(p);
        if (contourImage[pidx])
          g[pidx] = 0;
        else if (g[domain_.pointToIndex({p.x(), p.y()-1})] < INT_MAX)
          g[pidx] = 1 + g[domain_.pointToIndex({p.x(), p.y()-1})];
        else
          g[pidx] = INT_MAX;
      }
      
      // scan 2
      for (p.y() = domain_.bottom()-1; p.y() >= domain_.top(); p.y()--) {
        uint32 pidx = domain_.pointToIndex(p);
        uint32 qidx = domain_.pointToIndex({p.x(), p.y()+1});
        if (g[qidx] < g[pidx])
          g[pidx] = 1 + g[qidx];
      }      
    }
    // Second Phase
    std::vector<uint32> s(domain_.width(), 0);
    std::vector<uint32> t(domain_.width(), 0);

    // Scan 3
    p = domain_.topleft();
    for (p.y() = domain_.top(); p.y() <= domain_.bottom(); p.y()++) {
      int q = 0;
      s[0] = 0;
      t[0] = 0;
      int *lg = &g[p.y()*domain_.width()];
      // scan 3
      for (int u = domain_.left()+1; u <= domain_.right(); u++) {
        
        while (q >= 0 && EDT_f(t[q], s[q], lg) > EDT_f(t[q], u, lg)) 
          q--;
        
        if (q < 0) {
          q = 0;
          s[0] = u;
        }
        else {
          int w = 1 + EDT_Sep(s[q], u, lg, domain_.width());
          if (w < domain_.width()) {
            q++;
            s[q] = u;
            t[q] = w;
          }
        }       
      }
       // scan 4:
      for (p.x() = domain_.right(); p.x() >= domain_.left(); p.x()--) {
        uint pidx = domain_.pointToIndex(p);
        edt[pidx] = EDT_f(p.x(), s[q], lg);
        if (p.x() == t[q])
          q--;
      }      
    }    
    
    // compute max dist for each node 
    for (NodePtr node : nodes) {
      int maxDistNode = 0;
      for (uint32 pidx : node->reconstruct()) {
        if (maxDistNode < edt[pidx])
          maxDistNode = edt[pidx];
      }
      maxDist[node->id()] = maxDistNode;
    }
  }

  return maxDist;
}


int ExactMaxDistComputer::EDT_f(int x, int i, int *g) const
{
  int gi = g[i];
  if (gi == INT_MAX)
    return INT_MAX;
  
  return (x - i) * (x - i) + gi*gi;
}

int ExactMaxDistComputer::EDT_Sep(int i, int u, int *g, int ncols) const
{
  int gu = g[u];
  int gi = g[i];
  if (gu == INT_MAX)
    return ncols;
  else 
    return (u*u - i*i + gu*gu - gi*gi) / (2*(u-i));
}


std::array<std::vector<ExactMaxDistComputer::NodePtr>, 256>
  ExactMaxDistComputer::extractLevelMap(const MTree &tree) const
{
  std::array<std::vector<NodePtr>, 256> levelToNodes;
  tree.tranverse([&levelToNodes](NodePtr node){
    levelToNodes[node->level()].push_back(node);
  });

  return levelToNodes;
}