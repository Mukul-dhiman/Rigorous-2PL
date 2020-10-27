# include<string>
# include<vector>

enum operation_type{read, write};

// typedef struct operation {
//     operation_type op_type;
//     std::string variable_name;
// } operation;

enum outcome{t_commit,t_abort};

class transaction{
    // Usage in main function:
    // operation a = {read,"x"};
    // vector<operation> operations = {a,a,a};
    // transaction t(1,operations,t_commit);
    
    int transaction_id;
    // std::vector<operation> list_of_operations;
    std::vector<std::string> list_of_operations;
    outcome transaction_outcome;
    public:
    transaction(int id, std::vector<std::string> operations, outcome oc)
    {
        transaction_id = id;
        list_of_operations = operations;
        transaction_outcome = oc;
    }
    std::vector<std::string> get_list_of_operations(int id)
    {
        return list_of_operations;
    }
    outcome get_outcome(int id)
    {
        return transaction_outcome;
    }
};


