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

        // constructor
        lockmanager(map<string, int> state_variables){
            
            // clearing all data structures for main function object
            read_count.clear();
            lock_state.clear();
            state_queue_lock.clear();
            Sequiential_output.clear();

            // initiate unbias data to start
            for(auto state:state_variables){
                lock_state[state]=0;
                read_count[state]=0;
                state_queue_lock[state]={};
            }
        }

        // bool return type for both read/write locks confirm the lock acquire 
        bool Read_lock(int transaction_id,string state_variable){
            
            // if state variable have no lock 
            if(lock_state[state_variable]==0){
                lock_state[state_variable]=1;
                read_count[state]++;
                return true;
            }

            // if state variable have read lock
            else if(lock_state[state_variable]==1){
                return true;
            }

            // have write lock
            else{

                // pushing to transaction id state variable queue 
                state_queue_lock[state_variable].push(transaction_id);
                return false;
            }
        }

        bool Write_lock(int transaction_id,string state_variable){

            // read count of state variable is non zero
            if(read_count[state_variable]!=0){

                // #special condition when only one lock is acquire and, by the same state variable which want write lock now
                if(read_count[state_variable]==1 && lock_state[state_variable]==1){
                    read_count[state_variable]--;
                    lock_state[state_variable]=2;
                    return true;
                }

                // if have lock state as read lock but not only read lock 
                if(lock_state[state_variable]==1){
                    read_count[state_variable]--;

                    // can be a trouble
                    lock_state[state_variable]=2;
                }
                state_queue_lock[state_variable].push(transaction_id);
                return false;
            }

            // read count zero
            else{
                if(state_queue_lock[state_variable].size()!=0){

                    // have write locks in state variable queue 
                    state_queue_lock[state_variable].push(transaction_id);
                    return false;
                }
                else{
                     
                    // fully safe for acquire write lock
                    lock_state[state_variable]=2;
                    return true;
                }
            }
        }

        bool Release_Lock(int transaction_id,string state_variable){

            // only when its read lock then we have to decrease read count 
            // and setting lock state to none and return true always
            if(lock_state[state_variable]==1){
                read_count[state_variable]--;
            }
            lock_state[state_variable]=0;
            return true;
        }

};
