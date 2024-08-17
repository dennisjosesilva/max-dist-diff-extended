#pragma once 

#include <morphotree/tree/mtree.hpp>
#include <morphotree/core/box.hpp>
#include <morphotree/attributes/areaComputer.hpp>

#include <gft.h>

#include <array>
#include <stack>

#define UNDEF -999999
namespace morphotree{


class UltimateAttributeOpening{

    
  public:
    using uint8 = morphotree::uint8;
    using uint32 = morphotree::uint32;
    using MTree = morphotree::MorphologicalTree<uint8>;
    using NodePtr = MTree::NodePtr;
        
    UltimateAttributeOpening(const MTree &ptree, std::vector<uint32> attrs_increasing);
    

    ~UltimateAttributeOpening();

    std::vector<uint8> createRandomColor(std::vector<uint32> img);

    std::vector<uint8> getMaxConstrastImage();

    std::vector<uint32> getAssociatedImage();

    std::vector<uint8> getAssociatedColorImage();
    
    void execute(int maxCriterion);
    void execute(int maxCriterion, int deltaMSER,  std::vector<uint32> attr_mser, int maxValue_mser);
    void execute(int maxCriterion, int deltaMSER);

    

  private:
    int maxCriterion;
    std::vector<uint32> attrs_increasing;
    std::vector<uint32> attr_mser;
    std::vector<uint32> attr_area;
    MTree maxtree;
    int* maxContrastLUT;
    int* associatedIndexLUT;
    bool hasNodeSelectedInPrimitive(NodePtr currentNode);
    void computeUAO(NodePtr currentNode, int levelNodeNotInNR, bool qPropag, bool isCalculateResidue);
    bool isSelectedForPruning(NodePtr currentNode);

    NodePtr getNodeAscendant(NodePtr node, int h);
    double getStability(NodePtr node);
    NodePtr getAscendant(NodePtr node);
    NodePtr getDescendant(NodePtr node);
    void maxAreaDescendants(NodePtr nodeAsc, NodePtr nodeDes);
    std::vector<bool> computerMSER(int delta);
		std::vector<NodePtr> ascendants;
	  std::vector<NodePtr> descendants;
	  std::vector<double> stability;
    std::vector<bool> selectedForFiltering; //mser
    int numMSER;
    int maxVariation;
    int minValue;
    int maxValue;
	
};


std::vector<bool> UltimateAttributeOpening::computerMSER(int delta){
    this->numMSER = 0;
    
		std::vector<NodePtr> tmp_asc (this->maxtree.numberOfNodes(), nullptr);
		this->ascendants = tmp_asc;

		std::vector<NodePtr> tmp_des (this->maxtree.numberOfNodes(), nullptr);
		this->descendants = tmp_des;

		std::vector<double> tmp_stab (this->maxtree.numberOfNodes(), UNDEF);
		this->stability = tmp_stab;
		
		maxtree.tranverse([delta, this](NodePtr node){
      NodePtr nodeAsc = this->getNodeAscendant(node, delta);
			this->maxAreaDescendants(nodeAsc, node);
			this->ascendants[node->id()] = nodeAsc;
    });
    maxtree.tranverse([this](NodePtr node){
			if(this->ascendants[node->id()] != nullptr && this->descendants[node->id()] != nullptr)
				this->stability[node->id()] = this->getStability(node);
		});
		
		
		
		std::vector<bool> mser(this->maxtree.numberOfNodes(), false);
		maxtree.tranverse([&mser, this](NodePtr node){
			if(this->stability[node->id()] != UNDEF && this->stability[this->getAscendant(node)->id()] != UNDEF && this->stability[this->getDescendant(node)->id()] != UNDEF){
				double maxStabilityDesc = this->stability[this->getDescendant(node)->id()];
				double maxStabilityAsc = this->stability[this->getAscendant(node)->id()];
				if(this->stability[node->id()] < maxStabilityDesc && this->stability[node->id()] < maxStabilityAsc){
					if(stability[node->id()] < this->maxVariation && this->attr_mser[node->id()] >= this->minValue && this->attr_mser[node->id()] <= this->maxValue){
						mser[node->id()] = true;
						this->numMSER++;
					}
				}
			}
		});
		return mser;
	}

void UltimateAttributeOpening::maxAreaDescendants(NodePtr nodeAsc, NodePtr nodeDes){
	if(this->descendants[nodeAsc->id()] == nullptr)
		this->descendants[nodeAsc->id()] = nodeDes;
		
	if( this->attr_area[ this->descendants[nodeAsc->id()]->id() ] < this->attr_area[ nodeDes->id() ])
		this->descendants[nodeAsc->id()] = nodeDes;
}

UltimateAttributeOpening::NodePtr morphotree::UltimateAttributeOpening::getNodeAscendant(NodePtr node, int h){
		NodePtr n = node;
		for(int i=0; i <= h; i++){
			if(this->maxtree.type()  == MorphoTreeType::MaxTree){
				if(node->level() >= n->level() + h)
					return n;
			}else{
				if(node->level() <= n->level() - h)
					return n;
			}
			if(n->parent() != nullptr)
				n = n->parent();
			else 
				return n;
		}
		return n;
	}

