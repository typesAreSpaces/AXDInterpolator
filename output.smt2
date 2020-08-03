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
 (let ((?x50 (rd a i3)))
 (= ?x50 e3)))
(assert
 (=> (and (distinct i1 i3) true) (= (rd a i1) (rd a1 i1))))
(assert
 (=> (and (distinct i3 i3) true) (= (rd a i3) (rd a1 i3))))
(assert
 (let ((?x52 (rd a1 i1)))
 (= ?x52 e1)))
(assert
 (=> (and (distinct i1 i1) true) (= (rd a1 i1) (rd b i1))))
(assert
 (let ((?x66 (rd b i3)))
 (let ((?x57 (rd a1 i3)))
 (let (($x67 (= ?x57 ?x66)))
 (=> (and (distinct i3 i1) true) $x67)))))
(assert
 (let ((?x62 (rd b i1)))
 (let ((?x70 (rd c2 i1)))
 (let (($x71 (= ?x70 ?x62)))
 (=> (> i1 i1) $x71)))))
(assert
 (let ((?x66 (rd b i3)))
 (let ((?x74 (rd c2 i3)))
 (let (($x75 (= ?x74 ?x66)))
 (=> (> i3 i1) $x75)))))
(assert
 (let (($x79 (= i1 0)))
 (let ((?x62 (rd b i1)))
 (let ((?x70 (rd c2 i1)))
 (let (($x71 (= ?x70 ?x62)))
 (=> $x71 $x79))))))
(assert
 (let ((?x53 (rd a i1)))
 (let ((?x81 (rd c1 i1)))
 (let (($x82 (= ?x81 ?x53)))
 (=> (> i1 i1) $x82)))))
(assert
 (=> (> i3 i1) (= (rd c1 i3) (rd a i3))))
(assert
 (let (($x79 (= i1 0)))
 (let ((?x53 (rd a i1)))
 (let ((?x81 (rd c1 i1)))
 (let (($x82 (= ?x81 ?x53)))
 (=> $x82 $x79))))))
(assert
 (let (($x127 (= fresh_index_0 0)))
 (let ((?x126 (rd c1 fresh_index_0)))
 (let ((?x124 (rd c2 fresh_index_0)))
 (let (($x125 (= ?x124 ?x126)))
 (=> $x125 $x127))))))
(assert
 (=> (> i1 fresh_index_0) (or (= (rd c2 i1) (rd c1 i1)) (= i1 fresh_index_0))))
(assert
 (=> (> i3 fresh_index_0) (or (= (rd c2 i3) (rd c1 i3)) (= i3 fresh_index_0))))
(assert
 (let ((?x126 (rd c1 fresh_index_0)))
 (let ((?x124 (rd c2 fresh_index_0)))
 (let (($x125 (= ?x124 ?x126)))
 (=> (> fresh_index_0 fresh_index_0) (or $x125 (= fresh_index_0 fresh_index_0)))))))
(assert
 (let ((?x88 (rd a1 fresh_index_0)))
 (let ((?x110 (rd a fresh_index_0)))
 (let (($x101 (= ?x110 ?x88)))
 (=> (and (distinct fresh_index_0 i3) true) $x101)))))
(assert
 (let ((?x137 (rd b fresh_index_0)))
 (let ((?x88 (rd a1 fresh_index_0)))
 (let (($x138 (= ?x88 ?x137)))
 (=> (and (distinct fresh_index_0 i1) true) $x138)))))
(assert
 (=> (> fresh_index_0 i1) (or (= (rd c2 fresh_index_0) (rd b fresh_index_0)))))
(assert
 (=> (> fresh_index_0 fresh_index_0) (or (= (rd c2 fresh_index_0) (rd c1 fresh_index_0)))))
(assert
 (=> (> fresh_index_0 i1) (or (= (rd c1 fresh_index_0) (rd a fresh_index_0)))))
(assert
 (>= fresh_index_0 fresh_index_1))
