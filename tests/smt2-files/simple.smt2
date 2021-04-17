(declare-fun a () Int)

; Part A
(assert (and 
(distinct a a) 
))

; Part B
(assert (and 
false
))

(check-sat)
