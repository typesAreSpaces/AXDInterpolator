(set-option :produce-interpolants true)
; 
(set-info :status unknown)
(assert (! -fun c_ULTIMATE.start___VERIFIER_assert_~cond () Int :interpolation-group part_a))
(assert (! (let (($x8 (not (= c_ULTIMATE.start___VERIFIER_assert_~cond 0)))) (and true $x8)) :interpolation-group part_b))
(assert (! (let (($x9 (= 0 c_ULTIMATE.start___VERIFIER_assert_~cond))) (and true $x9)) :interpolation-group part_b))
(check-sat)
(get-interpolant (part_a))
