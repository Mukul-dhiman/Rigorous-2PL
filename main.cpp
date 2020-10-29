#include <bits/stdc++.h>
#include <pthread.h>
#include "headers/transaction.h"
#include "headers/lockmanager.h"
using namespace std;

map<string, int> state_variables;

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

// TODO
void *execute_transaction(void *arg)
{
    // int i = (long long int)arg;
    // cout << i << endl;
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
    ifstream cin(argv[1]);
    ofstream cout(argv[2]);

    // Number of transactions
    string read_line;
    getline(cin, read_line);
    int N = stoi(read_line);

    cout << N << endl;

    // State variables
    getline(cin, read_line);
    vector<string> state_variable_list = split_string(read_line);

    for (int i = 0; i < state_variable_list.size(); i += 2)
    {
        state_variables[state_variable_list[i]] = stoi(state_variable_list[i + 1]);
    }

    for (auto it : state_variables)
    {
        cout << it.first << ": " << it.second << endl;
    }

    // Transactions
    vector<transaction> transaction_array;
    for (int i = 0; i < N; i++)
    {
        cout << endl;

        int transaction_id;
        vector<string> operation_list;
        outcome oc;

        getline(cin, read_line);
        transaction_id = stoi(read_line);

        getline(cin, read_line);
        while (read_line != "C" && read_line != "A")
        {
            cout << read_line << endl;
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
        // for(auto t:transaction_array[i].list_of_operations){
        //     cout<<t<<" | ";
        // }
        // cout<<endl;
        pthread_create(&tid[i], NULL, execute_transaction, (void *)(long long int)i);
    }

    for (int i = 0; i < N; i++)
    {
        pthread_join(tid[i], NULL);
    }

    return 0;
}
