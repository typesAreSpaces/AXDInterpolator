; Function declarartions
(declare-fun diffInt ((Array Int Int) (Array Int Int)) Int)

(declare-fun a () (Array Int Int))
(declare-fun b () (Array Int Int))
(declare-fun j () Int)
(declare-fun i1 () Int)
(declare-fun i2 () Int)
(declare-fun i3 () Int)
(declare-fun i4 () Int)
(declare-fun e1 () Int)
(declare-fun e2 () Int)
(declare-fun e3 () Int)
(declare-fun e4 () Int)
(declare-fun p1 (Int) Bool)
(declare-fun p2 (Int) Bool)
(declare-fun p3 (Int) Bool)
(declare-fun p4 (Int) Bool)

(assert (and
(= a (store (store (store b i3 e3) i2 e2) i1 e1))
(= (p1 i1) true)
(= (p2 i2) true)
(= (p3 i3) true)
(= (p4 i4) true)
))

(assert (and 
(distinct (select a j) (select b j))
(= (p1 j) false)
(= (p2 j) false)
(= (p3 j) false)
(= (p4 j) false)
))

(check-sat)
