; Sorts
(declare-sort ArraySort 0)

; Function declarartions
(declare-fun diff (ArraySort ArraySort) Int)
(declare-fun wr (ArraySort Int Int) ArraySort)
(declare-fun rd (ArraySort Int) Int)

; Paper example
(declare-fun a () ArraySort)
(declare-fun b () ArraySort)
(declare-fun c1 () ArraySort)
(declare-fun c2 () ArraySort)
(declare-fun i1 () Int)
(declare-fun i2 () Int)
(declare-fun i3 () Int)
(declare-fun i4 () Int)
(declare-fun i5 () Int)
(declare-fun a1 () ArraySort)
(declare-fun e1 () Int)
(declare-fun e2 () Int)
(declare-fun e3 () Int)
(declare-fun e4 () Int)
(declare-fun e5 () Int)

; Part A
(assert 
(and 
(= (diff a b) i1) 
(= a (wr (wr (wr (wr b i1 e1) i3 e2) i4 e3) i5 e4))
))

; Part B
(assert 
(and (< i1 i2) (< i2 i3) (distinct (rd a i2) (rd b i2)))
)

(check-sat)
