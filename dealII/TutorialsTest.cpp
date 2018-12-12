#include <gtest.hh>

#include <Spacy/Util/CompareVTK.h>

TEST(dealii_Tutorial, Step4)
{
    const auto referenceFileName = "/home/deps/Spacy/Examples/deal.II_Tutorial/Tutorial/Step4/build/solution-2d.vtk";
    const auto solutionFileName = "/home/deps/Spacy/Examples/deal.II_Tutorial/Tutorial_Spacy/Step4/build/sol.vtk";
    EXPECT_TRUE(Spacy::compareLookupTable(referenceFileName, solutionFileName));
}
