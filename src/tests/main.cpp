#include <iostream>
#include <vector>
#include <algorithm>

#include <mdlutils/typedefs.hpp>
#include <mdlutils/accessor.hpp>
#include <mdlutils/exceptions.hpp>

#include <mdlutils/algorithms.hpp>
#include <mdlutils/timeit.hpp>

#include <mdlutils/types/sorted_list.hpp>
#include <mdlutils/types/const_vector.hpp>

#include <mdlutils/multithreading/thread_pool.hpp>

std::vector<int> topofn_test_data;

void time_topofn_set()
{
    auto k = mdl::top_of_n_set(topofn_test_data.begin(), topofn_test_data.end(), 100, std::greater<int>());
    k.clear();
}

void time_topofn_list()
{
    auto k = mdl::top_of_n_list(topofn_test_data.begin(), topofn_test_data.end(), 100, std::greater<int>());
    k.clear();
}

void time_topofn_vector()
{
    auto k = mdl::top_of_n_vector(topofn_test_data.begin(), topofn_test_data.end(), 100, std::greater<int>());
    k.clear();
}

void time_sorted_list()
{
    mdl::sorted_list<int> test;
    for (int i : topofn_test_data)
        test.insert(i);
    test.clear();
}

void time_sorted_set()
{
    std::set<int> test;
    for (int i : topofn_test_data)
        test.insert(i);
    test.clear();
}

void time_sorted_vect()
{
    std::vector<int> test(topofn_test_data);
    std::sort(test.begin(), test.end());
    test.clear();
}

