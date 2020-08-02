; 
(set-info :status unknown)
(declare-sort ElementSort 0)
 (declare-sort ArraySort 0)
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
 (let ((?x65 (rd a i3)))
 (= ?x65 e3)))
(assert
 (=> (and (distinct i1 i3) true) (= (rd a i1) (rd a1 i1))))
(assert
 (=> (and (distinct i3 i3) true) (= (rd a i3) (rd a1 i3))))
(assert
 (let ((?x67 (rd a1 i1)))
 (= ?x67 e1)))
(assert
 (=> (and (distinct i1 i1) true) (= (rd a1 i1) (rd b i1))))
(assert
 (let ((?x81 (rd b i3)))
 (let ((?x72 (rd a1 i3)))
 (let (($x82 (= ?x72 ?x81)))
 (=> (and (distinct i3 i1) true) $x82)))))
(assert
 (let ((?x77 (rd b i1)))
 (let ((?x85 (rd c2 i1)))
 (let (($x86 (= ?x85 ?x77)))
 (let (($x87 (> i1 i1)))
 (=> $x87 $x86))))))
(assert
 (let ((?x81 (rd b i3)))
 (let ((?x89 (rd c2 i3)))
 (let (($x90 (= ?x89 ?x81)))
 (let (($x91 (> i3 i1)))
 (=> $x91 $x90))))))
(assert
 (let (($x93 (= i1 0)))
 (let ((?x77 (rd b i1)))
 (let ((?x85 (rd c2 i1)))
 (let (($x86 (= ?x85 ?x77)))
 (=> $x86 $x93))))))
(assert
 (let ((?x68 (rd a i1)))
 (let ((?x95 (rd c1 i1)))
 (let (($x96 (= ?x95 ?x68)))
 (let (($x87 (> i1 i1)))
 (=> $x87 $x96))))))
(assert
 (let (($x91 (> i3 i1)))
 (=> $x91 (= (rd c1 i3) (rd a i3)))))
(assert
 (let (($x93 (= i1 0)))
 (let ((?x68 (rd a i1)))
 (let ((?x95 (rd c1 i1)))
 (let (($x96 (= ?x95 ?x68)))
 (=> $x96 $x93))))))
(assert
 (let (($x127 (= fresh_index_0 0)))
 (let ((?x126 (rd c1 fresh_index_0)))
 (let ((?x124 (rd c2 fresh_index_0)))
 (let (($x125 (= ?x124 ?x126)))
 (=> $x125 $x127))))))
(assert
 (let (($x121 (= i1 fresh_index_0)))
 (let ((?x95 (rd c1 i1)))
 (let ((?x85 (rd c2 i1)))
 (let (($x113 (= ?x85 ?x95)))
 (let (($x122 (or $x113 $x121)))
 (=> (> i1 fresh_index_0) $x122)))))))
(assert
 (=> (> i3 fresh_index_0) (or (= (rd c2 i3) (rd c1 i3)) (= i3 fresh_index_0))))
(assert
 (let (($x107 (= fresh_index_0 fresh_index_0)))
 (let ((?x126 (rd c1 fresh_index_0)))
 (let ((?x124 (rd c2 fresh_index_0)))
 (let (($x125 (= ?x124 ?x126)))
 (let (($x142 (> fresh_index_0 fresh_index_0)))
 (=> $x142 (or $x125 $x107))))))))
(assert
 (let ((?x144 (rd a1 fresh_index_0)))
 (let ((?x145 (rd a fresh_index_0)))
 (let (($x146 (= ?x145 ?x144)))
 (=> (and (distinct fresh_index_0 i3) true) $x146)))))
(assert
 (let ((?x149 (rd b fresh_index_0)))
 (let ((?x144 (rd a1 fresh_index_0)))
 (let (($x150 (= ?x144 ?x149)))
 (=> (and (distinct fresh_index_0 i1) true) $x150)))))
(assert
 (let (($x155 (> fresh_index_0 i1)))
 (=> $x155 (or (= (rd c2 fresh_index_0) (rd b fresh_index_0))))))
(assert
 (let (($x142 (> fresh_index_0 fresh_index_0)))
 (=> $x142 (or (= (rd c2 fresh_index_0) (rd c1 fresh_index_0))))))
(assert
 (let (($x155 (> fresh_index_0 i1)))
 (=> $x155 (or (= (rd c1 fresh_index_0) (rd a fresh_index_0))))))
(assert
 (>= fresh_index_0 fresh_index_1))
