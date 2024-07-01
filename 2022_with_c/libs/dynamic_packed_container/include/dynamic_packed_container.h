#pragma once

#include <stdbool.h>
#include <stddef.h>

/*
 * A Dynamic Packed Container (DPC) data structure is a container for elements.
 *
 * A user of a DPC:
 *
 * - must specify up-front the size of the elements to be stored in the DPC
 *   with dpc_set_element_size()
 *
 * - must control the size of the DPC (how many elements can fit in) with
 *   dpc_get_capacity() and dpc_double_capacity()
 *
 * - can put (copy in) elements into the DPC with dpc_put()
 *
 * - can get (copy out) elements from the DPC with dpc_get()
 *
 * - must specify a valid position when getting/putting
 *
 * - can erase (remove) the last element from the DPC with dpc_erase()
 *   (a DPC cannot have any "holes")
 *
 * Under the hood, a DPC is just a resizable block of consecutive memory.
 * The elements contained in a DPC are stored directly in the DPC memory,
 * one right the other (that is, the memory of a DPC stores the elements
 * themselves, not pointers to them). Elements can be of any type, but all
 * elements in a DPC must have the same size.
 *
 * The struct dynamic_packed_container is an opaque / incomplete struct.
 * The implementation of the struct is hidden from the user of the struct.
 * Users of the struct can maintain pointers (opaque pointers) to the
 * struct and can use the provided functions to interact with the struct.
 * The struct size is not visible to the user,
 * and thus static initialization is not available for users.
 * Users of the struct do not (need to) know the implementation of the struct.
 * See also:
 * https://stackoverflow.com/q/2301454
 * https://stackoverflow.com/q/7553750
 * https://en.wikipedia.org/wiki/Opaque_pointer#C
 *
 * A note on typedef.
 * Linus Torvalds says[1] that using typedef should be avoided,
 * and using "struct xxx" should be enough,
 * unless there is a reason for the type abstraction
 * (for example the underlying type implementation is architecture-specific).
 * Using typedef to save typing is not a good reason.
 * Using typedef to make lines shorter is not a good reason.
 * Using typedef to make code look prettier is not a good reason.
 * Also never ever make the pointerness part of the typedef.
 * [1] Linus on Typedefs: https://yarchive.net/comp/linux/typedefs.html
 * See also:
 * https://www.reddit.com/r/programming/comments/dr0n5/linus_on_typedef_counterproductivity/
 */
struct dynamic_packed_container;

/*
 * Create a new dynamic packed container.
 * On success, returns true and the new DPC is pointed by `dpcpp`.
 * Otherwise, returns false.
 */
bool dpc_create(struct dynamic_packed_container** dpcpp);

/*
 * Destroy the dynamic packed container pointed by `dpcp`.
 * On success, returns true and `dpcpp` is `NULL`.
 * Otherwise, returns false.
 */
bool dpc_destroy(struct dynamic_packed_container** dpcpp);

/*
 * Set the size of elements to be stored in `dpcp`.
 * Returns true on success, false otherwise.
 */
bool dpc_set_element_size(struct dynamic_packed_container* dpcp,
                          size_t element_size);

/*
 * Put in `cap` the maximum number of elements that can fit in `dpcp`.
 * Returns true on success, false otherwise.
 */
bool dpc_get_capacity(struct dynamic_packed_container* dpcp, size_t* cap);

/*
 * Double the capacity of `dpcp`.
 * If current capacity is zero elements, new capacity is one element.
 * Returns true on success, false otherwise.
 */
bool dpc_double_capacity(struct dynamic_packed_container* dpcp);

/*
 * Put in `cnt` the number of elements currently in `dpcp`.
 * Returns true on success, false otherwise.
 */
bool dpc_element_count(struct dynamic_packed_container* dpcp, size_t* cnt);

/*
 * Copy the `index`-th element (0 based) in `dpcp` to `element`.
 * Returns true on success, false otherwise.
 */
bool dpc_get(struct dynamic_packed_container* dpcp, size_t index,
             void* element);

/*
 * Copy `element` to the `index`-th position (0 based) in `dpcp`.
 * Returns true on success, false otherwise.
 */
bool dpc_put(struct dynamic_packed_container* dpcp, size_t index,
             const void* element);

/*
 * Erase the last element from `dpcp`.
 * Returns true on success, false otherwise.
 */
bool dpc_erase(struct dynamic_packed_container* dpcp);

/*
 * Sort `dpcp` in place. Compare the elements using `element_compare_function`.
 * Returns true on success, false otherwise.
 */
bool dpc_sort(struct dynamic_packed_container* dpcp,
              int (*element_compare_function)(const void* a, const void* b));
