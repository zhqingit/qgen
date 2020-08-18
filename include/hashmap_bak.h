#include <stdbool.h>

typedef struct entry{
  void * key;
  void * value;
  struct entry * next;
}*Entry;

//#define newEntry() NEW(struct entry)
#define newEntryList(length) (Entry)malloc(length * sizeof(struct entry))

typedef struct hashMap *HashMap;
//#define newHashMap() NEW(hashMap)

// type of the hashtable
typedef int(*HashCode)(HashMap, void * key);

//equal function types
typedef bool(*Equal)(void * key1, void * key2);

//add key
typedef void(*Put)(HashMap hashMap, void * key, void * value);

//get value from key
typedef void * (*Get)(HashMap hashMap, void * key);

//del key
typedef bool(*Remove)(HashMap hashMap, void * key);

//clear map
typedef void(*Clear)(HashMap hashMap);

//if key exists
typedef bool(*Exists)(HashMap hashMap, void *key);


typedef struct hashMap{
  int size;                       //current size
  int listSize;                   // efficient space
  HashCode hashCode;              // hash function
  Equal equal;                    //if same
  Entry list;                    //save space
  Put put;                       //add key
  Get get;                       // get value from key
  Remove remove;                 // delete key
  Clear clear;                   // clear map
  Exists exists;                 // if key exists
  bool autoAssign;                // if dynamic adjsut memeory defaul yes
}*HashMap;

// 默认哈希函数
static int defaultHashCode(HashMap hashMap, void * key);

// 默认判断键值是否相等
static bool defaultEqual(void * key1, void * key2);

// 默认添加键值对
static void defaultPut(HashMap hashMap, void * key, void * value);

// 默认获取键对应值
static void * defaultGet(HashMap hashMap, void * key);

// 默认删除键
static bool defaultRemove(HashMap hashMap, void * key);

// 默认判断键是否存在
static bool defaultExists(HashMap hashMap, void * key);

// 默认清空Map
static void defaultClear(HashMap hashMap);

// 创建一个哈希结构
//HashMap createHashMap(HashCode hashCode, Equal equal);

// 重新构建
static void resetHashMap(HashMap hashMap, int listSize);



//iterator structure
typedef struct hashMapIterator{
  Entry entry; // point
  int count; // iterate counts
  int hashCode; //hash value
  HashMap hashMap;
}*HashMapIterator;

//#define newHashMapIterator() NEW(struct hashMapIterator)
#define newHashMapIterator() NEW(hashMapIterator)

//create hash struccture
HashMap createHashMap(HashCode hashCode, Equal equal);
// 创建哈希结构迭代器
HashMapIterator createHashMapIterator(HashMap hashMap);

// 迭代器是否有下一个
bool hasNextHashMapIterator(HashMapIterator iterator);

// 迭代到下一次
HashMapIterator nextHashMapIterator(HashMapIterator iterator);

// 释放迭代器内存
void freeHashMapIterator(HashMapIterator * iterator);


#define Put(map, key, value) map->put(map, (void *)key, (void *)value)
#define Get(map, key) (char *)map->get(map, (void *)key)
#define Remove(map, key) map->remove(map, (void *)key)
#define Existe(map, key) map->exists(map, (void *)key)