  double UltimateAttributeOpening::getStability(NodePtr node){
		return (double) (this->attrs_increasing[this->getAscendant(node)->id()] - this->attrs_increasing[this->getDescendant(node)->id()]) / this->attrs_increasing[node->id()]  ;
	}


	UltimateAttributeOpening::NodePtr UltimateAttributeOpening::getAscendant(NodePtr node){
		return this->ascendants[node->id()];
	}
	
	UltimateAttributeOpening::NodePtr UltimateAttributeOpening::getDescendant(NodePtr node){
		return this->descendants[node->id()];
	}

  void morphotree::UltimateAttributeOpening::execute(int maxCriterion, int deltaMSER){
    this->attr_area = AreaComputer<uint8>().computeAttribute(maxtree);
    this->execute(maxCriterion, deltaMSER, this->attr_area , this->attr_area[0]/2);
  }

  void morphotree::UltimateAttributeOpening::execute(int maxCriterion, int deltaMSER,  std::vector<uint32> attr_mser, int maxValue_mser){
    this->maxCriterion = maxCriterion;

    if(deltaMSER != UNDEF){  
      this->attr_area = AreaComputer<uint8>().computeAttribute(maxtree);
      this->maxValue = maxValue_mser;//this->attr_area[0]/2;// maxCriterion;
      this->maxVariation = 10.0;
      this->minValue = 0;
      this->attr_mser = attr_mser;
        
      this->selectedForFiltering = this->computerMSER(deltaMSER);
    }
    else{
      std::vector<bool> tmp(this->maxtree.numberOfNodes(), true);
      this->selectedForFiltering = tmp;
    }
      
    for (int id=0; id < maxtree.numberOfNodes(); id++) {
      maxContrastLUT[id] = 0;
      associatedIndexLUT[id] = 0;
    }

    for(NodePtr son: this->maxtree.root()->children()){
      computeUAO(son, this->maxtree.root()->level(), false, false);
    }
  }

  void morphotree::UltimateAttributeOpening::execute(int maxCriterion){
    this->execute(maxCriterion, UNDEF, {}, UNDEF);
  }

  morphotree::UltimateAttributeOpening::UltimateAttributeOpening(const MTree &pmaxtree, std::vector<morphotree::uint32> attrs_increasing)
    :maxtree{pmaxtree}
  {      
      this->maxContrastLUT = new int[this->maxtree.numberOfNodes()];
      this->associatedIndexLUT = new int[this->maxtree.numberOfNodes()];
      this->attrs_increasing = attrs_increasing;
    }

    morphotree::UltimateAttributeOpening::~UltimateAttributeOpening(){
      free(maxContrastLUT);
      free(associatedIndexLUT);
    }

