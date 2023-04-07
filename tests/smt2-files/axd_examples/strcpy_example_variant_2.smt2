(declare-fun a () (Array Int Int))
(declare-fun a_primed () (Array Int Int))
(declare-fun a_primed_primed () (Array Int Int))
(declare-fun b () (Array Int Int))
(declare-fun b_primed () (Array Int Int))
(declare-fun b_primed_primed () (Array Int Int))
(declare-fun j () Int)
(declare-fun j_primed () Int)
(declare-fun j_primed_primed () Int)
(declare-fun n () Int)
(declare-fun diffInt ((Array Int Int) (Array Int Int)) Int)
(declare-fun undefinedInt () Int)
(declare-fun empty_arrayInt () (Array Int Int))
(declare-fun lengthInt ((Array Int Int)) Int)

(assert (and 

(= j 0) 
(= (lengthInt a) (- n 1))
(= (lengthInt b) (- n 1))
(> n 0)

(< j n)
(= j_primed (+ j 1))
(= a_primed a)
(= b_primed (store b j (select a j)))

))

(assert (and

(< j_primed n)
(= j_primed_primed (+ j_primed 1))
(= a_primed_primed a_primed)
(= b_primed_primed (store b_primed j_primed (select a_primed j_primed)))


(distinct a b)
(= j n)

))


(check-sat)
