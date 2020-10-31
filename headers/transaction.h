#include<bits/stdc++.h>

enum operation_type
{
    read,
    write
};

// typedef struct operation {
//     operation_type op_type;
//     std::string variable_name;
// } operation;

enum outcome
{
    t_commit,
    t_abort
};

class transaction
{
    // Usage in main function:
    // operation a = {read,"x"};
    // vector<operation> operations = {a,a,a};
    // transaction t(1,operations,t_commit);

    int transaction_id;
    // std::vector<operation> list_of_operations;
    std::vector<std::string> list_of_operations;
    std::map<std::string,std::string> locks_acquired;
    outcome transaction_outcome;

public:
    transaction(int id, std::vector<std::string> operations, outcome oc)
    {
        transaction_id = id;
        list_of_operations = operations;
        transaction_outcome = oc;
    }

    int get_id()
    {
        return transaction_id;
    }

    std::vector<std::string> get_list_of_operations()
    {
        return list_of_operations;
    }

    outcome get_outcome()
    {
        return transaction_outcome;
    }

    std::map<std::string,std::string> get_locks_acquired()
    {
        return locks_acquired;
    }

    void debug()
    {
        std::cout<<"Transaction read:"<<std::endl;
        std::cout<<transaction_id<<std::endl;
        for(int i=0;i<list_of_operations.size();i++)
            std::cout<<list_of_operations[i]<<std::endl;
        std::cout<<transaction_outcome<<std::endl;
    }
};
