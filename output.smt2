; 
(set-info :status unknown)
(declare-sort ElementSort)
 (declare-sort ArraySort)
 (declare-fun e3 () ElementSort)
(declare-fun rd (ArraySort Int) ElementSort)
(declare-fun i3 () Int)
(declare-fun a () ArraySort)
(declare-fun i1 () Int)
(declare-fun a1 () ArraySort)
(declare-fun e1 () ElementSort)
(declare-fun b () ArraySort)
(declare-fun c2 () ArraySort)
(declare-fun c1 () ArraySort)
(declare-fun fresh_index_0 () Int)
(declare-fun fresh_index_1 () Int)
(declare-fun i2 () Int)
(assert
 (let ((?x51 (rd a i3)))
 (= ?x51 e3)))
(assert
 (let ((?x53 (rd a1 i1)))
 (let ((?x54 (rd a i1)))
 (let (($x55 (= ?x54 ?x53)))
 (=> (and (distinct i1 i3) true) $x55)))))
(assert
 (=> (and (distinct i3 i3) true) (= (rd a i3) (rd a1 i3))))
(assert
 (let ((?x53 (rd a1 i1)))
 (= ?x53 e1)))
(assert
 (=> (and (distinct i1 i1) true) (= (rd a1 i1) (rd b i1))))
(assert
 (let ((?x67 (rd b i3)))
 (let ((?x58 (rd a1 i3)))
 (let (($x68 (= ?x58 ?x67)))
 (=> (and (distinct i3 i1) true) $x68)))))
(assert
 (let ((?x63 (rd b i1)))
 (let ((?x71 (rd c2 i1)))
 (let (($x72 (= ?x71 ?x63)))
 (=> (> i1 i1) $x72)))))
(assert
 (let ((?x67 (rd b i3)))
 (let ((?x75 (rd c2 i3)))
 (let (($x76 (= ?x75 ?x67)))
 (let (($x77 (> i3 i1)))
 (=> $x77 $x76))))))
(assert
 (let (($x80 (= i1 0)))
 (let ((?x63 (rd b i1)))
 (let ((?x71 (rd c2 i1)))
 (let (($x72 (= ?x71 ?x63)))
 (=> $x72 $x80))))))
(assert
 (let ((?x54 (rd a i1)))
 (let ((?x82 (rd c1 i1)))
 (let (($x83 (= ?x82 ?x54)))
 (=> (> i1 i1) $x83)))))
(assert
 (let ((?x51 (rd a i3)))
 (let ((?x85 (rd c1 i3)))
 (let (($x86 (= ?x85 ?x51)))
 (let (($x77 (> i3 i1)))
 (=> $x77 $x86))))))
(assert
 (let (($x80 (= i1 0)))
 (let ((?x54 (rd a i1)))
 (let ((?x82 (rd c1 i1)))
 (let (($x83 (= ?x82 ?x54)))
 (=> $x83 $x80))))))
(assert
 (let (($x236 (= fresh_index_0 0)))
 (let ((?x239 (rd c1 fresh_index_0)))
 (let ((?x237 (rd c2 fresh_index_0)))
 (let (($x234 (= ?x237 ?x239)))
 (=> $x234 $x236))))))
(assert
 (let (($x231 (> i1 fresh_index_0)))
 (=> $x231 (or (= (rd c2 i1) (rd c1 i1)) (= i1 fresh_index_0)))))
(assert
 (let (($x193 (> i3 fresh_index_0)))
 (=> $x193 (or (= (rd c2 i3) (rd c1 i3)) (= i3 fresh_index_0)))))
(assert
 (let ((?x239 (rd c1 fresh_index_0)))
 (let ((?x237 (rd c2 fresh_index_0)))
 (let (($x234 (= ?x237 ?x239)))
 (=> (> fresh_index_0 fresh_index_0) (or $x234 (= fresh_index_0 fresh_index_0)))))))
(assert
 (let ((?x183 (rd a1 fresh_index_0)))
 (let ((?x184 (rd a fresh_index_0)))
 (let (($x185 (= ?x184 ?x183)))
 (=> (and (distinct fresh_index_0 i3) true) $x185)))))
(assert
 (let ((?x188 (rd b fresh_index_0)))
 (let ((?x183 (rd a1 fresh_index_0)))
 (let (($x189 (= ?x183 ?x188)))
 (=> (and (distinct fresh_index_0 i1) true) $x189)))))
(assert
 (let (($x147 (> fresh_index_0 i1)))
 (=> $x147 (or (= (rd c2 fresh_index_0) (rd b fresh_index_0))))))
