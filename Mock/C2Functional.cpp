#include "C2Functional.h"

#include <Spacy/Spaces/ScalarSpace/Real.h>
#include <Spacy/Vector.h>
#include <Spacy/VectorSpace.h>
#include "LinearOperator.h"

namespace Mock
{
  C2Functional::C2Functional(const Spacy::VectorSpace& space)
    : domain_(&space)
  {}

  Spacy::Real C2Functional::operator()(const Spacy::Vector&) const
  {
    return Spacy::Real(3.);
  }

  Spacy::Vector C2Functional::d1(const Spacy::Vector&) const
  {
    return Spacy::Real(2.);
  }

  Spacy::Vector C2Functional::d2(const Spacy::Vector&, const Spacy::Vector&) const
  {
    return Spacy::Real(1.);
  }

  LinearOperator C2Functional::hessian(const Spacy::Vector&) const
  {
    return LinearOperator();
  }

  const Spacy::VectorSpace& C2Functional::domain() const
  {
    return *domain_;
  }
}
