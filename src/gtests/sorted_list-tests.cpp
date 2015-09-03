//
// Created by Marandil on 01.09.15.
//

#include <algorithm>

#include <gtest/gtest.h>

#include <mdlutils/types/sorted_list.hpp>
#include <mdlutils/exceptions/base_exception.hpp>


class SortedListTest : public ::testing::Test
{
protected:
    using string=std::string;
    size_t size = 10;

    std::vector<int> intdata;
    std::vector<int> intsort;
    std::vector<int> intsortinv;
    std::vector<int> intsortbizzare;
    mdl::sorted_list<int> intlist;
    mdl::sorted_list<int, std::greater<int>> intlistinv;
    mdl::sorted_list<int, std::greater_equal<int>> intlistbizzare;

    std::vector<float> floatdata;
    std::vector<float> floatsort;
    std::vector<float> floatsortinv;
    std::vector<float> floatsortbizzare;
    mdl::sorted_list<float> floatlist;
    mdl::sorted_list<float, std::greater<float>> floatlistinv;
    mdl::sorted_list<float, std::greater_equal<float>> floatlistbizzare;

    std::vector<string> stringdata;
    std::vector<string> stringsort;
    std::vector<string> stringsortinv;
    std::vector<string> stringsortbizzare;
    mdl::sorted_list<string> stringlist;
    mdl::sorted_list<string, std::greater<string>> stringlistinv;
    mdl::sorted_list<string, std::greater_equal<string>> stringlistbizzare;

    SortedListTest()
    {
        intdata.reserve(size);
        for (int i = 0; i < size; ++i)
            intdata.push_back(i);
        intsort = intdata;

        floatdata.reserve(size);
        for (int i = 0; i < size; ++i)
            floatdata.push_back(1.0f / (i + 1));
        floatsort = floatdata;
        std::reverse(floatsort.begin(), floatsort.end());

        stringdata.reserve(size);
        for (int i = 0; i < size; ++i)
            stringdata.push_back("Alice " + std::to_string(i));
        stringsort = stringdata;
        std::sort(stringsort.begin(), stringsort.end());

        std::random_shuffle(intdata.begin(), intdata.end());
        std::random_shuffle(floatdata.begin(), floatdata.end());
        std::random_shuffle(stringdata.begin(), stringdata.end());

        intsortinv = intsort;
        intsortbizzare = intsort;

        std::sort(intsortinv.begin(), intsortinv.end(), std::greater<int>());
        std::sort(intsortbizzare.begin(), intsortbizzare.end(), std::greater_equal<int>());

        floatsortinv = floatsort;
        floatsortbizzare = floatsort;

        std::sort(floatsortinv.begin(), floatsortinv.end(), std::greater<float>());
        std::sort(floatsortbizzare.begin(), floatsortbizzare.end(), std::greater_equal<float>());

        stringsortinv = stringsort;
        stringsortbizzare = stringsort;

        std::sort(stringsortinv.begin(), stringsortinv.end(), std::greater<string>());
        std::sort(stringsortbizzare.begin(), stringsortbizzare.end(), std::greater_equal<string>());
    }
};

template<typename T, typename U, typename V>
void test_easy_insertion(U &data, U &sort, V &list)
{
    for (T i : data)
        list.insert(i);

    auto it1 = sort.begin(), end1 = sort.end();
    auto it2 = list.begin(), end2 = list.end();
    for (; it1 != end1 && it2 != end2; ++it1, ++it2)
            EXPECT_EQ(*it1, *it2);

    EXPECT_EQ(it1, end1);
    EXPECT_EQ(it2, end2);
}


template<typename T, typename U, typename V>
void test_hint_insertion(U &data, U &sort, V &list)
{
    auto hint = list.begin();
    for (T i : data)
        hint = list.insert(hint, i);

    auto it1 = sort.begin(), end1 = sort.end();
    auto it2 = list.begin(), end2 = list.end();
    for (; it1 != end1 && it2 != end2; ++it1, ++it2)
            EXPECT_EQ(*it1, *it2);

    EXPECT_EQ(it1, end1);
    EXPECT_EQ(it2, end2);
}

