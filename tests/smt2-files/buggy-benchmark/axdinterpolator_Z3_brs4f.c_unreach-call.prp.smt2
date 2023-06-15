; 
(set-info :status unknown)
(declare-fun |c_ULTIMATE.start_main_~i~0#1| () Int)
(declare-fun |c_ULTIMATE.start_main_~#sum~0#1.offset| () Int)
(declare-fun |c_ULTIMATE.start_main_~#sum~0#1.base| () Int)
(declare-fun |c_#memory_int| () (Array Int (Array Int Int)))
(declare-fun |c_#memory_int_primed| () (Array Int (Array Int Int)))
(declare-fun c_~N~0 () Int)
(assert
 (let (($x16 (= |c_ULTIMATE.start_main_~i~0#1| 0)))
 (let ((?x11 (store (select |c_#memory_int| |c_ULTIMATE.start_main_~#sum~0#1.base|) |c_ULTIMATE.start_main_~#sum~0#1.offset| 40)))
 (let (($x13 (= |c_#memory_int_primed| (store |c_#memory_int| |c_ULTIMATE.start_main_~#sum~0#1.base| ?x11))))
 (and true $x13 $x16)))))
(assert
 (let (($x43 (or (not (= |c_ULTIMATE.start_main_~i~0#1| 0)) (not (<= 1 c_~N~0)))))
 (let (($x20 (<= 1 c_~N~0)))
 (and true $x20 $x43))))
(check-sat)