(assert
 (let (($x223 (> fresh_index_0 fresh_index_1)))
 (=> $x223 (and (distinct (rd c2 fresh_index_0) (rd c1 fresh_index_0)) true))))
(assert
 (let (($x127 (= fresh_index_0 0)))
 (let (($x220 (= fresh_index_0 fresh_index_1)))
 (=> $x220 $x127))))
(assert
 (let (($x216 (= fresh_index_1 0)))
 (let ((?x215 (rd c1 fresh_index_1)))
 (let ((?x208 (rd c2 fresh_index_1)))
 (let (($x210 (= ?x208 ?x215)))
 (=> $x210 $x216))))))
(assert
 (let (($x207 (= i1 fresh_index_1)))
 (let (($x121 (= i1 fresh_index_0)))
 (let ((?x95 (rd c1 i1)))
 (let ((?x85 (rd c2 i1)))
 (let (($x113 (= ?x85 ?x95)))
 (=> (> i1 fresh_index_1) (or $x113 $x121 $x207))))))))
(assert
 (let (($x194 (= i3 fresh_index_1)))
 (let (($x102 (= i3 fresh_index_0)))
 (let ((?x98 (rd c1 i3)))
 (let ((?x89 (rd c2 i3)))
 (let (($x111 (= ?x89 ?x98)))
 (=> (> i3 fresh_index_1) (or $x111 $x102 $x194))))))))
(assert
 (let (($x220 (= fresh_index_0 fresh_index_1)))
 (let (($x107 (= fresh_index_0 fresh_index_0)))
 (let ((?x126 (rd c1 fresh_index_0)))
 (let ((?x124 (rd c2 fresh_index_0)))
 (let (($x125 (= ?x124 ?x126)))
 (let (($x223 (> fresh_index_0 fresh_index_1)))
 (=> $x223 (or $x125 $x107 $x220)))))))))
(assert
 (let (($x170 (= fresh_index_1 fresh_index_0)))
 (let ((?x215 (rd c1 fresh_index_1)))
 (let ((?x208 (rd c2 fresh_index_1)))
 (let (($x210 (= ?x208 ?x215)))
 (let (($x184 (> fresh_index_1 fresh_index_1)))
 (=> $x184 (or $x210 $x170 (= fresh_index_1 fresh_index_1)))))))))
(assert
 (let ((?x174 (rd a1 fresh_index_1)))
 (let ((?x176 (rd a fresh_index_1)))
 (let (($x167 (= ?x176 ?x174)))
 (=> (and (distinct fresh_index_1 i3) true) $x167)))))
(assert
 (let ((?x221 (rd b fresh_index_1)))
 (let ((?x174 (rd a1 fresh_index_1)))
 (let (($x218 (= ?x174 ?x221)))
 (=> (and (distinct fresh_index_1 i1) true) $x218)))))
(assert
 (let (($x209 (> fresh_index_1 i1)))
 (=> $x209 (or (= (rd c2 fresh_index_1) (rd b fresh_index_1))))))
(assert
 (=> (> fresh_index_1 fresh_index_0) (or (= (rd c2 fresh_index_1) (rd c1 fresh_index_1)))))
(assert
 (let (($x170 (= fresh_index_1 fresh_index_0)))
 (let ((?x215 (rd c1 fresh_index_1)))
 (let ((?x208 (rd c2 fresh_index_1)))
 (let (($x210 (= ?x208 ?x215)))
 (let (($x184 (> fresh_index_1 fresh_index_1)))
 (=> $x184 (or $x210 $x170))))))))
(assert
 (let (($x209 (> fresh_index_1 i1)))
 (=> $x209 (or (= (rd c1 fresh_index_1) (rd a fresh_index_1))))))
(assert
 (< i1 i2))
(assert
 (< i2 i3))
(assert
 (let ((?x45 (rd c2 i2)))
 (let ((?x44 (rd c1 i2)))
 (and (distinct ?x44 ?x45) true))))
(assert
 (let (($x127 (= fresh_index_0 0)))
 (let ((?x126 (rd c1 fresh_index_0)))
 (let ((?x124 (rd c2 fresh_index_0)))
 (let (($x125 (= ?x124 ?x126)))
 (=> $x125 $x127))))))
(assert
 (let (($x121 (= i1 fresh_index_0)))
 (let ((?x95 (rd c1 i1)))
 (let ((?x85 (rd c2 i1)))
 (let (($x113 (= ?x85 ?x95)))
 (let (($x122 (or $x113 $x121)))
 (=> (> i1 fresh_index_0) $x122)))))))
