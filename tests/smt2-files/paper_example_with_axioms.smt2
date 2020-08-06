(set-option :produce-proofs true)
; Sorts
(declare-sort ElementSort 0)
(declare-sort ArraySort 0)

; Function declarartions
(declare-fun diff (ArraySort ArraySort) Int)
(declare-fun wr (ArraySort Int ElementSort) ArraySort)
(declare-fun rd (ArraySort Int) ElementSort)

; Paper example
(declare-fun a () ArraySort)
(declare-fun b () ArraySort)
(declare-fun c1 () ArraySort)
(declare-fun c2 () ArraySort)
(declare-fun i1 () Int)
(declare-fun i2 () Int)
(declare-fun i3 () Int)
(declare-fun a1 () ArraySort)
(declare-fun e1 () ElementSort)
(declare-fun e3 () ElementSort)

;(assert (forall ( (y ArraySort) (i Int) (e ElementSort)) (= (rd (wr y i e) i) e)))
;(assert (forall ( (y ArraySort) (i Int) (j Int) (e ElementSort)) (implies (distinct i j) (= (rd (wr y i e) j) (rd y j)))))
;(assert (forall ((x ArraySort) (y ArraySort)) (implies (distinct x y) (distinct (rd x (diff x y)) (rd y (diff x y))))))
;(assert (forall ((x ArraySort) (y ArraySort) (i Int)) (implies (> i (diff x y)) (= (rd x i) (rd y i)))))
;(assert (forall ((x ArraySort)) (= (diff x x) 0)))

; Axioms
(assert (forall ( (y ArraySort) (i Int) (e ElementSort)) 
(= (rd (wr y i e) i) e)
;(! (= (rd (wr y i e) i) e) :pattern ((wr y i e)))
))


(assert 
(forall ( (y ArraySort) (i Int) (j Int) (e ElementSort)) 
(implies (distinct i j) (= (rd (wr y i e) j) (rd y j)))
;(! () :pattern ((rd (wr y i e) j)))
))


(assert
(forall ((x ArraySort) (y ArraySort))
(implies (distinct x y) (distinct (rd x (diff x y)) (rd y (diff x y))))
)
)


(assert 
(forall ((x ArraySort) (y ArraySort) (i Int))
(implies (> i (diff x y)) (= (rd x i) (rd y i)))
)
)


(assert 
(forall ((x ArraySort))
(= (diff x x) 0)
)
)


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;(assert (forall ((a ArraySort) (b ArraySort) (i Int)) 
;(iff
;(= (diff a b) i) 
;(and (implies (distinct i 0) (distinct (rd a i) (rd b i)) ) (forall ((h Int)) (implies (> h i) (= (rd a h) (rd b h)))))
;)))

;(assert (forall ((a ArraySort) (b ArraySort) (e ElementSort) (i Int)) 
;(iff 
;(= a (wr b i e))
;(and (= (rd a i) e) (forall ((h Int)) (implies (distinct h i) (= (rd a h) (rd b h)))))
;)
;))

; Part A
(assert 
(and 
(= (diff a c1) i1) 
(= (diff b c2) i1) 
(= a (wr a1 i3 e3)) 
(= a1 (wr b i1 e1))
)
)

; Part B
(assert 
(and
(< i1 i2)
(< i2 i3)
(distinct (rd c1 i2) (rd c2 i2))
)
)


(check-sat)
;(get-proof)
