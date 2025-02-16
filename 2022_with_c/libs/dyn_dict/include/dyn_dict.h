#pragma once

#include <stdbool.h>
#include <stddef.h>

struct dyn_dict;
struct dyn_dict_elem;

/*
 * Create a new (empty) dynamic array.
 * On success, returns true and the new dynamic array is pointed by `ddpp`.
 * Otherwise, returns false.
 */
bool dyn_dict_create(struct dyn_dict** ddpp);

/*
 * Destroy the dynamic array pointed by `ddpp`.
 * On success, returns true and `ddpp` is `NULL`.
 * Otherwise, returns false.
 */
bool dyn_dict_destroy(struct dyn_dict** ddpp);

bool dyn_dict_clear(struct dyn_dict* ddp);

bool dyn_dict_count(struct dyn_dict* ddp, size_t* cnt);

bool dyn_dict_set_key_size(struct dyn_dict* ddp, size_t key_size);

bool dyn_dict_set_value_size(struct dyn_dict* ddp, size_t value_size);

bool dyn_dict_get(struct dyn_dict* ddp, void* key, void* value);

bool dyn_dict_set(struct dyn_dict* ddp, void* key, void* value);

// bool dyn_dict_remove(struct dyn_dict* ddp, void* key);
//
// bool dyn_dict_containsKey(struct dyn_dict* ddp, void* key, bool *res);
//
// /*
//  * ddep pointer:
//  * pass in NULL to start a new iteration
//  * pass in some previously-returned value to ask for the next item
//  */
// bool dyn_dict_iterate(struct dyn_dict* ddp, struct dyn_dict_elem *ddep);