(assert
 (let (($x163 (= i2 fresh_index_0)))
 (let ((?x44 (rd c1 i2)))
 (let ((?x45 (rd c2 i2)))
 (let (($x162 (= ?x45 ?x44)))
 (let (($x164 (or $x162 $x163)))
 (=> (> i2 fresh_index_0) $x164)))))))
(assert
 (=> (> i3 fresh_index_0) (or (= (rd c2 i3) (rd c1 i3)) (= i3 fresh_index_0))))
(assert
 (let (($x107 (= fresh_index_0 fresh_index_0)))
 (let ((?x126 (rd c1 fresh_index_0)))
 (let ((?x124 (rd c2 fresh_index_0)))
 (let (($x125 (= ?x124 ?x126)))
 (let (($x142 (> fresh_index_0 fresh_index_0)))
 (=> $x142 (or $x125 $x107))))))))
(assert
 (let (($x142 (> fresh_index_0 fresh_index_0)))
 (=> $x142 (or (= (rd c2 fresh_index_0) (rd c1 fresh_index_0))))))
(assert
 (>= fresh_index_0 fresh_index_1))
(assert
 (let (($x223 (> fresh_index_0 fresh_index_1)))
 (=> $x223 (and (distinct (rd c2 fresh_index_0) (rd c1 fresh_index_0)) true))))
(assert
 (let (($x127 (= fresh_index_0 0)))
 (let (($x220 (= fresh_index_0 fresh_index_1)))
 (=> $x220 $x127))))
(assert
 (let (($x216 (= fresh_index_1 0)))
 (let ((?x215 (rd c1 fresh_index_1)))
 (let ((?x208 (rd c2 fresh_index_1)))
 (let (($x210 (= ?x208 ?x215)))
 (=> $x210 $x216))))))
(assert
 (let (($x207 (= i1 fresh_index_1)))
 (let (($x121 (= i1 fresh_index_0)))
 (let ((?x95 (rd c1 i1)))
 (let ((?x85 (rd c2 i1)))
 (let (($x113 (= ?x85 ?x95)))
 (=> (> i1 fresh_index_1) (or $x113 $x121 $x207))))))))
(assert
 (let (($x173 (= i2 fresh_index_1)))
 (let (($x163 (= i2 fresh_index_0)))
 (let ((?x44 (rd c1 i2)))
 (let ((?x45 (rd c2 i2)))
 (let (($x162 (= ?x45 ?x44)))
 (=> (> i2 fresh_index_1) (or $x162 $x163 $x173))))))))
(assert
 (let (($x194 (= i3 fresh_index_1)))
 (let (($x102 (= i3 fresh_index_0)))
 (let ((?x98 (rd c1 i3)))
 (let ((?x89 (rd c2 i3)))
 (let (($x111 (= ?x89 ?x98)))
 (=> (> i3 fresh_index_1) (or $x111 $x102 $x194))))))))
(assert
 (let (($x220 (= fresh_index_0 fresh_index_1)))
 (let (($x107 (= fresh_index_0 fresh_index_0)))
 (let ((?x126 (rd c1 fresh_index_0)))
 (let ((?x124 (rd c2 fresh_index_0)))
 (let (($x125 (= ?x124 ?x126)))
 (let (($x223 (> fresh_index_0 fresh_index_1)))
 (=> $x223 (or $x125 $x107 $x220)))))))))
(assert
 (let (($x170 (= fresh_index_1 fresh_index_0)))
 (let ((?x215 (rd c1 fresh_index_1)))
 (let ((?x208 (rd c2 fresh_index_1)))
 (let (($x210 (= ?x208 ?x215)))
 (let (($x184 (> fresh_index_1 fresh_index_1)))
 (=> $x184 (or $x210 $x170 (= fresh_index_1 fresh_index_1)))))))))
(assert
 (=> (> fresh_index_1 fresh_index_0) (or (= (rd c2 fresh_index_1) (rd c1 fresh_index_1)))))
(assert
 (let (($x170 (= fresh_index_1 fresh_index_0)))
 (let ((?x215 (rd c1 fresh_index_1)))
 (let ((?x208 (rd c2 fresh_index_1)))
 (let (($x210 (= ?x208 ?x215)))
 (let (($x184 (> fresh_index_1 fresh_index_1)))
 (=> $x184 (or $x210 $x170))))))))
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
