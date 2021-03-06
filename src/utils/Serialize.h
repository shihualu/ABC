/*
 * Serialize.h
 *
 *  Created on: Oct 31, 2016
 *      Author: baki
 */

#ifndef SRC_UTILS_SERIALIZE_H_
#define SRC_UTILS_SERIALIZE_H_

#include "../boost/multiprecision/cpp_int.hpp"
#include "../Eigen/SparseCore"
#include "../cereal/archives/binary.hpp"
#include "Math.h"

namespace Vlab {
namespace Util {
namespace Serialize {

template<class Archive>
void save(Archive& ar, const Theory::BigInteger& big_integer) {

  bool s = big_integer.sign();
  ar(s);
  std::size_t c = big_integer.backend().size();
  ar(c);
  ar(cereal::binary_data(big_integer.backend().limbs(), c * sizeof(boost::multiprecision::limb_type)));
}

template<class Archive>
void load(Archive& ar, Theory::BigInteger& big_integer) {
  bool s;
  ar(s);
  std::size_t c;
  ar(c);
  big_integer.backend().resize(c, c);
  ar(cereal::binary_data(big_integer.backend().limbs(), c * sizeof(boost::multiprecision::limb_type)));
  if(s != big_integer.sign()) {
      big_integer.backend().negate();
  }
  big_integer.backend().normalize();
}

template<class Archive>
void save(Archive& ar, const Eigen::SparseVector<Theory::BigInteger>& sparse_vector) {
  ar(sparse_vector.size());
  ar(sparse_vector.innerSize());
  ar(sparse_vector.nonZeros());
  for (Eigen::SparseVector<Theory::BigInteger>::Index i = 0; i < sparse_vector.nonZeros(); ++i) {
    Util::Serialize::save(ar, sparse_vector.valuePtr()[i]);
  }
  ar(cereal::binary_data(sparse_vector.innerIndexPtr(), sparse_vector.nonZeros() * sizeof(Eigen::SparseVector<Theory::BigInteger>::StorageIndex)));

}

template<class Archive>
void load(Archive& ar, Eigen::SparseVector<Theory::BigInteger>& sparse_vector) {
  Eigen::SparseVector<Theory::BigInteger>::Index size = 0, inner_size = 0, non_zeros = 0;
  ar(size);
  ar(inner_size);
  ar(non_zeros);
  sparse_vector.resize(size);
  sparse_vector.resizeNonZeros(non_zeros);
  for (Eigen::SparseVector<Theory::BigInteger>::Index i = 0; i < non_zeros; ++i) {
    Util::Serialize::load(ar, sparse_vector.valuePtr()[i]);
  }
  ar(cereal::binary_data(sparse_vector.innerIndexPtr(), non_zeros * sizeof(Eigen::SparseVector<Theory::BigInteger>::StorageIndex)));
  sparse_vector.finalize();
}

template<class Archive>
void save(Archive& ar, const Eigen::SparseMatrix<Theory::BigInteger>& sparse_matrix) {
//  std::cout << std::left;
//  std::cout << std::setw(20) << "sizeof(int):" << sizeof(int) << std::endl;
//  std::cout << std::setw(20) << "sizeof(Index):" << sizeof(Eigen::SparseMatrix<Theory::BigInteger>::Index) << std::endl;
//  std::cout << std::setw(20) << "sizeof(StorageIndex):" << sizeof(Eigen::SparseMatrix<Theory::BigInteger>::StorageIndex) << std::endl;
//  std::cout << std::setw(20) << "sizeof(size_t):" << sizeof(std::size_t) << std::endl;
//  std::cout << "--------------------------------------------" << std::endl;
//
//  std::cout << std::setw(20) << "rows - cols:" << sparse_matrix.rows() << " - " << sparse_matrix.cols() << std::endl;
//  std::cout << std::setw(20) << "non zeros:" << sparse_matrix.nonZeros() << std::endl;
//  std::cout << std::setw(20) << "outer size:" << sparse_matrix.outerSize() << std::endl;
//  std::cout << std::setw(20) << "inner size:" << sparse_matrix.innerSize() << std::endl;
//  std::cout << "values:" << std::endl;
//  for (Eigen::SparseMatrix<Theory::BigInteger>::Index i = 0; i < sparse_matrix.nonZeros(); ++i) {
//    std::cout << sparse_matrix.valuePtr()[i] << " ";
//  }
//  std::cout << std::endl;
//  std::cout << "outer pointers:" << std::endl;
//  for (Eigen::SparseMatrix<Theory::BigInteger>::Index i = 0; i <= sparse_matrix.outerSize(); ++i) {
//    std::cout << sparse_matrix.outerIndexPtr()[i] << ",";
//    std::cout << sparse_matrix.derived().outerIndexPtr()[i] << "  ";
//  }
//  std::cout << std::endl;
//  std::cout << "inner pointers:" << std::endl;
//  for (Eigen::SparseMatrix<Theory::BigInteger>::Index i = 0; i < sparse_matrix.nonZeros(); ++i) {
//    std::cout << sparse_matrix.innerIndexPtr()[i] << ",";
//    std::cout << sparse_matrix.derived().innerIndexPtr()[i] << " ";
//  }
//  std::cout << std::endl;
//  std::cout << "------------------------------------------------" << std::endl;

  Eigen::SparseMatrix<Theory::BigInteger>::Index rows = sparse_matrix.rows(), cols = sparse_matrix.cols(),
      non_zeros = sparse_matrix.nonZeros(), outer_size = sparse_matrix.outerSize(), inner_size = sparse_matrix.innerSize();
  ar(rows);
  ar(cols);
  ar(non_zeros);
  ar(outer_size);
  ar(inner_size);
  for (Eigen::SparseMatrix<Theory::BigInteger>::Index i = 0; i < non_zeros; ++i) {
    Util::Serialize::save(ar, sparse_matrix.valuePtr()[i]);
  }
  ar(cereal::binary_data(sparse_matrix.outerIndexPtr(), (outer_size + 1) * sizeof(Eigen::SparseMatrix<Theory::BigInteger>::StorageIndex)));
  ar(cereal::binary_data(sparse_matrix.innerIndexPtr(), non_zeros * sizeof(Eigen::SparseMatrix<Theory::BigInteger>::StorageIndex)));
}

template<class Archive>
void load(Archive& ar, Eigen::SparseMatrix<Theory::BigInteger>& sparse_matrix) {
  Eigen::SparseMatrix<Theory::BigInteger>::Index rows = 0, cols = 0, non_zeros = 0, outer_size = 0, inner_size = 0;
  ar(rows);
  ar(cols);
  ar(non_zeros);
  ar(outer_size);
  ar(inner_size);
  sparse_matrix.resize(rows, cols);
  sparse_matrix.makeCompressed();
  sparse_matrix.resizeNonZeros(non_zeros);
  for (Eigen::SparseMatrix<Theory::BigInteger>::Index i = 0; i < non_zeros; ++i) {
    Util::Serialize::load(ar, sparse_matrix.valuePtr()[i]);
  }
  ar(cereal::binary_data(sparse_matrix.outerIndexPtr(), (outer_size + 1) * sizeof(Eigen::SparseMatrix<Theory::BigInteger>::StorageIndex)));
  ar(cereal::binary_data(sparse_matrix.innerIndexPtr(), non_zeros * sizeof(Eigen::SparseMatrix<Theory::BigInteger>::StorageIndex)));
  sparse_matrix.finalize();
//
//  std::cout << std::left;
//  std::cout << std::setw(20) << "rows - cols:" << rows << " - " << cols << std::endl;
//  std::cout << std::setw(20) << "non zeros:" << non_zeros << std::endl;
//  std::cout << std::setw(20) << "outer size:" << outer_size << std::endl;
//  std::cout << std::setw(20) << "inner size:" << inner_size << std::endl;
//  std::cout << "values:" << std::endl;
//  for (Eigen::SparseMatrix<Theory::BigInteger>::Index i = 0; i < non_zeros; ++i) {
//    std::cout << sparse_matrix.valuePtr()[i] << " ";
//  }
//  std::cout << std::endl;
//  std::cout << "outer pointers:" << std::endl;
//  for (Eigen::SparseMatrix<Theory::BigInteger>::Index i = 0; i < outer_size; ++i) {
//    std::cout << sparse_matrix.outerIndexPtr()[i] << " ";
//  }
//  std::cout << std::endl;
//  std::cout << "inner pointers:" << std::endl;
//  for (Eigen::SparseMatrix<Theory::BigInteger>::Index i = 0; i < non_zeros; ++i) {
//    std::cout << sparse_matrix.innerIndexPtr()[i] << " ";
//  }
//
//  std::cout << std::endl;
//  std::cout << "what about nonzeros: " << sparse_matrix.nonZeros() << std::endl;
//  std::cout << "------------------------------------------------" << std::endl;

}

} /* namespace Serialize */
} /* namespace Util */
} /* namespace Vlab */

#endif /* SRC_UTILS_SERIALIZE_H_ */
