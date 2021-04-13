; Function declarartions
(declare-fun diffInt ((Array Int Int) (Array Int Int)) Int)

; Paper example
(declare-fun a () (Array Int Int))
(declare-fun test () (Array Int (Array Int Int)))
(declare-fun b () (Array Int Int))
(declare-fun c1 () (Array Int Int))
(declare-fun c2 () (Array Int Int))
(declare-fun i1 () Int)
(declare-fun i2 () Int)
(declare-fun i3 () Int)
(declare-fun a1 () (Array Int Int))
(declare-fun e1 () Int)
(declare-fun e3 () Int)

; Part A
(assert (and 
;(= (diffInt a c1) i1) 
;(= (diffInt b c2) i1) 
(= a (store a1 i3 e3)) 
;(= a1 (store b i1 e1))
))

; Part B
(assert (and 
(= test test)
(< i1 i2) 
(< i2 i3) 
(distinct (select c1 i2) (select c2 i2))
))

(check-sat)