(assert
 (=> (> fresh_index_0 fresh_index_1) (and (distinct (rd c2 fresh_index_0) (rd c1 fresh_index_0)) true)))
(assert
 (let (($x127 (= fresh_index_0 0)))
 (let (($x220 (= fresh_index_0 fresh_index_1)))
 (=> $x220 $x127))))
(assert
 (let (($x215 (= fresh_index_1 0)))
 (let ((?x216 (rd c1 fresh_index_1)))
 (let ((?x212 (rd c2 fresh_index_1)))
 (let (($x213 (= ?x212 ?x216)))
 (=> $x213 $x215))))))
(assert
 (let (($x209 (= i1 fresh_index_1)))
 (let (($x123 (= i1 fresh_index_0)))
 (let ((?x81 (rd c1 i1)))
 (let ((?x70 (rd c2 i1)))
 (let (($x122 (= ?x70 ?x81)))
 (=> (> i1 fresh_index_1) (or $x122 $x123 $x209))))))))
(assert
 (let (($x203 (= i3 fresh_index_1)))
 (let (($x112 (= i3 fresh_index_0)))
 (let ((?x84 (rd c1 i3)))
 (let ((?x74 (rd c2 i3)))
 (let (($x111 (= ?x74 ?x84)))
 (=> (> i3 fresh_index_1) (or $x111 $x112 $x203))))))))
(assert
 (let (($x220 (= fresh_index_0 fresh_index_1)))
 (let ((?x126 (rd c1 fresh_index_0)))
 (let ((?x124 (rd c2 fresh_index_0)))
 (let (($x125 (= ?x124 ?x126)))
 (=> (> fresh_index_0 fresh_index_1) (or $x125 (= fresh_index_0 fresh_index_0) $x220)))))))
(assert
 (let ((?x216 (rd c1 fresh_index_1)))
 (let ((?x212 (rd c2 fresh_index_1)))
 (let (($x213 (= ?x212 ?x216)))
 (let (($x182 (or $x213 (= fresh_index_1 fresh_index_0) (= fresh_index_1 fresh_index_1))))
 (=> (> fresh_index_1 fresh_index_1) $x182))))))
(assert
 (let ((?x177 (rd a1 fresh_index_1)))
 (let ((?x174 (rd a fresh_index_1)))
 (let (($x175 (= ?x174 ?x177)))
 (=> (and (distinct fresh_index_1 i3) true) $x175)))))
(assert
 (let ((?x171 (rd b fresh_index_1)))
 (let ((?x177 (rd a1 fresh_index_1)))
 (let (($x172 (= ?x177 ?x171)))
 (=> (and (distinct fresh_index_1 i1) true) $x172)))))
(assert
 (=> (> fresh_index_1 i1) (or (= (rd c2 fresh_index_1) (rd b fresh_index_1)))))
(assert
 (=> (> fresh_index_1 fresh_index_0) (or (= (rd c2 fresh_index_1) (rd c1 fresh_index_1)))))
(assert
 (let ((?x216 (rd c1 fresh_index_1)))
 (let ((?x212 (rd c2 fresh_index_1)))
 (let (($x213 (= ?x212 ?x216)))
 (=> (> fresh_index_1 fresh_index_1) (or $x213 (= fresh_index_1 fresh_index_0)))))))
(assert
 (=> (> fresh_index_1 i1) (or (= (rd c1 fresh_index_1) (rd a fresh_index_1)))))
(assert
 (< i1 i2))
(assert
 (< i2 i3))
(assert
 (let ((?x27 (rd c2 i2)))
 (let ((?x26 (rd c1 i2)))
 (and (distinct ?x26 ?x27) true))))
(assert
 (let (($x127 (= fresh_index_0 0)))
 (let ((?x126 (rd c1 fresh_index_0)))
 (let ((?x124 (rd c2 fresh_index_0)))
 (let (($x125 (= ?x124 ?x126)))
 (=> $x125 $x127))))))
