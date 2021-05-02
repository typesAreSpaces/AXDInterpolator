(set-option :produce-interpolants true)
(declare-fun a () (Array Int Int))
(declare-fun b () (Array Int Int))
(declare-fun i () Int)
(declare-fun e () Int)
(declare-fun j1 () Int)
(declare-fun j2 () Int)

(assert (! (= a (store b i e)) :interpolation-group _part_a))
(assert (! 
(and 
(distinct (select a j1) (select b j1)) 
(distinct (select a j2) (select b j2))
(distinct j1 j2)
) :interpolation-group _part_b))
(check-sat)
(echo "Interpolant obtained using Mathsat")
(get-interpolant (_part_a))
