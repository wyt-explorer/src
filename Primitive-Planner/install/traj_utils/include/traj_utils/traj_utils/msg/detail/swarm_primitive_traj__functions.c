// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from traj_utils:msg/SwarmPrimitiveTraj.idl
// generated code does not contain a copyright notice
#include "traj_utils/msg/detail/swarm_primitive_traj__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `start_time`
#include "builtin_interfaces/msg/detail/time__functions.h"

bool
traj_utils__msg__SwarmPrimitiveTraj__init(traj_utils__msg__SwarmPrimitiveTraj * msg)
{
  if (!msg) {
    return false;
  }
  // drone_id
  // start_time
  if (!builtin_interfaces__msg__Time__init(&msg->start_time)) {
    traj_utils__msg__SwarmPrimitiveTraj__fini(msg);
    return false;
  }
  // start_p
  // start_v
  // end_p
  // rot_mat
  // vel_id
  // select_path_id
  return true;
}

void
traj_utils__msg__SwarmPrimitiveTraj__fini(traj_utils__msg__SwarmPrimitiveTraj * msg)
{
  if (!msg) {
    return;
  }
  // drone_id
  // start_time
  builtin_interfaces__msg__Time__fini(&msg->start_time);
  // start_p
  // start_v
  // end_p
  // rot_mat
  // vel_id
  // select_path_id
}

bool
traj_utils__msg__SwarmPrimitiveTraj__are_equal(const traj_utils__msg__SwarmPrimitiveTraj * lhs, const traj_utils__msg__SwarmPrimitiveTraj * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // drone_id
  if (lhs->drone_id != rhs->drone_id) {
    return false;
  }
  // start_time
  if (!builtin_interfaces__msg__Time__are_equal(
      &(lhs->start_time), &(rhs->start_time)))
  {
    return false;
  }
  // start_p
  for (size_t i = 0; i < 3; ++i) {
    if (lhs->start_p[i] != rhs->start_p[i]) {
      return false;
    }
  }
  // start_v
  for (size_t i = 0; i < 3; ++i) {
    if (lhs->start_v[i] != rhs->start_v[i]) {
      return false;
    }
  }
  // end_p
  for (size_t i = 0; i < 3; ++i) {
    if (lhs->end_p[i] != rhs->end_p[i]) {
      return false;
    }
  }
  // rot_mat
  for (size_t i = 0; i < 9; ++i) {
    if (lhs->rot_mat[i] != rhs->rot_mat[i]) {
      return false;
    }
  }
  // vel_id
  if (lhs->vel_id != rhs->vel_id) {
    return false;
  }
  // select_path_id
  if (lhs->select_path_id != rhs->select_path_id) {
    return false;
  }
  return true;
}

bool
traj_utils__msg__SwarmPrimitiveTraj__copy(
  const traj_utils__msg__SwarmPrimitiveTraj * input,
  traj_utils__msg__SwarmPrimitiveTraj * output)
{
  if (!input || !output) {
    return false;
  }
  // drone_id
  output->drone_id = input->drone_id;
  // start_time
  if (!builtin_interfaces__msg__Time__copy(
      &(input->start_time), &(output->start_time)))
  {
    return false;
  }
  // start_p
  for (size_t i = 0; i < 3; ++i) {
    output->start_p[i] = input->start_p[i];
  }
  // start_v
  for (size_t i = 0; i < 3; ++i) {
    output->start_v[i] = input->start_v[i];
  }
  // end_p
  for (size_t i = 0; i < 3; ++i) {
    output->end_p[i] = input->end_p[i];
  }
  // rot_mat
  for (size_t i = 0; i < 9; ++i) {
    output->rot_mat[i] = input->rot_mat[i];
  }
  // vel_id
  output->vel_id = input->vel_id;
  // select_path_id
  output->select_path_id = input->select_path_id;
  return true;
}

traj_utils__msg__SwarmPrimitiveTraj *
traj_utils__msg__SwarmPrimitiveTraj__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  traj_utils__msg__SwarmPrimitiveTraj * msg = (traj_utils__msg__SwarmPrimitiveTraj *)allocator.allocate(sizeof(traj_utils__msg__SwarmPrimitiveTraj), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(traj_utils__msg__SwarmPrimitiveTraj));
  bool success = traj_utils__msg__SwarmPrimitiveTraj__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
traj_utils__msg__SwarmPrimitiveTraj__destroy(traj_utils__msg__SwarmPrimitiveTraj * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    traj_utils__msg__SwarmPrimitiveTraj__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
traj_utils__msg__SwarmPrimitiveTraj__Sequence__init(traj_utils__msg__SwarmPrimitiveTraj__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  traj_utils__msg__SwarmPrimitiveTraj * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(traj_utils__msg__SwarmPrimitiveTraj)) {
      return false;
    }
    data = (traj_utils__msg__SwarmPrimitiveTraj *)allocator.zero_allocate(size, sizeof(traj_utils__msg__SwarmPrimitiveTraj), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = traj_utils__msg__SwarmPrimitiveTraj__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        traj_utils__msg__SwarmPrimitiveTraj__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
traj_utils__msg__SwarmPrimitiveTraj__Sequence__fini(traj_utils__msg__SwarmPrimitiveTraj__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      traj_utils__msg__SwarmPrimitiveTraj__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

traj_utils__msg__SwarmPrimitiveTraj__Sequence *
traj_utils__msg__SwarmPrimitiveTraj__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  traj_utils__msg__SwarmPrimitiveTraj__Sequence * array = (traj_utils__msg__SwarmPrimitiveTraj__Sequence *)allocator.allocate(sizeof(traj_utils__msg__SwarmPrimitiveTraj__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = traj_utils__msg__SwarmPrimitiveTraj__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
traj_utils__msg__SwarmPrimitiveTraj__Sequence__destroy(traj_utils__msg__SwarmPrimitiveTraj__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    traj_utils__msg__SwarmPrimitiveTraj__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
traj_utils__msg__SwarmPrimitiveTraj__Sequence__are_equal(const traj_utils__msg__SwarmPrimitiveTraj__Sequence * lhs, const traj_utils__msg__SwarmPrimitiveTraj__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!traj_utils__msg__SwarmPrimitiveTraj__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
traj_utils__msg__SwarmPrimitiveTraj__Sequence__copy(
  const traj_utils__msg__SwarmPrimitiveTraj__Sequence * input,
  traj_utils__msg__SwarmPrimitiveTraj__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(traj_utils__msg__SwarmPrimitiveTraj)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(traj_utils__msg__SwarmPrimitiveTraj);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    traj_utils__msg__SwarmPrimitiveTraj * data =
      (traj_utils__msg__SwarmPrimitiveTraj *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!traj_utils__msg__SwarmPrimitiveTraj__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          traj_utils__msg__SwarmPrimitiveTraj__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!traj_utils__msg__SwarmPrimitiveTraj__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
