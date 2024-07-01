#pragma once

#include <stdbool.h>
#include <stddef.h>

/*
 * A "dynamic stack" data structure is a stack-like container of elements,
 * built on top of the Dynamic Packed Container (DPC) data structure.
 *
 * Elements can be of any type, but all elements must have the same type.
 * The stack stores the elements themselves (not pointers to them).
 */
struct dyn_stack;

/*
 * Create a new (empty) dynamic stack.
 * On success, returns true and the new dynamic stack is pointed by `dspp`.
 * Otherwise, returns false.
 */
bool dyn_stack_create(struct dyn_stack** dspp);

/*
 * Destroy the dynamic array pointed by `dspp`.
 * On success, returns true and `dspp` is `NULL`.
 * Otherwise, returns false.
 */
bool dyn_stack_destroy(struct dyn_stack** dspp);

/*
 * The number of elements in the dynamic stack pointed by `dsp`
 * is copied to the variable pointed by `cnt`.
 *
 * Returns true on success, false otherwise.
 */
bool dyn_stack_count(struct dyn_stack* dsp, size_t* cnt);

/*
 * Push the element pointed by `element`
 * which is of size `element_size`
 * at the top of the dynamic stack pointed by `dsp`.
 *
 * Returns true on success, false otherwise.
 */
bool dyn_stack_push(struct dyn_stack* dsp, const void* element,
                    size_t element_size);

/*
 * The element at the top of the dynamic stack pointed by `dsp`
 * is removed from the stack
 * and a copy of the element is pointed by the pointer `element`.
 *
 * Returns true on success, false otherwise.
 */
bool dyn_stack_pop(struct dyn_stack* dsp, void* element);

/*
 * A copy of the element at the top of the dynamic stack pointed by `dsp`
 * is created and pointed by the pointer `element`.
 *
 * Returns true on success, false otherwise.
 */
bool dyn_stack_peek(struct dyn_stack* dsp, void* element);
