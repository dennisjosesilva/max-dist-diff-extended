


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

#include <iostream>
#include <string>

#include <MaxDist/MaxDistComputer.hpp>



namespace py = pybind11;

 

py::tuple executeUAO(py::array_t<int> &input, int numRows, int numCols, int maxCriterion, int deltaMSER){
   
    // import morphotree types
    using morphotree::uint8;
    using morphotree::uint32;
    using morphotree::Box;
    using morphotree::UI32Point;
    using morphotree::MorphologicalTree;
    using morphotree::Adjacency8C;
    using morphotree::Adjacency;
    using morphotree::buildMaxTree;
    using NodePtr = MorphologicalTree<uint8>::NodePtr;
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
    
    std::vector<uint32> attr_area = AreaComputer<uint8>().computeAttribute(maxtree);
    uao.execute(maxCriterion, deltaMSER, attr_area, attr_area[0]/2);
    std::vector<uint8> imgMaxConstrast = uao.getMaxConstrastImage();
    std::vector<uint8> imgRGBAssociated = uao.getAssociatedColorImage();


    int n = numRows * numCols;
		
	auto imgMaxConstrast_numpy = py::array(py::buffer_info(
		imgMaxConstrast.data(),            // Pointer to data (nullptr -> ask NumPy to allocate!) 
		sizeof(int),     // Size of one item 
		py::format_descriptor<int>::value, // Buffer format 
		1,          // How many dimensions? 
		{ ( n ) },  // Number of elements for each dimension 
		{ sizeof(uint8) }  // Strides for each dimension 
	));
	auto imgAssociated_numpy = py::array(py::buffer_info(
		imgRGBAssociated.data(),            // Pointer to data (nullptr -> ask NumPy to allocate!) 
		sizeof(int),     // Size of one item 
		py::format_descriptor<int>::value, // Buffer format 
		1,          // How many dimensions? 
		{ ( n ) },  // Number of elements for each dimension 
		{ sizeof(uint8) }  // Strides for each dimension 
	));
	
    return py::make_tuple(imgMaxConstrast_numpy, imgAssociated_numpy);

}

PYBIND11_MODULE(pythonApps, m) {
    // Optional docstring
    m.doc() = "...";
    m.def("executeUAO", &executeUAO);
}
