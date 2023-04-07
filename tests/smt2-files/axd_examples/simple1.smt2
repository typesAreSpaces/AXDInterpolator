(declare-fun a () Int)
(declare-fun b () Int)

; Part A
(assert (and 
(distinct a b) 
))

; Part B
(assert (and 
(= a b)
))

(check-sat)
