(declare-fun c1 () (Array Int Int))
(declare-fun c2 () (Array Int Int))
(declare-fun i () Int)
(declare-fun e () Int)
(declare-fun lengthInt ((Array Int Int)) Int)

;; A part
(assert
 (and
  (=
   c1
   (store c2 i e))
  (distinct
   (lengthInt c1)
   (lengthInt c2))))

;; B part
(assert
 true)
