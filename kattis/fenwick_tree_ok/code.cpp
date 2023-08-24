
#include <iostream>
#include <functional>
#include <cstdio>
#include <vector>
using namespace std;
using vvi = vector<vector<long>>;

namespace ops {
    using op_t = std::function<long(long, long)>;
    constexpr auto sum = [](long a, long b){return a + b;};
    constexpr auto sub = [](long a, long b){return a - b;};
    constexpr auto xorr = [](long a, long b){return a xor b;};
}

/*
Benjamin Priour - Fenwick Tree implementation
Each operations is in O(log n) time complexity.

Tree in itself is stored using (N + 1) elements.
*/
class FenwickTree {
    public:
        using elem_t = long long;
        /**
         * @brief Construct a new Fenwick Tree object
         * 
         * @param size Size of the tree, left blank in wait for later updates.
         * @param op binary operation returning an integer that is used to compute the ranges.
         *  Consider the tree's element will be the left hand side argument.
         */
        FenwickTree(size_t size, ops::op_t const& op = ops::sum)
        : m_tree(size+1, 0),
        m_op {op}
        {}

        // This constructor creates Fenwick Tree in O(n)
        // O(N) is for the copy of the given array ARRAY
        // then O(N) is used for the construction  
        FenwickTree(std::vector<elem_t> array, ops::op_t const& op = ops::sum)
            : m_tree{array},
            m_op {op}
        {
            for (size_t i =1; i < m_tree.size(); i++) {
                auto parent {i + (i & -i)};
                if (parent < m_tree.size())
                    m_tree[parent] = m_op(m_tree[parent], m_tree[i]);
            }
        }

        // delta is directly provided
        void update(size_t idx, elem_t delta) {
            // implementation detail, requested indices are 0-based,
            // while in the fenwick tree indices are 1-based. 
            for (size_t i = idx+1; i < m_tree.size(); i += i & -i) {
                // m_tree[i] = m_op(m_tree[i], delta); GENERAL CASE AFTER KATTIS
                m_tree[i] += delta; // KATTIS TUNED
            }
        }
        // General case, a routine to compute the delta has to be provided.
        // The 'old' value of the tree at index IDX is expected to be right hand side
        // argument of DELTA_OP.
        // e.g. delta might be sub(value, old) = value - old;
        void update(size_t idx, elem_t value, ops::op_t const& delta_op) {
            auto old {m_tree[idx+1]};
            elem_t delta {delta_op(value, old)};
            for (size_t i = idx+1; i < m_tree.size(); i += i & -i) {
                m_tree[i] = m_op(m_tree[i], delta);
            }
        }

        // Return computed range [0, IDX_END[
        elem_t range(size_t idx_end) const {
            // go up the tree
            elem_t result {0}; // wasn't passing cuz integer overflow here, result was an int only.
            // implementation detail, requested indices are 0-based,
            // while in the fenwick tree indices are 1-based. 
            for (size_t i = (idx_end +1)-1; i > 0; i -= i & -i)
                // result = m_op(result, m_tree[i]);   GENERAL CASE AFTER KATTIS
                result += m_tree[i]; // KATTIS tuned
            return result;
        } 

    private:
        std::vector<elem_t> m_tree;
        ops::op_t m_op;
};

int main(){
    size_t N, Q;
    ios_base::sync_with_stdio(false); // disable flushing after each newline
    cin.tie(NULL);
    cout.tie(NULL);
    cin >> N >> Q; 
    FenwickTree fenwick {N};

    for (size_t i = 0; i < Q; i++) {
        char op;
        size_t idx;
        cin >> op >> idx;
        switch (op) {
            case '+':
                long long delta;
                cin >> delta;
                fenwick.update(idx, delta);
                break;
            case '?':
                cout << fenwick.range(idx) << '\n';
                break;
            default: 
                exit(5);
                break;
        }
    }

    cout << flush;

    return 0;
}
