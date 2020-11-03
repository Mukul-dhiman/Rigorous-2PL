#include <bits/stdc++.h>
#include <pthread.h>
#include "headers/transaction.h"
#include "headers/lockmanager.h"
using namespace std;

string inputfile, outputfile;
map<string, int> state_variables_list_Initial;
map<string, int> state_variables;
vector<string> state_variable_list;
lockmanager L;
vector<transaction> transaction_array;

// R(X)
int get_value(string variable_name)
{
    return state_variables[variable_name];
}

// W(X)
void set_value(string variable_name, int value)
{
    state_variables[variable_name] = value;
}

vector<string> split_string(string s)
{
    string x = "";
    vector<string> v;
    for (int i = 0; i < s.size(); i++)
    {
        if (s[i] != ' ')
            x += s[i];
        else
        {
            v.push_back(x);
            x = "";
        }
    }
    if (x != "")
        v.push_back(x);
    return v;
}

void operation_eval(map<string, int> &variables, vector<string> op_tokens)
{
    // Structure of statement: lval = rval1 op rval2
    // lval: a var_name which may or may not exist in local_copy
    // rvali: var_name which exists in local_copy or constant
    // op: + or -
    string lval = op_tokens[0];
    string rval1 = op_tokens[2];
    string rval2 = op_tokens[4];
    string op = op_tokens[3];

    int rval = 0;

    if (variables.find(rval1) == variables.end())
        rval += stoi(rval1);
    else
        rval += variables[rval1];

    int op_sign = 2 * (op[0] == '+') - 1;

    if (variables.find(rval2) == variables.end())
        rval += (op_sign * stoi(rval2));
    else
        rval += (op_sign * variables[rval2]);

    variables[lval] = rval;
}

// TODO
void *execute_transaction(void *arg)
{
    int id = (long long int)arg+1;
    transaction t = transaction_array[id-1];
    // t.debug();
    vector<string> ops = t.get_list_of_operations();
    map<string, string> locks = t.get_locks_acquired();
    map<string, int> local_var_copies;

    if (t.get_outcome() == t_abort)
        return NULL;

    vector<string> tokens;
    for (int i = 0; i < ops.size(); i++)
    {
        tokens = split_string(ops[i]);
        
        // If read operation
        if (tokens[0] == "R")
        {
            string lock_var = tokens[1];

            // If no lock acquired.
            if (locks.find(lock_var) == locks.end())
            {
                L.read_lock_acquire(id, lock_var);
            }
            else
            {
                // If write lock acquired.
                if (locks[lock_var] == "W")
                {
                    L.downgrade_lock(id, lock_var);
                }
                // If read lock acquired.
                else
                {
                    // Do nothing.
                }
            }

            locks[lock_var] = "R";

            // Read.
            local_var_copies[lock_var] = get_value(lock_var);
        }
        // If write operation
        else if (tokens[0] == "W")
        {
            string lock_var = tokens[1];

            // If no lock acquired.
            if (locks.find(lock_var) == locks.end())
            {
                L.write_lock_acquire(id, lock_var);
            }
            else
            {
                // If read lock acquired.
                if (locks[lock_var] == "R")
                {
                    L.upgrade_lock(id, lock_var);
                }
                // If write lock acquired.
                else
                {
                    // Do nothing.
                }
            }

            locks[lock_var] = "W";

            // Write
            set_value(lock_var, local_var_copies[lock_var]);
        }
        // Operation.
        else
        {
            operation_eval(local_var_copies, tokens);
        }
    }

    for (auto lock : locks)
    {
        if (lock.second == "R")
        {
            L.read_lock_release(id, lock.first);
        }
        else
        {
            L.write_lock_release(id, lock.first);
        }
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    // Argument validation
    if (argc != 3)
    {
        fprintf(stderr, "usage: %s <input_file> <output_file>\n", argv[0]);
        exit(1);
    }

    // Redirect I/O to files provided as argument
    inputfile = argv[1];
    ifstream cin(inputfile);
    outputfile = argv[2];
    ofstream cout(outputfile);

    // Number of transactions
    string read_line;
    getline(cin, read_line);
    int N = stoi(read_line);

    // State variables
    getline(cin, read_line);
    vector<string> state_variable_line = split_string(read_line);

    for (int i = 0; i < state_variable_line.size(); i += 2)
    {
        state_variable_list.push_back(state_variable_line[i]);
        state_variables[state_variable_line[i]] = stoi(state_variable_line[i + 1]);
    }

    // for(auto var: state_variables)
    // {
    //     cout<<var.first<<": "<<var.second<<endl;
    // }
    
    state_variables_list_Initial = state_variables;

    L = lockmanager(state_variable_list);

    // Transactions
    for (int i = 0; i < N; i++)
    {
        int transaction_id;
        vector<string> operation_list;
        outcome oc;

        getline(cin, read_line);
        transaction_id = stoi(read_line);

        getline(cin, read_line);
        while (read_line != "C" && read_line != "A")
        {
            operation_list.push_back(read_line);
            getline(cin, read_line);
        }

        if (read_line == "A")
            oc = t_abort;
        else
            oc = t_commit;

        transaction t(transaction_id, operation_list, oc);
        transaction_array.push_back(t);
    }

    pthread_t tid[N];
    for (int i = 0; i < N; i++)
    {
        pthread_create(&tid[i], NULL, execute_transaction, (void *)(long long int)i);
    }

    for (int i = 0; i < N; i++)
    {
        pthread_join(tid[i], NULL);
    }

    for(auto var: state_variables)
    {
        cout<<var.first<<": "<<var.second<<" ";
    }
    cout<<endl;

    vector<string> logs = L.get_logs();
    for(auto log: logs)
    {
        cout<<log<<endl;
    }

    cout<<"Successfully executed all the transactions."<<endl;
    
    return 0;
}
