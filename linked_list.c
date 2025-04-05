#include "linked_list.h"

#include <stdlib.h>

struct list_node *new_node(size_t value)
{
  // creates node with next = NULL and value = value
  struct list_node *node = malloc(sizeof(struct list_node));
  if (!node)
  {
    return NULL;
  }
  node->value = value;
  node->next = NULL;
  return node;
}

void insert_at_head(struct linked_list *list, size_t value)
{
  struct list_node *ptr = new_node(value);
  if (!ptr)
    return;
  ptr->next = list->head;
  list->head = ptr;
}

void insert_at_tail(struct linked_list *list, size_t value)
{
  // insert value at tail
  if (list->head == NULL)
  {
    // create new node and insert at head
    struct list_node *ptr = new_node(value);
    list->head = ptr;
    return;
  }

  // else case where there is something in ll
  struct list_node *ptr = list->head;
  while ((ptr->next) != NULL)
  {
    ptr = ptr->next;
  }
  ptr->next = new_node(value);
}

size_t remove_from_head(struct linked_list *list)
{
  // remove value from head
  struct list_node *ptr = list->head;
  size_t value = ptr->value;

  // if ll is empty, do nothing
  if (ptr == NULL)
  {
    return 0;
  }
  // place head's child as new head
  list->head = ptr->next;
  free(ptr);
  return value;
}

size_t remove_from_tail(struct linked_list *list)
{
  // remove value from tail
  struct list_node *parent = list->head;
  struct list_node *ptr = list->head;
  if (ptr == NULL)
  {
    return 0;
  }

  // walk to the end of the ll
  while ((ptr->next) != NULL)
  {
    parent = ptr;
    ptr = ptr->next;
  }

  // if head is last, set head to null; otherwise disconnect child from parent
  if (ptr == list->head)
  {
    list->head = NULL;
  }
  else
  {
    parent->next = NULL;
  }
  size_t value = ptr->value;
  free(ptr);
  return value;
}

void free_list(struct linked_list list)
{
  struct list_node *cur = list.head;
  while (cur)
  {
    struct list_node *next = cur->next;
    free(cur);
    cur = next;
  }
}

// Utility function to help you debugging, do not modify
void dump_list(FILE *fp, struct linked_list list)
{
  fprintf(fp, "[ ");
  for (struct list_node *cur = list.head; cur != NULL; cur = cur->next)
  {
    fprintf(fp, "%zu ", cur->value);
  }
  fprintf(fp, "]\n");
}
