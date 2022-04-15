#ifndef LIST_H
#define LIST_H

class lnode
{
public:
  lnode* next;
  lnode* prev;
  void* data;
};

class list
{
public:
  void initialize();
  void add(void* data);
  long getLength();
  void* get(long position);
  void* get(long pos, long offset);
  void del(long position);

  int incPointer();
  int decPointer();
  void* getPointer();
  int setPointer(long position);
  int delPointer();
private:
  lnode* start;
  lnode* end;
  lnode* pointer;
  long length;
};

#endif
