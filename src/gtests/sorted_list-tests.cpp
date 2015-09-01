//
// Created by marandil on 01.09.15.
//

#include <gtest/gtest.h>

#include <mdlutils/types/sorted_list.hpp>
#include <mdlutils/exceptions/base_exception.hpp>

#include <algorithm>

class SortedListTest : public ::testing::Test
{
protected:
    using string=std::string;
    int size = 10000;

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

TEST_F(SortedListTest, SortedListIntEasyInsertion)
{
    test_easy_insertion<int>(intdata, intsort, intlist);
    test_easy_insertion<int>(intdata, intsortinv, intlistinv);
    test_easy_insertion<int>(intdata, intsortbizzare, intlistbizzare);
}

TEST_F(SortedListTest, SortedListFloatEasyInsertion)
{
    test_easy_insertion<float>(floatdata, floatsort, floatlist);
    test_easy_insertion<float>(floatdata, floatsortinv, floatlistinv);
    test_easy_insertion<float>(floatdata, floatsortbizzare, floatlistbizzare);
}

TEST_F(SortedListTest, SortedListStringEasyInsertion)
{
    test_easy_insertion<string>(stringdata, stringsort, stringlist);
    test_easy_insertion<string>(stringdata, stringsortinv, stringlistinv);
    test_easy_insertion<string>(stringdata, stringsortbizzare, stringlistbizzare);
}

TEST_F(SortedListTest, SortedListIntHintInsertion)
{
    test_hint_insertion<int>(intdata, intsort, intlist);
    test_hint_insertion<int>(intdata, intsortinv, intlistinv);
    test_hint_insertion<int>(intdata, intsortbizzare, intlistbizzare);
}

TEST_F(SortedListTest, SortedListFloatHintInsertion)
{
    test_hint_insertion<float>(floatdata, floatsort, floatlist);
    test_hint_insertion<float>(floatdata, floatsortinv, floatlistinv);
    test_hint_insertion<float>(floatdata, floatsortbizzare, floatlistbizzare);
}

TEST_F(SortedListTest, SortedListStringHintInsertion)
{
    test_hint_insertion<string>(stringdata, stringsort, stringlist);
    test_hint_insertion<string>(stringdata, stringsortinv, stringlistinv);
    test_hint_insertion<string>(stringdata, stringsortbizzare, stringlistbizzare);
}

TEST_F(SortedListTest, SortedListIntRangeInsertion)
{
    test_range_insertion<int>(intdata, intsort, intlist);
    test_range_insertion<int>(intdata, intsortinv, intlistinv);
    test_range_insertion<int>(intdata, intsortbizzare, intlistbizzare);
}

TEST_F(SortedListTest, SortedListFloatRangeInsertion)
{
    test_range_insertion<float>(floatdata, floatsort, floatlist);
    test_range_insertion<float>(floatdata, floatsortinv, floatlistinv);
    test_range_insertion<float>(floatdata, floatsortbizzare, floatlistbizzare);
}

TEST_F(SortedListTest, SortedListStringRangeInsertion)
{
    test_range_insertion<string>(stringdata, stringsort, stringlist);
    test_range_insertion<string>(stringdata, stringsortinv, stringlistinv);
    test_range_insertion<string>(stringdata, stringsortbizzare, stringlistbizzare);
}