(assert
 (=> (> i1 fresh_index_0) (or (= (rd c2 i1) (rd c1 i1)) (= i1 fresh_index_0))))
(assert
 (=> (> i2 fresh_index_0) (or (= (rd c2 i2) (rd c1 i2)) (= i2 fresh_index_0))))
(assert
 (=> (> i3 fresh_index_0) (or (= (rd c2 i3) (rd c1 i3)) (= i3 fresh_index_0))))
(assert
 (let ((?x126 (rd c1 fresh_index_0)))
 (let ((?x124 (rd c2 fresh_index_0)))
 (let (($x125 (= ?x124 ?x126)))
 (=> (> fresh_index_0 fresh_index_0) (or $x125 (= fresh_index_0 fresh_index_0)))))))
(assert
 (=> (> fresh_index_0 fresh_index_0) (or (= (rd c2 fresh_index_0) (rd c1 fresh_index_0)))))
(assert
 (>= fresh_index_0 fresh_index_1))
(assert
 (=> (> fresh_index_0 fresh_index_1) (and (distinct (rd c2 fresh_index_0) (rd c1 fresh_index_0)) true)))
(assert
 (let (($x127 (= fresh_index_0 0)))
 (let (($x220 (= fresh_index_0 fresh_index_1)))
 (=> $x220 $x127))))
(assert
 (let (($x215 (= fresh_index_1 0)))
 (let ((?x216 (rd c1 fresh_index_1)))
 (let ((?x212 (rd c2 fresh_index_1)))
 (let (($x213 (= ?x212 ?x216)))
 (=> $x213 $x215))))))
(assert
 (let (($x209 (= i1 fresh_index_1)))
 (let (($x123 (= i1 fresh_index_0)))
 (let ((?x81 (rd c1 i1)))
 (let ((?x70 (rd c2 i1)))
 (let (($x122 (= ?x70 ?x81)))
 (=> (> i1 fresh_index_1) (or $x122 $x123 $x209))))))))
(assert
 (let (($x218 (= i2 fresh_index_1)))
 (let (($x151 (= i2 fresh_index_0)))
 (let ((?x26 (rd c1 i2)))
 (let ((?x27 (rd c2 i2)))
 (let (($x150 (= ?x27 ?x26)))
 (=> (> i2 fresh_index_1) (or $x150 $x151 $x218))))))))
(assert
 (let (($x203 (= i3 fresh_index_1)))
 (let (($x112 (= i3 fresh_index_0)))
 (let ((?x84 (rd c1 i3)))
 (let ((?x74 (rd c2 i3)))
 (let (($x111 (= ?x74 ?x84)))
 (=> (> i3 fresh_index_1) (or $x111 $x112 $x203))))))))
(assert
 (let (($x220 (= fresh_index_0 fresh_index_1)))
 (let ((?x126 (rd c1 fresh_index_0)))
 (let ((?x124 (rd c2 fresh_index_0)))
 (let (($x125 (= ?x124 ?x126)))
 (=> (> fresh_index_0 fresh_index_1) (or $x125 (= fresh_index_0 fresh_index_0) $x220)))))))
(assert
 (let ((?x216 (rd c1 fresh_index_1)))
 (let ((?x212 (rd c2 fresh_index_1)))
 (let (($x213 (= ?x212 ?x216)))
 (let (($x182 (or $x213 (= fresh_index_1 fresh_index_0) (= fresh_index_1 fresh_index_1))))
 (=> (> fresh_index_1 fresh_index_1) $x182))))))
(assert
 (=> (> fresh_index_1 fresh_index_0) (or (= (rd c2 fresh_index_1) (rd c1 fresh_index_1)))))
(assert
 (let ((?x216 (rd c1 fresh_index_1)))
 (let ((?x212 (rd c2 fresh_index_1)))
 (let (($x213 (= ?x212 ?x216)))
 (=> (> fresh_index_1 fresh_index_1) (or $x213 (= fresh_index_1 fresh_index_0)))))))
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