    std::vector<morphotree::uint8> morphotree::UltimateAttributeOpening::createRandomColor(std::vector<morphotree::uint32> img){
      int max = 0;
      for(int i=0; i < img.size(); i++){
        if(max < img[i])
          max = img[i];
      }

      int r[max+1];
      int g[max+1];
      int b[max+1];
      r[0] = 0;
      g[0] = 0;
      r[0] = 0;
      for(int i= 1; i <= max; i++){
        r[i] = rand() % 256;
        g[i] = rand() % 256;
        b[i] = rand() % 256;
      }
      std::vector<uint8> output(img.size()*3);
      for (int pidx=0; pidx < img.size(); pidx++) {

        uint32 cpidx = pidx * 3;           // (coloured) for 3 channels
        output[cpidx ] = r[ img[pidx] ];
        output[cpidx +1] = g[ img[pidx] ];
        output[cpidx +2] = b[ img[pidx] ];
          
      }
      return output;
    }

    std::vector<morphotree::uint8> morphotree::UltimateAttributeOpening::getMaxConstrastImage(){
      std::vector<uint8> out(maxtree.numberOfCNPs(), 0);
      
      for (int pidx=0; pidx < maxtree.numberOfCNPs(); pidx++) {
        out[pidx] = maxContrastLUT[maxtree.smallComponent(pidx)->id()];
      }
      return out;
    }

    std::vector<morphotree::uint32> morphotree::UltimateAttributeOpening::getAssociatedImage(){
      std::vector<uint32> out(maxtree.numberOfCNPs(), 0);
      for (int pidx=0; pidx < maxtree.numberOfCNPs(); pidx++) {
        out[pidx] = associatedIndexLUT[maxtree.smallComponent(pidx)->id()];
      }
      return out;
    }

    std::vector<morphotree::uint8> morphotree::UltimateAttributeOpening::getAssociatedColorImage(){
      return createRandomColor(getAssociatedImage());
    }

  
    bool morphotree::UltimateAttributeOpening::isSelectedForPruning(NodePtr currentNode){
      return this->attrs_increasing[currentNode->id()] != this->attrs_increasing[currentNode->parent()->id()];
    }

    void morphotree::UltimateAttributeOpening::computeUAO(NodePtr currentNode, int levelNodeNotInNR, bool qPropag, bool isCalculateResidue){
      NodePtr parentNode = currentNode->parent();
      int levelNodeInNR = currentNode->level();
      bool flagPropag = false;
      int contrast = 0;
      if (this->isSelectedForPruning(currentNode)){ //new primitive?
          levelNodeNotInNR = parentNode->level();
          
          if(this->attrs_increasing[currentNode->id()] <= this->maxCriterion){ //node selected for pruning = first node in Nr
            isCalculateResidue =  hasNodeSelectedInPrimitive(currentNode);
          }
      }
      
      if(this->attrs_increasing[currentNode->id()] <= this->maxCriterion){
        
        if( isCalculateResidue ) //non Filter?
          contrast = (int) std::abs( levelNodeInNR - levelNodeNotInNR );

        if (this->maxContrastLUT[parentNode->id()] >= contrast) {
            this->maxContrastLUT[currentNode->id()] = this->maxContrastLUT[parentNode->id()];
            this->associatedIndexLUT[currentNode->id()] = this->associatedIndexLUT[parentNode->id()];
        }
        else{
          this->maxContrastLUT[currentNode->id()] = contrast;
          if(qPropag){
            this->associatedIndexLUT[currentNode->id()] = this->associatedIndexLUT[parentNode->id()];
          }
          else{
            this->associatedIndexLUT[currentNode->id()] = this->attrs_increasing[currentNode->id()] + 1;	
          }
          flagPropag = true;
        }
      }

      for(NodePtr son: currentNode->children()){
        this->computeUAO(son, levelNodeNotInNR, flagPropag, isCalculateResidue);
      }
    }


bool morphotree::UltimateAttributeOpening::hasNodeSelectedInPrimitive(NodePtr currentNode){
    std::stack<NodePtr> s;
    s.push(currentNode);
    while(!s.empty()){
			NodePtr node = s.top(); s.pop();
			if(selectedForFiltering[node->id()]){ 
				return true;
			}
				
			for(NodePtr n: node->children()){
        if (this->attrs_increasing[n->id()] == this->attrs_increasing[n->parent()->id()]){ //if n in Nr?
					s.push(n);
			  }
		  }
    }
		return false;
	}
	
 
}