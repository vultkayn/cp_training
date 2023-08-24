/**
 * @author benpr438
 * @brief build a disjoint Set structure, also known as a Union-Find structure.
 * Supports efficiently two operations, union, and find.
 * Union merge two sets, while find(x) will returns the set x is part of.
 *  
 */
#pragma once
#include <vector>


class disjointSet
{

private:
    inline long long &parent(long long elem)
    {
        return m_tree[elem - m_minElem];
    }
    inline long long parent(long long elem) const
    {
        return m_tree[elem - m_minElem];
    }


    // When building a library it's sometimes good to put away the actual implementation away
    // This function is the actual implementation of the "find" operation, without bothering
    // with edge cases checks. Such checks are instead done in the user-facing interface.
    // I'm not using the recursive version of this algorithm but the iterative one to improve
    // performance. The recursive relationship is not difficult enough that it justifies
    // not transforming it to an iterative version, and loops are always better as we save a
    // few machine instructions (frame pushing and popping), and a loop is easier to optimize
    // than a recursive call for the compiler.  
    long long find_impl(long long elem);

public:
    // minElem and maxElem are both included in the struct.
    /**
     * @brief Construct a new disjoint Set object
     *
     * Differently to the expected solution in Kattis, where the set can only contains
     * positive integers, I'm using minElem and maxElem as my bounds. Both can be negative,
     * as long as maxElem >= minElem
     * I am then ensuring in each methods that we do not cross these boundaries.
     *
     * @param minElem The lesser bound of any elements than can be included in a set.
     * @param maxElem The upper bound of any elements than can be included in a set.
     */
    disjointSet(long long minElem, long long maxElem);
    /**
     * @brief Find the set ELEM is contained in, or minElem -1 otherwise.
     *
     * @param elem ELEM to look for
     * @return long long minElem -1 if not found, or the unique representative of the set ELEM is in.
     */
    long long find(long long elem);

    /**
     * @brief Delete Set1 and Set2 but add the set resulting of their merging.
     *  Resulting set's representative is equals to the deepest element set's representative.
     *  Basically 2*O(log n) in amortized time complexity.
     *
     * @param ofSet1 An element within the set Set1
     * @param ofSet2 An element within the set Set2
     */
    void doUnion(long long ofSet1, long long ofSet2) ;
    bool equals(long long elem1, long long elem2);

    long long size(long long ofSet1);

private:
    const long long m_minElem;
    const long long m_maxElem;
    mutable std::vector<long long> m_tree;
    /* Store sizes as a standalone array
     * as they are only ever accessed in doUnion.
     * This should reduce the amount of cache misses in "equals" and "find"
     * as less memory will need to be loaded into the cache.
     * Thus, this should actually be better than having a vector of "struct Node"
     */
    std::vector<std::size_t> m_sizes;
};
