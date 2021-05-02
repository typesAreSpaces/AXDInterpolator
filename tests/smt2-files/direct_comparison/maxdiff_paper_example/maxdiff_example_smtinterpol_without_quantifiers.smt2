(set-option :print-success false)
(set-option :produce-interpolants true)
(set-option :produce-proofs true)
(set-logic QF_AUFLIA)

(declare-fun a () (Array Int Int))
(declare-fun a1 () (Array Int Int))
(declare-fun b () (Array Int Int))
(declare-fun c1 () (Array Int Int))
(declare-fun c2 () (Array Int Int))
(declare-fun i1 () Int)
(declare-fun i2 () Int)
(declare-fun i3 () Int)
(declare-fun e1 () Int)
(declare-fun e2 () Int)
(declare-fun e3 () Int)
(declare-fun undefinedInt () Int)
(declare-fun empty_arrayInt () (Array Int Int))


(push)
(assert (! 
(and
(= (@diff a c1) i1)
(= (@diff b c2) i1)
(= a (store a1 i3 e3))
(= a1 (store b i1 e1))
) 
:named _part_a))

(assert (! 
(and 
(< i1 i2)
(< i2 i3)
(distinct (select c1 i2) (select c2 i2))
) 
:named _part_b))

(check-sat)
;(echo "Interpolant obtained using SMTInterpol")
;(get-interpolants _part_a  _part_b)
(pop)
