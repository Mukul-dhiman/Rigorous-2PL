#include <semaphore.h>
#include <bits/stdc++.h>

class lockmanager
{
    sem_t lock_manager_lock;
    std::map<std::string, int> var_map;
    std::vector<sem_t> variable_write_locks;
    std::vector<std::multiset<int>> transaction_set;

public:
    lockmanager()
    {

    }
    
    lockmanager(std::vector<std::string> state_variables)
    {
        sem_init(&lock_manager_lock, 0, 1);

        variable_write_locks = std::vector<sem_t>(state_variables.size());
        for (int i = 0; i < state_variables.size(); i++)
        {
            var_map[state_variables[i]] = i;
            sem_init(&variable_write_locks[i], 0, 1);
        }

        transaction_set = std::vector<std::multiset<int>>(state_variables.size());
    }

    void read_lock_acquire(int t_id, std::string var_name)
    {
        sem_wait(&lock_manager_lock);
        transaction_set[var_map[var_name]].insert(t_id);
        if (transaction_set[var_map[var_name]].size() == 1)
            sem_wait(&variable_write_locks[var_map[var_name]]);
        std::cout << "R-lock" << "[" << t_id << "," << var_name << "]" << std::endl;
        sem_post(&lock_manager_lock);
    }

    void read_lock_release(int t_id, std::string var_name)
    {
        sem_wait(&lock_manager_lock);
        transaction_set[var_map[var_name]].erase(t_id);
        if (transaction_set[var_map[var_name]].size() == 0)
            sem_post(&variable_write_locks[var_map[var_name]]);
        std::cout << "unlock" << "[" << t_id << "," << var_name << "]" << std::endl;
        sem_post(&lock_manager_lock);
    }

    void write_lock_acquire(int t_id, std::string var_name)
    {
        sem_wait(&variable_write_locks[var_map[var_name]]);
        sem_wait(&lock_manager_lock);
        std::cout << "W-lock" << "[" << t_id << "," << var_name << "]" << std::endl;
        sem_post(&lock_manager_lock);
    }

    void write_lock_release(int t_id, std::string var_name)
    {
        sem_post(&variable_write_locks[var_map[var_name]]);
        sem_wait(&lock_manager_lock);
        std::cout << "unlock" << "[" << t_id << "," << var_name << "]" << std::endl;
        sem_post(&lock_manager_lock);
    }

    void upgrade_lock(int t_id, std::string var_name)
    {
        sem_wait(&lock_manager_lock);
        transaction_set[var_map[var_name]].erase(t_id);
        if (transaction_set[var_map[var_name]].size() == 0)
            sem_post(&variable_write_locks[var_map[var_name]]);
        sem_post(&lock_manager_lock);
        sem_wait(&variable_write_locks[var_map[var_name]]);
        sem_wait(&lock_manager_lock);
        std::cout << "upgrade" << "[" << t_id << "," << var_name << "]" << std::endl;
        sem_post(&lock_manager_lock);
    }

    void downgrade_lock(int t_id, std::string var_name)
    {
        sem_post(&variable_write_locks[var_map[var_name]]);
        sem_wait(&lock_manager_lock);
        transaction_set[var_map[var_name]].insert(t_id);
        if (transaction_set[var_map[var_name]].size() == 1)
            sem_wait(&variable_write_locks[var_map[var_name]]);
        std::cout << "downgrade" << "[" << t_id << "," << var_name << "]" << std::endl;
        sem_post(&lock_manager_lock);
    }
};
