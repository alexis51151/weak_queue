# MS_queue_strong

Variant of the MS Queue with strong memory model and using **double-word CAS**.\
ABA is solved by adding a reference counter to node pointers.