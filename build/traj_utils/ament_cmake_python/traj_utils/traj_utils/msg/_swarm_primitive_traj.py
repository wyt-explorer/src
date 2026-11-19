# generated from rosidl_generator_py/resource/_idl.py.em
# with input from traj_utils:msg/SwarmPrimitiveTraj.idl
# generated code does not contain a copyright notice

# This is being done at the module level and not on the instance level to avoid looking
# for the same variable multiple times on each instance. This variable is not supposed to
# change during runtime so it makes sense to only look for it once.
from os import getenv

ros_python_check_fields = getenv('ROS_PYTHON_CHECK_FIELDS', default='')


# Import statements for member types

import builtins  # noqa: E402, I100

import math  # noqa: E402, I100

# Member 'start_p'
# Member 'start_v'
# Member 'end_p'
# Member 'rot_mat'
import numpy  # noqa: E402, I100

import rosidl_parser.definition  # noqa: E402, I100


class Metaclass_SwarmPrimitiveTraj(type):
    """Metaclass of message 'SwarmPrimitiveTraj'."""

    _CREATE_ROS_MESSAGE = None
    _CONVERT_FROM_PY = None
    _CONVERT_TO_PY = None
    _DESTROY_ROS_MESSAGE = None
    _TYPE_SUPPORT = None

    __constants = {
    }

    @classmethod
    def __import_type_support__(cls):
        try:
            from rosidl_generator_py import import_type_support
            module = import_type_support('traj_utils')
        except ImportError:
            import logging
            import traceback
            logger = logging.getLogger(
                'traj_utils.msg.SwarmPrimitiveTraj')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__msg__swarm_primitive_traj
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__msg__swarm_primitive_traj
            cls._CONVERT_TO_PY = module.convert_to_py_msg__msg__swarm_primitive_traj
            cls._TYPE_SUPPORT = module.type_support_msg__msg__swarm_primitive_traj
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__msg__swarm_primitive_traj

            from builtin_interfaces.msg import Time
            if Time.__class__._TYPE_SUPPORT is None:
                Time.__class__.__import_type_support__()

    @classmethod
    def __prepare__(cls, name, bases, **kwargs):
        # list constant names here so that they appear in the help text of
        # the message class under "Data and other attributes defined here:"
        # as well as populate each message instance
        return {
        }


