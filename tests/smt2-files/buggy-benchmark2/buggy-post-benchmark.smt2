; 
(set-info :status unknown)
(declare-fun c_build_nondet_String_~length~0_primed () Int)
(declare-fun c_build_nondet_String_~length~0 () Int)
(assert
 (let (($x7 (= c_build_nondet_String_~length~0_primed 1)))
 (and true $x7)))
(assert
 (let (($x12 (not (<= 1 c_build_nondet_String_~length~0_primed))))
 (let (($x9 (< c_build_nondet_String_~length~0 1)))
 (and true $x9 $x12))))
(check-sat)
