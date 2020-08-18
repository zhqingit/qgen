#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/hashmap.h"

HM_entry hm_init_entry(){
  HM_entry entry = (HM_entry)malloc(sizeof(struct hm_entry));
  entry->next = NULL;
  entry->key = NULL;
  entry->value = NULL;
  return entry;
};

int defaultHashCode(HashMap hashMap, void * key){
  //IN_STACK;
  char * k = (char *)key;
  //printf("in function:%s",k);
  unsigned long h = 0;
  while (*k){
    h = (h << 4) + *k++;
    unsigned long g = h & 0xF0000000L;
    if (g){
      h ^= g >> 24;
    }
    h &= ~g;
  }
  //OUT_STACK;
  return h % hashMap->listSize;
}

int BKDRHash(HashMap hashMap, void * key)
{
    char * str = (char *)key;
    unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
    unsigned int hash = 0;

    while (*str)
    {
        hash = hash * seed + (*str++);
    }

    //return (hash & 0x7FFFFFFF);
    return hash % hashMap->listSize;
}

bool defaultEqual(void *key1, void *key2) {
  char *k1 = (char *)key1;
  char *k2 = (char *)key2;
  return !strcmp(k1,k2);
}

void * defaultGet(HashMap hashMap, void * key) {
    int index = hashMap->hashCode(hashMap, key);
    HM_entry entry = &hashMap->list[index];
    while (entry->key != NULL && !hashMap->equal(entry->key, key)) {
        entry = entry->next;
    }
    return entry->value;
}

void defaultDis(HashMap hashMap, int *dis) {
  //int diss[hashMap->listSize];
  HM_entry entry;
  for (int i=0; i<hashMap->listSize;i++){
    entry = &hashMap->list[i];
    if (entry->key == NULL) dis[i] = 0;
    else{
      dis[i] = 1;
      while (entry->next != NULL && entry->next->key != NULL){
        dis[i] += 1;
        entry = entry->next;
      }
    }
    if (dis[i] == 38){
      HM_entry entry1 = &(hashMap->list[i]);
      while(entry1->next!=NULL && entry1->next->key != NULL){
        printf("key:%s -- %d\n",entry1->key,i);
        entry1 = entry1->next;
      }
      entry1 = NULL;

    }
    //printf("---%d\n",dis[i]);
  }
  //*dis = diss;
}

bool defaultExists(HashMap hashMap, void * key) {
    int index = hashMap->hashCode(hashMap, key);
    //printf("%s--%d\n",key, index);
    HM_entry entry = &hashMap->list[index];
    if (entry->key == NULL) {
      return 0;
    }
    if (hashMap->equal(entry->key, key)) {
        return 1;
    }
    if (entry->next != NULL) {
      do {
        if (hashMap->equal(entry->key, key)) {
          return 1;
        }
        entry = entry->next;
      } while (entry != NULL);
        return 0;
    }else {
      return 0;
    }
}

void defaultClear(HashMap hashMap) {
    for (int i = 0; i < hashMap->listSize; i++) {
        // 释放冲突值内存
        //HM_entry entry = hashMap->list[i].next;
        HM_entry entry = &(hashMap->list[i]);
        while (entry != NULL) {
            HM_entry next = entry->next;
            free(entry);
            entry = next;
        }
        hashMap->list[i].next = NULL;
    }
    // 释放存储空间
    free(hashMap->list);
    hashMap->list = NULL;
    hashMap->size = -1;
    hashMap->listSize = 0;
}

void resetHashMap(HashMap hashMap, int listSize){
  if (listSize < 8) return;

  HM_entry tempList = newEntryList(hashMap->size);

  HashMapIterator iterator = createHashMapIterator(hashMap);
  int length = hashMap->size;
  for (int index = 0; hasNextHashMapIterator(iterator); index++){
    iterator = nextHashMapIterator(iterator);
    tempList[index].key = iterator->entry->key;
    tempList[index].value = iterator->entry->value;
    tempList[index].next = NULL;
  }
  freeHashMapIterator(&iterator);

  //clear old key values
  hashMap->size = 0;
  for (int i = 0; i< hashMap->listSize; i++){
    HM_entry current = &hashMap->list[i];
    current->key = NULL;
    current->value = NULL;
    if (current->next != NULL){
      while(current->next != NULL){
        HM_entry temp = current->next->next;
        free(current->next);
        current->next = temp;
      }
    }
  }

  //change memory size
  hashMap->listSize = listSize;
  HM_entry relist = (HM_entry)realloc(hashMap->list, hashMap->listSize*sizeof(struct hm_entry));
  if (relist != NULL){
    hashMap->list = relist;
    relist = NULL;
  }

  // init data
  for (int i=0; i< hashMap->listSize;i++){
    hashMap->list[i].key = NULL;
    hashMap->list[i].value = NULL;
    hashMap->list[i].next = NULL;
  }

  // rewrite key value
  for (int i = 0; i < length; i++){
    //Array x = tempList[i].value
    hashMap->put(hashMap, tempList[i].key, tempList[i].value);
  }
  free(tempList);
}

