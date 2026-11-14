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

class Polynomial {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.start_time = null;
      this.coeff_x = null;
      this.coeff_y = null;
      this.coeff_z = null;
      this.duration = null;
    }
    else {
      if (initObj.hasOwnProperty('start_time')) {
        this.start_time = initObj.start_time
      }
      else {
        this.start_time = {secs: 0, nsecs: 0};
      }
      if (initObj.hasOwnProperty('coeff_x')) {
        this.coeff_x = initObj.coeff_x
      }
      else {
        this.coeff_x = new Array(6).fill(0);
      }
      if (initObj.hasOwnProperty('coeff_y')) {
        this.coeff_y = initObj.coeff_y
      }
      else {
        this.coeff_y = new Array(6).fill(0);
      }
      if (initObj.hasOwnProperty('coeff_z')) {
        this.coeff_z = initObj.coeff_z
      }
      else {
        this.coeff_z = new Array(6).fill(0);
      }
      if (initObj.hasOwnProperty('duration')) {
        this.duration = initObj.duration
      }
      else {
        this.duration = 0.0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type Polynomial
    // Serialize message field [start_time]
    bufferOffset = _serializer.time(obj.start_time, buffer, bufferOffset);
    // Check that the constant length array field [coeff_x] has the right length
    if (obj.coeff_x.length !== 6) {
      throw new Error('Unable to serialize array field coeff_x - length must be 6')
    }
    // Serialize message field [coeff_x]
    bufferOffset = _arraySerializer.float64(obj.coeff_x, buffer, bufferOffset, 6);
    // Check that the constant length array field [coeff_y] has the right length
    if (obj.coeff_y.length !== 6) {
      throw new Error('Unable to serialize array field coeff_y - length must be 6')
    }
    // Serialize message field [coeff_y]
    bufferOffset = _arraySerializer.float64(obj.coeff_y, buffer, bufferOffset, 6);
    // Check that the constant length array field [coeff_z] has the right length
    if (obj.coeff_z.length !== 6) {
      throw new Error('Unable to serialize array field coeff_z - length must be 6')
    }
    // Serialize message field [coeff_z]
    bufferOffset = _arraySerializer.float64(obj.coeff_z, buffer, bufferOffset, 6);
    // Serialize message field [duration]
    bufferOffset = _serializer.float64(obj.duration, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type Polynomial
    let len;
    let data = new Polynomial(null);
    // Deserialize message field [start_time]
    data.start_time = _deserializer.time(buffer, bufferOffset);
    // Deserialize message field [coeff_x]
    data.coeff_x = _arrayDeserializer.float64(buffer, bufferOffset, 6)
    // Deserialize message field [coeff_y]
    data.coeff_y = _arrayDeserializer.float64(buffer, bufferOffset, 6)
    // Deserialize message field [coeff_z]
    data.coeff_z = _arrayDeserializer.float64(buffer, bufferOffset, 6)
    // Deserialize message field [duration]
    data.duration = _deserializer.float64(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 160;
  }

  static datatype() {
    // Returns string type for a message object
    return 'traj_utils/Polynomial';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'fc673789e19bfa2f79fdc2d43aa52bb6';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    time start_time
    float64[6] coeff_x
    float64[6] coeff_y
    float64[6] coeff_z
    float64 duration
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new Polynomial(null);
    if (msg.start_time !== undefined) {
      resolved.start_time = msg.start_time;
    }
    else {
      resolved.start_time = {secs: 0, nsecs: 0}
    }

    if (msg.coeff_x !== undefined) {
      resolved.coeff_x = msg.coeff_x;
    }
    else {
      resolved.coeff_x = new Array(6).fill(0)
    }

    if (msg.coeff_y !== undefined) {
      resolved.coeff_y = msg.coeff_y;
    }
    else {
      resolved.coeff_y = new Array(6).fill(0)
    }

    if (msg.coeff_z !== undefined) {
      resolved.coeff_z = msg.coeff_z;
    }
    else {
      resolved.coeff_z = new Array(6).fill(0)
    }

    if (msg.duration !== undefined) {
      resolved.duration = msg.duration;
    }
    else {
      resolved.duration = 0.0
    }

    return resolved;
    }
};

module.exports = Polynomial;
