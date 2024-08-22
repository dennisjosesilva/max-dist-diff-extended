


#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>


#include <morphotree/core/box.hpp>
#include <morphotree/core/alias.hpp>
#include <morphotree/tree/ct_builder.hpp>
#include <morphotree/tree/mtree.hpp>
#include <morphotree/adjacency/adjacency4c.hpp>
#include <morphotree/adjacency/adjacency8c.hpp>
#include <morphotree/attributes/areaComputer.hpp>
#include <morphotree/residues/UltimateAttributeOpening.hpp>

#include <morphotree/attributes/extinctionValues/ExtinctionValueLeavesComputer.hpp>
#include <morphotree/attributes/areaComputer.hpp>
#include <morphotree/filtering/extinctionFilter.hpp>


#include <iostream>
#include <string>

#include <MaxDist/MaxDistComputer.hpp>
#include <Attributes/InertiaComputer.hpp>
#include <Attributes/BasicAttributeComputer.hpp>


namespace py = pybind11;

py::array executeDirectFilter(py::array_t<int> &input, int numRows, int numCols, std::string attr_name, double thresholdValue){
    
    using morphotree::uint8;
    using morphotree::uint32;
    using morphotree::int32;
    using morphotree::Adjacency;
    using morphotree::Adjacency8C;
    using morphotree::MorphologicalTree;
    using morphotree::Box;
    using morphotree::buildMaxTree;
    using MTree = morphotree::MorphologicalTree<uint8>;
    using NodePtr = typename MTree::NodePtr;
    
    auto buf_input = input.request();
	int* img = (int *) buf_input.ptr;

	Box domain = Box::fromSize({static_cast<uint32>(numCols), static_cast<uint32>(numRows)});
    std::vector<uint8> f(img, img + domain.numberOfPoints());

    // build max-tree
    std::shared_ptr<Adjacency> adj = std::make_shared<Adjacency8C>(domain);
    MorphologicalTree<uint8> maxtree = buildMaxTree(f, adj);
    

    std::function<bool (float, float)> op = [](float lvalue, float rvalue) { return lvalue > rvalue; };
    
    // perform filter
    if(attr_name == "maxDist"){
        std::vector<uint32> maxDist = computeMaxDistanceAttribute(domain, f, maxtree);
        maxtree.idirectFilter([&op, &maxDist, thresholdValue](NodePtr node) { 
            return op(maxDist[node->id()], thresholdValue);
        });
    }else{
        // compute attributes
        std::vector<BasicAttributes> attrs = computeBasicAttributes(domain, f, maxtree);

        if (attr_name == "area") {
        maxtree.idirectFilter([&op, &attrs, thresholdValue](NodePtr node) { 
            return op(attrs[node->id()].area(), thresholdValue);
        });  
        }
        else if (attr_name == "volume") {
        maxtree.idirectFilter([&op, &attrs, thresholdValue](NodePtr node) { 
            return op(attrs[node->id()].volume(), thresholdValue);
        });  
        }
        else if (attr_name == "level") {
        maxtree.idirectFilter([&op, &attrs, thresholdValue](NodePtr node) { 
            return op(attrs[node->id()].level(), thresholdValue);
        });  
        }
        else if (attr_name == "mean_level") {
        maxtree.idirectFilter([&op, &attrs, thresholdValue](NodePtr node) { 
            return op(attrs[node->id()].meanLevel(), thresholdValue);
        });  
        }
        else if (attr_name == "level_variance") {
        maxtree.idirectFilter([&op, &attrs, thresholdValue](NodePtr node) { 
            return op(attrs[node->id()].varianceLevel(), thresholdValue);
        });  
        }
        else if (attr_name == "box_width") {
        maxtree.idirectFilter([&op, &attrs, thresholdValue](NodePtr node) { 
            return op(attrs[node->id()].width(), thresholdValue);
        });  
        }
        else if (attr_name == "box_height") {
        maxtree.idirectFilter([&op, &attrs, thresholdValue](NodePtr node) { 
            return op(attrs[node->id()].height(), thresholdValue);
        });  
        }
        else if (attr_name == "rectangularity") {
        maxtree.idirectFilter([&op, &attrs, thresholdValue](NodePtr node) { 
            return op(attrs[node->id()].rectangularity(), thresholdValue);
        });  
        }
        else if (attr_name == "ratioWH") {
        maxtree.idirectFilter([&op, &attrs, thresholdValue](NodePtr node) { 
            return op(attrs[node->id()].ratioWH(), thresholdValue);
        });  
        }
        else if (attr_name == "moment20") {
        maxtree.idirectFilter([&op, &attrs, thresholdValue](NodePtr node) { 
            return op(attrs[node->id()].moment20(), thresholdValue);
        });  
        }
        else if (attr_name == "moment02") {
        maxtree.idirectFilter([&op, &attrs, thresholdValue](NodePtr node) { 
            return op(attrs[node->id()].moment02(), thresholdValue);
        });  
        }
        else if (attr_name == "moment11") {
        maxtree.idirectFilter([&op, &attrs, thresholdValue](NodePtr node) { 
            return op(attrs[node->id()].moment11(), thresholdValue);
        });  
        }
        else if (attr_name == "inertia") {
            std::vector<double> inertia = computeInertia(domain, maxtree);
        maxtree.idirectFilter([&op, &inertia, thresholdValue](NodePtr node) { 
            return op(inertia[node->id()], thresholdValue);
        });  
        }
        else if (attr_name == "orientation") {
        maxtree.idirectFilter([&op, &attrs, thresholdValue](NodePtr node) { 
            return op(attrs[node->id()].orientation(), thresholdValue);
        });  
        }
        else if (attr_name == "length_major_axis") {
        maxtree.idirectFilter([&op, &attrs, thresholdValue](NodePtr node) { 
            return op(attrs[node->id()].lenMajorAxis(), thresholdValue);
        });  
        }
        else if (attr_name == "length_minor_axis") {
        maxtree.idirectFilter([&op, &attrs, thresholdValue](NodePtr node) { 
            return op(attrs[node->id()].lenMinorAxis(), thresholdValue);
        });  
        }
        else if (attr_name == "eccentricity") {
        maxtree.idirectFilter([&op, &attrs, thresholdValue](NodePtr node) { 
            return op(attrs[node->id()].eccentricity(), thresholdValue);
        });  
        }
        else if (attr_name == "compactness") {
        maxtree.idirectFilter([&op, &attrs, thresholdValue](NodePtr node) { 
            return op(attrs[node->id()].compactness(), thresholdValue);
        });  
        }
    
  }


    std::vector<uint8> imgOut = maxtree.reconstructImage();

    auto imgOut_numpy = py::array(py::buffer_info(
		imgOut.data(),            // Pointer to data (nullptr -> ask NumPy to allocate!) 
		sizeof(uint8),     // Size of one item 
		py::format_descriptor<uint8>::value, // Buffer format 
		1,          // How many dimensions? 
		{ ( numRows * numCols ) },  // Number of elements for each dimension 
		{ sizeof(uint8) }  // Strides for each dimension 
	));
    return imgOut_numpy;
}


