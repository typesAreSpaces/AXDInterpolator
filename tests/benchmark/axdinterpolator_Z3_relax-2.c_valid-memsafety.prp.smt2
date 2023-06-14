; 
(set-info :status unknown)
(declare-fun |c_ULTIMATE.start_main_~pat~0#1.base| () Int)
(declare-fun |c_#valid| () (Array Int Int))
(assert
 (let (($x9 (= (select |c_#valid| |c_ULTIMATE.start_main_~pat~0#1.base|) 1)))
 (let (($x10 (not $x9)))
 (and true $x10))))
(assert
 (let (($x9 (= (select |c_#valid| |c_ULTIMATE.start_main_~pat~0#1.base|) 1)))
 (and true $x9)))
(check-sat)
