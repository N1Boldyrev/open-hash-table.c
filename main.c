#include "open_hash_table.h"

int main()
{
    HashTable* open_hash_table=create_table(10,20,1000);
    add_data(open_hash_table);
    find_data(open_hash_table);
    free_table(open_hash_table);
    return 0;
}