; Auto-generated. Do not edit!


(cl:in-package traj_utils-msg)


;//! \htmlinclude swarmPrimitiveTraj.msg.html

(cl:defclass <swarmPrimitiveTraj> (roslisp-msg-protocol:ros-message)
  ((drone_id
    :reader drone_id
    :initarg :drone_id
    :type cl:fixnum
    :initform 0)
   (start_time
    :reader start_time
    :initarg :start_time
    :type cl:real
    :initform 0)
   (start_p
    :reader start_p
    :initarg :start_p
    :type (cl:vector cl:float)
   :initform (cl:make-array 3 :element-type 'cl:float :initial-element 0.0))
   (start_v
    :reader start_v
    :initarg :start_v
    :type (cl:vector cl:float)
   :initform (cl:make-array 3 :element-type 'cl:float :initial-element 0.0))
   (end_p
    :reader end_p
    :initarg :end_p
    :type (cl:vector cl:float)
   :initform (cl:make-array 3 :element-type 'cl:float :initial-element 0.0))
   (rot_mat
    :reader rot_mat
    :initarg :rot_mat
    :type (cl:vector cl:float)
   :initform (cl:make-array 9 :element-type 'cl:float :initial-element 0.0))
   (vel_id
    :reader vel_id
    :initarg :vel_id
    :type cl:integer
    :initform 0)
   (select_path_id
    :reader select_path_id
    :initarg :select_path_id
    :type cl:integer
    :initform 0))
)

(cl:defclass swarmPrimitiveTraj (<swarmPrimitiveTraj>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <swarmPrimitiveTraj>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'swarmPrimitiveTraj)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name traj_utils-msg:<swarmPrimitiveTraj> is deprecated: use traj_utils-msg:swarmPrimitiveTraj instead.")))

(cl:ensure-generic-function 'drone_id-val :lambda-list '(m))
(cl:defmethod drone_id-val ((m <swarmPrimitiveTraj>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader traj_utils-msg:drone_id-val is deprecated.  Use traj_utils-msg:drone_id instead.")
  (drone_id m))

(cl:ensure-generic-function 'start_time-val :lambda-list '(m))
(cl:defmethod start_time-val ((m <swarmPrimitiveTraj>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader traj_utils-msg:start_time-val is deprecated.  Use traj_utils-msg:start_time instead.")
  (start_time m))

(cl:ensure-generic-function 'start_p-val :lambda-list '(m))
(cl:defmethod start_p-val ((m <swarmPrimitiveTraj>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader traj_utils-msg:start_p-val is deprecated.  Use traj_utils-msg:start_p instead.")
  (start_p m))

(cl:ensure-generic-function 'start_v-val :lambda-list '(m))
(cl:defmethod start_v-val ((m <swarmPrimitiveTraj>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader traj_utils-msg:start_v-val is deprecated.  Use traj_utils-msg:start_v instead.")
  (start_v m))

(cl:ensure-generic-function 'end_p-val :lambda-list '(m))
(cl:defmethod end_p-val ((m <swarmPrimitiveTraj>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader traj_utils-msg:end_p-val is deprecated.  Use traj_utils-msg:end_p instead.")
  (end_p m))

(cl:ensure-generic-function 'rot_mat-val :lambda-list '(m))
(cl:defmethod rot_mat-val ((m <swarmPrimitiveTraj>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader traj_utils-msg:rot_mat-val is deprecated.  Use traj_utils-msg:rot_mat instead.")
  (rot_mat m))

(cl:ensure-generic-function 'vel_id-val :lambda-list '(m))
(cl:defmethod vel_id-val ((m <swarmPrimitiveTraj>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader traj_utils-msg:vel_id-val is deprecated.  Use traj_utils-msg:vel_id instead.")
  (vel_id m))

(cl:ensure-generic-function 'select_path_id-val :lambda-list '(m))
(cl:defmethod select_path_id-val ((m <swarmPrimitiveTraj>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader traj_utils-msg:select_path_id-val is deprecated.  Use traj_utils-msg:select_path_id instead.")
  (select_path_id m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <swarmPrimitiveTraj>) ostream)
  "Serializes a message object of type '<swarmPrimitiveTraj>"
  (cl:let* ((signed (cl:slot-value msg 'drone_id)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
  (cl:let ((__sec (cl:floor (cl:slot-value msg 'start_time)))
        (__nsec (cl:round (cl:* 1e9 (cl:- (cl:slot-value msg 'start_time) (cl:floor (cl:slot-value msg 'start_time)))))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __sec) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __sec) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __sec) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __sec) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 0) __nsec) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __nsec) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __nsec) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __nsec) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (cl:let ((bits (roslisp-utils:encode-single-float-bits ele)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)))
   (cl:slot-value msg 'start_p))
  (cl:map cl:nil #'(cl:lambda (ele) (cl:let ((bits (roslisp-utils:encode-single-float-bits ele)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)))
   (cl:slot-value msg 'start_v))
  (cl:map cl:nil #'(cl:lambda (ele) (cl:let ((bits (roslisp-utils:encode-single-float-bits ele)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)))
   (cl:slot-value msg 'end_p))
  (cl:map cl:nil #'(cl:lambda (ele) (cl:let ((bits (roslisp-utils:encode-single-float-bits ele)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)))
   (cl:slot-value msg 'rot_mat))
  (cl:let* ((signed (cl:slot-value msg 'vel_id)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'select_path_id)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <swarmPrimitiveTraj>) istream)
  "Deserializes a message object of type '<swarmPrimitiveTraj>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'drone_id) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
    (cl:let ((__sec 0) (__nsec 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __sec) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __sec) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __sec) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __sec) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 0) __nsec) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __nsec) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __nsec) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __nsec) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'start_time) (cl:+ (cl:coerce __sec 'cl:double-float) (cl:/ __nsec 1e9))))
  (cl:setf (cl:slot-value msg 'start_p) (cl:make-array 3))
  (cl:let ((vals (cl:slot-value msg 'start_p)))
    (cl:dotimes (i 3)
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:aref vals i) (roslisp-utils:decode-single-float-bits bits)))))
  (cl:setf (cl:slot-value msg 'start_v) (cl:make-array 3))
  (cl:let ((vals (cl:slot-value msg 'start_v)))
    (cl:dotimes (i 3)
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:aref vals i) (roslisp-utils:decode-single-float-bits bits)))))
  (cl:setf (cl:slot-value msg 'end_p) (cl:make-array 3))
  (cl:let ((vals (cl:slot-value msg 'end_p)))
    (cl:dotimes (i 3)
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:aref vals i) (roslisp-utils:decode-single-float-bits bits)))))
  (cl:setf (cl:slot-value msg 'rot_mat) (cl:make-array 9))
  (cl:let ((vals (cl:slot-value msg 'rot_mat)))
    (cl:dotimes (i 9)
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:aref vals i) (roslisp-utils:decode-single-float-bits bits)))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'vel_id) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'select_path_id) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<swarmPrimitiveTraj>)))
  "Returns string type for a message object of type '<swarmPrimitiveTraj>"
  "traj_utils/swarmPrimitiveTraj")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'swarmPrimitiveTraj)))
  "Returns string type for a message object of type 'swarmPrimitiveTraj"
  "traj_utils/swarmPrimitiveTraj")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<swarmPrimitiveTraj>)))
  "Returns md5sum for a message object of type '<swarmPrimitiveTraj>"
  "e581206f10087d6877710b9c852f330f")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'swarmPrimitiveTraj)))
  "Returns md5sum for a message object of type 'swarmPrimitiveTraj"
  "e581206f10087d6877710b9c852f330f")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<swarmPrimitiveTraj>)))
  "Returns full string definition for message of type '<swarmPrimitiveTraj>"
  (cl:format cl:nil "int16 drone_id~%time start_time~%float32[3] start_p~%float32[3] start_v~%float32[3] end_p~%float32[9] rot_mat~%int32 vel_id~%int32 select_path_id~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'swarmPrimitiveTraj)))
  "Returns full string definition for message of type 'swarmPrimitiveTraj"
  (cl:format cl:nil "int16 drone_id~%time start_time~%float32[3] start_p~%float32[3] start_v~%float32[3] end_p~%float32[9] rot_mat~%int32 vel_id~%int32 select_path_id~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <swarmPrimitiveTraj>))
  (cl:+ 0
     2
     8
     0 (cl:reduce #'cl:+ (cl:slot-value msg 'start_p) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 4)))
     0 (cl:reduce #'cl:+ (cl:slot-value msg 'start_v) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 4)))
     0 (cl:reduce #'cl:+ (cl:slot-value msg 'end_p) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 4)))
     0 (cl:reduce #'cl:+ (cl:slot-value msg 'rot_mat) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 4)))
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <swarmPrimitiveTraj>))
  "Converts a ROS message object to a list"
  (cl:list 'swarmPrimitiveTraj
    (cl:cons ':drone_id (drone_id msg))
    (cl:cons ':start_time (start_time msg))
    (cl:cons ':start_p (start_p msg))
    (cl:cons ':start_v (start_v msg))
    (cl:cons ':end_p (end_p msg))
    (cl:cons ':rot_mat (rot_mat msg))
    (cl:cons ':vel_id (vel_id msg))
    (cl:cons ':select_path_id (select_path_id msg))
))
