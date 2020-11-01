#include <pthread.h>
#include <bits/stdc++.h>

class lockmanager
{
    pthread_mutex_t lock_manager_lock;
    std::map<std::string, int> var_map;
    std::vector<pthread_cond_t> variable_write_locks;
    std::vector<bool> variable_is_write_locked;
    std::vector<std::multiset<int>> transaction_set;

public:
    lockmanager()
    {

    }
    
    lockmanager(std::vector<std::string> state_variables)
    {
        lock_manager_lock = PTHREAD_MUTEX_INITIALIZER;
        variable_write_locks = std::vector<pthread_cond_t>(state_variables.size());
        variable_is_write_locked = std::vector<bool>(state_variables.size(),false);

        for (int i = 0; i < state_variables.size(); i++)
        {
            var_map[state_variables[i]] = i;
            variable_write_locks[i] = PTHREAD_COND_INITIALIZER;
        }

        transaction_set = std::vector<std::multiset<int>>(state_variables.size());
    }

    void read_lock_acquire(int t_id, std::string var_name)
    {
        pthread_mutex_lock(&lock_manager_lock);
        while(variable_is_write_locked[var_map[var_name]])
        {
            std::cout << "wait_R-lock" << t_id << "," << var_name << "]" << std::endl;
            pthread_cond_wait(&variable_write_locks[var_map[var_name]], &lock_manager_lock);
        }
        transaction_set[var_map[var_name]].insert(t_id);
        std::cout << "R-lock" << "[" << t_id << "," << var_name << "]" << std::endl;
        pthread_mutex_unlock(&lock_manager_lock);
    }

    void read_lock_release(int t_id, std::string var_name)
    {
        pthread_mutex_lock(&lock_manager_lock);
        transaction_set[var_map[var_name]].erase(t_id);
        std::cout << "R-unlock" << "[" << t_id << "," << var_name << "]" << std::endl;
        pthread_cond_signal(&variable_write_locks[var_map[var_name]]);
        pthread_mutex_unlock(&lock_manager_lock);
    }

    void write_lock_acquire(int t_id, std::string var_name)
    {
        pthread_mutex_lock(&lock_manager_lock);
        while(variable_is_write_locked[var_map[var_name]] || transaction_set[var_map[var_name]].size()!=0)
        {
            std::cout << "wait_W-lock" << "[" << t_id << "," << var_name << "]" << std::endl;
            pthread_cond_wait(&variable_write_locks[var_map[var_name]],&lock_manager_lock);
        }
        variable_is_write_locked[var_map[var_name]] = true;
        std::cout << "W-lock" << "[" << t_id << "," << var_name << "]" << std::endl;
        pthread_mutex_unlock(&lock_manager_lock);
    }

    void write_lock_release(int t_id, std::string var_name)
    {
        pthread_mutex_lock(&lock_manager_lock);
        std::cout << "W-unlock" << "[" << t_id << "," << var_name << "]" << std::endl;
        variable_is_write_locked[var_map[var_name]] = false;
        pthread_cond_signal(&variable_write_locks[var_map[var_name]]);
        pthread_mutex_unlock(&lock_manager_lock);
    }

    void upgrade_lock(int t_id, std::string var_name)
    {
        pthread_mutex_lock(&lock_manager_lock);
        transaction_set[var_map[var_name]].erase(t_id);
        while (variable_is_write_locked[var_map[var_name]] || transaction_set[var_map[var_name]].size() != 0)
        {
            std::cout << "wait_upgrade" << "[" << t_id << "," << var_name << "]" << std::endl;
            pthread_cond_wait(&variable_write_locks[var_map[var_name]], &lock_manager_lock);
        }
        variable_is_write_locked[var_map[var_name]] = true;
        std::cout << "upgrade" << "[" << t_id << "," << var_name << "]" << std::endl;
        pthread_mutex_unlock(&lock_manager_lock);
    }

    void downgrade_lock(int t_id, std::string var_name)
    {
        pthread_mutex_lock(&lock_manager_lock);
        variable_is_write_locked[var_map[var_name]] = false;
        pthread_cond_signal(&variable_write_locks[var_map[var_name]]);
        while (variable_is_write_locked[var_map[var_name]])
        {
            std::cout << "wait_downgrade" << t_id << "," << var_name << "]" << std::endl;
            pthread_cond_wait(&variable_write_locks[var_map[var_name]], &lock_manager_lock);
        }
        transaction_set[var_map[var_name]].insert(t_id);
        std::cout << "downgrade" << "[" << t_id << "," << var_name << "]" << std::endl;
        pthread_mutex_unlock(&lock_manager_lock);
    }
};
