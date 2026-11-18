// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from traj_utils:msg/Polynomial.idl
// generated code does not contain a copyright notice
#include "traj_utils/msg/detail/polynomial__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `start_time`
#include "builtin_interfaces/msg/detail/time__functions.h"

bool
traj_utils__msg__Polynomial__init(traj_utils__msg__Polynomial * msg)
{
  if (!msg) {
    return false;
  }
  // start_time
  if (!builtin_interfaces__msg__Time__init(&msg->start_time)) {
    traj_utils__msg__Polynomial__fini(msg);
    return false;
  }
  // coeff_x
  // coeff_y
  // coeff_z
  // duration
  return true;
}

void
traj_utils__msg__Polynomial__fini(traj_utils__msg__Polynomial * msg)
{
  if (!msg) {
    return;
  }
  // start_time
  builtin_interfaces__msg__Time__fini(&msg->start_time);
  // coeff_x
  // coeff_y
  // coeff_z
  // duration
}

bool
traj_utils__msg__Polynomial__are_equal(const traj_utils__msg__Polynomial * lhs, const traj_utils__msg__Polynomial * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // start_time
  if (!builtin_interfaces__msg__Time__are_equal(
      &(lhs->start_time), &(rhs->start_time)))
  {
    return false;
  }
  // coeff_x
  for (size_t i = 0; i < 6; ++i) {
    if (lhs->coeff_x[i] != rhs->coeff_x[i]) {
      return false;
    }
  }
  // coeff_y
  for (size_t i = 0; i < 6; ++i) {
    if (lhs->coeff_y[i] != rhs->coeff_y[i]) {
      return false;
    }
  }
  // coeff_z
  for (size_t i = 0; i < 6; ++i) {
    if (lhs->coeff_z[i] != rhs->coeff_z[i]) {
      return false;
    }
  }
  // duration
  if (lhs->duration != rhs->duration) {
    return false;
  }
  return true;
}

bool
traj_utils__msg__Polynomial__copy(
  const traj_utils__msg__Polynomial * input,
  traj_utils__msg__Polynomial * output)
{
  if (!input || !output) {
    return false;
  }
  // start_time
  if (!builtin_interfaces__msg__Time__copy(
      &(input->start_time), &(output->start_time)))
  {
    return false;
  }
  // coeff_x
  for (size_t i = 0; i < 6; ++i) {
    output->coeff_x[i] = input->coeff_x[i];
  }
  // coeff_y
  for (size_t i = 0; i < 6; ++i) {
    output->coeff_y[i] = input->coeff_y[i];
  }
  // coeff_z
  for (size_t i = 0; i < 6; ++i) {
    output->coeff_z[i] = input->coeff_z[i];
  }
  // duration
  output->duration = input->duration;
  return true;
}

traj_utils__msg__Polynomial *
traj_utils__msg__Polynomial__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  traj_utils__msg__Polynomial * msg = (traj_utils__msg__Polynomial *)allocator.allocate(sizeof(traj_utils__msg__Polynomial), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(traj_utils__msg__Polynomial));
  bool success = traj_utils__msg__Polynomial__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
traj_utils__msg__Polynomial__destroy(traj_utils__msg__Polynomial * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    traj_utils__msg__Polynomial__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
traj_utils__msg__Polynomial__Sequence__init(traj_utils__msg__Polynomial__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  traj_utils__msg__Polynomial * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(traj_utils__msg__Polynomial)) {
      return false;
    }
    data = (traj_utils__msg__Polynomial *)allocator.zero_allocate(size, sizeof(traj_utils__msg__Polynomial), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = traj_utils__msg__Polynomial__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        traj_utils__msg__Polynomial__fini(&data[i - 1]);
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
traj_utils__msg__Polynomial__Sequence__fini(traj_utils__msg__Polynomial__Sequence * array)
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
      traj_utils__msg__Polynomial__fini(&array->data[i]);
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

traj_utils__msg__Polynomial__Sequence *
traj_utils__msg__Polynomial__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  traj_utils__msg__Polynomial__Sequence * array = (traj_utils__msg__Polynomial__Sequence *)allocator.allocate(sizeof(traj_utils__msg__Polynomial__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = traj_utils__msg__Polynomial__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
traj_utils__msg__Polynomial__Sequence__destroy(traj_utils__msg__Polynomial__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    traj_utils__msg__Polynomial__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
traj_utils__msg__Polynomial__Sequence__are_equal(const traj_utils__msg__Polynomial__Sequence * lhs, const traj_utils__msg__Polynomial__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!traj_utils__msg__Polynomial__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
traj_utils__msg__Polynomial__Sequence__copy(
  const traj_utils__msg__Polynomial__Sequence * input,
  traj_utils__msg__Polynomial__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(traj_utils__msg__Polynomial)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(traj_utils__msg__Polynomial);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    traj_utils__msg__Polynomial * data =
      (traj_utils__msg__Polynomial *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!traj_utils__msg__Polynomial__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          traj_utils__msg__Polynomial__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!traj_utils__msg__Polynomial__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
