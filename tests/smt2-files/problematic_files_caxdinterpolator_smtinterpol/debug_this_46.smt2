
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





























(declare-fun c_aux_v_ArrVal_12 () (Array Int Int))

(declare-fun c_aux_v_ArrVal_10 () (Array Int Int))

(declare-fun c_aux_v_ArrVal_2 () (Array Int Int))

(declare-fun c_aux_v_ArrVal_9 () Int)

(declare-fun c_aux_v_ArrVal_4 () (Array Int Int))

(declare-fun c_aux_v_ArrVal_7 () Int)

(declare-fun c_aux_v_ArrVal_14 () (Array Int Int))

(declare-fun c_aux_v_ArrVal_17 () Int)

(declare-fun c_aux_v_ArrVal_1 () Int)

(declare-fun c_aux_v_ArrVal_18 () Int)

(declare-fun c_aux_v_ArrVal_19 () Int)

(declare-fun c_aux_v_ArrVal_6 () (Array Int Int))

(declare-fun c_aux_v_ArrVal_13 () Int)

(declare-fun c_aux_v_ArrVal_5 () Int)

(declare-fun c_aux_v_ArrVal_8 () (Array Int Int))

(declare-fun c_aux_v_ArrVal_15 () Int)

(declare-fun c_aux_v_ArrVal_3 () Int)

(declare-fun c_aux_v_ArrVal_16 () Int)

(declare-fun c_aux_v_ArrVal_20 () Int)

(declare-fun c_aux_v_ArrVal_21 () Int)

(declare-fun c_aux_v_ArrVal_11 () Int)

(declare-fun c_aux_v_ArrVal_22 () Int)

(declare-fun c_aux_v_ArrVal_23 () Int)

(declare-fun c_aux_v_ArrVal_33 () (Array Int Int))

(declare-fun c_aux_v_ArrVal_31 () (Array Int Int))

(declare-fun c_aux_v_ArrVal_37 () (Array Int Int))

(declare-fun c_aux_v_ArrVal_27 () (Array Int Int))

(declare-fun c_aux_v_ArrVal_35 () (Array Int Int))

(declare-fun c_aux_v_ArrVal_25 () (Array Int Int))

(declare-fun c_aux_v_ArrVal_29 () (Array Int Int))

(declare-fun c_aux_v_ArrVal_28 () Int)

(declare-fun c_aux_v_ArrVal_39 () Int)

(declare-fun c_aux_v_ArrVal_24 () Int)

(declare-fun c_aux_v_ArrVal_46 () Int)

(declare-fun c_aux_v_ArrVal_36 () Int)

(declare-fun c_aux_v_ArrVal_26 () Int)

(declare-fun c_aux_v_ArrVal_38 () Int)

(declare-fun c_aux_v_ArrVal_42 () Int)

(declare-fun c_aux_v_ArrVal_32 () Int)

(declare-fun c_aux_v_ArrVal_43 () Int)

(declare-fun c_aux_v_ArrVal_44 () Int)

(declare-fun c_aux_v_ArrVal_34 () Int)

(declare-fun c_aux_v_ArrVal_45 () Int)

(declare-fun c_aux_v_ArrVal_40 () Int)

(declare-fun c_aux_v_ArrVal_30 () Int)

(declare-fun c_aux_v_ArrVal_41 () Int)

(declare-fun c_aux_v_ArrVal_52 () (Array Int Int))

(declare-fun c_aux_v_ArrVal_60 () (Array Int Int))

(declare-fun c_aux_v_ArrVal_50 () (Array Int Int))

(declare-fun c_aux_v_ArrVal_56 () (Array Int Int))

(declare-fun c_aux_v_ArrVal_54 () (Array Int Int))

(declare-fun c_aux_v_ArrVal_48 () (Array Int Int))

(declare-fun c_aux_v_ArrVal_58 () (Array Int Int))

(declare-fun c_aux_v_ArrVal_57 () Int)

(declare-fun c_aux_v_ArrVal_68 () Int)

(declare-fun c_aux_v_ArrVal_47 () Int)

(declare-fun c_aux_v_ArrVal_69 () Int)

(declare-fun c_aux_v_ArrVal_59 () Int)

(declare-fun c_aux_v_ArrVal_49 () Int)

(declare-fun c_aux_v_ArrVal_53 () Int)

(declare-fun c_aux_v_ArrVal_64 () Int)

(declare-fun c_aux_v_ArrVal_65 () Int)

(declare-fun c_aux_v_ArrVal_55 () Int)

(declare-fun c_aux_v_ArrVal_66 () Int)

(declare-fun c_aux_v_ArrVal_67 () Int)

(declare-fun c_aux_v_ArrVal_61 () Int)

(declare-fun c_aux_v_ArrVal_51 () Int)

(declare-fun c_aux_v_ArrVal_62 () Int)

(declare-fun c_aux_v_ArrVal_63 () Int)

(declare-fun c_aux_v_ArrVal_73 () (Array Int Int))

(declare-fun c_aux_v_ArrVal_90 () Int)

(declare-fun c_aux_v_ArrVal_71 () (Array Int Int))

(declare-fun c_aux_v_ArrVal_80 () Int)

(declare-fun c_aux_v_ArrVal_91 () Int)

(declare-fun c_aux_v_ArrVal_70 () Int)

(declare-fun c_aux_v_ArrVal_83 () (Array Int Int))

(declare-fun c_aux_v_ArrVal_92 () Int)

(declare-fun c_aux_v_ArrVal_77 () (Array Int Int))

(declare-fun c_aux_v_ArrVal_75 () (Array Int Int))

(declare-fun c_aux_v_ArrVal_79 () (Array Int Int))

(declare-fun c_aux_v_ArrVal_86 () Int)

(declare-fun c_aux_v_ArrVal_76 () Int)

(declare-fun c_aux_v_ArrVal_87 () Int)

(declare-fun c_aux_v_ArrVal_88 () Int)

(declare-fun c_aux_v_ArrVal_78 () Int)

(declare-fun c_aux_v_ArrVal_89 () Int)

(declare-fun c_aux_v_ArrVal_82 () Int)

(declare-fun c_aux_v_ArrVal_72 () Int)

(declare-fun c_aux_v_ArrVal_81 () (Array Int Int))

(declare-fun c_aux_v_ArrVal_84 () Int)

(declare-fun c_aux_v_ArrVal_74 () Int)

(declare-fun c_aux_v_ArrVal_85 () Int)

(declare-fun c_aux_v_ArrVal_95 () (Array Int Int))

(declare-fun c_aux_v_ArrVal_96 () (Array Int Int))

(declare-fun c_aux_v_ArrVal_93 () (Array Int Int))

(declare-fun c_aux_v_ArrVal_94 () (Array Int Int))

(declare-fun c_aux_v_ArrVal_98 () (Array Int Int))

(declare-fun c_aux_v_ArrVal_97 () Int)














(declare-fun |ULTIMATE.start_assume_abort_if_not_~cond#1| () Int)

(assert true)







(assert (not (= |ULTIMATE.start_assume_abort_if_not_~cond#1| 0)))




(assert (= |ULTIMATE.start_assume_abort_if_not_~cond#1| 1))

(check-sat)

