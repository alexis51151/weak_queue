# MS_queue_strong_nofree

Variant of the MS Queue with strong memory model and using **single-word CAS**.\
This version does not use reference counter, because CDSChecker atomic header does not support it.
Instead, we simply **do not** free memory.
This version is solely use to prove correctness of the algorithm, and will not be used in practice.
