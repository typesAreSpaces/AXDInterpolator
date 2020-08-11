(set-info :status unknown)
(declare-sort ArraySort 0)
 (declare-sort ElementSort 0)
 (declare-fun diff_ (Int ArraySort ArraySort) Int)
(declare-fun b () ArraySort)
(declare-fun c2 () ArraySort)
(declare-fun i1 () Int)
(declare-fun c1 () ArraySort)
(declare-fun fresh_index_0 () Int)
(declare-fun fresh_index_1 () Int)
(declare-fun a () ArraySort)
(declare-fun i3 () Int)
(declare-fun rd (ArraySort Int) ElementSort)

(assert (and
  (= i1 (diff_ 1 c2 b))
  (= fresh_index_0 (diff_ 1 c2 c1))
  (= fresh_index_1 (diff_ 2 c2 c1))
  (= i1 (diff_ 1 c1 a))
  (let ((a!1 (or (= (rd c2 fresh_index_0) (rd c1 fresh_index_0))
                 (= fresh_index_0 i3)
                 (= fresh_index_0 i1)
                 (>= (+ i1 (* (- 1) fresh_index_0)) 0)))
        (a!2 (= (not (= (rd c1 fresh_index_1) (rd c2 fresh_index_1)))
                (= (rd c2 fresh_index_1) (rd c1 fresh_index_1))))
        (a!5 (= (not (= (rd c1 fresh_index_0) (rd c2 fresh_index_0)))
                (= (rd c2 fresh_index_0) (rd c1 fresh_index_0)))))
  (let ((a!3 (or (= (rd c2 fresh_index_1) (rd c1 fresh_index_1))
                 (>= (+ i1 (* (- 1) fresh_index_1)) 0)
                 (= fresh_index_1 i1)
                 a!2
                 (= fresh_index_1 i3))))
  (let ((a!4 (and a!1 a!3 (>= (+ i1 (* (- 1) fresh_index_0)) 0))))
    (or (and (= (rd c2 fresh_index_0) (rd c1 fresh_index_0)) a!1 a!3)
        a!4
        (and a!1 a!3 (= fresh_index_0 i1))
        a!5
        (= fresh_index_0 i3)))))
  (or (= (rd c2 fresh_index_0) (rd c1 fresh_index_0))
      (= fresh_index_0 i3)
      (= fresh_index_0 i1)
      (>= (+ i1 (* (- 1) fresh_index_0)) 0))
  (let ((a!1 (= (not (= (rd c1 fresh_index_1) (rd c2 fresh_index_1)))
                (= (rd c2 fresh_index_1) (rd c1 fresh_index_1)))))
    (or (= (rd c2 fresh_index_1) (rd c1 fresh_index_1))
        (>= (+ i1 (* (- 1) fresh_index_1)) 0)
        (= fresh_index_1 i1)
        a!1
        (= fresh_index_1 i3)))
  )
)
(check-sat)
(apply solve-eqs)
;(apply (then simplify solve-eqs))
