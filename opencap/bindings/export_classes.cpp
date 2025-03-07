/*Copyright (c) 2021 James Gayvert

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <pybind11/functional.h>
#include "System.h"
#include "CAP.h"
#include <pybind11/eigen.h>
#include <Eigen/Dense>
#include <pybind11/stl_bind.h>

namespace py = pybind11;


PYBIND11_MODULE(pyopencap_cpp, m) {
    py::class_<System>(m, "System")
		.def(py::init<py::dict>(),py::arg("sys_dict"),"Constructs System object from python dictionary.")
		.def("get_overlap_mat",&System::get_overlap_mat,py::arg("ordering")="molden",py::arg("basis_file")="","Returns overlap matrix. Supported orderings: pyscf, openmolcas, qchem, psi4, molden.")
		.def("check_overlap_mat",&System::check_overlap_mat,py::arg("smat"),
				py::arg("ordering"),py::arg("basis_file")="","Compares input overlap matrix to "
						"internal overlap matrix to check basis set ordering. Supported orderings: pyscf, openmolcas, qchem, psi4, molden.")
		.def("get_basis_ids",&System::get_basis_ids,"Returns a string of the basis function ids. "
				"Each ID has the following format:"
				"atom index,shell number,l,m")
		;
    py::class_<CAP>(m, "CAP")
		.def(py::init<System,py::dict,size_t>(),py::arg("system"),py::arg("cap_dict"),py::arg("nstates"),
			"Constructs CAP object from system, cap dictionary, and number of states.")
		.def(py::init<System,py::dict,size_t,const std::function<std::vector<double>(std::vector<double> &, std::vector<double> &, 
			std::vector<double> &, std::vector<double> &)>&>(),py::arg("system"),py::arg("cap_dict"),py::arg("nstates"),
			py::arg("cap_func"),"Constructs CAP object from system, cap dictionary, number of states, and cap function.")
        .def("get_ao_cap",&CAP::get_ao_cap,py::arg("ordering")="molden",py::arg("basis_file") = "","Returns CAP matrix in AO basis. Supported orderings: pyscf, openmolcas, qchem, psi4, molden.")
        .def("get_ao_capG",&CAP::get_ao_capG,py::arg("ordering")="molden",py::arg("basis_file") = "","Returns CAPG matrix in AO basis. Supported orderings: pyscf, openmolcas, qchem, psi4, molden.")
    	.def("get_projected_cap",&CAP::get_projected_cap, "Returns CAP matrix in state basis.")
    	.def("get_projected_capG",&CAP::get_projected_capG, "Returns CAPG matrix in state basis.")
    	.def("get_projected_cap_der",&CAP::get_projected_cap_der, "Returns CAP functional derivative matrix in state basis.")
    	.def("get_density",&CAP::get_density, py::arg("initial_idx"),py::arg("final_idx"), py::arg("beta")=false, "Returns specified density matrix.")
    	.def("compute_ao_cap",&CAP::compute_ao_cap,py::arg("cap_dict"),py::arg("cap_func")=nullptr, "Computes CAP matrix in AO basis.")
    	.def("compute_projected_cap",&CAP::compute_projected_cap, "Computes CAP matrix in state basis using"
    			" transition density matrices.")
    	.def("compute_projected_capG",&CAP::compute_projected_capG, "Computes CAPG matrix in state basis using"
    			" transition density matrices.")
		.def("compute_projected_cap_der",&CAP::compute_projected_cap_der, "Computes CAP functional derivative matrix in state basis using"
    			" transition density matrices.")
		.def("get_H",&CAP::get_H, "Returns zeroth order Hamiltonian read from file.")
		.def("add_tdm",  &CAP::add_tdm,py::arg("tdm"),
				py::arg("initial_idx"),py::arg("final_idx"),
				py::arg("ordering"),py::arg("basis_file") = "",
				"Adds spin-traced tdm to CAP object at specified indices. The optional argument basis_file"
				" is required when using the OpenMolcas interface, and it must point to the path to the rassi.5 file. Supported orderings: pyscf, openmolcas, qchem, psi4, molden.")
		.def("qcsoftware_to_opencap_ordering",&CAP::qcsoftware_to_opencap_ordering,py::arg("Matrix to convert"), 
				py::arg("ordering"), py::arg("basis_file") = "", "Returns QC-Software to OpenCAP ordering for a matrix.")
		.def("add_tdms", &CAP::add_tdms,py::arg("alpha_density"),
				py::arg("beta_density"),py::arg("initial_idx"),py::arg("final_idx"),
				py::arg("ordering"),py::arg("basis_file") = "",
				"Adds alpha/beta tdms to CAP object at specified indices. The optional argument basis_file"
				" is required when using the OpenMolcas interface, and it must point to the path to the rassi.5 file. Supported orderings: pyscf, openmolcas, qchem, psi4, molden.")
		.def("read_data",&CAP::read_electronic_structure_data, py::arg("es_dict"), "Reads electronic structure data "
				"specified in dictionary.")
		.def("renormalize_cap",&CAP::renormalize_cap, py::arg("smat"),
				py::arg("ordering"),py::arg("basis_file") = "","Re-normalizes AO CAP matrix using input overlap"
						" matrix.")
		.def("renormalize",&CAP::renormalize,"Re-normalizes AO CAP using electronic structure data.")
		.def("compute_cap_on_grid",&CAP::compute_cap_on_grid,py::arg("x"),py::arg("y"),py::arg("z"),py::arg("w"),"Computes CAP matrix on supplied grid. "
		"Sum will cumulated for each successive grid until compute_projected_cap is called.")
	;
}