(assert
 (=> (> fresh_index_0 fresh_index_0) (or (= (rd c2 fresh_index_0) (rd c1 fresh_index_0)))))
(assert
 (let (($x147 (> fresh_index_0 i1)))
 (=> $x147 (or (= (rd c1 fresh_index_0) (rd a fresh_index_0))))))
(assert
 (>= fresh_index_0 fresh_index_1))
(assert
 (let (($x472 (> fresh_index_0 fresh_index_1)))
 (=> $x472 (and (distinct (rd c2 fresh_index_0) (rd c1 fresh_index_0)) true))))
(assert
 (let (($x236 (= fresh_index_0 0)))
 (let (($x470 (= fresh_index_0 fresh_index_1)))
 (=> $x470 $x236))))
(assert
 (let (($x468 (= fresh_index_1 0)))
 (let ((?x444 (rd c1 fresh_index_1)))
 (let ((?x445 (rd c2 fresh_index_1)))
 (let (($x446 (= ?x445 ?x444)))
 (=> $x446 $x468))))))
(assert
 (let (($x448 (= i1 fresh_index_1)))
 (let (($x229 (= i1 fresh_index_0)))
 (let ((?x82 (rd c1 i1)))
 (let ((?x71 (rd c2 i1)))
 (let (($x228 (= ?x71 ?x82)))
 (let (($x452 (> i1 fresh_index_1)))
 (=> $x452 (or $x228 $x229 $x448)))))))))
(assert
 (let (($x454 (= i3 fresh_index_1)))
 (let (($x191 (= i3 fresh_index_0)))
 (let ((?x85 (rd c1 i3)))
 (let ((?x75 (rd c2 i3)))
 (let (($x233 (= ?x75 ?x85)))
 (let (($x456 (> i3 fresh_index_1)))
 (=> $x456 (or $x233 $x191 $x454)))))))))
(assert
 (let (($x470 (= fresh_index_0 fresh_index_1)))
 (let ((?x239 (rd c1 fresh_index_0)))
 (let ((?x237 (rd c2 fresh_index_0)))
 (let (($x234 (= ?x237 ?x239)))
 (let (($x472 (> fresh_index_0 fresh_index_1)))
 (=> $x472 (or $x234 (= fresh_index_0 fresh_index_0) $x470))))))))
(assert
 (let (($x460 (= fresh_index_1 fresh_index_0)))
 (let ((?x444 (rd c1 fresh_index_1)))
 (let ((?x445 (rd c2 fresh_index_1)))
 (let (($x446 (= ?x445 ?x444)))
 (=> (> fresh_index_1 fresh_index_1) (or $x446 $x460 (= fresh_index_1 fresh_index_1))))))))
(assert
 (let ((?x465 (rd a1 fresh_index_1)))
 (let ((?x466 (rd a fresh_index_1)))
 (let (($x478 (= ?x466 ?x465)))
 (=> (and (distinct fresh_index_1 i3) true) $x478)))))
(assert
 (let ((?x372 (rd b fresh_index_1)))
 (let ((?x465 (rd a1 fresh_index_1)))
 (let (($x99 (= ?x465 ?x372)))
 (=> (and (distinct fresh_index_1 i1) true) $x99)))))
(assert
 (let (($x439 (> fresh_index_1 i1)))
 (=> $x439 (or (= (rd c2 fresh_index_1) (rd b fresh_index_1))))))
(assert
 (=> (> fresh_index_1 fresh_index_0) (or (= (rd c2 fresh_index_1) (rd c1 fresh_index_1)))))
(assert
 (let (($x460 (= fresh_index_1 fresh_index_0)))
 (let ((?x444 (rd c1 fresh_index_1)))
 (let ((?x445 (rd c2 fresh_index_1)))
 (let (($x446 (= ?x445 ?x444)))
 (=> (> fresh_index_1 fresh_index_1) (or $x446 $x460)))))))
(assert
 (let (($x439 (> fresh_index_1 i1)))
 (=> $x439 (or (= (rd c1 fresh_index_1) (rd a fresh_index_1))))))
(assert
 (< i1 i2))
(assert
 (< i2 i3))
(assert
 (let ((?x27 (rd c2 i2)))
 (let ((?x26 (rd c1 i2)))
 (and (distinct ?x26 ?x27) true))))
(assert
 (let (($x236 (= fresh_index_0 0)))
 (let ((?x239 (rd c1 fresh_index_0)))
 (let ((?x237 (rd c2 fresh_index_0)))
 (let (($x234 (= ?x237 ?x239)))
 (=> $x234 $x236))))))
