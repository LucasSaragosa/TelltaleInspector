#ifndef LUADEC_STRUCTS_H
#define LUADEC_STRUCTS_H

/*
** List is a doubly-linked list
*/
typedef struct ListItem_ ListItem;
struct ListItem_ {
	ListItem* next;
	ListItem* prev;
};

typedef struct List_ LDList;
struct List_ {
	ListItem* head;
	ListItem* tail;
	int size;
};

typedef void (*ListItemFn)(ListItem*, void*);
typedef int (*ListItemCmpFn)(ListItem*, const void*);

LDList* NewList();
void InitList(LDList* list);
void AddToList(LDList* list, ListItem* item);
void AddToListHead(LDList* list, ListItem* item);
ListItem* FirstItem(LDList* list);
ListItem* LastItem(LDList* list);
ListItem* PopFromList(LDList* list);
void LoopList(LDList* list, ListItemFn fn, void* param);
void ClearList(LDList* list, ListItemFn fn);
ListItem* FindFromListHead(LDList* list, ListItemCmpFn cmp, const void* sample);
ListItem* FindFromListTail(LDList* list, ListItemCmpFn cmp, const void* sample);
ListItem* RemoveFromList(LDList* list, ListItem* item);
int AddAllAfterListItem(LDList* list, ListItem* pos, ListItem* item);
int AddAllBeforeListItem(LDList* list, ListItem* pos, ListItem* item);

/*
** IntSet is a Set that contains int
*/
typedef struct IntSet_ IntSet;
struct IntSet_ {
	LDList list;
	int mayRepeat;
};

typedef struct IntSetItem_ IntSetItem;
struct IntSetItem_ {
	ListItem super;
	int value;
};

#define SET_SIZE(s) ((s)->list.size)
#define SET_IS_EMPTY(s) ((s)->list.size == 0)

IntSet* NewIntSet(int mayRepeat);
void InitIntSet(IntSet* set, int mayRepeat);
void DeleteIntSet(IntSet* set);
int AddToSet(IntSet* set, int a);
int PeekSet(IntSet* set, int a);
int PopSet(IntSet* set);
int RemoveFromSet(IntSet* set, int a);

/*
** a Stack that use List
*/
typedef struct VarListItem_ VarListItem;
struct VarListItem_ {
	ListItem super;
	char* dest;
	char* src;
	int reg;
};

void AddToVarList(LDList* stack, char* dest, char* src, int reg);
void ClearVarListItem(VarListItem* item, void* dummy);

#endif // #ifndef LUADEC_STRUCTS_H