void defaultPut(HashMap hashMap, void *key, void *value){
  if (hashMap->autoAssign && hashMap->size >= hashMap->listSize){
    resetHashMap(hashMap, hashMap->listSize *2);
  }
  int index = hashMap->hashCode(hashMap, key);
  if (hashMap->list[index].key == NULL){
    hashMap->size++;
    hashMap->list[index].key = key;
    hashMap->list[index].value = value;
  }
  else{
    HM_entry current = &hashMap->list[index];
    while(current != NULL){
      if (hashMap->equal(key, current->key)){
        current->value = value;
        return;
      }
      current = current->next;
    };

    HM_entry entry = malloc(sizeof(struct hm_entry));
    // = newEntry();
    entry->key = key;
    entry->value = value;
    entry->next = hashMap->list[index].next;
    hashMap->list[index].next = entry;
    hashMap->size++;
  }
}

bool defaultRemove(HashMap hashMap, void * key) {
    int index = hashMap->hashCode(hashMap, key);
    HM_entry entry = &hashMap->list[index];
    if (entry->key == NULL) {
        return 0;
    }
    bool result = 0;
    if (hashMap->equal(entry->key, key)) {
        hashMap->size--;
        if (entry->next != NULL) {
            HM_entry temp = entry->next;
            entry->key = temp->key;
            entry->value = temp->value;
            entry->next = temp->next;
            free(temp);
        }
        else {
            entry->key = entry->value = NULL;
        }
        result = 1;
    }
    else {
        HM_entry p = entry;
        entry = entry->next;
        while (entry != NULL) {
            if (hashMap->equal(entry->key, key)) {
                hashMap->size--;
                p->next = entry->next;
                free(entry);
                result = 1;
                break;
            }
            p = entry;
            entry = entry->next;
        };
    }

    // 如果空间占用不足一半，则释放多余内存
    if (result && hashMap->autoAssign &&  hashMap->size < hashMap->listSize / 2) {
        resetHashMap(hashMap, hashMap->listSize / 2);
    }
    return result;
}

//HashMap createHashMap(HashCode hashCode, Equal equal) {
HashMap createHashMap(HM_HashCode hashCode) {
    //HashMap hashMap = newHashMap();
    HashMap hashMap = malloc(sizeof(struct hashMap));
    hashMap->size = 0;
    hashMap->listSize = 8;
    //hashMap->hashCode = hashCode == NULL ? defaultHashCode : hashCode;
    hashMap->hashCode = hashCode == NULL ? BKDRHash : hashCode;
    
    //hashMap->equal = equal == NULL ? defaultEqual : equal;
    hashMap->equal = defaultEqual;
    hashMap->get = defaultGet;
    hashMap->exists = defaultExists;
    hashMap->put = defaultPut;
    hashMap->remove = defaultRemove;
    hashMap->clear = defaultClear;
    hashMap->dis = defaultDis;
    hashMap->autoAssign = 1;
    // 起始分配8个内存空间，溢出时会自动扩充
    hashMap->list = newEntryList(hashMap->listSize);
    HM_entry p = hashMap->list;
    for (int i = 0; i < hashMap->listSize; i++) {
        p[i].key = p[i].value = p[i].next = NULL;
    }
    return hashMap;
}

HashMapIterator createHashMapIterator(HashMap hashMap) {
    HashMapIterator iterator = malloc(sizeof(HashMapIterator));
    //HashMapIterator iterator = newHashMapIterator();
    iterator->hashMap = hashMap;
    iterator->count = 0;
    iterator->hashCode = -1;
    iterator->entry = NULL;
    return iterator;
}

bool hasNextHashMapIterator(HashMapIterator iterator) {
    return iterator->count < iterator->hashMap->size ? 1 : 0;
}

HashMapIterator nextHashMapIterator(HashMapIterator iterator) {
    if (hasNextHashMapIterator(iterator)) {
        if (iterator->entry != NULL && iterator->entry->next != NULL) {
            //printf("--------%d",iterator->count);
            iterator->count++;
            iterator->entry = iterator->entry->next;
            return iterator;
        }
        while (++iterator->hashCode < iterator->hashMap->listSize) {
            //printf("=====%d",iterator->count);
            HM_entry entry = &iterator->hashMap->list[iterator->hashCode];
            if (entry->key != NULL) {
                iterator->count++;
                iterator->entry = entry;
                break;
            }
        }
    }
    return iterator;
}

void freeHashMapIterator(HashMapIterator * iterator) {
    free(*iterator);
    *iterator = NULL;
}