(assert
 (let (($x231 (> i1 fresh_index_0)))
 (=> $x231 (or (= (rd c2 i1) (rd c1 i1)) (= i1 fresh_index_0)))))
(assert
 (let (($x127 (> i2 fresh_index_0)))
 (=> $x127 (or (= (rd c2 i2) (rd c1 i2)) (= i2 fresh_index_0)))))
(assert
 (let (($x193 (> i3 fresh_index_0)))
 (=> $x193 (or (= (rd c2 i3) (rd c1 i3)) (= i3 fresh_index_0)))))
(assert
 (let ((?x239 (rd c1 fresh_index_0)))
 (let ((?x237 (rd c2 fresh_index_0)))
 (let (($x234 (= ?x237 ?x239)))
 (=> (> fresh_index_0 fresh_index_0) (or $x234 (= fresh_index_0 fresh_index_0)))))))
(assert
 (=> (> fresh_index_0 fresh_index_0) (or (= (rd c2 fresh_index_0) (rd c1 fresh_index_0)))))
(assert
 (>= fresh_index_0 fresh_index_1))
(assert
 (let (($x472 (> fresh_index_0 fresh_index_1)))
 (=> $x472 (and (distinct (rd c2 fresh_index_0) (rd c1 fresh_index_0)) true))))
(assert
 (let (($x236 (= fresh_index_0 0)))
 (let (($x470 (= fresh_index_0 fresh_index_1)))
 (=> $x470 $x236))))
(assert
 (let (($x468 (= fresh_index_1 0)))
 (let ((?x444 (rd c1 fresh_index_1)))
 (let ((?x445 (rd c2 fresh_index_1)))
 (let (($x446 (= ?x445 ?x444)))
 (=> $x446 $x468))))))
(assert
 (let (($x448 (= i1 fresh_index_1)))
 (let (($x229 (= i1 fresh_index_0)))
 (let ((?x82 (rd c1 i1)))
 (let ((?x71 (rd c2 i1)))
 (let (($x228 (= ?x71 ?x82)))
 (let (($x452 (> i1 fresh_index_1)))
 (=> $x452 (or $x228 $x229 $x448)))))))))
(assert
 (let (($x408 (= i2 fresh_index_1)))
 (let (($x180 (= i2 fresh_index_0)))
 (let ((?x26 (rd c1 i2)))
 (let ((?x27 (rd c2 i2)))
 (let (($x179 (= ?x27 ?x26)))
 (let (($x410 (> i2 fresh_index_1)))
 (=> $x410 (or $x179 $x180 $x408)))))))))
(assert
 (let (($x454 (= i3 fresh_index_1)))
 (let (($x191 (= i3 fresh_index_0)))
 (let ((?x85 (rd c1 i3)))
 (let ((?x75 (rd c2 i3)))
 (let (($x233 (= ?x75 ?x85)))
 (let (($x456 (> i3 fresh_index_1)))
 (=> $x456 (or $x233 $x191 $x454)))))))))
(assert
 (let (($x470 (= fresh_index_0 fresh_index_1)))
 (let ((?x239 (rd c1 fresh_index_0)))
 (let ((?x237 (rd c2 fresh_index_0)))
 (let (($x234 (= ?x237 ?x239)))
 (let (($x472 (> fresh_index_0 fresh_index_1)))
 (=> $x472 (or $x234 (= fresh_index_0 fresh_index_0) $x470))))))))
(assert
 (let (($x460 (= fresh_index_1 fresh_index_0)))
 (let ((?x444 (rd c1 fresh_index_1)))
 (let ((?x445 (rd c2 fresh_index_1)))
 (let (($x446 (= ?x445 ?x444)))
 (=> (> fresh_index_1 fresh_index_1) (or $x446 $x460 (= fresh_index_1 fresh_index_1))))))))
(assert
 (=> (> fresh_index_1 fresh_index_0) (or (= (rd c2 fresh_index_1) (rd c1 fresh_index_1)))))
(assert
 (let (($x460 (= fresh_index_1 fresh_index_0)))
 (let ((?x444 (rd c1 fresh_index_1)))
 (let ((?x445 (rd c2 fresh_index_1)))
 (let (($x446 (= ?x445 ?x444)))
 (=> (> fresh_index_1 fresh_index_1) (or $x446 $x460)))))))
(assert
 (>= i1 0))
(assert
 (>= i3 0))
(assert
 (>= fresh_index_0 0))
(assert
 (>= fresh_index_1 0))
(assert
 (>= i1 0))
(assert
 (>= i2 0))
(assert
 (>= i3 0))
(assert
 (>= fresh_index_0 0))
(assert
 (>= fresh_index_1 0))
(check-sat)
