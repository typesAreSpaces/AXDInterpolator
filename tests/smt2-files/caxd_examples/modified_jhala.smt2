(declare-fun c1 () (Array Int Int))
(declare-fun c2 () (Array Int Int))
(declare-fun i () Int)
(declare-fun e () Int)
(declare-fun i1 () Int)
(declare-fun i2 () Int)

(assert
 (= c1 (store c2 i e)))
(assert
 (and
  (or (< (- i1 1) i2) (> i1 i2))
  (distinct (select c1 i1) (select c2 i1))
  (distinct (select c1 i2) (select c2 i2))))
