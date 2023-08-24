/**
 * @author benpr438
 * @brief build a disjoint Set structure, also known as a Union-Find structure.
 * Supports efficiently two operations, union, and find.
 * Union merge two sets, while find(x) will returns the set x is part of.
 *  
 */

#include <iostream>
#include <vector>
#include <cassert>
#include <cstdio>
using namespace std;
using vvi = vector<vector<long>>;

/**
 * @brief Construct a new disjoint Set object, also known as a Union-Find structure.
 * Supports efficiently two operations, union, and find.
 * Union merge two sets, while find(x) will returns the set x is part of.
 * */
class disjointSet
{

private:
    inline int &parent(int elem)
    {
        return m_tree[elem - m_minElem];
    }
    inline int parent(int elem) const
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
    int find_impl(int elem)
    {
        auto repr = elem;
        // only representatives have their parent set to themselves.
        // thus loop until we find ELEM's representative.
        // Tarjan's solution one pass is better than my previous 2 passes
        // I was doing first loop to find the representative
        // then relooped once more to flatten the tree to fix the nodes' parent
        // to the representative.
        while (parent(repr) != repr)
        {

            repr = parent(repr);
        }
        while (parent(elem) != elem)
        {
            auto tmp = parent(elem);
            parent(elem) = repr;
            elem = tmp;
        }

        return repr;
    }

public:
    // minElem and maxElem are both included in the struct.
    /**
     * @brief Construct a new disjoint Set object, also known as a Union-Find structure.
     * Supports efficiently two operations, union, and find.
     * Union merge two sets, while find(x) will returns the set x is part of.
     *
     * Differently to the expected solution in Kattis, where the set can only contains
     * positive integers, I'm using minElem and maxElem as my bounds. Both can be negative,
     * as long as maxElem >= minElem
     * I am then ensuring in each methods that we do not cross these boundaries.
     *
     * @param minElem The lesser bound of any elements than can be included in a set.
     * @param maxElem The upper bound of any elements than can be included in a set.
     */
    disjointSet(int minElem, int maxElem)
        : m_minElem{minElem},
          m_maxElem{maxElem},
          m_tree(m_maxElem - m_minElem + 1),
          m_sizes(m_maxElem - m_minElem + 1)
    {
        assert(minElem <= maxElem);
        /* I could fill m_tree with -1 all over the vector
        as it probably would improve performance for Kattis,
        but I went with a more general version that support negative numbers,
        so I cannot do that.
        Instead I initiliaze it such as m_tree[x - m_minElem] = x;
        so that initially everyone is its own parent.
        */
        for (int x = minElem; x <= maxElem; x++)
        {
            m_tree[x - minElem] = x;
            m_sizes[x - minElem] = 1;
        }
    }
    /**
     * @brief Find the set ELEM is contained in, or minElem -1 otherwise.
     *
     * @param elem ELEM to look for
     * @return int minElem -1 if not found, or the unique representative of the set ELEM is in.
     */
    int find(int elem)
    {
        if (elem > m_maxElem)
            return m_minElem - 1;
        if (elem < m_minElem)
            return m_minElem - 1;

        return find_impl(elem);
    }

    /**
     * @brief Delete Set1 and Set2 but add the set resulting of their merging.
     *  Resulting set's representative is equals to the deepest element set's representative.
     *  Basically 2*O(log n) in amortized time complexity.
     *
     * @param ofSet1 An element within the set Set1
     * @param ofSet2 An element within the set Set2
     */
    void doUnion(int ofSet1, int ofSet2) //
    {
        // If any of the requested elements falls out of [minElem, maxElem] range,
        // or if they both are equals, then there is nothing to do.
        if (ofSet1 < m_minElem or m_maxElem < ofSet1 or ofSet2 < m_minElem or m_maxElem < ofSet2 or ofSet1 == ofSet2)
            return;
        // Basically doing "equals" method here
        ofSet1 = find_impl(ofSet1);
        ofSet2 = find_impl(ofSet2);
        if (ofSet1 == ofSet2)
            return;

        // flatten the load by fixing the smallest tree as a branch
        // and the biggest as root.
        if (m_sizes[ofSet1 - m_minElem] > m_sizes[ofSet2 - m_minElem])
        {
            parent(ofSet2) = ofSet1;
            m_sizes[ofSet1 - m_minElem] += m_sizes[ofSet2 - m_minElem];
        }
        else
        {
            parent(ofSet1) = ofSet2;
            m_sizes[ofSet2 - m_minElem] += m_sizes[ofSet1 - m_minElem];
        }
    }

    /**
     * @brief Returns TRUE iif elem1 are within the same set.
     *
     * @param elem1
     * @param elem2
     * @return true
     * @return false
     */
    bool equals(int elem1, int elem2)
    {
        /* function could be more concise,
        but doing as below we save one call to find if 'elem1'
        is not found */

        int repr1{find(elem1)};
        if (repr1 == m_minElem - 1)
            return false;
        return elem1 == elem2 or repr1 == find(elem2); // little optimization if elem1 == elem2, don't have to look for elem2
    }

private:
    const int m_minElem;
    const int m_maxElem;
    mutable vector<int> m_tree;
    /* Store sizes as a standalone array
     * as they are only ever accessed in doUnion.
     * This should reduce the amount of cache misses in "equals" and "find"
     * as less memory will need to be loaded into the cache.
     * Thus, this should actually be better than having a vector of "struct Node"
     */
    vector<unsigned> m_sizes;
};

/**
 * @brief Kattis specific function, that maps a 'char' query from stdin
 * to the corresponding action on the disjoint set.
 *
 * @param uf
 * @param yes
 * @param no
 */
void computeQuery(disjointSet &uf, char const *yes, char const *no)
{
    char op;
    int a, b;
    cin >> op >> a >> b;
    switch (op)
    {
    case '?':
        cout << (uf.equals(a, b) ? yes : no);
        break;
    case '=':
        uf.doUnion(a, b);
        break;
    default:
        assert(false && "unknown op");
        break;
    }
}

int main()
{
    /*
        Code within main is Kattis specific.
    */
    int N, Q;
    ios_base::sync_with_stdio(false);
    cout.tie(NULL);
    cin.tie(NULL);
    // std::setvbuf(stdout, nullptr, _IOFBF, BUFSIZ); // dammit buffering was still per line.
    cin >> N >> Q;
    disjointSet uf{0, N - 1};

    for (int i = 0; i < Q - 1; i++)
    {
        // flush once in a while
        // if (i % 1024 == 0)
        // {
        // computeQuery(uf, "yes", "no");
        // cout << endl;
        // }
        // else
        computeQuery(uf, "yes\n", "no\n");
    }
    computeQuery(uf, "yes", "no");

    cout << endl;

    return 0;
}