class SwarmPrimitiveTraj(metaclass=Metaclass_SwarmPrimitiveTraj):
    """Message class 'SwarmPrimitiveTraj'."""

    __slots__ = [
        '_drone_id',
        '_start_time',
        '_start_p',
        '_start_v',
        '_end_p',
        '_rot_mat',
        '_vel_id',
        '_select_path_id',
        '_check_fields',
    ]

    _fields_and_field_types = {
        'drone_id': 'int16',
        'start_time': 'builtin_interfaces/Time',
        'start_p': 'float[3]',
        'start_v': 'float[3]',
        'end_p': 'float[3]',
        'rot_mat': 'float[9]',
        'vel_id': 'int32',
        'select_path_id': 'int32',
    }

    # This attribute is used to store an rosidl_parser.definition variable
    # related to the data type of each of the components the message.
    SLOT_TYPES = (
        rosidl_parser.definition.BasicType('int16'),  # noqa: E501
        rosidl_parser.definition.NamespacedType(['builtin_interfaces', 'msg'], 'Time'),  # noqa: E501
        rosidl_parser.definition.Array(rosidl_parser.definition.BasicType('float'), 3),  # noqa: E501
        rosidl_parser.definition.Array(rosidl_parser.definition.BasicType('float'), 3),  # noqa: E501
        rosidl_parser.definition.Array(rosidl_parser.definition.BasicType('float'), 3),  # noqa: E501
        rosidl_parser.definition.Array(rosidl_parser.definition.BasicType('float'), 9),  # noqa: E501
        rosidl_parser.definition.BasicType('int32'),  # noqa: E501
        rosidl_parser.definition.BasicType('int32'),  # noqa: E501
    )

    def __init__(self, **kwargs):
        if 'check_fields' in kwargs:
            self._check_fields = kwargs['check_fields']
        else:
            self._check_fields = ros_python_check_fields == '1'
        if self._check_fields:
            assert all('_' + key in self.__slots__ for key in kwargs.keys()), \
                'Invalid arguments passed to constructor: %s' % \
                ', '.join(sorted(k for k in kwargs.keys() if '_' + k not in self.__slots__))
        self.drone_id = kwargs.get('drone_id', int())
        from builtin_interfaces.msg import Time
        self.start_time = kwargs.get('start_time', Time())
        if 'start_p' not in kwargs:
            self.start_p = numpy.zeros(3, dtype=numpy.float32)
        else:
            self.start_p = kwargs.get('start_p')
        if 'start_v' not in kwargs:
            self.start_v = numpy.zeros(3, dtype=numpy.float32)
        else:
            self.start_v = kwargs.get('start_v')
        if 'end_p' not in kwargs:
            self.end_p = numpy.zeros(3, dtype=numpy.float32)
        else:
            self.end_p = kwargs.get('end_p')
        if 'rot_mat' not in kwargs:
            self.rot_mat = numpy.zeros(9, dtype=numpy.float32)
        else:
            self.rot_mat = kwargs.get('rot_mat')
        self.vel_id = kwargs.get('vel_id', int())
        self.select_path_id = kwargs.get('select_path_id', int())

    def __repr__(self):
        typename = self.__class__.__module__.split('.')
        typename.pop()
        typename.append(self.__class__.__name__)
        args = []
        for s, t in zip(self.get_fields_and_field_types().keys(), self.SLOT_TYPES):
            field = getattr(self, s)
            fieldstr = repr(field)
            # We use Python array type for fields that can be directly stored
            # in them, and "normal" sequences for everything else.  If it is
            # a type that we store in an array, strip off the 'array' portion.
            if (
                isinstance(t, rosidl_parser.definition.AbstractSequence) and
                isinstance(t.value_type, rosidl_parser.definition.BasicType) and
                t.value_type.typename in ['float', 'double', 'int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32', 'int64', 'uint64']
            ):
                if len(field) == 0:
                    fieldstr = '[]'
                else:
                    if self._check_fields:
                        assert fieldstr.startswith('array(')
                    prefix = "array('X', "
                    suffix = ')'
                    fieldstr = fieldstr[len(prefix):-len(suffix)]
            args.append(s + '=' + fieldstr)
        return '%s(%s)' % ('.'.join(typename), ', '.join(args))

    def __eq__(self, other):
        if not isinstance(other, self.__class__):
            return False
        if self.drone_id != other.drone_id:
            return False
        if self.start_time != other.start_time:
            return False
        if any(self.start_p != other.start_p):
            return False
        if any(self.start_v != other.start_v):
            return False
        if any(self.end_p != other.end_p):
            return False
        if any(self.rot_mat != other.rot_mat):
            return False
        if self.vel_id != other.vel_id:
            return False
        if self.select_path_id != other.select_path_id:
            return False
        return True

    @classmethod
    def get_fields_and_field_types(cls):
        from copy import copy
        return copy(cls._fields_and_field_types)

    @builtins.property
    def drone_id(self):
        """Message field 'drone_id'."""
        return self._drone_id

    @drone_id.setter
    def drone_id(self, value):
        if self._check_fields:
            assert \
                isinstance(value, int), \
                "The 'drone_id' field must be of type 'int'"
            assert value >= -32768 and value < 32768, \
                "The 'drone_id' field must be an integer in [-32768, 32767]"
        self._drone_id = value

    @builtins.property
    def start_time(self):
        """Message field 'start_time'."""
        return self._start_time

    @start_time.setter
    def start_time(self, value):
        if self._check_fields:
            from builtin_interfaces.msg import Time
            assert \
                isinstance(value, Time), \
                "The 'start_time' field must be a sub message of type 'Time'"
        self._start_time = value

    @builtins.property
    def start_p(self):
        """Message field 'start_p'."""
        return self._start_p

    @start_p.setter
    def start_p(self, value):
        if self._check_fields:
            if isinstance(value, numpy.ndarray):
                assert value.dtype == numpy.float32, \
                    "The 'start_p' numpy.ndarray() must have the dtype of 'numpy.float32'"
                assert value.size == 3, \
                    "The 'start_p' numpy.ndarray() must have a size of 3"
                self._start_p = value
                return
            from collections.abc import Sequence
            from collections.abc import Set
            from collections import UserList
            from collections import UserString
            assert \
                ((isinstance(value, Sequence) or
                  isinstance(value, Set) or
                  isinstance(value, UserList)) and
                 not isinstance(value, str) and
                 not isinstance(value, UserString) and
                 len(value) == 3 and
                 all(isinstance(v, float) for v in value) and
                 all(not (val < -3.402823466e+38 or val > 3.402823466e+38) or math.isinf(val) for val in value)), \
                "The 'start_p' field must be a set or sequence with length 3 and each value of type 'float' and each float in [-340282346600000016151267322115014000640.000000, 340282346600000016151267322115014000640.000000]"
        self._start_p = numpy.array(value, dtype=numpy.float32)

    @builtins.property
    def start_v(self):
        """Message field 'start_v'."""
        return self._start_v

    @start_v.setter
    def start_v(self, value):
        if self._check_fields:
            if isinstance(value, numpy.ndarray):
                assert value.dtype == numpy.float32, \
                    "The 'start_v' numpy.ndarray() must have the dtype of 'numpy.float32'"
                assert value.size == 3, \
                    "The 'start_v' numpy.ndarray() must have a size of 3"
                self._start_v = value
                return
            from collections.abc import Sequence
            from collections.abc import Set
            from collections import UserList
            from collections import UserString
            assert \
                ((isinstance(value, Sequence) or
                  isinstance(value, Set) or
                  isinstance(value, UserList)) and
                 not isinstance(value, str) and
                 not isinstance(value, UserString) and
                 len(value) == 3 and
                 all(isinstance(v, float) for v in value) and
                 all(not (val < -3.402823466e+38 or val > 3.402823466e+38) or math.isinf(val) for val in value)), \
                "The 'start_v' field must be a set or sequence with length 3 and each value of type 'float' and each float in [-340282346600000016151267322115014000640.000000, 340282346600000016151267322115014000640.000000]"
        self._start_v = numpy.array(value, dtype=numpy.float32)

    @builtins.property
    def end_p(self):
        """Message field 'end_p'."""
        return self._end_p

    @end_p.setter
    def end_p(self, value):
        if self._check_fields:
            if isinstance(value, numpy.ndarray):
                assert value.dtype == numpy.float32, \
                    "The 'end_p' numpy.ndarray() must have the dtype of 'numpy.float32'"
                assert value.size == 3, \
                    "The 'end_p' numpy.ndarray() must have a size of 3"
                self._end_p = value
                return
            from collections.abc import Sequence
            from collections.abc import Set
            from collections import UserList
            from collections import UserString
            assert \
                ((isinstance(value, Sequence) or
                  isinstance(value, Set) or
                  isinstance(value, UserList)) and
                 not isinstance(value, str) and
                 not isinstance(value, UserString) and
                 len(value) == 3 and
                 all(isinstance(v, float) for v in value) and
                 all(not (val < -3.402823466e+38 or val > 3.402823466e+38) or math.isinf(val) for val in value)), \
                "The 'end_p' field must be a set or sequence with length 3 and each value of type 'float' and each float in [-340282346600000016151267322115014000640.000000, 340282346600000016151267322115014000640.000000]"
        self._end_p = numpy.array(value, dtype=numpy.float32)

    @builtins.property
    def rot_mat(self):
        """Message field 'rot_mat'."""
        return self._rot_mat

    @rot_mat.setter
    def rot_mat(self, value):
        if self._check_fields:
            if isinstance(value, numpy.ndarray):
                assert value.dtype == numpy.float32, \
                    "The 'rot_mat' numpy.ndarray() must have the dtype of 'numpy.float32'"
                assert value.size == 9, \
                    "The 'rot_mat' numpy.ndarray() must have a size of 9"
                self._rot_mat = value
                return
            from collections.abc import Sequence
            from collections.abc import Set
            from collections import UserList
            from collections import UserString
            assert \
                ((isinstance(value, Sequence) or
                  isinstance(value, Set) or
                  isinstance(value, UserList)) and
                 not isinstance(value, str) and
                 not isinstance(value, UserString) and
                 len(value) == 9 and
                 all(isinstance(v, float) for v in value) and
                 all(not (val < -3.402823466e+38 or val > 3.402823466e+38) or math.isinf(val) for val in value)), \
                "The 'rot_mat' field must be a set or sequence with length 9 and each value of type 'float' and each float in [-340282346600000016151267322115014000640.000000, 340282346600000016151267322115014000640.000000]"
        self._rot_mat = numpy.array(value, dtype=numpy.float32)

    @builtins.property
    def vel_id(self):
        """Message field 'vel_id'."""
        return self._vel_id

    @vel_id.setter
    def vel_id(self, value):
        if self._check_fields:
            assert \
                isinstance(value, int), \
                "The 'vel_id' field must be of type 'int'"
            assert value >= -2147483648 and value < 2147483648, \
                "The 'vel_id' field must be an integer in [-2147483648, 2147483647]"
        self._vel_id = value

    @builtins.property
    def select_path_id(self):
        """Message field 'select_path_id'."""
        return self._select_path_id

    @select_path_id.setter
    def select_path_id(self, value):
        if self._check_fields:
            assert \
                isinstance(value, int), \
                "The 'select_path_id' field must be of type 'int'"
            assert value >= -2147483648 and value < 2147483648, \
                "The 'select_path_id' field must be an integer in [-2147483648, 2147483647]"
        self._select_path_id = value
