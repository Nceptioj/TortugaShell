#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "bst.h"
#include "shell.h"
#include "myStrings.h"

node *init_node()
{
	node *temp;
	temp = (node *)emalloc(sizeof(node));
	temp->left = NULL;
	temp->right = NULL;
	temp->type = 0;
	return temp;
}

void bst_insert(node *root, node *new_node)
{
	int comparison = wcscmp(new_node->title, root->title);

	// Skip if it's already there
	if (comparison == 0){
		return;
	}

	if (comparison < 0)
	{
		if (root->left == NULL) {
			root->left = new_node;
		}
		else {
			bst_insert(root->left, new_node);
		}
	} else
	{
		if (root->right == NULL) {
			root->right = new_node;
		}
		else {
			bst_insert(root->right, new_node);
		}
	}
}

node *bst_search(node *root, wchar_t *key, node **parent)
{
	node *temp;
	temp = root;
	int comparison;

	while (temp != NULL)
	{
		comparison = wcscmp(temp->title, key);
		if (comparison == 0)
		{
			return temp;
		}
		*parent = temp;

		if (comparison > 0)
			temp = temp->left;
		else
			temp = temp->right;
	}

	return NULL;
}


node *bst_partial_search(node *root, wchar_t *key, node **parent)
{
	node *temp;
	temp = root;
	int comparison;

	while (temp != NULL)
	{
		comparison = wcscmp(temp->title, key);
		if (comparison == 0 || wcswcs(temp->title, key) != NULL)
		{
			return temp;
		}
		*parent = temp;

		if (comparison > 0)
			temp = temp->left;
		else
			temp = temp->right;
	}

	return NULL;
}

void inorder(node *temp)
{
	if (temp != NULL)
	{
		inorder(temp->left);
		wprintf(L"%s", temp->description);
		inorder(temp->right);
	}
}


void bst_free(node *temp)
{
	if (temp != NULL)
	{
		bst_free(temp->left);
		free(temp->title);
		free(temp->description);
		bst_free(temp->right);
	}
	else {
		return;
	}
}