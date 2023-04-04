(declare-fun c1 () (Array Int Int))
(declare-fun c2 () (Array Int Int))
(declare-fun i () Int)
(declare-fun e () Int)
(declare-fun lengthInt ((Array Int Int)) Int)

(assert (= c1 (store c2 i e)))
(assert (= (lengthInt c1) (lengthInt c2)))