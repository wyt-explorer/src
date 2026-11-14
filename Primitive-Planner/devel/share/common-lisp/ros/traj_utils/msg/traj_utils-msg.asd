
(cl:in-package :asdf)

(defsystem "traj_utils-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "Polynomial" :depends-on ("_package_Polynomial"))
    (:file "_package_Polynomial" :depends-on ("_package"))
    (:file "primitiveTraj" :depends-on ("_package_primitiveTraj"))
    (:file "_package_primitiveTraj" :depends-on ("_package"))
    (:file "swarmPrimitiveTraj" :depends-on ("_package_swarmPrimitiveTraj"))
    (:file "_package_swarmPrimitiveTraj" :depends-on ("_package"))
  ))