py::array executeExtinctionFilter(py::array_t<int> &input, int numRows, int numCols, std::string attr_name, int nleaves){
    // import morphotree types
    using morphotree::uint8;
    using morphotree::uint32;
    using morphotree::Box;
    using morphotree::MorphologicalTree;
    using morphotree::Adjacency8C;
    using morphotree::Adjacency;
    using morphotree::buildMaxTree;
    using morphotree::AreaComputer;
    using morphotree::ExtinctionValueLeavesComputer;
    using NodePtr = MorphologicalTree<uint8>::NodePtr;
    using ExtinctionValueComputer = ExtinctionValueLeavesComputer<uint8, uint32>;
    using ExtinctionValueMapType = typename ExtinctionValueComputer::MapType;
    using morphotree::extinctionFilter;
    using morphotree::iextinctionFilter;

    auto buf_input = input.request();
	int* img = (int *) buf_input.ptr;

	Box domain = Box::fromSize({static_cast<uint32>(numCols), static_cast<uint32>(numRows)});
    std::vector<uint8> f(img, img + domain.numberOfPoints());

    // build max-tree
    std::shared_ptr<Adjacency> adj = std::make_shared<Adjacency8C>(domain);
    MorphologicalTree<uint8> maxtree = buildMaxTree(f, adj);

    // Extract leaves
    std::vector<NodePtr> leaves;
    maxtree.tranverse([&leaves](NodePtr node){
    if (node->children().empty()) // node is leaf
      leaves.push_back(node);
    });

    if(attr_name == "area"){
        iextinctionFilter(maxtree, AreaComputer<uint8>().computeAttribute(maxtree), nleaves);    
    }
    else if(attr_name == "maxDist"){
        iextinctionFilter(maxtree, computeMaxDistanceAttribute(domain, f, maxtree), nleaves);    
    }
    else if(attr_name == "inertia"){
        std::vector<double> inertia = computeInertia(domain, maxtree);
       iextinctionFilter(maxtree, inertia, nleaves);
    }

    
    std::vector<uint8> imgOut = maxtree.reconstructImage();

    auto imgOut_numpy = py::array(py::buffer_info(
		imgOut.data(),            // Pointer to data (nullptr -> ask NumPy to allocate!) 
		sizeof(uint8),     // Size of one item 
		py::format_descriptor<uint8>::value, // Buffer format 
		1,          // How many dimensions? 
		{ ( numRows * numCols ) },  // Number of elements for each dimension 
		{ sizeof(uint8) }  // Strides for each dimension 
	));
    return imgOut_numpy;
}
 

