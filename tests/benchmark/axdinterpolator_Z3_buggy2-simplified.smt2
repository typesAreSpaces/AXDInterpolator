; 
(set-info :status unknown)
(declare-fun |c_ULTIMATE.start_main_~#sum~0#1.offset| () Int)
(declare-fun |c_ULTIMATE.start_main_~#sum~0#1.base| () Int)
(declare-fun |c_#memory_int| () (Array Int (Array Int Int)))
(declare-fun c_~N~0 () Int)
(assert
 (let ((?x9 (select (select |c_#memory_int| |c_ULTIMATE.start_main_~#sum~0#1.base|) |c_ULTIMATE.start_main_~#sum~0#1.offset|)))
 (let (($x11 (= ?x9 0)))
 (and true $x11))))
(assert
 (let (($x16 (< c_~N~0 1)))
 (let (($x14 (<= 1 c_~N~0)))
 (and true $x14 $x16))))
(check-sat)
