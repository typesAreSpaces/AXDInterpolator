(set-info :status unknown)
(declare-sort ElementSort 0)
 (declare-sort ArraySort 0)
 (declare-fun e3 () ElementSort)
(declare-fun rd (ArraySort Int) ElementSort)
(declare-fun i3 () Int)
(declare-fun a () ArraySort)
(declare-fun i1 () Int)
(declare-fun a1 () ArraySort)
(declare-fun i2 () Int)
(declare-fun e1 () ElementSort)
(declare-fun b () ArraySort)
(declare-fun c2 () ArraySort)
(declare-fun c1 () ArraySort)
(assert
 (let ((?x65 (rd a i3)))
 (= ?x65 e3)))
(assert
 (=> (and (distinct i1 i3) true) (= (rd a i1) (rd a1 i1))))
(assert
 (= 1 1))
(assert
 (let ((?x76 (rd a1 i2)))
 (let ((?x77 (rd a i2)))
 (let (($x78 (= ?x77 ?x76)))
 (=> (and (distinct i2 i3) true) $x78)))))
(assert
 (let ((?x67 (rd a1 i1)))
 (= ?x67 e1)))
(assert
 (= 1 1))
(assert
 (let ((?x86 (rd b i3)))
 (let ((?x72 (rd a1 i3)))
 (let (($x87 (= ?x72 ?x86)))
 (=> (and (distinct i3 i1) true) $x87)))))
(assert
 (let ((?x90 (rd b i2)))
 (let ((?x76 (rd a1 i2)))
 (let (($x91 (= ?x76 ?x90)))
 (=> (and (distinct i2 i1) true) $x91)))))
(assert
 (let ((?x82 (rd b i1)))
 (let ((?x94 (rd c2 i1)))
 (let (($x95 (= ?x94 ?x82)))
 (let (($x96 (> i1 i1)))
 (= 1 1))))))
(assert
 (let ((?x86 (rd b i3)))
 (let ((?x98 (rd c2 i3)))
 (let (($x99 (= ?x98 ?x86)))
 (let (($x100 (> i3 i1)))
 (=> $x100 $x99))))))
(assert
 (let ((?x90 (rd b i2)))
 (let ((?x45 (rd c2 i2)))
 (let (($x102 (= ?x45 ?x90)))
 (let (($x103 (> i2 i1)))
 (=> $x103 $x102))))))
(assert
 (let (($x105 (= i1 0)))
 (let ((?x82 (rd b i1)))
 (let ((?x94 (rd c2 i1)))
 (let (($x95 (= ?x94 ?x82)))
 (=> $x95 $x105))))))
(assert
 (let ((?x68 (rd a i1)))
 (let ((?x107 (rd c1 i1)))
 (let (($x108 (= ?x107 ?x68)))
 (let (($x96 (> i1 i1)))
 (= 1 1))))))
(assert
 (let (($x100 (> i3 i1)))
 (=> $x100 (= (rd c1 i3) (rd a i3)))))
(assert
 (let ((?x77 (rd a i2)))
 (let ((?x44 (rd c1 i2)))
 (let (($x113 (= ?x44 ?x77)))
 (let (($x103 (> i2 i1)))
 (=> $x103 $x113))))))
(assert
 (let (($x105 (= i1 0)))
 (let ((?x68 (rd a i1)))
 (let ((?x107 (rd c1 i1)))
 (let (($x108 (= ?x107 ?x68)))
 (=> $x108 $x105))))))
(assert
 (< i1 i2))
(assert
 (< i2 i3))
(assert
 (let ((?x45 (rd c2 i2)))
 (let ((?x44 (rd c1 i2)))
 (and (distinct ?x44 ?x45) true))))
(check-sat)