template<typename T, typename U, typename V>
void test_range_insertion(U &data, U &sort, V &list)
{
    list.insert(data.begin(), data.end());

    auto it1 = sort.begin(), end1 = sort.end();
    auto it2 = list.begin(), end2 = list.end();
    for (; it1 != end1 && it2 != end2; ++it1, ++it2)
            EXPECT_EQ(*it1, *it2);

    EXPECT_EQ(it1, end1);
    EXPECT_EQ(it2, end2);
}

template<typename T, typename U, typename V>
void test_easy_erase(U &data, U &sort, V &list)
{
    // fill the list
    list.insert(data.begin(), data.end());
    size_t s = data.size();
    EXPECT_EQ(s, list.size());

    for (T i : data)
    {
        list.erase(i);
        s--;
        EXPECT_EQ(s, list.size());
    }
    auto begin = list.begin(), end = list.end();

    EXPECT_EQ(begin, end);
}

template<size_t n, typename T, typename U, typename V>
void test_multi_erase(U &data, U &sort, V &list)
{
    // fill the list n times
    for (int i = 0; i < n; ++i)
        list.insert(data.begin(), data.end());
    size_t s = data.size() * n;
    EXPECT_EQ(s, list.size());

    for (T i : data)
    {
        EXPECT_EQ(n, list.count(i));
        list.erase(i);
        s -= n;
        EXPECT_EQ(s, list.size());
        EXPECT_EQ(0, list.count(i));
    }
    auto begin = list.begin(), end = list.end();

    EXPECT_EQ(begin, end);
}

template<size_t n, typename T, typename U, typename V>
void test_bounds_erase(U &data, U &sort, V &list)
{
    // fill the list n times
    for (int i = 0; i < n; ++i)
        list.insert(data.begin(), data.end());
    size_t s = data.size() * n;
    EXPECT_EQ(s, list.size());

    for (T i : data)
    {
        EXPECT_EQ(n, list.count(i));
        auto lb = list.lower_bound(i),
                ub = list.upper_bound(i);
        list.erase(lb, ub);
        s -= n;
        EXPECT_EQ(s, list.size());
        EXPECT_EQ(0, list.count(i));
    }
    auto begin = list.begin(), end = list.end();

    EXPECT_EQ(begin, end);
}

template<size_t n, typename T, typename U, typename V>
void test_bounds2_erase(U &data, U &sort, V &list)
{
    // fill the list n times
    for (int i = 0; i < n; ++i)
        list.insert(data.begin(), data.end());
    size_t s = data.size() * n;
    EXPECT_EQ(s, list.size());

    for (T i : data)
    {
        EXPECT_EQ(n, list.count(i));
        auto pair = list.equal_range(i);
        list.erase(pair.first, pair.second);
        s -= n;
        EXPECT_EQ(s, list.size());
        EXPECT_EQ(0, list.count(i));
    }
    auto begin = list.begin(), end = list.end();

    EXPECT_EQ(begin, end);
}

template<typename T, typename U, typename V>
void test_easy_find(U &data, U &sort, V &list)
{
    // fill the list
    list.insert(data.begin(), data.end());

    for (T i : data)
    {
        auto it = list.find(i);
        EXPECT_EQ(i, *it);
    }
}

template<typename T, typename U, typename V>
void test_easy_finderase(U &data, U &sort, V &list)
{
    // fill the list
    list.insert(data.begin(), data.end());
    size_t s = data.size();
    EXPECT_EQ(s, list.size());

    for (T i : data)
    {
        auto it = list.find(i);
        EXPECT_EQ(i, *it);
        list.erase(it);
        s--;
        EXPECT_EQ(s, list.size());
    }
    auto begin = list.begin(), end = list.end();

    EXPECT_EQ(begin, end);
}

