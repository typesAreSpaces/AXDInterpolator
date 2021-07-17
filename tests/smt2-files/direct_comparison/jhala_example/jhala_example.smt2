(set-option :produce-interpolants true)
(set-logic AUFLIA)
(declare-fun a () (Array Int Int))
(declare-fun b () (Array Int Int))
(declare-fun i () Int)
(declare-fun e () Int)
(declare-fun j1 () Int)
(declare-fun j2 () Int)

(define-fun part_a () Bool 
(= a (store b i e))
)
(define-fun part_b () Bool (and 
(distinct (select a j1) (select b j1)) 
(distinct (select a j2) (select b j2))
(distinct j1 j2)
))

(push)
(assert (! part_a :named _part_a))
(assert (! part_b :named _part_b))
(check-sat)
(echo "Interpolant obtained using Z3")
(get-interpolant _part_a _part_b)
(pop)

(push)
(declare-fun diff ((Array Int Int) (Array Int Int)) Int)
(declare-fun undefinedInt () Int)
(declare-fun empty_arrayInt () (Array Int Int))

(define-fun b2 () (Array Int Int)
(store b (diff a b) (select a (diff a b)))
)
(define-fun diff2ab () Int
(diff a b2)
)
(define-fun _my_interpolant_by_prev_paper () Bool
(or (= (diff a b) 0) (and (= (select a diff2ab) (select b diff2ab))))
)

(define-fun _my_interpolant_by_hand () Bool
(= a (store b (diff a b) (select a (diff a b))))
)

(define-fun _interpolant_computed_by_axdinterpolator_with_z3 () Bool
(let ((a!1 (or (not (<= (diff a b) 0)) (not (>= (diff a b) 0))))
      (a!3 (or (<= (select a diff2ab) (select b diff2ab))
               (<= (diff a b) diff2ab)))
      (a!4 (+ undefinedInt (* (- 1) (select empty_arrayInt diff2ab))))
      (a!7 (or (<= (select b diff2ab) (select a diff2ab))
               (<= (diff a b) diff2ab)))
      (a!10 (not (<= (select a (diff a b)) (select b (diff a b)))))
      (a!11 (not (<= (select b (diff a b)) (select a (diff a b)))))
      (a!12 (or (<= diff2ab 0) (not (<= (diff a b) 0)))))
(let ((a!2 (or (not (<= (diff a b) diff2ab)) (not a!1)))
      (a!5 (or (= diff2ab a!4) (not (<= diff2ab 0))))
      (a!8 (and (not (= diff2ab a!4))
                (or (= diff2ab a!4)
                    (<= (diff a b) diff2ab))))
      (a!13 (or (= a!4 (diff a b)) (not (<= (diff a b) 0)))))
(let ((a!6 (and (not (= diff2ab a!4)) a!5))
      (a!14 (and (not (or a!10 a!11))
                 (>= (diff a b) 0)
                 a!12
                 (<= (select empty_arrayInt diff2ab) undefinedInt)
                 (<= undefinedInt (select empty_arrayInt diff2ab))
                 a!13
                 (or (<= (diff a b) 0) a!10 a!11)
                 (or (<= diff2ab 0) a!10 a!11)
                 (or (= a!4 (diff a b)) a!10 a!11))))
(let ((a!9 (and a!3
                (or (= diff2ab 0) a!6)
                a!7
                (or (<= diff2ab 0)
                    (<= (diff a b) diff2ab))
                (or (= diff2ab 0) a!8))))
  (and a!2
       (or a!9 a!14)
       (<= diff2ab (diff a b))
       (>= (diff a b) 0))))))
)
(define-fun _interpolant_computed_by_axdinterpolator_with_mathsat () Bool
(let ((a!1 (and (or (= (diff a b) 0) (= (select a 0) (select b 0)))
                (<= (diff a b) diff2ab)
                (<= diff2ab 0)))
      (a!2 (and (= (select b (diff a b)) (select a (diff a b)))
                (<= diff2ab 0))))
(let ((a!3 (and (not (= (select a 0) (select b 0))) (not a!1) (not a!2))))
  (and (<= diff2ab 0)
       (or (<= (diff a b) diff2ab) (= (select a 0) (select b 0)))
       (not a!3))))
)
(define-fun _interpolant_computed_by_axdinterpolator_with_SMTInterpol () Bool
(let ((a!1 (and (= (select a diff2ab) (select b diff2ab))
                (<= diff2ab 0))))
(let ((a!2 (or (and (<= (diff a b) diff2ab)
                    (<= diff2ab 0))
               a!1)))
(let ((a!3 (and a!2 (or (<= (diff a b) diff2ab) a!1))))
  (and (or a!1 a!3)))))
)
(define-fun _interpolant_computed_by_SMTInterpol () Bool
(let ((.cse1 (diff b a))) (let ((.cse2 (= (store b .cse1 (select a .cse1)) a))) (and (let ((.cse0 (= b a))) (or .cse0 (and (or .cse0 (= .cse1 .cse1)) .cse2))) .cse2)))
)

