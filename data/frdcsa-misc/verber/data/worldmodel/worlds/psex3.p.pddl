

(define
 (problem PSEX3)
 (:domain PSEX3)
 (:objects TEST RELEASE_PANOPLY_GIT_VM REDACT_ALL_INFORMATION - pse-entry andy - person)
 (:init
  (= (budget andy) 500)
  (depends RELEASE_PANOPLY_GIT_VM REDACT_ALL_INFORMATION)
  (depends RELEASE_PANOPLY_GIT_VM TEST))
 (:goal
  (and
   (completed RELEASE_PANOPLY_GIT_VM)))
 (:metric minimize
  (total-time)))