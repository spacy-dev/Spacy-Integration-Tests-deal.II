#include <gtest.hh>

#include <deal.II/grid/tria.h>
#include <deal.II/grid/grid_generator.h>
#include <deal.II/lac/vector.h>
#include <deal.II/lac/block_vector.h>

#include <Spacy/zeroVectorCreator.hh>
#include <Spacy/Adapter/dealII/copy.hh>
#include <Spacy/Adapter/dealII/vector.hh>
#include <Spacy/Adapter/dealII/vectorSpace.hh>
#include <Spacy/Spaces/productSpace.hh>
#include <Spacy/Util/cast.hh>

using namespace Spacy;

namespace
{
    const constexpr int dim = 2;
    const int fe_order = 1;
    const int n_variables = 3u;

    class dealII_CopySingleVector : public ::testing::Test
    {
    public:
        dealII_CopySingleVector()
        {
            dealii::GridGenerator::hyper_cube(triangulation, -1, 1);
            n_components = triangulation.n_vertices();
            V = dealII::makeHilbertSpace(triangulation, fe_order);
        }

        auto dealII_test_vector()
        {
            auto v = dealii::Vector<double>(n_components);
            v(0) = 1;
            v(1) = 2;
            return v;
        }

        Vector Spacy_test_vector()
        {
            return dealII::Vector(dealII_test_vector(), V);
        }


        dealii::Triangulation<dim> triangulation;
        unsigned int n_components;
        VectorSpace V;
    };

    class dealII_CopyProductSpaceVector : public ::testing::Test
    {
    public:
        dealII_CopyProductSpaceVector()
        {
            dealii::GridGenerator::hyper_cube(triangulation, -1, 1);
            n_components = triangulation.n_vertices();
            V = dealII::makeHilbertSpace(triangulation, n_variables, fe_order);
        }

        auto dealII_test_vector()
        {
            auto v = dealii::BlockVector<double>(n_variables, n_components);

            for(auto i=0; i<n_variables; ++i)
                for(auto j=0u; j<n_components; ++j)
                    v.block(i)(j) = pow(10,i+1) + j;
            return v;
        }

        void check_dealII_vector(const dealii::BlockVector<double>& v)
        {
            for(auto i=0; i<n_variables; ++i)
                for(auto j=0u; j<n_components; ++j)
                    EXPECT_DOUBLE_EQ(v.block(i)(j), pow(10,i+1) + j);
        }

        void check_Spacy_vector(const Vector& v)
        {
            auto v_dealII = dealII_test_vector();
            auto& v_ = cast_ref<ProductSpace::Vector>(v);
            for(auto i=0u; i<n_variables; ++i)
                EXPECT_EQ(get(cast_ref<dealII::Vector>(v_.component(i))), v_dealII.block(i));
        }

        Vector Spacy_test_vector()
        {
            auto v_dealII = dealII_test_vector();
            auto v = zero(V);
            auto& v_ = cast_ref<ProductSpace::Vector>(v);
            for(auto i=0; i<n_variables; ++i)
                cast_ref<dealII::Vector>(v_.component(i)) = v_dealII.block(i);
            return v;
        }


        dealii::Triangulation<dim> triangulation;
        unsigned int n_components;
        VectorSpace V;
    };

    class dealII_CopyPermutedProductSpaceVector : public ::testing::Test
    {
    public:
        dealII_CopyPermutedProductSpaceVector()
        {
            dealii::GridGenerator::hyper_cube(triangulation, -1, 1);
            n_components = triangulation.n_vertices();
            V = dealII::makeHilbertSpace(triangulation, {2, 0, 1}, fe_order);
        }

        auto dealII_test_vector()
        {
            auto v = dealii::BlockVector<double>(n_variables, n_components);

            for(auto i=0; i<n_variables; ++i)
                for(auto j=0u; j<n_components; ++j)
                    v.block(i)(j) = pow(10,i+1) + j;
            return v;
        }

        void check_dealII_vector(const dealii::BlockVector<double>& v)
        {
            for(auto i=0; i<n_variables; ++i)
                for(auto j=0u; j<n_components; ++j)
                    EXPECT_DOUBLE_EQ(v.block(i)(j), pow(10,i+1) + j);
        }

        void check_Spacy_vector(const Vector& v)
        {
            auto v_dealII = dealII_test_vector();
            auto& v_ = cast_ref<ProductSpace::Vector>(v);
            const auto& creator_ = creator<ProductSpace::VectorCreator>(v.space());
            for(auto i=0u; i<n_variables; ++i)
                EXPECT_EQ(get(cast_ref<dealII::Vector>(v_.component(i))), v_dealII.block(creator_.inverseIdMap(i)));
        }

        Vector Spacy_test_vector()
        {
            auto v_dealII = dealII_test_vector();
            auto v = zero(V);
            auto& v_ = cast_ref<ProductSpace::Vector>(v);
            const auto& creator_ = creator<ProductSpace::VectorCreator>(v.space());
            for(auto i=0; i<n_variables; ++i)
                cast_ref<dealII::Vector>(v_.component(creator_.idMap(i))) = v_dealII.block(i);

            return v;
        }


        dealii::Triangulation<dim> triangulation;
        unsigned int n_components;
        VectorSpace V;
    };

    class dealII_CopyPrimalDualProductSpaceVector : public ::testing::Test
    {
    public:
        dealII_CopyPrimalDualProductSpaceVector()
        {
            dealii::GridGenerator::hyper_cube(triangulation, -1, 1);
            n_components = triangulation.n_vertices();
            V = dealII::makeHilbertSpace(triangulation, {0,1}, {2}, fe_order);
        }

