(set-option :print-success false)
(set-option :produce-interpolants true)
(set-option :produce-proofs true)
(set-logic QF_AUFLIA)
(declare-fun a () (Array Int Int))
(declare-fun b () (Array Int Int))
(declare-fun i () Int)
(declare-fun e () Int)
(declare-fun j1 () Int)
(declare-fun j2 () Int)

(assert (! (= a (store b i e)) :named _part_a))
(assert (! 
(and 
(distinct (select a j1) (select b j1)) 
(distinct (select a j2) (select b j2))
(distinct j1 j2)
) :named _part_b))
(check-sat)
(echo "Interpolant obtained using SMTInterpol")
(get-interpolants _part_a  _part_b)
