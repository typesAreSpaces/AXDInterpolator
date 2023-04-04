; Function declarartions
(declare-fun diffInt ((Array Int Int) (Array Int Int)) Int)
(declare-fun lengthInt ((Array Int Int)) Int)
(declare-fun empty_arrayInt () (Array Int Int))
(declare-fun undefinedInt () Int)

(declare-fun a () (Array Int Int))
(declare-fun e0 () Int)
(declare-fun e1 () Int)
(declare-fun i0 () Int)
(declare-fun i1 () Int)

(assert (and 
(= a (store (store empty_arrayInt i0 e0) i1 e1)) 
(>= i0 0)
))

(assert (and 
(< (lengthInt a) i0) 
(> i1 i0) 
(distinct e0 undefinedInt) 
(distinct e1 undefinedInt)
))

(check-sat)
