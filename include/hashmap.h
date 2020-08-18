
#include <stdbool.h>

typedef struct hm_entry{
  void * key;
  void * value;
  struct hm_entry * next;
}*HM_entry;

HM_entry hm_init_entry();

#define newEntryList(length) (HM_entry)malloc(length * sizeof(struct hm_entry))

typedef struct hashMap *HashMap;
//#define newHashMap() NEW(hashMap)

// type of the hashtable
typedef int(*HM_HashCode)(HashMap, void * key);
//equal function types
typedef bool(*HM_Equal)(void * key1, void * key2);
//add key
typedef void(*HM_Put)(HashMap hashMap, void * key, void * value);
//get value from key
typedef void * (*HM_Get)(HashMap hashMap, void * key);
//if key exists
typedef bool(*HM_Exists)(HashMap hashMap, void *key);
//clear map
typedef void(*HM_Clear)(HashMap hashMap);
//del key
typedef bool(*HM_Remove)(HashMap hashMap, void * key);
//distribution of entries
typedef void(*HM_Distribution)(HashMap hashMap, int *dis);

typedef struct hashMap{
  int size;                       //current size
  int listSize;                   // efficient space

  HM_HashCode hashCode;
  HM_Equal equal;
  HM_Put put;
  HM_Get get;
  HM_entry list;                    //save space
  HM_Exists exists;
  HM_Clear clear;
  HM_Remove remove;
  HM_Distribution dis;
  bool autoAssign;                // if dynamic adjsut memeory defaul yes
}*HashMap;

//create hash table
HashMap createHashMap(HM_HashCode hashCode);

// 默认哈希函数
static int defaultHashCode(HashMap hashMap, void * key);

// BDHR hash function
static int BKDRHash(HashMap hashMap, void * key);

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

//distribution of entries
static void defaultDis(HashMap hashMap, int *dis);

//reset hashtable
static void resetHashMap(HashMap hashMap, int listSize);


//iterator structure
typedef struct hashMapIterator{
  HM_entry entry; // point
  int count; // iterate counts
  int hashCode; //hash value
  HashMap hashMap;
}*HashMapIterator;

//#define newHashMapIterator() NEW(struct hashMapIterator)
//#define newHashMapIterator() NEW(hashMapIterator)

// 创建哈希结构迭代器
HashMapIterator createHashMapIterator(HashMap hashMap);

// 迭代器是否有下一个
bool hasNextHashMapIterator(HashMapIterator iterator);

// 迭代到下一次
HashMapIterator nextHashMapIterator(HashMapIterator iterator);

// 释放迭代器内存
void freeHashMapIterator(HashMapIterator * iterator);
