; 
(set-info :status unknown)
(declare-fun |c_#valid| () (Array Int Int))
(declare-fun |c_#NULL.offset| () Int)
(declare-fun |c_#NULL.base| () Int)
(assert (and true
     true
     (not (and (= |c_#NULL.base| |c_#NULL.base|)
               (= |c_#NULL.offset| |c_#NULL.offset|)
               (= |c_#valid| |c_#valid|)))))
(assert false)
(check-sat)
