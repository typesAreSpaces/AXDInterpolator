
(set-logic ALL)
(set-info :source |SMT script generated on 2021-04-02T18:53:52-06:00 by Ultimate (https://ultimate.informatik.uni-freiburg.de/)|)
(set-info :smt-lib-version 2.5)
(set-info :category "industrial")
(set-info :ultimate-id CfgBuilderScript)
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
(declare-fun |c_reach_error_#t~ret0| () Int)
(declare-fun |c_reach_error_#t~ret0_primed| () Int)
(declare-fun |c___VERIFIER_nondet_ulong_#res| () Int)
(declare-fun |c___VERIFIER_nondet_ulong_#res_primed| () Int)
(declare-fun |c_is_relaxed_prefix_#in~pat.base| () Int)
(declare-fun |c_is_relaxed_prefix_#in~pat.base_primed| () Int)
(declare-fun |c_is_relaxed_prefix_#in~pat.offset| () Int)
(declare-fun |c_is_relaxed_prefix_#in~pat.offset_primed| () Int)
(declare-fun |c_is_relaxed_prefix_#in~pat_length| () Int)
(declare-fun |c_is_relaxed_prefix_#in~pat_length_primed| () Int)
(declare-fun |c_is_relaxed_prefix_#in~a.base| () Int)
(declare-fun |c_is_relaxed_prefix_#in~a.base_primed| () Int)
(declare-fun |c_is_relaxed_prefix_#in~a.offset| () Int)
(declare-fun |c_is_relaxed_prefix_#in~a.offset_primed| () Int)
(declare-fun |c_is_relaxed_prefix_#in~a_length| () Int)
(declare-fun |c_is_relaxed_prefix_#in~a_length_primed| () Int)
(declare-fun |c_is_relaxed_prefix_#res| () Int)
(declare-fun |c_is_relaxed_prefix_#res_primed| () Int)
(declare-fun |c_is_relaxed_prefix_#t~mem2| () Int)
(declare-fun |c_is_relaxed_prefix_#t~mem2_primed| () Int)
(declare-fun |c_is_relaxed_prefix_#t~mem3| () Int)
(declare-fun |c_is_relaxed_prefix_#t~mem3_primed| () Int)
(declare-fun |c_is_relaxed_prefix_#t~post1| () Int)
(declare-fun |c_is_relaxed_prefix_#t~post1_primed| () Int)
(declare-fun c_is_relaxed_prefix_~i~0 () Int)
(declare-fun c_is_relaxed_prefix_~i~0_primed () Int)
(declare-fun c_is_relaxed_prefix_~pat.base () Int)
(declare-fun c_is_relaxed_prefix_~pat.base_primed () Int)
(declare-fun c_is_relaxed_prefix_~pat.offset () Int)
(declare-fun c_is_relaxed_prefix_~pat.offset_primed () Int)
(declare-fun c_is_relaxed_prefix_~pat_length () Int)
(declare-fun c_is_relaxed_prefix_~pat_length_primed () Int)
(declare-fun c_is_relaxed_prefix_~a.base () Int)
(declare-fun c_is_relaxed_prefix_~a.base_primed () Int)
(declare-fun c_is_relaxed_prefix_~a.offset () Int)
(declare-fun c_is_relaxed_prefix_~a.offset_primed () Int)
(declare-fun c_is_relaxed_prefix_~a_length () Int)
(declare-fun c_is_relaxed_prefix_~a_length_primed () Int)
(declare-fun c_is_relaxed_prefix_~shift~0 () Int)
(declare-fun c_is_relaxed_prefix_~shift~0_primed () Int)
(declare-fun |c_assume_abort_if_not_#in~cond| () Int)
(declare-fun |c_assume_abort_if_not_#in~cond_primed| () Int)
(declare-fun c_assume_abort_if_not_~cond () Int)
(declare-fun c_assume_abort_if_not_~cond_primed () Int)
(declare-fun |c_main_#res| () Int)
(declare-fun |c_main_#res_primed| () Int)
(declare-fun |c_main_#t~nondet4| () Int)
(declare-fun |c_main_#t~nondet4_primed| () Int)
(declare-fun |c_main_#t~nondet5| () Int)
(declare-fun |c_main_#t~nondet5_primed| () Int)
(declare-fun |c_main_#t~malloc6.base| () Int)
(declare-fun |c_main_#t~malloc6.base_primed| () Int)
(declare-fun |c_main_#t~malloc6.offset| () Int)
(declare-fun |c_main_#t~malloc6.offset_primed| () Int)
(declare-fun |c_main_#t~malloc7.base| () Int)
(declare-fun |c_main_#t~malloc7.base_primed| () Int)
(declare-fun |c_main_#t~malloc7.offset| () Int)
(declare-fun |c_main_#t~malloc7.offset_primed| () Int)
(declare-fun |c_main_#t~ret8| () Int)
(declare-fun |c_main_#t~ret8_primed| () Int)
(declare-fun |c_main_#t~nondet9| () Int)
(declare-fun |c_main_#t~nondet9_primed| () Int)
(declare-fun |c_main_#t~nondet11| () Int)
(declare-fun |c_main_#t~nondet11_primed| () Int)
(declare-fun |c_main_#t~nondet12| () Int)
(declare-fun |c_main_#t~nondet12_primed| () Int)
(declare-fun |c_main_#t~mem13| () Int)
(declare-fun |c_main_#t~mem13_primed| () Int)
(declare-fun |c_main_#t~mem14| () Int)
(declare-fun |c_main_#t~mem14_primed| () Int)
(declare-fun |c_main_#t~mem15| () Int)
(declare-fun |c_main_#t~mem15_primed| () Int)
(declare-fun |c_main_#t~mem16| () Int)
(declare-fun |c_main_#t~mem16_primed| () Int)
(declare-fun |c_main_#t~mem17| () Int)
(declare-fun |c_main_#t~mem17_primed| () Int)
(declare-fun |c_main_#t~mem18| () Int)
(declare-fun |c_main_#t~mem18_primed| () Int)
(declare-fun |c_main_#t~post10| () Int)
(declare-fun |c_main_#t~post10_primed| () Int)
(declare-fun c_main_~i~1 () Int)
(declare-fun c_main_~i~1_primed () Int)
(declare-fun c_main_~different~0 () Int)
(declare-fun c_main_~different~0_primed () Int)
(declare-fun |c_main_#t~mem20| () Int)
(declare-fun |c_main_#t~mem20_primed| () Int)
(declare-fun |c_main_#t~mem21| () Int)
(declare-fun |c_main_#t~mem21_primed| () Int)
(declare-fun |c_main_#t~pre22| () Int)
(declare-fun |c_main_#t~pre22_primed| () Int)
(declare-fun |c_main_#t~post19| () Int)
(declare-fun |c_main_#t~post19_primed| () Int)
(declare-fun c_main_~i~2 () Int)
(declare-fun c_main_~i~2_primed () Int)
(declare-fun |c_main_#t~pre23| () Int)
(declare-fun |c_main_#t~pre23_primed| () Int)
(declare-fun c_main_~differences~0 () Int)
(declare-fun c_main_~differences~0_primed () Int)
(declare-fun c_main_~pat_len~0 () Int)
(declare-fun c_main_~pat_len~0_primed () Int)
(declare-fun c_main_~a_len~0 () Int)
(declare-fun c_main_~a_len~0_primed () Int)
(declare-fun c_main_~pat~0.base () Int)
(declare-fun c_main_~pat~0.base_primed () Int)
(declare-fun c_main_~pat~0.offset () Int)
(declare-fun c_main_~pat~0.offset_primed () Int)
(declare-fun c_main_~a~0.base () Int)
(declare-fun c_main_~a~0.base_primed () Int)
(declare-fun c_main_~a~0.offset () Int)
(declare-fun c_main_~a~0.offset_primed () Int)
(declare-fun |c_write~int_#value| () Int)
(declare-fun |c_write~int_#value_primed| () Int)
(declare-fun |c_write~int_#ptr.base| () Int)
(declare-fun |c_write~int_#ptr.base_primed| () Int)
(declare-fun |c_write~int_#ptr.offset| () Int)
(declare-fun |c_write~int_#ptr.offset_primed| () Int)
(declare-fun |c_write~int_#sizeOfWrittenType| () Int)
(declare-fun |c_write~int_#sizeOfWrittenType_primed| () Int)
(declare-fun |c_#Ultimate.allocOnHeap_~size| () Int)
(declare-fun |c_#Ultimate.allocOnHeap_~size_primed| () Int)
(declare-fun |c_#Ultimate.allocOnHeap_#res.base| () Int)
(declare-fun |c_#Ultimate.allocOnHeap_#res.base_primed| () Int)
(declare-fun |c_#Ultimate.allocOnHeap_#res.offset| () Int)
(declare-fun |c_#Ultimate.allocOnHeap_#res.offset_primed| () Int)
(declare-fun |c___VERIFIER_nondet_int_#res| () Int)
(declare-fun |c___VERIFIER_nondet_int_#res_primed| () Int)
(declare-fun |c_assert_#in~0| () Int)
(declare-fun |c_assert_#in~0_primed| () Int)
(declare-fun |c_assert_#res| () Int)
(declare-fun |c_assert_#res_primed| () Int)
(declare-fun |c_ULTIMATE.start_#t~ret24| () Int)
(declare-fun |c_ULTIMATE.start_#t~ret24_primed| () Int)
(declare-fun |c_read~int_#ptr.base| () Int)
(declare-fun |c_read~int_#ptr.base_primed| () Int)
(declare-fun |c_read~int_#ptr.offset| () Int)
(declare-fun |c_read~int_#ptr.offset_primed| () Int)
(declare-fun |c_read~int_#sizeOfReadType| () Int)
(declare-fun |c_read~int_#sizeOfReadType_primed| () Int)
(declare-fun |c_read~int_#value| () Int)
(declare-fun |c_read~int_#value_primed| () Int)
(declare-fun |c___VERIFIER_assert_#in~cond| () Int)
(declare-fun |c___VERIFIER_assert_#in~cond_primed| () Int)
(declare-fun c___VERIFIER_assert_~cond () Int)
(declare-fun c___VERIFIER_assert_~cond_primed () Int)
(declare-fun c_ULTIMATE.dealloc_~addr.base () Int)
(declare-fun c_ULTIMATE.dealloc_~addr.base_primed () Int)
(declare-fun c_ULTIMATE.dealloc_~addr.offset () Int)
(declare-fun c_ULTIMATE.dealloc_~addr.offset_primed () Int)
(declare-fun |c_aux_v_reach_error_#t~ret0_3| () Int)

(assert (! true :named codeBlock))

(assert (! true :named precondition))

(assert (! (not (and (= |c_#NULL.base| |c_#NULL.base|) (= |c_#NULL.offset| |c_#NULL.offset|) (= |c_#valid| |c_#valid|))) :named negatedPostcondition))
(check-sat)