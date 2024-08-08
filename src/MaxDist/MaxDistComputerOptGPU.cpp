#include <Morphology/Adjacency.hpp>
#include <Morphology/ErosionGPU.hpp>

#include <MaxDist/MaxDistComputerOptGPU.hpp>

#include <morphotree/adjacency/adjacency.hpp>
#include <morphotree/adjacency/adjacency4c.hpp>

#include <MaxDist/edt_diff.hpp>

#include <Morphology/Adjacency.hpp>

#include <unordered_set>
#include <unordered_map>

// #define APPDEBUG

#ifdef APPDEBUG
  #include <sstream>
#endif

// Useful function
std::vector<morphotree::uint32> computeMaxDistanceAttributeOptGPU(
  const morphotree::Box &domain,
  const std::vector<morphotree::uint8> &f,
  const MaxDistComputerOptGPU::MTree &tree)
{
  MaxDistComputerOptGPU comp{domain, f};
  return comp.computeAttribute(tree);
}

MaxDistComputerOptGPU::MaxDistComputerOptGPU(const Box& domain,
  const std::vector<uint8> &f)
  : domain_{domain}, f_{f}, 
    OFFSET_NEIGHBOR{ {-1, 0}, {0, -1}, {1, 0}, {0, 1}}
{ }

std::vector<MaxDistComputerOptGPU::uint32> 
  MaxDistComputerOptGPU::computeAttribute(const MTree &tree) const 
{
  using NodePtr = MTree::NodePtr;
  using morphotree::InfAdjacency4C;

  // define useful images
  std::vector<uint32> maxDist(tree.numberOfNodes(), 0);
  std::array<std::vector<NodePtr>, 256> levelToNodes = extractLevelMap(tree);

  gft::sImage32 *bin = gft::Image32::Create(domain_.width(), domain_.height());
  gft::sImage32 *root = gft::Image32::Create(domain_.width(), domain_.height());
  gft::sImage32 *pred = gft::Image32::Create(domain_.width(), domain_.height());
  gft::sImage32 *cost = gft::Image32::Create(domain_.width(), domain_.height()); // cost = edt^2
  gft::sImage32 *Bedt = gft::Image32::Create(domain_.width(), domain_.height());

  // IFT adjacency
  gft::sAdjRel *A8 = gft::AdjRel::Neighborhood_8();

  initPredAndRoot(pred, root);

  // define priority queues 
  int nb = SQUARE(MIN(bin->ncols, bin->nrows) / 2.0 + 1);
  gft::sPQueue32 *Q_edt = gft::PQueue32::Create(nb, bin->n, cost->data);

  // define variables for incremental contour
  std::unordered_map<uint32, std::unordered_set<uint32>> contours;
  std::vector<int> f_eroded = erodeGPU(domain_, f_, StructuringElement::UnitCross());

  // process the level sets from 255 down to 0
  for (int level = 255; level >= 0; level--) {
    // skip level that does not contain nodes
    const std::vector<NodePtr> &nodes = levelToNodes[level];
    if (nodes.empty())
      continue;

    // There exist at least one node in "level"
    // So, we have to process them.
    for (NodePtr node : nodes) {
      // process node
      contours.insert({node->id(), std::unordered_set<uint32>()});

      // remove contour pixels of the node
      std::vector<uint32> toRemove;

      // define Ncontour which will be processed
      std::unordered_set<uint32> &Ncontour = contours[node->id()];

      // reuse children contour pixels and collect pixels which need
      // to be removed in the DIFT
      for (NodePtr c : node->children()) {
        for (uint32 pidx : contours[c->id()]) {
          if (f_eroded[pidx] < static_cast<int>(node->level())) 
            Ncontour.insert(pidx);                      
          else
            toRemove.push_back(pidx);
        }
        contours.erase(c->id());
      }

      if (!toRemove.empty())
        treeRemoval(toRemove, bin, Q_edt, root, pred, cost, A8);

      // Compute new contour points 
      for (uint32 pidx : node->cnps()) {
        // incrementally create level-set binary image
        bin->data[pidx] = 1;
        
        // check if CNP is contour pixel
        if (f_eroded[pidx] < static_cast<int>(node->level())) {
          // pidx is a contour pixel
          // add it to contour
          Ncontour.insert(pidx);

          // setting up for the differential ift.
          root->data[pidx] = pidx;
          pred->data[pidx] = NIL;
          cost->data[pidx] = 0;
          gft::PQueue32::FastInsertElem(Q_edt, static_cast<int>(pidx));
        }
        else {
          // pidx is pixel of level "level" and it is not contour pixel
          cost->data[pidx] = INT_MAX;
          insertNeighborsPQueue(pidx, bin, cost, Q_edt);
        }
      } // end loop on node cnps
    } // end of loop on nodes of the levels

    // The roots, cost and borders are set.
    // Compute maximum distance attributes for 
    // the nodes in "level"

    #ifdef APPDEBUG
      std::stringstream ss;
      ss << "../out/" << level << ".pgm";
      const char *filename = ss.str().c_str();
      gft::Image32::Write(bin, (char *)filename);
    #endif

    // Run differential EDT
    EDT_DIFF(Q_edt, A8, bin, root, pred, cost, Bedt);

    #ifdef APPDEBUG
      std::stringstream sscost;
      sscost << "../out/cost-" << level << ".pgm";
      const char *filenamecost = sscost.str().c_str();
      gft::Image32::Write(cost, (char *)filenamecost);
    #endif

    // compute the max dist attribute for each node 
    for (NodePtr node : nodes) {
      const std::unordered_set<uint32> &Ncontour = contours[node->id()];
      uint32 maxDistValue = 0;

      // search for maximum distance of N on its contour
      for (uint32 pidx : Ncontour) {
        if (Bedt->data[pidx] > maxDistValue)
          maxDistValue = Bedt->data[pidx];
      }

      maxDist[node->id()] = maxDistValue;
    }
  }

  // Clean up memory 
  gft::Image32::Destroy(&cost);
  gft::Image32::Destroy(&pred);
  gft::Image32::Destroy(&root);
  gft::PQueue32::Destroy(&Q_edt);
  gft::Image32::Destroy(&bin);
  gft::AdjRel::Destroy(&A8);

  return maxDist;
}

