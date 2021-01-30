I think that what you can do is the following. Take the code of any
benchmark program, translate it into a transition formula in our
language. Then invent some obvious bad state formula for which the
system is safe. Then unroll the transition (say 5 times or something
like that) and conjoin it with the unsafe formula. This must be
inconsistent and you can get interpolants at any step.
