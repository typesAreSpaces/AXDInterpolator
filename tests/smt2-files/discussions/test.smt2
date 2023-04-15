(declare-sort A)
(declare-fun a () A)
(declare-fun a1 () A)
(declare-fun a2 () A)
(declare-fun f (A A) A)
(declare-fun g (A A A) A)

(push)
(assert (= a1 (f a a2)))
(assert (= a2 (f a a)))

(assert (distinct (f a a1) (f a (f a (f a a)))))

(check-sat)

(pop)

(push)
(assert (= (f a a) (g a a a)))
(check-sat)
(pop)

