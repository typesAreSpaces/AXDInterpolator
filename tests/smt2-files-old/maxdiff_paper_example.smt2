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
(declare-fun a1 () ArraySort)
(declare-fun e1 () Int)
(declare-fun e3 () Int)

; Part A
(assert 
(and (= (diff a c1) i1) (= (diff b c2) i1) (= a (wr a1 i3 e3)) (= a1 (wr b i1 e1)))
)

; Part B
(assert 
(and (< i1 i2) (< i2 i3) (distinct (rd c1 i2) (rd c2 i2)))
)

(check-sat)
