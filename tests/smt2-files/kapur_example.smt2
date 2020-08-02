; Sorts
(declare-sort ElementSort)
(declare-sort ArraySort)

; Function declarartions
(declare-fun diff (ArraySort ArraySort) Int)
(declare-fun wr (ArraySort Int ElementSort) ArraySort)
(declare-fun rd (ArraySort Int) ElementSort)

(declare-fun a () ArraySort)
(declare-fun b () ArraySort)

(declare-fun i1 () Int)
(declare-fun i2 () Int)
(declare-fun i3 () Int)
(declare-fun j  () Int)

(declare-fun e1 () ElementSort)
(declare-fun e2 () ElementSort)
(declare-fun e3 () ElementSort)

;A: a = wr(wr(wr(wr(... wr(b, i1, e1), i2, e2), .., ik, ek))

;B: i1 < I2 ... <  j < ik and with a[j] \neq b[j]


; Part A
(assert 
(and (= a (wr (wr (wr b i1 e1) i2 e2) i3 e3)))
)

; Part B
(assert 
(and (< i1 i2) (< i2 i3) (< j i2) (distinct (rd a j) (rd b j)))
)

(check-sat)
