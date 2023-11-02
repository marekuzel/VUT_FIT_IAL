#include "btree.h"
#include "test_util.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
const int base_data_count = 15;
const char base_keys[] = {'H', 'D', 'L', 'B', 'F', 'J', 'N', 'A',
                          'C', 'E', 'G', 'I', 'K', 'M', 'O'};
const int base_values[] = {8, 4, 12, 2, 6, 10, 14, 1, 3, 5, 7, 9, 11, 13, 16};

const int additional_data_count = 6;
const char additional_keys[] = {'S', 'R', 'Q', 'P', 'X', 'Y', 'Z'};
const int additional_values[] = {10, 10, 10, 10, 10, 10};

const int traversal_data_count = 5;
const char traversal_keys[] = {'D', 'B', 'A', 'C', 'E'};
const int traversal_values[] = {1, 2, 3, 4, 5};

void init_test() {
  printf("Binary Search Tree - testing script\n");
  printf("-----------------------------------\n");
  printf("\n");
}

TEST(test_tree_init, "Initialize the tree")
bst_init(&test_tree);
ENDTEST

TEST(test_tree_dispose_empty, "Dispose the tree")
bst_init(&test_tree);
bst_dispose(&test_tree);
ENDTEST

TEST(test_tree_search_empty, "Search in an empty tree (A)")
bst_init(&test_tree);
int result;
bool found = bst_search(test_tree, 'A', &result);
assert(!found);
bst_print_tree(test_tree);
ENDTEST

TEST(test_tree_insert_root, "Insert an item (H,1)")
bst_init(&test_tree);
bst_insert(&test_tree, 'H', 1);
assert(tree_invariant(test_tree));
assert(test_tree != NULL);
assert(test_tree->key == 'H');
assert(test_tree->value == 1);
assert(test_tree->left == NULL);
assert(test_tree->right == NULL);
bst_print_tree(test_tree);
ENDTEST

TEST(test_tree_search_root, "Search in a single node tree (H)")
bst_init(&test_tree);
bst_insert(&test_tree, 'H', 1);
int result;
bool found = bst_search(test_tree, 'H', &result);
assert(found);
assert(result == 1);
bst_print_tree(test_tree);
ENDTEST

TEST(test_tree_update_root, "Update a node in a single node tree (H,1)->(H,8)")
bst_init(&test_tree);
bst_insert(&test_tree, 'H', 1);
bst_print_tree(test_tree);
bst_insert(&test_tree, 'H', 8);
assert(test_tree->value == 8);
bst_print_tree(test_tree);
ENDTEST

TEST(test_tree_insert_many, "Insert many values")
bst_init(&test_tree);
bst_insert_many(&test_tree, base_keys, base_values, base_data_count);
assert(tree_invariant(test_tree));
bst_print_tree(test_tree);
ENDTEST

TEST(test_tree_search, "Search for an item deeper in the tree (A)")
bst_init(&test_tree);
bst_insert_many(&test_tree, base_keys, base_values, base_data_count);
int result;
bool found = bst_search(test_tree, 'A', &result);
assert(result == 1);
assert(found);
bst_print_tree(test_tree);
ENDTEST

TEST(test_tree_search_missing, "Search for a missing key (X)")
bst_init(&test_tree);
bst_insert_many(&test_tree, base_keys, base_values, base_data_count);
int result;
bool found = bst_search(test_tree, 'X', &result);
assert(!found);
bst_print_tree(test_tree);
ENDTEST



TEST(test_tree_delete_leaf, "Delete a leaf node (A)")
bst_init(&test_tree);
bst_insert_many(&test_tree, base_keys, base_values, base_data_count);
bst_print_tree(test_tree);
bst_delete(&test_tree, 'A');
assert(tree_invariant(test_tree));
bst_print_tree(test_tree);
ENDTEST

TEST(test_tree_delete_left_subtree, "Delete a node with only left subtree (R)")
bst_init(&test_tree);
bst_insert_many(&test_tree, base_keys, base_values, base_data_count);
bst_insert_many(&test_tree, additional_keys, additional_values,
                additional_data_count);
bst_print_tree(test_tree);
bst_delete(&test_tree, 'R');
int value;
assert(!bst_search(test_tree,'R',&value));
assert(tree_invariant(test_tree));
bst_print_tree(test_tree);
ENDTEST

