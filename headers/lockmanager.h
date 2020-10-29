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

        // queue of transaction to acquire locks for every state variable 
        // queue of pairs -> ( state variable )
        unordered_map<string,queue<int>> state_queue_lock;

        lockmanager(map<string, int> state_variables){

            read_count.clear();
            lock_state.clear();
            for(auto state:state_variables){
                lock_state[state]=0;
                read_count[state]=0;
            }
            state_queue_lock.clear();
            Sequiential_output.clear();
        }
        bool Read_lock(int transaction_id,string state_variable){
            if(lock_state[state_variable]==0){
                lock_state[state_variable]=1;
                read_count[state]++;
                return true;
            }
            else if(lock_state[state_variable]==1){
                return true;
            }
            else{
                state_queue_lock[state_variable].push(transaction_id);
                return false;
            }
        }

        bool Write_lock(int transaction_id,string state_variable){
            if(read_count[state_variable]!=0){
                if(read_count[state_variable]==1 && lock_state[state_variable]==1){
                    read_count[state_variable]--;
                    lock_state[state_variable]=2;
                    return true;
                }
                if(lock_state[state_variable]==1){
                    read_count[state_variable]--;

                    // can be a trouble
                    lock_state[state_variable]=2;
                }
                state_queue_lock[state_variable].push(transaction_id);
                return false;
            }
            else{
                if(state_queue_lock[state_variable].size()!=0){
                    state_queue_lock[state_variable].push(transaction_id);
                    return false;
                }
                else{
                    lock_state[state_variable]=2;
                    return true;
                }
            }
        }

        bool Release_Lock(int transaction_id,string state_variable){
            if(lock_state[state_variable]==1){
                read_count[state_variable]--;
            }
            lock_state[state_variable]=0;
            return true;
        }

};
