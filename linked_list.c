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
  // insert value at head
  struct list_node *ptr = new_node(value);
  ptr->next = list->head;

  // update new head
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
  if (list == NULL || list->head == NULL)
  {
    return 0;
  }
  struct list_node *ptr = list->head;
  size_t value = ptr->value;

  // place head's child as new head
  list->head = ptr->next;
  free(ptr);
  return value;
}

size_t remove_from_tail(struct linked_list *list)
{
  if (list == NULL || list->head == NULL)
  {
    return 0;
  }
  // remove value from tail
  struct list_node *parent = list->head;
  struct list_node *ptr = list->head;

  // printf("%ld", ptr->value);
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
  // free linked list
  if (list.head == NULL)
  {
    return;
  }
  struct list_node *ptr1 = list.head;
  struct list_node *ptr2 = list.head;
  while (ptr1->next != NULL)
  {
    ptr1 = ptr1->next;
    free(ptr2);
    ptr2 = ptr1;
  }
  free(ptr1);
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