TEST(test_tree_delete_right_subtree,
     "Delete a node with only right subtree (X)")
bst_init(&test_tree);
bst_insert_many(&test_tree, base_keys, base_values, base_data_count);
bst_insert_many(&test_tree, additional_keys, additional_values,
                additional_data_count);

bst_print_tree(test_tree);
bst_delete(&test_tree, 'X');
int value;
assert(!bst_search(test_tree,'X',&value));
assert(tree_invariant(test_tree));
bst_print_tree(test_tree);
ENDTEST

TEST(test_tree_delete_both_subtrees, "Delete a node with both subtrees (L)")
bst_init(&test_tree);
bst_insert_many(&test_tree, base_keys, base_values, base_data_count);
bst_insert_many(&test_tree, additional_keys, additional_values,
                additional_data_count);
bst_print_tree(test_tree);
bst_delete(&test_tree, 'L');
assert(tree_invariant(test_tree));
int value;
assert(!bst_search(test_tree,'L',&value));
bst_print_tree(test_tree);
ENDTEST

TEST(test_tree_delete_missing, "Delete a node that doesn't exist (U)")
bst_init(&test_tree);
bst_insert_many(&test_tree, base_keys, base_values, base_data_count);
bst_print_tree(test_tree);
bst_delete(&test_tree, 'U');
bst_print_tree(test_tree);
ENDTEST

TEST(test_tree_delete_root, "Delete the root node (H)")
bst_init(&test_tree);
bst_insert_many(&test_tree, base_keys, base_values, base_data_count);
bst_print_tree(test_tree);
bst_delete(&test_tree, 'H');
assert(tree_invariant(test_tree));
int value;
assert(!bst_search(test_tree,'H',&value));
bst_print_tree(test_tree);
ENDTEST

TEST(test_tree_dispose_filled, "Dispose the whole tree")
bst_init(&test_tree);
bst_insert_many(&test_tree, base_keys, base_values, base_data_count);
bst_print_tree(test_tree);
bst_dispose(&test_tree);
assert(test_tree == NULL);
bst_print_tree(test_tree);
ENDTEST

TEST(test_tree_preorder, "Traverse the tree using preorder")
bst_init(&test_tree);
bst_insert_many(&test_tree, traversal_keys, traversal_values, traversal_data_count);
bst_preorder(test_tree, test_items);
bst_print_tree(test_tree);
bst_print_items(test_items);
ENDTEST

TEST(test_tree_inorder, "Traverse the tree using inorder")
bst_init(&test_tree);
bst_insert_many(&test_tree, traversal_keys, traversal_values, traversal_data_count);
bst_inorder(test_tree, test_items);
bst_print_tree(test_tree);
bst_print_items(test_items);
ENDTEST

TEST(test_tree_postorder, "Traverse the tree using postorder")
bst_init(&test_tree);
bst_insert_many(&test_tree, traversal_keys, traversal_values, traversal_data_count);
bst_postorder(test_tree, test_items);
bst_print_tree(test_tree);
bst_print_items(test_items);
ENDTEST

#ifdef EXA

TEST(test_letter_count, "Count letters");
bst_init(&test_tree);
letter_count(&test_tree, "abBcCc_ 123 *");
bst_print_tree(test_tree);
ENDTEST

TEST(test_balance, "Count letters and balance");
bst_init(&test_tree);
letter_count(&test_tree, "abBcCc_ 123 *");
bst_balance(&test_tree);
assert(tree_invariant(test_tree));
bst_print_tree(test_tree);
ENDTEST

#endif // EXA

int main(int argc, char *argv[]) {
  init_test();

  test_tree_init();
  test_tree_dispose_empty();
  test_tree_search_empty();
  test_tree_insert_root();
  test_tree_search_root();
  test_tree_update_root();
  test_tree_insert_many();
  test_tree_search();
  test_tree_search_missing();
  test_tree_delete_leaf();
  test_tree_delete_left_subtree();
  test_tree_delete_right_subtree();
  test_tree_delete_both_subtrees();
  test_tree_delete_missing();
  test_tree_delete_root();
  test_tree_dispose_filled();
  test_tree_preorder();
  test_tree_inorder();
  test_tree_postorder();

#ifdef EXA
  test_letter_count();
  test_balance();
#endif // EXA
}
