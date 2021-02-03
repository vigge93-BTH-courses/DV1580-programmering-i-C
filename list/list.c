#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

struct node
{
  char *key;
  int value;
  struct node *next;
};

struct node *last = NULL;
struct node *first = NULL;

int count = 0;

void insert_at_end(char *p_key, int p_value)
{
  if (p_key == NULL)
  {
    return;
  }

  struct node *t;
  int keylength = strlen(p_key) + 1;

  t = (struct node *)malloc(sizeof(struct node));
  t->key = (char *)malloc(keylength * sizeof(char));

  strcpy(t->key, p_key);
  t->value = p_value;
  t->next = NULL;

  if (first != NULL)
  {
    last->next = t;
  }
  else
  {
    first = t;
  }

  last = t;
  count++;
}

int get_element(char *p_key)
{
  struct node *t;
  t = first;

  for (int i = 0; i < count; i++)
  {
    if (t->key != NULL && strcmp(t->key, p_key) == 0)
    {
      return (t->value);
    }
    t = t->next;
  }
  return -1;
}

void print_all()
{
  struct node *t = first;
  while (t != NULL && t->key != NULL)
  {
    printf("Key: %s\tValue: %d\n", t->key, t->value);
    t = t->next;
  }
}