std::array<std::vector<MaxDistComputerOptGPU::NodePtr>, 256>
  MaxDistComputerOptGPU::extractLevelMap(const MTree &tree) const 
{
  std::array<std::vector<NodePtr>, 256> levelToNodes;
  tree.tranverse([&levelToNodes](NodePtr node){
    levelToNodes[node->level()].push_back(node);
  });

  return levelToNodes;
}

gft::sImage32 *MaxDistComputerOptGPU::createGFTImage() const 
{
  gft::sImage32 *gftImg = gft::Image32::Create(domain_.width(), domain_.height());
  for (uint32 pidx = 0; pidx < domain_.numberOfPoints(); pidx++)
    gftImg->data[pidx] = static_cast<int>(f_[pidx]);

  return gftImg;
}

void MaxDistComputerOptGPU::initPredAndRoot(gft::sImage32 *pred, gft::sImage32 *root) const 
{
  for (uint32 pidx = 0; pidx < pred->n; pidx++) {
    pred->data[pidx] = NIL;
    root->data[pidx] = pidx;
  }
}

void MaxDistComputerOptGPU::insertNeighborsPQueue(
  uint32 pidx,
  gft::sImage32 *bin,
  gft::sImage32 *cost,
  gft::sPQueue32 *Q) const 
{
  I32Point p = domain_.indexToPoint(pidx);

  for (I32Point offset : OFFSET_NEIGHBOR) {
    I32Point q = p + offset;

    if (domain_.contains(q)) { // qidx is in the domain
      uint32 qidx = domain_.pointToIndex(q);
      if (bin->data[qidx] > 0 &&
        cost->data[qidx] != INT_MAX &&
        Q->L.elem[qidx].color != GRAY) {
          Q->L.elem[qidx].color = WHITE;
          gft::PQueue32::FastInsertElem(Q, qidx);
        }
    }
  }
}