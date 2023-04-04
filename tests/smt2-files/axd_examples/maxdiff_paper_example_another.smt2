; Function declarartions
(declare-fun diffInt ((Array Int Int) (Array Int Int)) Int)

; Paper example
(declare-fun a () (Array Int Int))
(declare-fun b () (Array Int Int))
(declare-fun c1 () (Array Int Int))
(declare-fun c2 () (Array Int Int))
(declare-fun i1 () Int)
(declare-fun i2 () Int)
(declare-fun i3 () Int)
(declare-fun i4 () Int)
(declare-fun i5 () Int)
(declare-fun a1 () (Array Int Int))
(declare-fun e1 () Int)
(declare-fun e2 () Int)
(declare-fun e3 () Int)
(declare-fun e4 () Int)
(declare-fun e5 () Int)

; Part A
(assert 
(and 
(= (diffInt a b) i1) 
(= a (store (store (store (store b i1 e1) i3 e2) i4 e3) i5 e4))
))

; Part B
(assert 
(and 
(< i1 i2) 
(< i2 i3) 
(distinct (select a i2) (select b i2)))
)

(check-sat)
