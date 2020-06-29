 /*! \file molcas_interface.h
     \brief Functions pertaining to the OpenMolcas interface.
 */
#include <Eigen/Dense>
#ifndef MOLCAS_INTERFACE_H_
#define MOLCAS_INTERFACE_H_

/*! Read in TDMs from OpenMolcas rassi.h5 file.
    \param dmat_filename: file location of the rassi.h5 file.
    \return An array of size 2, each entry holding a 2D vector corresponding to the transition densities in AO basis.
     The first entry is the alpha densities, the second the beta densities.
*/
std::array<std::vector<std::vector<Eigen::MatrixXd>>,2> read_rassi_tdms(std::string dmat_filename);
/*! Read in overlap matrix from OpenMolcas rassi.h5 file.
    \param filename: String file location of the rassi.h5 file.
    \return Eigen matrix containing the overlap matrix. Dimension is NxN, where N is number of basis functions.
*/
Eigen::MatrixXd read_rassi_overlap(std::string filename);
/*! Read in effective Hamiltonian from OpenMolcas output file.
    \param nstates: number of states
    \param filename: file location of the OpenMolcas output file.
    \return Eigen matrix containing the effective Hamiltonian matrix. Dimension is MxM, where M is number of states.
*/
Eigen::MatrixXd read_mscaspt2_heff(size_t nstates, std::string filename);

#endif /* MOLCAS_INTERFACE_H_ */
