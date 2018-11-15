#pragma once

#include <Spacy/LinearSolver.h>
#include <Spacy/ZeroVectorCreator.h>
#include <Spacy/Util/Mixins/Get.h>
#include <Spacy/Util/Base/AddArithmeticOperators.h>
#include <Spacy/Util/Base/OperatorBase.h>
#include <Spacy/Util/Base/VectorBase.h>

namespace Mock
{
  class LinearOperator
      : public Spacy::Mixin::Get<double>,
        public Spacy::OperatorBase,
        public Spacy::VectorBase,
        public Spacy::AddArithmeticOperators<LinearOperator>
  {
  public:
    LinearOperator(double value=1);

    ::Spacy::Vector operator()(const ::Spacy::Vector& dx) const;

    ::Spacy::Real operator()(const LinearOperator& y) const;

    /// Access solver representing \f$A^{-1}\f$
    ::Spacy::LinearSolver solver() const;
  };
}
