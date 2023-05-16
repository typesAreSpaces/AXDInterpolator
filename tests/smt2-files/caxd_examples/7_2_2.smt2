(declare-fun a1 () (Array Int Int))
(declare-fun a2 () (Array Int Int))
(declare-fun a3 () (Array Int Int))
(declare-fun a4 () (Array Int Int))
(declare-fun c1 () (Array Int Int))
(declare-fun c2 () (Array Int Int))
(declare-fun l () Int)
(declare-fun j () Int)
(declare-fun j1 () Int)
(declare-fun j2 () Int)
(declare-fun i () Int)
(declare-fun i1 () Int)
(declare-fun i2 () Int)
(declare-fun diffInt ((Array Int Int) (Array Int Int)) Int)
(declare-fun lengthInt ((Array Int Int)) Int)

;; A part
(assert
 (and
  (= (store a1 i1 i1) a3)
  (= (store a1 1 i1) a3)
  (= (store a3 i2 i2) a4)
  (= (select a1 i1) 10)
  (= (lengthInt a1) 3)
  (= (diffInt a1 a2) j)
  (= (diffInt a1 c1) j1)
  (= (diffInt a2 c2) j2)))

;; B part
(assert
 (and
  (= (store a1 i2 i1) a3)
  (= (store a3 i1 i2) a4)
  (= (lengthInt a2) i2)
  (< j l)
  (< j1 l)
  (< j2 l)
  (distinct (select c1 l) (select c2 l))))