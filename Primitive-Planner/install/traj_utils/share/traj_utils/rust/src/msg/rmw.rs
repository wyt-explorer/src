#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};


#[link(name = "traj_utils__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__traj_utils__msg__SwarmPrimitiveTraj() -> *const std::ffi::c_void;
}

#[link(name = "traj_utils__rosidl_generator_c")]
extern "C" {
    fn traj_utils__msg__SwarmPrimitiveTraj__init(msg: *mut SwarmPrimitiveTraj) -> bool;
    fn traj_utils__msg__SwarmPrimitiveTraj__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<SwarmPrimitiveTraj>, size: usize) -> bool;
    fn traj_utils__msg__SwarmPrimitiveTraj__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<SwarmPrimitiveTraj>);
    fn traj_utils__msg__SwarmPrimitiveTraj__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<SwarmPrimitiveTraj>, out_seq: *mut rosidl_runtime_rs::Sequence<SwarmPrimitiveTraj>) -> bool;
}

// Corresponds to traj_utils__msg__SwarmPrimitiveTraj
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct SwarmPrimitiveTraj {

    // This member is not documented.
    #[allow(missing_docs)]
    pub drone_id: i16,


    // This member is not documented.
    #[allow(missing_docs)]
    pub start_time: builtin_interfaces::msg::rmw::Time,


    // This member is not documented.
    #[allow(missing_docs)]
    pub start_p: [f32; 3],


    // This member is not documented.
    #[allow(missing_docs)]
    pub start_v: [f32; 3],


    // This member is not documented.
    #[allow(missing_docs)]
    pub end_p: [f32; 3],


    // This member is not documented.
    #[allow(missing_docs)]
    pub rot_mat: [f32; 9],


    // This member is not documented.
    #[allow(missing_docs)]
    pub vel_id: i32,


    // This member is not documented.
    #[allow(missing_docs)]
    pub select_path_id: i32,

}



impl Default for SwarmPrimitiveTraj {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !traj_utils__msg__SwarmPrimitiveTraj__init(&mut msg as *mut _) {
        panic!("Call to traj_utils__msg__SwarmPrimitiveTraj__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for SwarmPrimitiveTraj {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { traj_utils__msg__SwarmPrimitiveTraj__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { traj_utils__msg__SwarmPrimitiveTraj__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { traj_utils__msg__SwarmPrimitiveTraj__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for SwarmPrimitiveTraj {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for SwarmPrimitiveTraj where Self: Sized {
  const TYPE_NAME: &'static str = "traj_utils/msg/SwarmPrimitiveTraj";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__traj_utils__msg__SwarmPrimitiveTraj() }
  }
}


#[link(name = "traj_utils__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__traj_utils__msg__Polynomial() -> *const std::ffi::c_void;
}

#[link(name = "traj_utils__rosidl_generator_c")]
extern "C" {
    fn traj_utils__msg__Polynomial__init(msg: *mut Polynomial) -> bool;
    fn traj_utils__msg__Polynomial__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<Polynomial>, size: usize) -> bool;
    fn traj_utils__msg__Polynomial__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<Polynomial>);
    fn traj_utils__msg__Polynomial__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<Polynomial>, out_seq: *mut rosidl_runtime_rs::Sequence<Polynomial>) -> bool;
}

// Corresponds to traj_utils__msg__Polynomial
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct Polynomial {

    // This member is not documented.
    #[allow(missing_docs)]
    pub start_time: builtin_interfaces::msg::rmw::Time,


    // This member is not documented.
    #[allow(missing_docs)]
    pub coeff_x: [f64; 6],


    // This member is not documented.
    #[allow(missing_docs)]
    pub coeff_y: [f64; 6],


    // This member is not documented.
    #[allow(missing_docs)]
    pub coeff_z: [f64; 6],


    // This member is not documented.
    #[allow(missing_docs)]
    pub duration: f64,

}



impl Default for Polynomial {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !traj_utils__msg__Polynomial__init(&mut msg as *mut _) {
        panic!("Call to traj_utils__msg__Polynomial__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for Polynomial {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { traj_utils__msg__Polynomial__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { traj_utils__msg__Polynomial__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { traj_utils__msg__Polynomial__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for Polynomial {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for Polynomial where Self: Sized {
  const TYPE_NAME: &'static str = "traj_utils/msg/Polynomial";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__traj_utils__msg__Polynomial() }
  }
}


