#include <gtest.hh>

#include <deal.II/grid/tria.h>
#include <deal.II/grid/grid_generator.h>
#include <deal.II/lac/vector.h>

#include <Spacy/zeroVectorCreator.hh>
#include <Spacy/Adapter/dealII/copy.hh>
#include <Spacy/Adapter/dealII/vector.hh>
#include <Spacy/Adapter/dealII/vectorSpace.hh>
#include <Spacy/Util/cast.hh>

namespace
{
    const int fe_order = 1;

    class dealII_VectorAdapter : public ::testing::Test
    {
    public:
        dealII_VectorAdapter()
        {
            dealii::GridGenerator::hyper_cube(triangulation, -1, 1);
            n_components = triangulation.n_vertices();
            V = Spacy::dealII::makeHilbertSpace(triangulation, fe_order);
            zeros = dealii::Vector<double>(n_components);
        }

        auto dealII_test_vector()
        {
            auto v = dealii::Vector<double>(n_components);
            v(0) = 1;
            v(1) = 2;
            return v;
        }

        Spacy::Vector Spacy_test_vector()
        {
            return Spacy::dealII::Vector(dealII_test_vector(), V);
        }


        dealii::Triangulation<2> triangulation;
        unsigned int n_components;
        Spacy::VectorSpace V;
        dealii::Vector<double> zeros;
    };
}


TEST_F(dealII_VectorAdapter,Create)
{
    auto v = Spacy_test_vector();

    const auto& v_ = get(Spacy::cast_ref<Spacy::dealII::Vector>(v));
    EXPECT_EQ(v_(0), 1.);
    EXPECT_EQ(v_(1), 2.);
}

TEST_F(dealII_VectorAdapter,AssignFromdealiiVector)
{
    auto v = dealII_test_vector();

    auto w0 = Spacy::dealII::Vector(zeros, V);
    auto w1 = Spacy::dealII::Vector(v, V);

    EXPECT_EQ( get(w0)(0), 0.);
    EXPECT_EQ( get(w0)(1), 0.);
    EXPECT_EQ( get(w1)(0), 1.);
    EXPECT_EQ( get(w1)(1), 2.);

    w0 = v;
    w1 = v;

    EXPECT_EQ( get(w0)(0), 1.);
    EXPECT_EQ( get(w0)(1), 2.);
    EXPECT_EQ( get(w1)(0), 1.);
    EXPECT_EQ( get(w1)(1), 2.);

    w0 = zeros;
    w1 = zeros;

    EXPECT_EQ( get(w0)(0), 0.);
    EXPECT_EQ( get(w0)(1), 0.);
    EXPECT_EQ( get(w1)(0), 0.);
    EXPECT_EQ( get(w1)(1), 0.);
}

TEST_F(dealII_VectorAdapter,AddAssign)
{
    auto v = dealII_test_vector();

    Spacy::dealII::Vector w0(v,V);
    Spacy::dealII::Vector w1(v,V);
    w0 += w1;

    EXPECT_EQ( get(w0)(0), 2.);
    EXPECT_EQ( get(w0)(1), 4.);

    Spacy::dealII::Vector w2(zeros, V);

    w2 += w0;

    EXPECT_EQ( get(w2)(0), 2.);
    EXPECT_EQ( get(w2)(1), 4.);
}

TEST_F(dealII_VectorAdapter,SubtractAssign)
{
    auto v = dealII_test_vector();

    Spacy::dealII::Vector w0(zeros,V);
    Spacy::dealII::Vector w1(v,V);
    w0 -= w1;

    EXPECT_EQ( get(w0)(0), -1.);
    EXPECT_EQ( get(w0)(1), -2.);
}

TEST_F(dealII_VectorAdapter,MultiplyWithScalar)
{
    auto v = dealII_test_vector();

    Spacy::dealII::Vector w(v,V);
    w *= 2;

    EXPECT_EQ( get(w)(0), 2.);
    EXPECT_EQ( get(w)(1), 4.);
}

TEST_F(dealII_VectorAdapter,ApplyAsDual)
{
    auto v = zeros;
    v(2) = 1;

    Spacy::dealII::Vector w(v,V);
    auto dp = w(w);

    EXPECT_EQ(get(dp), 1.);

    w *= 2;
    dp = w(w);

    EXPECT_EQ(get(dp), 4.);
}

TEST_F(dealII_VectorAdapter,Negation)
{
    auto v = dealII_test_vector();

    Spacy::dealII::Vector w0(v,V);
    auto w1 = -w0;

    v *= -1;
    EXPECT_EQ(get(w1), v);
}

TEST_F(dealII_VectorAdapter,Comparison)
{
    auto v = zeros;
    v(1) = 1;

    Spacy::dealII::Vector w0(v,V);
    Spacy::dealII::Vector w1(v,V);

    EXPECT_TRUE( w0 == w1 );

    const auto eps = 1e-5;
    V.set_eps(eps);

    get(w0)(1) = 1 - 0.5*eps;
    EXPECT_TRUE( w0 == w1 );

    get(w0)(1) = 1-1.1*eps;
    EXPECT_FALSE( w0 == w1 );
}
