#include <iostream>
#include <sstream>
#include <vector>
#include <bitset>
using namespace std;

#define abs(v) ((v) > 0 ? (v) : -(v))

// return true if clause is OK
bool check_clause(bitset<20> vars, bitset<40> clause, int idx)
{
    bitset<20> clause_vars = clause.to_ulong() & 0x00000FFFFF;
    // relevant = (relevant << (19 - idx)) >> (19 - idx);
    bitset<20> clause_values = (clause.to_ulong() & 0xFFFFF00000) >> 20; // technially and is useless here ...
    // any diverging value between clause and vars result in a 1.
    // any matching are set to 0
    bitset<20> _1 = vars ^ clause_values;
    // maks with relevant variables
    _1 &= clause_vars;
    // put any value not within [0..idx] to 0, their difference doesnt matter yet.
    bitset<20> _2 = (_1 << (19 - idx)) >> (19 - idx);
    // if number of difference within vars[0..idx] is equal to the number of variables within the clause
    // then we deduce all of them were different, thus no literal was set to its expected value.
    return _2.count() < clause_vars.count(); // clause is true if at least one of its literal was.
}

bool check_clause_simpler(vector<bool> const &vars, vector<int> const &clause)
{
    for (auto v : clause)
    {
        bool expected = v > 0;
        if (expected == vars[abs(v) - 1])
            return true;
    }
    return false;
}

bool check_clause2(bitset<20> vars, bitset<40> clause)
{
    bitset<20> clause_vars = clause.to_ulong() & 0x00000FFFFF;
    bitset<20> clause_values = (clause.to_ulong() & 0xFFFFF00000) >> 20;
    bitset<20> _1 = vars ^ clause_values;    // any different set at 1, any matching are 0
    _1 &= clause_vars;                       // only those within clause can stay at 1;
    return _1.count() < clause_vars.count(); // clause is true if at least one of its literal was not different.
}

// could mark already solved clause as solved in a bitset
// return true if a clause is already not doable
// i.e. all its relevant variables are already set, none of them to the mathing value
bool already_unsat(bitset<20> vars, vector<bitset<40>> const &clauses, int idx)
{

    // for each clause of clauses
    // _1 = clause[20..39] ^ vars[0..19] -> 1 everywhere different
    // _2 = _1 & clause[0..19] // only relevant variables of the clause and different stay at one.
    // if _2.count() == clause[0..19].count() then every variable was different -> unsat

    for (auto c : clauses)
    {
        if (check_clause(vars, c, idx) == false)
            return true;
    }
    return false;
}

/* bool already_sat (bitset<20> vars, vector<bitset<40>> const& clauses) {
    for (auto c: clauses) {
        if (check_clause2 (vars, c) == false)
            return false;
    }
    return true;
} */

/* bool solver_sat_r(bitset<20> vars, int current, vector<bitset<40>> const& clauses, int nvars) {
    if (current == nvars)
        return true;


    vars[current] = true;
    if (already_sat (vars, clauses)) return true;
    vars[current] = false;
    if (already_sat (vars, clauses)) return true;
    vars[current] = true;
    if (solver_sat_r (vars, current+1, clauses,  /* nvars))
        return true;

    vars[current] = false;
    return solver_sat_r (vars, current+1, clauses,  nvars);
}


bool solver_sat(bitset<20> vars, vector<bitset<40>>& clauses, int nvars)
{
    return solver_sat_r (vars, 0, clauses, nvars);
} */

bool already_sat(vector<bool> &vars, vector<vector<int>> const &clauses)
{
    for (auto c: clauses)
    {
        if (check_clause_simpler (vars, c) == false)
            return false;
    }
    return true;
}

bool solver_sat_r(vector<bool> &vars, int current, vector<vector<int>> const &clauses)
{
    if (current == vars.size())
        return false; // couldnt find SAT before

    vars[current] = true;
    if (already_sat (vars, clauses))
        return true;
    vars[current] = false;
    if (already_sat (vars, clauses))
        return true;
    vars[current] = true;
    if (solver_sat_r(vars, current + 1, clauses))
        return true;

    vars[current] = false;
    return solver_sat_r(vars, current + 1, clauses);
}

bool solver_sat(vector<bool> &vars, vector<vector<int>> const &clauses)
{
    return solver_sat_r(vars, 0, clauses);
}

/* bool parse_clause(vector<bitset<40>> &clauses )
{
    bitset<40> clause;
    clause.reset();
    int idx, counter = 2;
    string line;
    getline(std::cin, line);
    istringstream ss(line);
    for (string s; ss >> s;)
    {
        {
            stringstream cls{s};
            char t, x;
            if (s[1] == 'X')
                cls >> t >> x >> idx;
            else
                cls >> x >> idx;
        }
        clause[idx - 1] = 1;
        clause[20 + (idx - 1)] = s[0] != '~';
        counter--;
        ss >> s;
    }

        // if (counter == 1) { // if not bitset could use 0b10000'00000'00000'00000 % (clause >> 20) as a test ...
        //     if (fixed[idx -1]) // already fixed before, no need to repeat clause
        //         return fixed[20 + (idx - 1)] == clause[(20 + idx) - 1]; // UNSAT if two fixed differently
        //     fixed[idx-1] = true;
        //     fixed[(idx + 20) - 1] = clause[(idx + 20) - 1];
        // }
    // if (counter < 2)
    clauses.push_back(clause);
    return true;
} */

void parse_clause(vector<vector<int>> &clauses)
{
    vector<int> clause;
    int idx;
    string line;
    getline(std::cin, line);
    istringstream ss(line);
    for (string s; ss >> s;)
    {
        {
            stringstream cls{s};
            char t, x;
            if (s[1] == 'X')
                cls >> t >> x >> idx;
            else
                cls >> x >> idx;
        }
        clause.push_back(s[0] != '~' ? idx : -idx);
        ss >> s;
    }
    clauses.push_back(clause);
}

int main()
{
    int cases;
    for (cin >> cases; cases; cases--)
    {
        int m, n;
        cin >> n >> m;
        // vector<bitset<40>> clauses;
        vector<vector<int>> clauses;
        // bitset<40> fixed; // variables in unit clause that cannot change.
        // fixed.reset();
        bool sat = true;
        string buff;
        getline(std::cin, buff);
        for (auto c = 0; c < m; c++)
        {
            // if (not parse_clause(clauses))
                // sat = false;
            parse_clause(clauses);
            // keep parsing to stay in valid stream state
        }

        // bitset<20> vars((fixed.to_ulong() & 0xFFFFF00000) >> 20);
        vector<bool> vars(n, false);

        // if (sat)
        // sat = solver_sat(vars, clauses);
        sat = solver_sat(vars, clauses);

        if (sat)
            cout << "satisfiable" << endl;
        else
            cout << "unsatisfiable" << endl;
    }

    return 0;
}
