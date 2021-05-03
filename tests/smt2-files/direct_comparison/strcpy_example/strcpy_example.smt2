(set-option :produce-interpolants true)
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

(declare-fun diff ((Array Int Int) (Array Int Int)) Int)
(declare-fun undefinedInt () Int)
(declare-fun empty_arrayInt () (Array Int Int))
(declare-fun length ((Array Int Int)) Int)


(define-fun initial_state () Bool (and
(= j 0) 
(= (length a) (- n 1))
(= (length b) (- n 1))
(> n 0)
))

(define-fun transition_one_state () Bool (and
(< j n)
(= j_primed (+ j 1))
(= a_primed a)
(= b_primed (store b j (select a j)))
))

(define-fun transition_two_state () Bool (and
(< j_primed n)
(= j_primed_primed (+ j_primed 1))
(= a_primed_primed a_primed)
(= b_primed_primed (store b_primed j_primed (select a_primed j_primed)))
))

(define-fun error_state () Bool (and 
(distinct a b)
(= j n)
))

(push)
(assert (! (and initial_state transition_one_state transition_two_state) :named part_a))
(assert (! (and error_state) :named part_b))

(check-sat)
(get-interpolants part_a part_b)
(pop)

(push)
(assert (! (and initial_state transition_one_state) :named part_a))
(assert (! (and transition_two_state error_state) :named part_b))

(check-sat)
(get-interpolants part_a part_b)
(pop)

(push)
(assert (! (and initial_state) :named part_a))
(assert (! (and transition_one_state transition_two_state error_state) :named part_b))

(check-sat)
(get-interpolants part_a part_b)
(pop)

