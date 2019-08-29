#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"
typedef struct
{
	uint8_t* key;
	uint8_t* data;

}Bucket;


typedef struct
{
	size_t table_size;
	size_t buffer;
	size_t key_len;
	size_t data_len;
	Bucket** buckets;
}HashTable;

void istream(uint8_t* data, size_t data_len);
size_t my_getline(uint8_t* str);
Bucket* create_bucket(size_t key_len, size_t data_len);
void free_bucket(Bucket* bucket);
HashTable* create_table(size_t table_size, size_t key_len, size_t data_len);
void free_table(HashTable* table);
size_t hash_function(HashTable* table);
HashTable* resize(HashTable* table);
size_t is_empty(uint8_t p, size_t hash, HashTable* table);
HashTable* add_data(HashTable* table);
signed int find_key(HashTable* table, uint8_t* search_key);
void find_data(HashTable* table);
void delete_data(HashTable* table);


void istream(uint8_t* data, size_t data_len)
{
	uint8_t* tmp = malloc(sizeof(uint8_t) * data_len);
	size_t i = my_getline(tmp);
	memcpy(data, tmp, sizeof(uint8_t) * i);
	free(tmp);
}

size_t my_getline(uint8_t* str)
{
	size_t i = 0;
	while ((str[i] = getchar()) != EOF && str[i] != '\n')
	{
		if (str[i] == '\n')
			i++;
		i++;
	}
	str[i] = '\0';
	return i;
}

Bucket* create_bucket(size_t key_len, size_t data_len)
{
	Bucket* bucket = malloc(sizeof(*bucket));
	bucket->key = calloc(key_len, sizeof(uint8_t));
	bucket->data = calloc(data_len, sizeof(uint8_t));
	return bucket;
}

void free_bucket(Bucket* bucket)
{
	free(bucket->key);
	free(bucket->data);
	free(bucket);
}

HashTable* create_table(size_t table_size, size_t key_len, size_t data_len)
{
	HashTable* table = malloc(sizeof(*table));
	table->table_size = table_size;
	table->buffer = 0;
	table->buckets = malloc(table_size * sizeof(Bucket));
	for (size_t i = 0; i < table_size; i++)
	{
		table->buckets[i] = create_bucket(table->key_len, table->data_len);
	}
	return table;
}

void free_table(HashTable* table)
{
	for (size_t i = 0; i < table->table_size; i++)
	{
		free_bucket(table->buckets[i]);
	}
	free(table->buckets);
	free(table);
}

size_t hash_function(HashTable* table)
{
	return (size_t)(table->buffer % table->table_size);
}

HashTable* resize(HashTable* table)
{
	Bucket** tmp = malloc(sizeof(Bucket*) * table->table_size * 2);
	memcpy(tmp, table->buckets, sizeof(Bucket*) * table->table_size);
	table->table_size *= 2;
	for (size_t i = (table->table_size / 2) - 1; i < table->table_size; i++)
	{
		tmp[i] = create_bucket(table->key_len, table->data_len);
	}
	table->buckets = tmp;
	return table;
}

size_t is_empty(uint8_t p, size_t hash, HashTable* table)
{
	if (p != 0 && hash < table->table_size)
		return (size_t)0;
	else
		return (size_t)1;
}

HashTable* add_data(HashTable* table)
{
	if ((double)table->buffer / (double)table->table_size >= 0.5)
	{
		table = resize(table);
	}
	size_t hash = hash_function(table);
	size_t flg = 0;
	while (flg == 0)
	{
		if (is_empty(table->buckets[hash]->data[0], hash, table) == 0)
			hash++;
		else
			flg = 1;
	}
	printf("\nKEY->");
	uint8_t* tmp = malloc(sizeof(uint8_t) * table->key_len);
	size_t tmp_len = my_getline(tmp);
	if (find_key(table, tmp) >= 0)
	{
		printf("\n[KEY NOT AVALIBLE]\n");
	}
	else
	{
		memcpy(table->buckets[hash]->key, tmp, sizeof(uint8_t) * tmp_len);
		free(tmp);
		printf("\nDATA->");
		istream(table->buckets[hash]->data, table->data_len);
		table->buffer++;
		printf("\n[DONE]\n");
	}
	
	return table;
}

signed int find_key(HashTable* table, uint8_t* search_key)
{
	signed int flg = 0;
	for (size_t i = 0; i < table->buffer + 1; i++)
	{
		if (strcmp(table->buckets[i]->key, search_key) == 0)
		{
			flg = i;
			break;
		}
		else
			flg = -1;
	}
	return flg;
}


void find_data(HashTable* table)
{
	signed int i;
	printf("\nSEARCH KEY->");
	uint8_t* search_key = calloc(table->key_len, sizeof(uint8_t));
	istream(search_key, table->key_len);
	if ((i = find_key(table, search_key)) >= 0)
		printf("\nDATA->%s\n", table->buckets[i]->data);
	else
		printf("\n[KEY NOT FOUND]\n");
	free(search_key);
}


void delete_data(HashTable* table)
{
	uint8_t* search_key = calloc(table->key_len, sizeof(uint8_t));
	signed int i;
	printf("\nSEARCH KEY->");
	istream(search_key, table->key_len);
	if ((i = find_key(table, search_key)) >= 0)
	{
		memset(table->buckets[i]->key, 0, sizeof(uint8_t) * table->key_len);
		memset(table->buckets[i]->data, 0, sizeof(uint8_t) * table->data_len);
		table->buffer--;
		printf("\n[DONE]\n");
	}
	else
		printf("\n[KEY NOT FOUND]\n");
	free(search_key);
}