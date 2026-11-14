;; Auto-generated. Do not edit!


(when (boundp 'traj_utils::swarmPrimitiveTraj)
  (if (not (find-package "TRAJ_UTILS"))
    (make-package "TRAJ_UTILS"))
  (shadow 'swarmPrimitiveTraj (find-package "TRAJ_UTILS")))
(unless (find-package "TRAJ_UTILS::SWARMPRIMITIVETRAJ")
  (make-package "TRAJ_UTILS::SWARMPRIMITIVETRAJ"))

(in-package "ROS")
;;//! \htmlinclude swarmPrimitiveTraj.msg.html


(defclass traj_utils::swarmPrimitiveTraj
  :super ros::object
  :slots (_drone_id _start_time _start_p _start_v _end_p _rot_mat _vel_id _select_path_id ))

(defmethod traj_utils::swarmPrimitiveTraj
  (:init
   (&key
    ((:drone_id __drone_id) 0)
    ((:start_time __start_time) (instance ros::time :init))
    ((:start_p __start_p) (make-array 3 :initial-element 0.0 :element-type :float))
    ((:start_v __start_v) (make-array 3 :initial-element 0.0 :element-type :float))
    ((:end_p __end_p) (make-array 3 :initial-element 0.0 :element-type :float))
    ((:rot_mat __rot_mat) (make-array 9 :initial-element 0.0 :element-type :float))
    ((:vel_id __vel_id) 0)
    ((:select_path_id __select_path_id) 0)
    )
   (send-super :init)
   (setq _drone_id (round __drone_id))
   (setq _start_time __start_time)
   (setq _start_p __start_p)
   (setq _start_v __start_v)
   (setq _end_p __end_p)
   (setq _rot_mat __rot_mat)
   (setq _vel_id (round __vel_id))
   (setq _select_path_id (round __select_path_id))
   self)
  (:drone_id
   (&optional __drone_id)
   (if __drone_id (setq _drone_id __drone_id)) _drone_id)
  (:start_time
   (&optional __start_time)
   (if __start_time (setq _start_time __start_time)) _start_time)
  (:start_p
   (&optional __start_p)
   (if __start_p (setq _start_p __start_p)) _start_p)
  (:start_v
   (&optional __start_v)
   (if __start_v (setq _start_v __start_v)) _start_v)
  (:end_p
   (&optional __end_p)
   (if __end_p (setq _end_p __end_p)) _end_p)
  (:rot_mat
   (&optional __rot_mat)
   (if __rot_mat (setq _rot_mat __rot_mat)) _rot_mat)
  (:vel_id
   (&optional __vel_id)
   (if __vel_id (setq _vel_id __vel_id)) _vel_id)
  (:select_path_id
   (&optional __select_path_id)
   (if __select_path_id (setq _select_path_id __select_path_id)) _select_path_id)
  (:serialization-length
   ()
   (+
    ;; int16 _drone_id
    2
    ;; time _start_time
    8
    ;; float32[3] _start_p
    (* 4    3)
    ;; float32[3] _start_v
    (* 4    3)
    ;; float32[3] _end_p
    (* 4    3)
    ;; float32[9] _rot_mat
    (* 4    9)
    ;; int32 _vel_id
    4
    ;; int32 _select_path_id
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; int16 _drone_id
       (write-word _drone_id s)
     ;; time _start_time
       (write-long (send _start_time :sec) s) (write-long (send _start_time :nsec) s)
     ;; float32[3] _start_p
     (dotimes (i 3)
       (sys::poke (elt _start_p i) (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
       )
     ;; float32[3] _start_v
     (dotimes (i 3)
       (sys::poke (elt _start_v i) (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
       )
     ;; float32[3] _end_p
     (dotimes (i 3)
       (sys::poke (elt _end_p i) (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
       )
     ;; float32[9] _rot_mat
     (dotimes (i 9)
       (sys::poke (elt _rot_mat i) (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
       )
     ;; int32 _vel_id
       (write-long _vel_id s)
     ;; int32 _select_path_id
       (write-long _select_path_id s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; int16 _drone_id
     (setq _drone_id (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;; time _start_time
     (send _start_time :sec (sys::peek buf ptr- :integer)) (incf ptr- 4)  (send _start_time :nsec (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; float32[3] _start_p
   (dotimes (i (length _start_p))
     (setf (elt _start_p i) (sys::peek buf ptr- :float)) (incf ptr- 4)
     )
   ;; float32[3] _start_v
   (dotimes (i (length _start_v))
     (setf (elt _start_v i) (sys::peek buf ptr- :float)) (incf ptr- 4)
     )
   ;; float32[3] _end_p
   (dotimes (i (length _end_p))
     (setf (elt _end_p i) (sys::peek buf ptr- :float)) (incf ptr- 4)
     )
   ;; float32[9] _rot_mat
   (dotimes (i (length _rot_mat))
     (setf (elt _rot_mat i) (sys::peek buf ptr- :float)) (incf ptr- 4)
     )
   ;; int32 _vel_id
     (setq _vel_id (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; int32 _select_path_id
     (setq _select_path_id (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get traj_utils::swarmPrimitiveTraj :md5sum-) "e581206f10087d6877710b9c852f330f")
(setf (get traj_utils::swarmPrimitiveTraj :datatype-) "traj_utils/swarmPrimitiveTraj")
(setf (get traj_utils::swarmPrimitiveTraj :definition-)
      "int16 drone_id
time start_time
float32[3] start_p
float32[3] start_v
float32[3] end_p
float32[9] rot_mat
int32 vel_id
int32 select_path_id
")



(provide :traj_utils/swarmPrimitiveTraj "e581206f10087d6877710b9c852f330f")


