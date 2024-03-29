(declare-fun a1 () (Array Int Int))
(declare-fun a2 () (Array Int Int))
(declare-fun c1 () (Array Int Int))
(declare-fun c2 () (Array Int Int))
(declare-fun l () Int)
(declare-fun j () Int)
(declare-fun j1 () Int)
(declare-fun j2 () Int)
(declare-fun diffInt ((Array Int Int) (Array Int Int)) Int)

;; A part
(assert
 (and
  (= (diffInt a1 a2) j)
  (= (diffInt a1 c1) j1)
  (= (diffInt a2 c2) j2)))

;; B part
(assert
 (and
  (< j1 l)
  (< j2 l)
  (distinct (select c1 l) (select c2 l))))
