#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};



// Corresponds to traj_utils__msg__SwarmPrimitiveTraj

// This struct is not documented.
#[allow(missing_docs)]

#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct SwarmPrimitiveTraj {

    // This member is not documented.
    #[allow(missing_docs)]
    pub drone_id: i16,


    // This member is not documented.
    #[allow(missing_docs)]
    pub start_time: builtin_interfaces::msg::Time,


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
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::msg::rmw::SwarmPrimitiveTraj::default())
  }
}

impl rosidl_runtime_rs::Message for SwarmPrimitiveTraj {
  type RmwMsg = super::msg::rmw::SwarmPrimitiveTraj;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        drone_id: msg.drone_id,
        start_time: builtin_interfaces::msg::Time::into_rmw_message(std::borrow::Cow::Owned(msg.start_time)).into_owned(),
        start_p: msg.start_p,
        start_v: msg.start_v,
        end_p: msg.end_p,
        rot_mat: msg.rot_mat,
        vel_id: msg.vel_id,
        select_path_id: msg.select_path_id,
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
      drone_id: msg.drone_id,
        start_time: builtin_interfaces::msg::Time::into_rmw_message(std::borrow::Cow::Borrowed(&msg.start_time)).into_owned(),
        start_p: msg.start_p,
        start_v: msg.start_v,
        end_p: msg.end_p,
        rot_mat: msg.rot_mat,
      vel_id: msg.vel_id,
      select_path_id: msg.select_path_id,
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      drone_id: msg.drone_id,
      start_time: builtin_interfaces::msg::Time::from_rmw_message(msg.start_time),
      start_p: msg.start_p,
      start_v: msg.start_v,
      end_p: msg.end_p,
      rot_mat: msg.rot_mat,
      vel_id: msg.vel_id,
      select_path_id: msg.select_path_id,
    }
  }
}


// Corresponds to traj_utils__msg__Polynomial

// This struct is not documented.
#[allow(missing_docs)]

#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct Polynomial {

    // This member is not documented.
    #[allow(missing_docs)]
    pub start_time: builtin_interfaces::msg::Time,


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
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::msg::rmw::Polynomial::default())
  }
}

impl rosidl_runtime_rs::Message for Polynomial {
  type RmwMsg = super::msg::rmw::Polynomial;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        start_time: builtin_interfaces::msg::Time::into_rmw_message(std::borrow::Cow::Owned(msg.start_time)).into_owned(),
        coeff_x: msg.coeff_x,
        coeff_y: msg.coeff_y,
        coeff_z: msg.coeff_z,
        duration: msg.duration,
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        start_time: builtin_interfaces::msg::Time::into_rmw_message(std::borrow::Cow::Borrowed(&msg.start_time)).into_owned(),
        coeff_x: msg.coeff_x,
        coeff_y: msg.coeff_y,
        coeff_z: msg.coeff_z,
      duration: msg.duration,
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      start_time: builtin_interfaces::msg::Time::from_rmw_message(msg.start_time),
      coeff_x: msg.coeff_x,
      coeff_y: msg.coeff_y,
      coeff_z: msg.coeff_z,
      duration: msg.duration,
    }
  }
}


