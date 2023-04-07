(declare-fun a () (Array Int Int))
(declare-fun a1 () (Array Int Int))
(declare-fun b () (Array Int Int))
(declare-fun b1 () (Array Int Int))

(declare-fun c1 () (Array Int Int))
(declare-fun c2 () (Array Int Int))

(declare-fun i1 () Int)
(declare-fun i2 () Int)
(declare-fun i3 () Int)

(declare-fun e1 () Int)
(declare-fun e3 () Int)

(declare-fun k () Int)

(declare-fun diffInt ((Array Int Int) (Array Int Int)) Int)
(declare-fun lengthInt ((Array Int Int)) Int)

;; Part A
(assert
 (and
  (= (diffInt a c1) i1)
  (= (diffInt b c2) i1)
  (= a1 (store b i1 e1))
  (= (lengthInt a) k)
  (= a (store a1 i3 e3))
  (= (lengthInt a1) k)
  (= (lengthInt b1) k)
  (= (lengthInt c1) k)
  (= (lengthInt c2) k)))

;; Part B
(assert
 (and
  (distinct (select c1 i1) (select c2 i2))
  (< i1 i2)
  (< i2 i3)
  (< i3 k)
  (= (lengthInt c1) k)
  (= (lengthInt c2) k)))
