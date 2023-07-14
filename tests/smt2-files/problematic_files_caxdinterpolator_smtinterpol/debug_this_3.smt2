
(set-option :print-success false)

(set-logic ALL)

(set-info :source |SMT script generated on 2023-06-09T10:50:50-06:00 by Ultimate (https://ultimate.informatik.uni-freiburg.de/)|)

(set-info :smt-lib-version 2.5)

(set-info :category "industrial")

(set-info :ultimate-id CfgBuilderScript)

(declare-fun |c_old(~N~0)| () Int)

(declare-fun |c_old(~N~0)_primed| () Int)

(declare-fun c_~N~0 () Int)

(declare-fun c_~N~0_primed () Int)

(declare-fun |c_old(#NULL.base)| () Int)

(declare-fun |c_old(#NULL.base)_primed| () Int)

(declare-fun |c_#NULL.base| () Int)

(declare-fun |c_#NULL.base_primed| () Int)

(declare-fun |c_old(#NULL.offset)| () Int)

(declare-fun |c_old(#NULL.offset)_primed| () Int)

(declare-fun |c_#NULL.offset| () Int)

(declare-fun |c_#NULL.offset_primed| () Int)

(declare-fun |c_old(#valid)| () (Array Int Int))

(declare-fun |c_old(#valid)_primed| () (Array Int Int))

(declare-fun |c_#valid| () (Array Int Int))

(declare-fun |c_#valid_primed| () (Array Int Int))

(declare-fun |c_old(#length)| () (Array Int Int))

(declare-fun |c_old(#length)_primed| () (Array Int Int))

(declare-fun |c_#length| () (Array Int Int))

(declare-fun |c_#length_primed| () (Array Int Int))

(declare-fun |c_old(#memory_int)| () (Array Int (Array Int Int)))

(declare-fun |c_old(#memory_int)_primed| () (Array Int (Array Int Int)))

(declare-fun |c_#memory_int| () (Array Int (Array Int Int)))

(declare-fun |c_#memory_int_primed| () (Array Int (Array Int Int)))

(declare-fun |c_old(#StackHeapBarrier)| () Int)

(declare-fun |c_old(#StackHeapBarrier)_primed| () Int)

(declare-fun |c_#StackHeapBarrier| () Int)

(declare-fun |c_#StackHeapBarrier_primed| () Int)

(declare-fun |c_write~int_#value| () Int)

(declare-fun |c_write~int_#value_primed| () Int)

(declare-fun |c_write~int_#ptr.base| () Int)

(declare-fun |c_write~int_#ptr.base_primed| () Int)

(declare-fun |c_write~int_#ptr.offset| () Int)

(declare-fun |c_write~int_#ptr.offset_primed| () Int)

(declare-fun |c_write~int_#sizeOfWrittenType| () Int)

(declare-fun |c_write~int_#sizeOfWrittenType_primed| () Int)

(declare-fun |c_#Ultimate.allocInit_~size| () Int)

(declare-fun |c_#Ultimate.allocInit_~size_primed| () Int)

(declare-fun |c_#Ultimate.allocInit_ptrBase| () Int)

(declare-fun |c_#Ultimate.allocInit_ptrBase_primed| () Int)

(declare-fun |c_#Ultimate.allocOnHeap_~size| () Int)

(declare-fun |c_#Ultimate.allocOnHeap_~size_primed| () Int)

(declare-fun |c_#Ultimate.allocOnHeap_#res.base| () Int)

(declare-fun |c_#Ultimate.allocOnHeap_#res.base_primed| () Int)

(declare-fun |c_#Ultimate.allocOnHeap_#res.offset| () Int)

(declare-fun |c_#Ultimate.allocOnHeap_#res.offset_primed| () Int)

(declare-fun |c_ULTIMATE.start_#t~ret18#1| () Int)

(declare-fun |c_ULTIMATE.start_#t~ret18#1_primed| () Int)

(declare-fun |c_ULTIMATE.start_main_#res#1| () Int)

(declare-fun |c_ULTIMATE.start_main_#res#1_primed| () Int)

(declare-fun |c_ULTIMATE.start_main_#t~nondet4#1| () Int)

(declare-fun |c_ULTIMATE.start_main_#t~nondet4#1_primed| () Int)

(declare-fun |c_ULTIMATE.start_main_#t~malloc5#1.base| () Int)

(declare-fun |c_ULTIMATE.start_main_#t~malloc5#1.base_primed| () Int)

(declare-fun |c_ULTIMATE.start_main_#t~malloc5#1.offset| () Int)

(declare-fun |c_ULTIMATE.start_main_#t~malloc5#1.offset_primed| () Int)

(declare-fun |c_ULTIMATE.start_main_#t~malloc6#1.base| () Int)

(declare-fun |c_ULTIMATE.start_main_#t~malloc6#1.base_primed| () Int)

(declare-fun |c_ULTIMATE.start_main_#t~malloc6#1.offset| () Int)

(declare-fun |c_ULTIMATE.start_main_#t~malloc6#1.offset_primed| () Int)

(declare-fun |c_ULTIMATE.start_main_#t~malloc7#1.base| () Int)

(declare-fun |c_ULTIMATE.start_main_#t~malloc7#1.base_primed| () Int)

(declare-fun |c_ULTIMATE.start_main_#t~malloc7#1.offset| () Int)

(declare-fun |c_ULTIMATE.start_main_#t~malloc7#1.offset_primed| () Int)

(declare-fun |c_ULTIMATE.start_main_#t~mem9#1| () Int)

(declare-fun |c_ULTIMATE.start_main_#t~mem9#1_primed| () Int)

(declare-fun |c_ULTIMATE.start_main_#t~post8#1| () Int)

(declare-fun |c_ULTIMATE.start_main_#t~post8#1_primed| () Int)

(declare-fun |c_ULTIMATE.start_main_#t~mem11#1| () Int)

(declare-fun |c_ULTIMATE.start_main_#t~mem11#1_primed| () Int)

(declare-fun |c_ULTIMATE.start_main_#t~mem12#1| () Int)

(declare-fun |c_ULTIMATE.start_main_#t~mem12#1_primed| () Int)

(declare-fun |c_ULTIMATE.start_main_#t~post10#1| () Int)

(declare-fun |c_ULTIMATE.start_main_#t~post10#1_primed| () Int)

(declare-fun |c_ULTIMATE.start_main_#t~mem14#1| () Int)

(declare-fun |c_ULTIMATE.start_main_#t~mem14#1_primed| () Int)

(declare-fun |c_ULTIMATE.start_main_#t~mem15#1| () Int)

(declare-fun |c_ULTIMATE.start_main_#t~mem15#1_primed| () Int)

(declare-fun |c_ULTIMATE.start_main_#t~post13#1| () Int)

(declare-fun |c_ULTIMATE.start_main_#t~post13#1_primed| () Int)

(declare-fun |c_ULTIMATE.start_main_#t~mem17#1| () Int)

(declare-fun |c_ULTIMATE.start_main_#t~mem17#1_primed| () Int)

(declare-fun |c_ULTIMATE.start_main_#t~post16#1| () Int)

(declare-fun |c_ULTIMATE.start_main_#t~post16#1_primed| () Int)

(declare-fun |c_ULTIMATE.start_main_~i~0#1| () Int)

(declare-fun |c_ULTIMATE.start_main_~i~0#1_primed| () Int)

(declare-fun |c_ULTIMATE.start_main_~a~0#1.base| () Int)

(declare-fun |c_ULTIMATE.start_main_~a~0#1.base_primed| () Int)

(declare-fun |c_ULTIMATE.start_main_~a~0#1.offset| () Int)

(declare-fun |c_ULTIMATE.start_main_~a~0#1.offset_primed| () Int)

(declare-fun |c_ULTIMATE.start_main_~b~0#1.base| () Int)

(declare-fun |c_ULTIMATE.start_main_~b~0#1.base_primed| () Int)

(declare-fun |c_ULTIMATE.start_main_~b~0#1.offset| () Int)

(declare-fun |c_ULTIMATE.start_main_~b~0#1.offset_primed| () Int)

(declare-fun |c_ULTIMATE.start_main_~c~0#1.base| () Int)

(declare-fun |c_ULTIMATE.start_main_~c~0#1.base_primed| () Int)

(declare-fun |c_ULTIMATE.start_main_~c~0#1.offset| () Int)

(declare-fun |c_ULTIMATE.start_main_~c~0#1.offset_primed| () Int)

(declare-fun |c_ULTIMATE.start_assume_abort_if_not_#in~cond#1| () Int)

(declare-fun |c_ULTIMATE.start_assume_abort_if_not_#in~cond#1_primed| () Int)

(declare-fun |c_ULTIMATE.start_assume_abort_if_not_~cond#1| () Int)

(declare-fun |c_ULTIMATE.start_assume_abort_if_not_~cond#1_primed| () Int)

(declare-fun |c_ULTIMATE.start___VERIFIER_assert_#in~cond#1| () Int)

(declare-fun |c_ULTIMATE.start___VERIFIER_assert_#in~cond#1_primed| () Int)

(declare-fun |c_ULTIMATE.start___VERIFIER_assert_~cond#1| () Int)

(declare-fun |c_ULTIMATE.start___VERIFIER_assert_~cond#1_primed| () Int)

(declare-fun |c_write~init~int_#value| () Int)

(declare-fun |c_write~init~int_#value_primed| () Int)

(declare-fun |c_write~init~int_#ptr.base| () Int)

(declare-fun |c_write~init~int_#ptr.base_primed| () Int)

(declare-fun |c_write~init~int_#ptr.offset| () Int)

(declare-fun |c_write~init~int_#ptr.offset_primed| () Int)

(declare-fun |c_write~init~int_#sizeOfWrittenType| () Int)

(declare-fun |c_write~init~int_#sizeOfWrittenType_primed| () Int)

(declare-fun |c_read~int_#ptr.base| () Int)

(declare-fun |c_read~int_#ptr.base_primed| () Int)

(declare-fun |c_read~int_#ptr.offset| () Int)

(declare-fun |c_read~int_#ptr.offset_primed| () Int)

(declare-fun |c_read~int_#sizeOfReadType| () Int)

(declare-fun |c_read~int_#sizeOfReadType_primed| () Int)

(declare-fun |c_read~int_#value| () Int)

(declare-fun |c_read~int_#value_primed| () Int)






(declare-fun |v_ULTIMATE.start___VERIFIER_assert_~cond#1_3_const_-1232128146| () Int)

(assert (= |v_ULTIMATE.start___VERIFIER_assert_~cond#1_3_const_-1232128146| 0))

(check-sat)

