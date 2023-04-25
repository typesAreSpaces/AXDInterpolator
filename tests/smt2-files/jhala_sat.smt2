(declare-fun a () (Array Int Int))
(declare-fun b () (Array Int Int))
(declare-fun i () Int)
(declare-fun e () Int)
(declare-fun j1 () Int)
(declare-fun j2 () Int)

(assert (= a (store b i e)))
(assert (and
(distinct (select a i) (select b i))))

(check-sat) 