int main()
{
    {
        mdl::sorted_list<int> test;
        auto seeder = mdl::range<int>(7000);
        for (int i : seeder)
            test.insert(i);
        for (int i : test) std::cout << " " << i;
        std::cout << std::endl;
    }
    {
        mdl::sorted_list<int, std::greater<int>> test;
        auto seeder = mdl::range<int>(7000);
        for (int i : seeder)
            test.insert(i);
        for (int i : test) std::cout << " " << i;
        std::cout << std::endl;
    }
    {
        std::vector<int> test = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 10, 0, 12, 11, 0, 13, 15};
        auto tops = mdl::top_of_n_set(test.begin(), test.end(), 5, std::greater<int>());
        for (int i : tops) std::cout << " " << i;
        std::cout << std::endl;
    }
    {
        std::vector<int> test = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 10, 0, 12, 11, 0, 13, 15};
        auto tops = mdl::top_of_n_vector(test.begin(), test.end(), 5, std::greater<int>());
        for (int i : tops) std::cout << " " << i;
        std::cout << std::endl;
    }
    {
        std::vector<int> test = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 10, 0, 12, 11, 0, 13, 15};
        auto tops = mdl::top_of_n_list(test.begin(), test.end(), 5, std::greater<int>());
        for (int i : tops) std::cout << " " << i;
        std::cout << std::endl;
    }
    {
        auto seeder = mdl::range<int>(7000);
        topofn_test_data = std::vector<int>(seeder.begin(), seeder.end());
        std::cout << "\tOrdered data set:" << std::endl;
        for (int i : topofn_test_data) std::cout << " " << i;
        std::cout << std::endl;
    }
    {
        std::cout << "Top of N : sets: ";
        mdl::timeitv(time_topofn_set);
        std::cout << "           list: ";
        mdl::timeitv(time_topofn_list);
        std::cout << "           vect: ";
        mdl::timeitv(time_topofn_vector);
    }
    {
        std::cout << "Top of N : sets: ";
        mdl::timeitv(time_topofn_set, 500);
        std::cout << "           list: ";
        mdl::timeitv(time_topofn_list, 500);
        std::cout << "           vect: ";
        mdl::timeitv(time_topofn_vector, 500);
    }
    {
        std::cout << "Sorted   : sets: ";
        mdl::timeitv(time_sorted_set);
        std::cout << "           list: ";
        mdl::timeitv(time_sorted_list);
        std::cout << "           vect: ";
        mdl::timeitv(time_sorted_vect);
    }
    {
        std::cout << "Sorted   : sets: ";
        mdl::timeitv(time_sorted_set, 10);
        std::cout << "           list: ";
        mdl::timeitv(time_sorted_list, 10);
        std::cout << "           vect: ";
        mdl::timeitv(time_sorted_vect, 10);
    }
    {
        auto seeder = mdl::range<int>(7000);
        topofn_test_data = std::vector<int>(seeder.begin(), seeder.end());
        random_shuffle(topofn_test_data.begin(), topofn_test_data.end());
        std::cout << "\tShuffled data set:" << std::endl;
        for (int i : topofn_test_data) std::cout << " " << i;
        std::cout << std::endl;
    }
    {
        std::cout << "Top of N : sets: ";
        mdl::timeitv(time_topofn_set);
        std::cout << "           list: ";
        mdl::timeitv(time_topofn_list);
        std::cout << "           vect: ";
        mdl::timeitv(time_topofn_vector);
    }
    {
        std::cout << "Top of N : sets: ";
        mdl::timeitv(time_topofn_set, 500);
        std::cout << "           list: ";
        mdl::timeitv(time_topofn_list, 500);
        std::cout << "           vect: ";
        mdl::timeitv(time_topofn_vector, 500);
    }
    {
        std::cout << "Sorted   : sets: ";
        mdl::timeitv(time_sorted_set);
        std::cout << "           list: ";
        mdl::timeitv(time_sorted_list);
        std::cout << "           vect: ";
        mdl::timeitv(time_sorted_vect);
    }
    {
        std::cout << "Sorted   : sets: ";
        mdl::timeitv(time_sorted_set, 10);
        std::cout << "           list: ";
        mdl::timeitv(time_sorted_list, 10);
        std::cout << "           vect: ";
        mdl::timeitv(time_sorted_vect, 10);
    }
    {
        auto seeder = mdl::range<int>(7000);
        topofn_test_data = std::vector<int>(seeder.begin(), seeder.end());
        std::reverse(topofn_test_data.begin(), topofn_test_data.end());
        std::cout << "\tOrdered data set:" << std::endl;
        for (int i : topofn_test_data) std::cout << " " << i;
        std::cout << std::endl;
    }
    {
        std::cout << "Top of N : sets: ";
        mdl::timeitv(time_topofn_set);
        std::cout << "           list: ";
        mdl::timeitv(time_topofn_list);
        std::cout << "           vect: ";
        mdl::timeitv(time_topofn_vector);
    }
    {
        std::cout << "Top of N : sets: ";
        mdl::timeitv(time_topofn_set, 500);
        std::cout << "           list: ";
        mdl::timeitv(time_topofn_list, 500);
        std::cout << "           vect: ";
        mdl::timeitv(time_topofn_vector, 500);
    }
    {
        std::cout << "Sorted   : sets: ";
        mdl::timeitv(time_sorted_set);
        std::cout << "           list: ";
        mdl::timeitv(time_sorted_list);
        std::cout << "           vect: ";
        mdl::timeitv(time_sorted_vect);
    }
    {
        std::cout << "Sorted   : sets: ";
        mdl::timeitv(time_sorted_set, 10);
        std::cout << "           list: ";
        mdl::timeitv(time_sorted_list, 10);
        std::cout << "           vect: ";
        mdl::timeitv(time_sorted_vect, 10);
    }
    {
        std::cout << mdl::stringify(std::cout) << std::endl;
    }
    try
    {
        mdl_throw(mdl::make_ia_exception, "lambda", []()
            { });
    } catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
    try
    {
        mdl_throw(mdl::make_ia_exception, "ptr", &(std::cout));
    } catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
    try
    {
        mdl_throw(mdl::make_ia_exception, "cout", std::cout);
    } catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
    try
    {
        mdl_throw(mdl::make_ia_exception, "main", main);
    } catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
    try
    {
        mdl_throw(mdl::make_ia_exception, "main", &main);
    } catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
    {
        mdl::thread_pool workers;
        std::cout << "Created a thread_pool with " << workers.workers << " workers" << std::endl;
    }
    {
        mdl::const_vector<int> c(10);
        std::vector<int> v(10);
        std::cout << sizeof(c) << std::endl;
        std::cout << sizeof(v) << std::endl;
    }
    return 0;
}
