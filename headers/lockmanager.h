class lockmanager
{
    public:
        // current number of reading threads on a state
        unordered_map<string,int> read_count;

        // read_lock, write_lock and none status for every state
        // none -> "0"
        // read_lock -> "1"
        // write_lock -> "2"
        unordered_map<string,int> lock_state;

        // sequence of operations in threads
        vector<string> Sequiential_output;

        lockmanager(map<string, int> state_variables){
            read_count.clear();

            lock_state.clear();
            for(auto state:state_variables){
                lock_state[state]=0;
                read_count[state]=0;
            }

            Sequiential_output.clear();
        }
        void Read_lock(int transaction_id,string state_variable){
            if(lock_state[state_variable]==0){
                lock_state[state_variable]=1;
                read_count[state]++;
            }
            else if(lock_state[state_variable]==1){
                continue;
            }
            else{

            }
        }

};