template<typename T, typename U, typename V>
void test_range_erase(U &data, U &sort, V &list)
{
    list.insert(data.begin(), data.end());
    size_t s = data.size();
    EXPECT_EQ(s, list.size());

    auto comp = list.key_comp();

    T rvb = data.front();
    T rve = data.back();
    if (!comp(rvb, rve))
        std::swap(rvb, rve);

    auto r_begin = list.find(rvb);
    auto r_end = list.find(rve);

    // erase the range
    list.erase(r_begin, r_end);
    auto end = list.end();

    for (T i : data)
    {
        auto it = list.find(i);
        if (comp(i, rvb))
        {
            EXPECT_EQ(i, *it);
            EXPECT_EQ(1, list.count(i));
        }
        else if (comp(i, rve))
        {
            EXPECT_EQ(it, end);
            EXPECT_EQ(0, list.count(i));
        }
        else
        {
            EXPECT_EQ(i, *it);
            EXPECT_EQ(1, list.count(i));
        }
    }
}

TEST_F(SortedListTest, IntEasyInsertion)
{
    test_easy_insertion<int>(intdata, intsort, intlist);
    test_easy_insertion<int>(intdata, intsortinv, intlistinv);
    test_easy_insertion<int>(intdata, intsortbizzare, intlistbizzare);
}

TEST_F(SortedListTest, FloatEasyInsertion)
{
    test_easy_insertion<float>(floatdata, floatsort, floatlist);
    test_easy_insertion<float>(floatdata, floatsortinv, floatlistinv);
    test_easy_insertion<float>(floatdata, floatsortbizzare, floatlistbizzare);
}

TEST_F(SortedListTest, StringEasyInsertion)
{
    test_easy_insertion<string>(stringdata, stringsort, stringlist);
    test_easy_insertion<string>(stringdata, stringsortinv, stringlistinv);
    test_easy_insertion<string>(stringdata, stringsortbizzare, stringlistbizzare);
}

TEST_F(SortedListTest, IntHintInsertion)
{
    test_hint_insertion<int>(intdata, intsort, intlist);
    test_hint_insertion<int>(intdata, intsortinv, intlistinv);
    test_hint_insertion<int>(intdata, intsortbizzare, intlistbizzare);
}

TEST_F(SortedListTest, FloatHintInsertion)
{
    test_hint_insertion<float>(floatdata, floatsort, floatlist);
    test_hint_insertion<float>(floatdata, floatsortinv, floatlistinv);
    test_hint_insertion<float>(floatdata, floatsortbizzare, floatlistbizzare);
}

TEST_F(SortedListTest, StringHintInsertion)
{
    test_hint_insertion<string>(stringdata, stringsort, stringlist);
    test_hint_insertion<string>(stringdata, stringsortinv, stringlistinv);
    test_hint_insertion<string>(stringdata, stringsortbizzare, stringlistbizzare);
}

TEST_F(SortedListTest, IntRangeInsertion)
{
    test_range_insertion<int>(intdata, intsort, intlist);
    test_range_insertion<int>(intdata, intsortinv, intlistinv);
    test_range_insertion<int>(intdata, intsortbizzare, intlistbizzare);
}

TEST_F(SortedListTest, FloatRangeInsertion)
{
    test_range_insertion<float>(floatdata, floatsort, floatlist);
    test_range_insertion<float>(floatdata, floatsortinv, floatlistinv);
    test_range_insertion<float>(floatdata, floatsortbizzare, floatlistbizzare);
}

TEST_F(SortedListTest, StringRangeInsertion)
{
    test_range_insertion<string>(stringdata, stringsort, stringlist);
    test_range_insertion<string>(stringdata, stringsortinv, stringlistinv);
    test_range_insertion<string>(stringdata, stringsortbizzare, stringlistbizzare);
}

TEST_F(SortedListTest, IntEasyErase)
{
    test_easy_erase<int>(intdata, intsort, intlist);
}

TEST_F(SortedListTest, IntMultiErase)
{
    test_multi_erase<1, int>(intdata, intsort, intlist);
    test_multi_erase<10, int>(intdata, intsort, intlist);
    test_multi_erase<100, int>(intdata, intsort, intlist);
    test_multi_erase<1000, int>(intdata, intsort, intlist);
}

TEST_F(SortedListTest, IntBoundsErase)
{
    test_bounds_erase<1, int>(intdata, intsort, intlist);
    test_bounds_erase<10, int>(intdata, intsort, intlist);
    test_bounds_erase<100, int>(intdata, intsort, intlist);
    test_bounds_erase<1000, int>(intdata, intsort, intlist);
}

