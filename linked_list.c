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
}

void insert_at_tail(struct linked_list *list, size_t value)
{
  // insert value at tail
  if (list->head == NULL)
  {
    return new_node(value);
  }
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
  if (ptr == NULL)
  {
    return NULL;
  }
  list->head = ptr->next;
  free(ptr);
  return value;
}

size_t remove_from_tail(struct linked_list *list)
{
  // remove value from tail
  struct list_node *ptr = list->head;
  if (ptr == NULL)
  {
    return NULL;
  }
  while ((ptr->next) != NULL)
  {
    ptr = ptr->next;
  }
  size_t value = ptr->value;
  free(ptr);
  return value;
}

void free_list(struct linked_list list)
{
  // free linked list
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
