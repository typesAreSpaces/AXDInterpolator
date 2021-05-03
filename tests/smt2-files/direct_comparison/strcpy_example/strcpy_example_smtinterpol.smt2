(set-option :print-success false)
(set-option :produce-interpolants true)
(set-option :produce-proofs true)
(set-logic QF_AUFLIA)
(declare-fun a () (Array Int Int))
(declare-fun a_primed () (Array Int Int))
(declare-fun a_primed_primed () (Array Int Int))
(declare-fun b () (Array Int Int))
(declare-fun b_primed () (Array Int Int))
(declare-fun b_primed_primed () (Array Int Int))
(declare-fun j () Int)
(declare-fun j_primed () Int)
(declare-fun j_primed_primed () Int)
(declare-fun n () Int)

;(declare-fun undefinedInt () Int)
;(declare-fun empty_arrayInt () (Array Int Int))
(declare-fun length ((Array Int Int)) Int)


(push)
(assert (! (and 

(= j 0) 
(= (length a) (- n 1))
(= (length b) (- n 1))
(> n 0)

) :named _part_a))

(assert (! 
(and 

(< j n)
(= j_primed (+ j 1))
(= a_primed a)
(= b_primed (store b j (select a j)))


(< j_primed n)
(= j_primed_primed (+ j_primed 1))
(= a_primed_primed a_primed)
(= b_primed_primed (store b_primed j_primed (select a_primed j_primed)))


(distinct a b)
(= j n)


) :named _part_b))

(check-sat)
(echo "Interpolant obtained using SMTInterpol")
(get-interpolants _part_a _part_b)
(pop)

(push)
(assert (! (and 

(= j 0) 
(= (length a) (- n 1))
(= (length b) (- n 1))
(> n 0)

(< j n)
(= j_primed (+ j 1))
(= a_primed a)
(= b_primed (store b j (select a j)))

) :named _part_a))

(assert (! 
(and 

(< j_primed n)
(= j_primed_primed (+ j_primed 1))
(= a_primed_primed a_primed)
(= b_primed_primed (store b_primed j_primed (select a_primed j_primed)))


(distinct a b)
(= j n)


) :named _part_b))

(check-sat)
(echo "Interpolant obtained using SMTInterpol")
(get-interpolants _part_a _part_b)
(pop)

(push)
(assert (! (and 

(= j 0) 
(= (length a) (- n 1))
(= (length b) (- n 1))
(> n 0)

(< j n)
(= j_primed (+ j 1))
(= a_primed a)
(= b_primed (store b j (select a j)))


(< j_primed n)
(= j_primed_primed (+ j_primed 1))
(= a_primed_primed a_primed)
(= b_primed_primed (store b_primed j_primed (select a_primed j_primed)))


) :named _part_a))

(assert (! 
(and 

(distinct a b)
(= j n)

) :named _part_b))

(check-sat)
(echo "Interpolant obtained using SMTInterpol")
(get-interpolants _part_a _part_b)
(pop)
