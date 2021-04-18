(declare-fun a () Int)
(declare-fun b () Int)

; Part A
(assert 
(distinct a b)
)

; Part B
(assert 
(= a b)
)

(check-sat)