TEST_F(SortedListTest, IntBounds2Erase)
{
    test_bounds2_erase<1, int>(intdata, intsort, intlist);
    test_bounds2_erase<10, int>(intdata, intsort, intlist);
    test_bounds2_erase<100, int>(intdata, intsort, intlist);
    test_bounds2_erase<1000, int>(intdata, intsort, intlist);
}

TEST_F(SortedListTest, IntEasyFind)
{
    test_easy_find<int>(intdata, intsort, intlist);
}

TEST_F(SortedListTest, IntEasyFindErase)
{
    test_easy_finderase<int>(intdata, intsort, intlist);
}

TEST_F(SortedListTest, IntRangeErase)
{
    test_range_erase<int>(intdata, intsort, intlist);
}

TEST_F(SortedListTest, FloatEasyErase)
{
    test_easy_erase<float>(floatdata, floatsort, floatlist);
}

TEST_F(SortedListTest, FloatMultiErase)
{
    test_multi_erase<1, float>(floatdata, floatsort, floatlist);
    test_multi_erase<10, float>(floatdata, floatsort, floatlist);
    test_multi_erase<100, float>(floatdata, floatsort, floatlist);
    test_multi_erase<1000, float>(floatdata, floatsort, floatlist);
}

TEST_F(SortedListTest, FloatBoundsErase)
{
    test_bounds_erase<1, float>(floatdata, floatsort, floatlist);
    test_bounds_erase<10, float>(floatdata, floatsort, floatlist);
    test_bounds_erase<100, float>(floatdata, floatsort, floatlist);
    test_bounds_erase<1000, float>(floatdata, floatsort, floatlist);
}

TEST_F(SortedListTest, FloatBounds2Erase)
{
    test_bounds2_erase<1, float>(floatdata, floatsort, floatlist);
    test_bounds2_erase<10, float>(floatdata, floatsort, floatlist);
    test_bounds2_erase<100, float>(floatdata, floatsort, floatlist);
    test_bounds2_erase<1000, float>(floatdata, floatsort, floatlist);
}

TEST_F(SortedListTest, FloatEasyFind)
{
    test_easy_find<float>(floatdata, floatsort, floatlist);
}

TEST_F(SortedListTest, FloatEasyFindErase)
{
    test_easy_finderase<float>(floatdata, floatsort, floatlist);
}

TEST_F(SortedListTest, FloatRangeErase)
{
    test_range_erase<float>(floatdata, floatsort, floatlist);
}

TEST_F(SortedListTest, StringEasyErase)
{
    test_easy_erase<string>(stringdata, stringsort, stringlist);
}

TEST_F(SortedListTest, StringMultiErase)
{
    test_multi_erase<1, string>(stringdata, stringsort, stringlist);
    test_multi_erase<10, string>(stringdata, stringsort, stringlist);
    test_multi_erase<100, string>(stringdata, stringsort, stringlist);
    test_multi_erase<1000, string>(stringdata, stringsort, stringlist);
}

TEST_F(SortedListTest, StringBoundsErase)
{
    test_bounds_erase<1, string>(stringdata, stringsort, stringlist);
    test_bounds_erase<10, string>(stringdata, stringsort, stringlist);
    test_bounds_erase<100, string>(stringdata, stringsort, stringlist);
    test_bounds_erase<1000, string>(stringdata, stringsort, stringlist);
}

TEST_F(SortedListTest, StringBounds2Erase)
{
    test_bounds2_erase<1, string>(stringdata, stringsort, stringlist);
    test_bounds2_erase<10, string>(stringdata, stringsort, stringlist);
    test_bounds2_erase<100, string>(stringdata, stringsort, stringlist);
    test_bounds2_erase<1000, string>(stringdata, stringsort, stringlist);
}

TEST_F(SortedListTest, StringEasyFind)
{
    test_easy_find<string>(stringdata, stringsort, stringlist);
}

TEST_F(SortedListTest, StringEasyFindErase)
{
    test_easy_finderase<string>(stringdata, stringsort, stringlist);
}

TEST_F(SortedListTest, StringRangeErase)
{
    test_range_erase<string>(stringdata, stringsort, stringlist);
}