py::tuple executeUAO(py::array_t<int> &input, int numRows, int numCols, int maxCriterion, int deltaMSER, std::string attr_name){
   
    // import morphotree types
    using morphotree::uint8;
    using morphotree::uint32;
    using morphotree::Box;
    using morphotree::MorphologicalTree;
    using morphotree::Adjacency8C;
    using morphotree::Adjacency;
    using morphotree::buildMaxTree;
    using morphotree::AreaComputer;
    using morphotree::UltimateAttributeOpening;

    auto buf_input = input.request();
	int* img = (int *) buf_input.ptr;

	Box domain = Box::fromSize({static_cast<uint32>(numCols), static_cast<uint32>(numRows)});
    std::vector<uint8> f(img, img + domain.numberOfPoints());

    // build max-tree
    std::shared_ptr<Adjacency> adj = std::make_shared<Adjacency8C>(domain);
    MorphologicalTree<uint8> maxtree = buildMaxTree(f, adj);
    
    std::vector<uint32> maxDist = computeMaxDistanceAttribute(domain, f, maxtree);
    int maxValue = 0;
    for (int id=0; id < maxtree.numberOfNodes(); id++) {
        if(maxDist[id] > maxValue)
            maxValue = maxDist[id];
    }
    
    UltimateAttributeOpening uao(maxtree, maxDist);
    if(attr_name == "area"){
        std::vector<uint32> attr_area = AreaComputer<uint8>().computeAttribute(maxtree);
        uao.execute(maxCriterion, deltaMSER, attr_area, attr_area[0]/2);
    }else if(attr_name == "maxDist"){
      uao.execute(maxCriterion, deltaMSER, maxDist, maxValue);
    }
    else{
      uao.execute(maxCriterion, 0);
    }
    std::vector<uint8> imgMaxConstrast = uao.getMaxConstrastImage();
    std::vector<uint32> imgAssociated = uao.getAssociatedImage();


    int n = numRows * numCols;		
	auto imgMaxConstrast_numpy = py::array(py::buffer_info(
		imgMaxConstrast.data(),            // Pointer to data (nullptr -> ask NumPy to allocate!) 
		sizeof(uint8),     // Size of one item 
		py::format_descriptor<uint8>::value, // Buffer format 
		1,          // How many dimensions? 
		{ ( n ) },  // Number of elements for each dimension 
		{ sizeof(uint8) }  // Strides for each dimension 
	));
	auto imgAssociated_numpy = py::array(py::buffer_info(
		imgAssociated.data(),            // Pointer to data (nullptr -> ask NumPy to allocate!) 
		sizeof(uint32),     // Size of one item 
		py::format_descriptor<uint32>::value, // Buffer format 
		1,          // How many dimensions? 
		{ ( n ) },  // Number of elements for each dimension 
		{ sizeof(uint32) }  // Strides for each dimension 
	));
	
    return py::make_tuple(imgMaxConstrast_numpy, imgAssociated_numpy);

}

PYBIND11_MODULE(pythonApps, m) {
    // Optional docstring
    m.doc() = "...";
    m.def("executeUAO", &executeUAO, "Ultimate attribute opening operators", 
        py::arg("input"),
        py::arg("numRows"),
        py::arg("numCols"),
        py::arg("maxCriterion"),
        py::arg("deltaMSER") = 0,
        py::arg("attr_name") = "None"
    );
    m.def("executeExtinctionFilter", &executeExtinctionFilter, "extinction filter",
        py::arg("input"),
        py::arg("numRows"),
        py::arg("numCols"),
        py::arg("attr_name") = "maxDist",
        py::arg("nleaves") = 15
    );
    m.def("executeDirectFilter", &executeDirectFilter, "direct filter",
        py::arg("input"),
        py::arg("numRows"),
        py::arg("numCols"),
        py::arg("attr_name") = "maxDist",
        py::arg("attr_value") = 15
    );
    

}
