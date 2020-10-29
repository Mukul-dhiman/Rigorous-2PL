class lockmanager
{
    public:
        // current number of reading threads
        int read_count;

        // read_lock, write_lock and none status for every state
        // none -> "0"
        // read_lock -> "1"
        // write_lock -> "2"
        unordered_map<string,int> lock_state;

        // sequence of operations in threads
        vector<string> Sequiential_output;

        lockmanager(map<string, int> state_variables){
            read_count=0;

            lock_state.clear();
            for(auto state:state_variables){
                lock_state[state]=0;
            }
        }
        void Read_lock(int transaction_id,string state_variable){
            // if(lock_state.find(state_variable)==lock)
        }

};