        auto dealII_test_vector()
        {
            auto v = dealii::BlockVector<double>(n_variables, n_components);

            for(auto i=0; i<n_variables; ++i)
                for(auto j=0u; j<n_components; ++j)
                    v.block(i)(j) = pow(10,i+1) + j;
            return v;
        }

        void check_dealII_vector(const dealii::BlockVector<double>& v)
        {
            for(auto i=0; i<n_variables; ++i)
                for(auto j=0u; j<n_components; ++j)
                    EXPECT_DOUBLE_EQ(v.block(i)(j), pow(10,i+1) + j);
        }

        void check_Spacy_vector(const Vector& v)
        {
            auto v_dealII = dealII_test_vector();
            auto& v_ = cast_ref<ProductSpace::Vector>(v);
            auto& vp_ = cast_ref<ProductSpace::Vector>(v_.component(PRIMAL));
            auto& vd_ = cast_ref<ProductSpace::Vector>(v_.component(DUAL));
            const auto& primal_creator = creator<ProductSpace::VectorCreator>(vp_.space());
            const auto& dual_creator = creator<ProductSpace::VectorCreator>(vd_.space());

            EXPECT_EQ(get(cast_ref<dealII::Vector>(vp_.component(0))), v_dealII.block(primal_creator.inverseIdMap(0)));
            EXPECT_EQ(get(cast_ref<dealII::Vector>(vp_.component(1))), v_dealII.block(primal_creator.inverseIdMap(1)));
            EXPECT_EQ(get(cast_ref<dealII::Vector>(vd_.component(0))), v_dealII.block(dual_creator.inverseIdMap(0)));
        }

        Vector Spacy_test_vector()
        {
            auto v_dealII = dealII_test_vector();
            auto v = zero(V);
            auto& v_ = cast_ref<ProductSpace::Vector>(v);
            auto& vp_ = cast_ref<ProductSpace::Vector>(v_.component(PRIMAL));
            auto& vd_ = cast_ref<ProductSpace::Vector>(v_.component(DUAL));
            const auto& primal_creator = creator<ProductSpace::VectorCreator>(vp_.space());
            const auto& dual_creator = creator<ProductSpace::VectorCreator>(vd_.space());

            cast_ref<dealII::Vector>(vp_.component(primal_creator.idMap(0))) = v_dealII.block(0);
            cast_ref<dealII::Vector>(vp_.component(primal_creator.idMap(1))) = v_dealII.block(1);
            cast_ref<dealII::Vector>(vd_.component(dual_creator.idMap(0))) = v_dealII.block(2);

            return v;
        }


        dealii::Triangulation<dim> triangulation;
        unsigned int n_components;
        VectorSpace V;
    };
}


TEST_F(dealII_CopySingleVector, dealIIVectorToSpacyVector)
{
    auto v_dealii = dealII_test_vector();
    auto v_Spacy = zero(V);

    dealII::copy(v_dealii, v_Spacy);

    EXPECT_EQ(get(cast_ref<dealII::Vector>(v_Spacy)), v_dealii);
}

TEST_F(dealII_CopySingleVector, SpacyVectorTodealIIVector)
{
    auto v_Spacy = Spacy_test_vector();
    dealii::Vector<double> v_dealii(n_components);

    dealII::copy(v_Spacy, v_dealii);
    EXPECT_EQ(v_dealii, get(cast_ref<dealII::Vector>(v_Spacy)));
}


TEST_F(dealII_CopyProductSpaceVector, dealIIVectorToSpacyVector)
{
    auto v_dealii = dealII_test_vector();
    auto v_Spacy = zero(V);

    dealII::copy(v_dealii, v_Spacy);
    check_Spacy_vector(v_Spacy);
}

TEST_F(dealII_CopyProductSpaceVector, SpacyVectorTodealIIVector)
{
    auto v_Spacy = Spacy_test_vector();
    dealii::BlockVector<double> v_dealii(n_variables, n_components);

    dealII::copy(v_Spacy, v_dealii);
    check_dealII_vector(v_dealii);
}


TEST_F(dealII_CopyPermutedProductSpaceVector, dealIIVectorToSpacyVector)
{
    auto v_dealii = dealII_test_vector();
    auto v_Spacy = zero(V);

    dealII::copy(v_dealii, v_Spacy);
    check_Spacy_vector(v_Spacy);
}

TEST_F(dealII_CopyPermutedProductSpaceVector, SpacyVectorTodealIIVector)
{
    auto v_Spacy = Spacy_test_vector();
    dealii::BlockVector<double> v_dealii(n_variables, n_components);

    dealII::copy(v_Spacy, v_dealii);
    check_dealII_vector(v_dealii);
}


TEST_F(dealII_CopyPrimalDualProductSpaceVector, dealIIVectorToSpacyVector)
{
    auto v_dealii = dealII_test_vector();
    auto v_Spacy = zero(V);

    dealII::copy(v_dealii, v_Spacy);
    check_Spacy_vector(v_Spacy);
}

TEST_F(dealII_CopyPrimalDualProductSpaceVector, SpacyVectorTodealIIVector)
{
    auto v_Spacy = Spacy_test_vector();
    dealii::BlockVector<double> v_dealii(n_variables, n_components);

    dealII::copy(v_Spacy, v_dealii);
    check_dealII_vector(v_dealii);
}