(assert (forall ((y (Array Int Int)) (i Int) (e Int)) 
(=> (>= i 0) (= (select (store y i e) i) e))
))
(assert (forall ((y (Array Int Int)) (i Int) (j Int) (e Int)) 
(=> (distinct i j) (= (select (store y i e) j) (select y j)))
))
(assert (forall ((x (Array Int Int)) (y (Array Int Int)) ) 
(=> (distinct x y) (distinct (select x (diff x y)) (select y (diff x y))))
))
(assert (forall ((x (Array Int Int)) (y (Array Int Int)) (i Int)) 
(=> (> i (diff x y)) (= (select x i) (select y i)))
))
(assert (forall ((x (Array Int Int))) (= (diff x x) 0)
))
(assert (forall ((x (Array Int Int)) (i Int)) 
(=> (< i 0) (= (select x i) undefinedInt))
))
(assert (forall ((i Int)) 
(= (select empty_arrayInt i) undefinedInt)
))

;(push)
;(assert (not (=> part_a _my_interpolant_by_hand))) 
;(echo "A-part test using my interpolant by hand")
;(check-sat)
;(pop)

;(push)
;(assert part_b)
;(assert _my_interpolant_by_hand)
;(echo "B-part test using my interpolant by hand")
;(check-sat)
;(pop)

(push)
(assert (not (=> _my_interpolant_by_prev_paper _my_interpolant_by_hand))) 
(echo "Testing strength of manual interpolants - I")
(check-sat)
(pop)

(push)
(assert (not (=> _my_interpolant_by_hand _my_interpolant_by_prev_paper))) 
(echo "Testing strength of manual interpolants - II")
(check-sat)
(pop)

;(push)
;(assert (not (=> part_a  _interpolant_computed_by_axdinterpolator_with_z3))) 
;(echo "A-part test using interpolant computed by AXDInterpolator using Z3")
;(check-sat)
;(pop)

;(push)
;(assert part_b)
;(assert _interpolant_computed_by_axdinterpolator_with_z3)
;(echo "B-part test using interpolant computed by AXDInterpolator using Z3")
;(check-sat)
;(pop)

;(push)
;(assert (not (=> part_a  _interpolant_computed_by_axdinterpolator_with_mathsat))) 
;(echo "A-part test using interpolant computed by AXDInterpolator using Mathsat")
;(check-sat)
;(pop)

;(push)
;(assert part_b)
;(assert _interpolant_computed_by_axdinterpolator_with_mathsat)
;(echo "B-part test using interpolant computed by AXDInterpolator using Mathsat")
;(check-sat)
;(pop)

;(push)
;(assert (not (=> part_a  _interpolant_computed_by_axdinterpolator_with_SMTInterpol))) 
;(echo "A-part test using interpolant computed by AXDInterpolator using SMTInterpol")
;(check-sat)
;(pop)

;(push)
;(assert part_b)
;(assert _interpolant_computed_by_axdinterpolator_with_SMTInterpol)
;(echo "B-part test using interpolant computed by AXDInterpolator using SMTInterpol")
;(check-sat)
;(pop)

;(push)
;(assert (not (=> _interpolant_computed_by_axdinterpolator_with_z3 _interpolant_computed_by_SMTInterpol)))
;(echo "If the following unsats, axdinterpolator with z3 result is stronger than SMTInterpol result")
;(check-sat)
;(pop)

;(push)
;(assert (not (=> _interpolant_computed_by_SMTInterpol _interpolant_computed_by_axdinterpolator_with_mathsat)))
;(echo "If the following unsats, SMTInterpol result is stronger than axdinterpolator with z3 result")
;(check-sat)
;(pop)

;(push)
;(assert (not (=> _interpolant_computed_by_axdinterpolator_with_mathsat _interpolant_computed_by_SMTInterpol)))
;(echo "If the following unsats, axdinterpolator with Mathsat result is stronger than SMTInterpol result")
;(check-sat)
;(pop)

;(push)
;(assert (not (=> _interpolant_computed_by_SMTInterpol _interpolant_computed_by_axdinterpolator_with_mathsat)))
;(echo "If the following unsats, SMTInterpol result is stronger than axdinterpolator with Mathsat result")
;(check-sat)
;(pop)

;(push)
;(assert (not (=> _interpolant_computed_by_axdinterpolator_with_SMTInterpol _interpolant_computed_by_SMTInterpol)))
;(echo "If the following unsats, axdinterpolator with SMTInterpol result is stronger than SMTInterpol result")
;(check-sat)
;(pop)

;(push)
;(assert (not (=> _interpolant_computed_by_SMTInterpol _interpolant_computed_by_axdinterpolator_with_SMTInterpol)))
;(echo "If the following unsats, SMTInterpol result is stronger than axdinterpolator with SMTInterpol result")
;(check-sat)
;(pop)

;(pop)
