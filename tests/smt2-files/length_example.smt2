; Sorts
(declare-sort ElementSort 0)
(declare-sort ArraySort 0)

; Function declarartions
(declare-fun diff (ArraySort ArraySort) Int)
(declare-fun wr (ArraySort Int ElementSort) ArraySort)
(declare-fun rd (ArraySort Int) ElementSort)
(declare-fun length (ArraySort) Int)
(declare-fun pred (Int) Int)
(declare-fun succ (Int) Int)
(declare-fun empty_array () ArraySort)
(declare-fun undefined () ElementSort)

(declare-fun a () ArraySort)
(declare-fun e0 () ElementSort)
(declare-fun e1 () ElementSort)
(declare-fun i0 () Int)
(declare-fun i1 () Int)

(assert (and (= a (wr (wr empty_array i0 e0) i1 e1)) (>= i0 0)))
(assert (and (< (length a) i0) (> i1 i0) (distinct e0 undefined) (distinct e1 undefined)))
(check-sat)
