(declare-sort Arr)
(declare-fun c1 () Arr)
(declare-fun c2 () Arr)
(declare-fun i () Int)
(declare-fun i1 () Int)
(declare-fun i2 () Int)
(declare-fun k1 () Int)
(declare-fun k2 () Int)
(declare-fun lc1 () Int)
(declare-fun lc2 () Int)
(declare-fun e () Int)
(declare-fun bot () Int)
(declare-fun rd (Arr Int) Int)

(assert
 (and
  (=> (and (distinct e bot) (<= 0 i) (<= i lc2)) (= (rd c1 i) e))
  (=> (or (< i 0) (> i lc2) (= e bot)) (= (rd c1 i) (rd c2 i)))
  (=> (distinct i i) (= (rd c1 i) (rd c2 i)))
  (=> (distinct lc1 i) (= (rd c1 lc1) (rd c2 lc1)))
  (=> (distinct lc2 i) (= (rd c1 lc2) (rd c2 lc2)))
  (>= lc1 0)
  (= (distinct (rd c1 i) bot) (and (<= 0 i) (<= i lc1)))
  (= (distinct (rd c1 lc1) bot) (and (<= 0 lc1) (<= lc1 lc1)))
  (= (distinct (rd c1 lc2) bot) (and (<= 0 lc2) (<= lc2 lc1)))
  (>= lc2 0)
  (= (distinct (rd c2 i) bot) (and (<= 0 i) (<= i lc2)))
  (= (distinct (rd c2 lc1) bot) (and (<= 0 lc1) (<= lc1 lc2)))
  (= (distinct (rd c2 lc2) bot) (and (<= 0 lc2) (<= lc2 lc2)))
  ))

(push)
(assert
 (distinct lc1 lc2))
(check-sat)
(pop)

(push)
(assert
 (and
  (distinct i1 i2)
  (distinct (rd c1 i1) (rd c2 i1))
  (distinct (rd c1 i2) (rd c2 i2))
  ))
(assert
 (and
  (=> (distinct k1 i) (= (rd c1 k1) (rd c2 k1)))
  (=> (distinct k2 i) (= (rd c1 k2) (rd c2 k2)))
  (= (distinct (rd c1 i1) bot) (and (<= 0 i1) (<= i1 lc1)))
  (= (distinct (rd c2 i1) bot) (and (<= 0 i1) (<= i1 lc2)))
  (= (distinct (rd c1 i2) bot) (and (<= 0 i2) (<= i2 lc1)))
  (= (distinct (rd c2 i2) bot) (and (<= 0 i2) (<= i2 lc2)))
  (>= k1 k2)
  (>= k2 0)
  (=> (= (rd c1 k1) (rd c2 k1)) (= k1 0))
  (=> (= (rd c1 k2) (rd c2 k2)) (= k2 0))
  (=> (and (= lc1 lc2) (= k1 k2)) (= k1 0))

  (or (=> (> i1 k2) (= (rd c1 i1) (rd c2 i1))) (= i1 k1))
  (or (=> (> i2 k2) (= (rd c1 i2) (rd c2 i2))) (= i2 k1))
  ))
(check-sat)
(pop)

