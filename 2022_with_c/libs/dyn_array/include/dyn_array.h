#pragma once

#include <stdbool.h>
#include <stddef.h>

/*
 * A "dynamic array" data structure is an array-like container of elements,
 * built on top of the Dynamic Packed Container (DPC) data structure.
 *
 * Elements can be of any type, but all elements must have the same type.
 * The array stores the elements themselves (not pointers to them).
 *
 * Appending elements at the end is supported.
 * Access at a given index is supported.
 */
struct dyn_array;

/*
 * Create a new (empty) dynamic array.
 * On success, returns true and the new dynamic array is pointed by `dapp`.
 * Otherwise, returns false.
 */
bool dyn_array_create(struct dyn_array** dapp);

/*
 * Destroy the dynamic array pointed by `dapp`.
 * On success, returns true and `dapp` is `NULL`.
 * Otherwise, returns false.
 */
bool dyn_array_destroy(struct dyn_array** dapp);

/*
 * Add the element pointed by `element`
 * which is of size `element_size`
 * at the end of the dynamic array pointed by `dap`.
 *
 * Returns true on success, false otherwise.
 */
bool dyn_array_append(struct dyn_array* dap, const void* element,
                      size_t element_size);

/*
 * The number of elements in the dynamic array pointed by `dap`
 * is copied to the variable pointed by `cnt`.
 *
 * Returns true on success, false otherwise.
 */
bool dyn_array_count(struct dyn_array* dap, size_t* cnt);

/*
 * The pointer `element` points at a copy of the element
 * found at position `index`
 * in the dynamic array pointed by `dap`.
 *
 * Returns true on success, false otherwise.
 */
bool dyn_array_get(struct dyn_array* dap, size_t index, void* element);

/*
 * Sort the dynamic array pointed by `dap` is sorted, in place.
 * To compare the elements in the dynamic array
 * use the function pointed by `element_compare_function`.
 *
 * Returns true on success, false otherwise.
 */
bool dyn_array_sort(struct dyn_array* dap,
                    int (*element_compare_function)(const void* a,
                                                    const void* b));
