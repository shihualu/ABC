(set-logic QF_S)

(declare-fun a () String)
(declare-fun b () String)


(assert (ends (concat a "vlab") b))

(check-sat)

