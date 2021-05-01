(set-option :produce-interpolants true)
(set-logic QF_AUFLIA)
; 
(set-info :status unknown)
(assert (! -fun c_ULTIMATE.start___VERIFIER_assert_~cond () Int :named part_a))
(assert (! (let (($x8 (not (= c_ULTIMATE.start___VERIFIER_assert_~cond 0)))) (and true $x8)) :named part_b))
(assert (! (let (($x9 (= 0 c_ULTIMATE.start___VERIFIER_assert_~cond))) (and true $x9)) :named part_b))
(check-sat)
(get-interpolant part_a part_b)
