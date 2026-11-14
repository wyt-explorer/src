// Auto-generated. Do not edit!

// (in-package traj_utils.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;

//-----------------------------------------------------------

class swarmPrimitiveTraj {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.drone_id = null;
      this.start_time = null;
      this.start_p = null;
      this.start_v = null;
      this.end_p = null;
      this.rot_mat = null;
      this.vel_id = null;
      this.select_path_id = null;
    }
    else {
      if (initObj.hasOwnProperty('drone_id')) {
        this.drone_id = initObj.drone_id
      }
      else {
        this.drone_id = 0;
      }
      if (initObj.hasOwnProperty('start_time')) {
        this.start_time = initObj.start_time
      }
      else {
        this.start_time = {secs: 0, nsecs: 0};
      }
      if (initObj.hasOwnProperty('start_p')) {
        this.start_p = initObj.start_p
      }
      else {
        this.start_p = new Array(3).fill(0);
      }
      if (initObj.hasOwnProperty('start_v')) {
        this.start_v = initObj.start_v
      }
      else {
        this.start_v = new Array(3).fill(0);
      }
      if (initObj.hasOwnProperty('end_p')) {
        this.end_p = initObj.end_p
      }
      else {
        this.end_p = new Array(3).fill(0);
      }
      if (initObj.hasOwnProperty('rot_mat')) {
        this.rot_mat = initObj.rot_mat
      }
      else {
        this.rot_mat = new Array(9).fill(0);
      }
      if (initObj.hasOwnProperty('vel_id')) {
        this.vel_id = initObj.vel_id
      }
      else {
        this.vel_id = 0;
      }
      if (initObj.hasOwnProperty('select_path_id')) {
        this.select_path_id = initObj.select_path_id
      }
      else {
        this.select_path_id = 0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type swarmPrimitiveTraj
    // Serialize message field [drone_id]
    bufferOffset = _serializer.int16(obj.drone_id, buffer, bufferOffset);
    // Serialize message field [start_time]
    bufferOffset = _serializer.time(obj.start_time, buffer, bufferOffset);
    // Check that the constant length array field [start_p] has the right length
    if (obj.start_p.length !== 3) {
      throw new Error('Unable to serialize array field start_p - length must be 3')
    }
    // Serialize message field [start_p]
    bufferOffset = _arraySerializer.float32(obj.start_p, buffer, bufferOffset, 3);
    // Check that the constant length array field [start_v] has the right length
    if (obj.start_v.length !== 3) {
      throw new Error('Unable to serialize array field start_v - length must be 3')
    }
    // Serialize message field [start_v]
    bufferOffset = _arraySerializer.float32(obj.start_v, buffer, bufferOffset, 3);
    // Check that the constant length array field [end_p] has the right length
    if (obj.end_p.length !== 3) {
      throw new Error('Unable to serialize array field end_p - length must be 3')
    }
    // Serialize message field [end_p]
    bufferOffset = _arraySerializer.float32(obj.end_p, buffer, bufferOffset, 3);
    // Check that the constant length array field [rot_mat] has the right length
    if (obj.rot_mat.length !== 9) {
      throw new Error('Unable to serialize array field rot_mat - length must be 9')
    }
    // Serialize message field [rot_mat]
    bufferOffset = _arraySerializer.float32(obj.rot_mat, buffer, bufferOffset, 9);
    // Serialize message field [vel_id]
    bufferOffset = _serializer.int32(obj.vel_id, buffer, bufferOffset);
    // Serialize message field [select_path_id]
    bufferOffset = _serializer.int32(obj.select_path_id, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type swarmPrimitiveTraj
    let len;
    let data = new swarmPrimitiveTraj(null);
    // Deserialize message field [drone_id]
    data.drone_id = _deserializer.int16(buffer, bufferOffset);
    // Deserialize message field [start_time]
    data.start_time = _deserializer.time(buffer, bufferOffset);
    // Deserialize message field [start_p]
    data.start_p = _arrayDeserializer.float32(buffer, bufferOffset, 3)
    // Deserialize message field [start_v]
    data.start_v = _arrayDeserializer.float32(buffer, bufferOffset, 3)
    // Deserialize message field [end_p]
    data.end_p = _arrayDeserializer.float32(buffer, bufferOffset, 3)
    // Deserialize message field [rot_mat]
    data.rot_mat = _arrayDeserializer.float32(buffer, bufferOffset, 9)
    // Deserialize message field [vel_id]
    data.vel_id = _deserializer.int32(buffer, bufferOffset);
    // Deserialize message field [select_path_id]
    data.select_path_id = _deserializer.int32(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 90;
  }

  static datatype() {
    // Returns string type for a message object
    return 'traj_utils/swarmPrimitiveTraj';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'e581206f10087d6877710b9c852f330f';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    int16 drone_id
    time start_time
    float32[3] start_p
    float32[3] start_v
    float32[3] end_p
    float32[9] rot_mat
    int32 vel_id
    int32 select_path_id
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new swarmPrimitiveTraj(null);
    if (msg.drone_id !== undefined) {
      resolved.drone_id = msg.drone_id;
    }
    else {
      resolved.drone_id = 0
    }

    if (msg.start_time !== undefined) {
      resolved.start_time = msg.start_time;
    }
    else {
      resolved.start_time = {secs: 0, nsecs: 0}
    }

    if (msg.start_p !== undefined) {
      resolved.start_p = msg.start_p;
    }
    else {
      resolved.start_p = new Array(3).fill(0)
    }

    if (msg.start_v !== undefined) {
      resolved.start_v = msg.start_v;
    }
    else {
      resolved.start_v = new Array(3).fill(0)
    }

    if (msg.end_p !== undefined) {
      resolved.end_p = msg.end_p;
    }
    else {
      resolved.end_p = new Array(3).fill(0)
    }

    if (msg.rot_mat !== undefined) {
      resolved.rot_mat = msg.rot_mat;
    }
    else {
      resolved.rot_mat = new Array(9).fill(0)
    }

    if (msg.vel_id !== undefined) {
      resolved.vel_id = msg.vel_id;
    }
    else {
      resolved.vel_id = 0
    }

    if (msg.select_path_id !== undefined) {
      resolved.select_path_id = msg.select_path_id;
    }
    else {
      resolved.select_path_id = 0
    }

    return resolved;
    }
};

module.exports = swarmPrimitiveTraj;
