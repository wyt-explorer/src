;; Auto-generated. Do not edit!


(when (boundp 'traj_utils::Polynomial)
  (if (not (find-package "TRAJ_UTILS"))
    (make-package "TRAJ_UTILS"))
  (shadow 'Polynomial (find-package "TRAJ_UTILS")))
(unless (find-package "TRAJ_UTILS::POLYNOMIAL")
  (make-package "TRAJ_UTILS::POLYNOMIAL"))

(in-package "ROS")
;;//! \htmlinclude Polynomial.msg.html


(defclass traj_utils::Polynomial
  :super ros::object
  :slots (_start_time _coeff_x _coeff_y _coeff_z _duration ))

(defmethod traj_utils::Polynomial
  (:init
   (&key
    ((:start_time __start_time) (instance ros::time :init))
    ((:coeff_x __coeff_x) (make-array 6 :initial-element 0.0 :element-type :float))
    ((:coeff_y __coeff_y) (make-array 6 :initial-element 0.0 :element-type :float))
    ((:coeff_z __coeff_z) (make-array 6 :initial-element 0.0 :element-type :float))
    ((:duration __duration) 0.0)
    )
   (send-super :init)
   (setq _start_time __start_time)
   (setq _coeff_x __coeff_x)
   (setq _coeff_y __coeff_y)
   (setq _coeff_z __coeff_z)
   (setq _duration (float __duration))
   self)
  (:start_time
   (&optional __start_time)
   (if __start_time (setq _start_time __start_time)) _start_time)
  (:coeff_x
   (&optional __coeff_x)
   (if __coeff_x (setq _coeff_x __coeff_x)) _coeff_x)
  (:coeff_y
   (&optional __coeff_y)
   (if __coeff_y (setq _coeff_y __coeff_y)) _coeff_y)
  (:coeff_z
   (&optional __coeff_z)
   (if __coeff_z (setq _coeff_z __coeff_z)) _coeff_z)
  (:duration
   (&optional __duration)
   (if __duration (setq _duration __duration)) _duration)
  (:serialization-length
   ()
   (+
    ;; time _start_time
    8
    ;; float64[6] _coeff_x
    (* 8    6)
    ;; float64[6] _coeff_y
    (* 8    6)
    ;; float64[6] _coeff_z
    (* 8    6)
    ;; float64 _duration
    8
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; time _start_time
       (write-long (send _start_time :sec) s) (write-long (send _start_time :nsec) s)
     ;; float64[6] _coeff_x
     (dotimes (i 6)
       (sys::poke (elt _coeff_x i) (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
       )
     ;; float64[6] _coeff_y
     (dotimes (i 6)
       (sys::poke (elt _coeff_y i) (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
       )
     ;; float64[6] _coeff_z
     (dotimes (i 6)
       (sys::poke (elt _coeff_z i) (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
       )
     ;; float64 _duration
       (sys::poke _duration (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; time _start_time
     (send _start_time :sec (sys::peek buf ptr- :integer)) (incf ptr- 4)  (send _start_time :nsec (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; float64[6] _coeff_x
   (dotimes (i (length _coeff_x))
     (setf (elt _coeff_x i) (sys::peek buf ptr- :double)) (incf ptr- 8)
     )
   ;; float64[6] _coeff_y
   (dotimes (i (length _coeff_y))
     (setf (elt _coeff_y i) (sys::peek buf ptr- :double)) (incf ptr- 8)
     )
   ;; float64[6] _coeff_z
   (dotimes (i (length _coeff_z))
     (setf (elt _coeff_z i) (sys::peek buf ptr- :double)) (incf ptr- 8)
     )
   ;; float64 _duration
     (setq _duration (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;;
   self)
  )

(setf (get traj_utils::Polynomial :md5sum-) "fc673789e19bfa2f79fdc2d43aa52bb6")
(setf (get traj_utils::Polynomial :datatype-) "traj_utils/Polynomial")
(setf (get traj_utils::Polynomial :definition-)
      "time start_time
float64[6] coeff_x
float64[6] coeff_y
float64[6] coeff_z
float64 duration
")



(provide :traj_utils/Polynomial "fc673789e19bfa2f79fdc2d43aa52bb6")


