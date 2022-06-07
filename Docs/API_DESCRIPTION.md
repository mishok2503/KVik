### Get

Here is how `get` operation works in **KVik**:

1. filter is applied, if it says that value isn't in storage we return `nullptr` immediately (with probability
   around `91%` for values that are not in storage it will work properly and say so)
2. request is passed to `Log Hash Table` and it is checked if such key is present there. If so, read corresponding value
   from `Log File` and return to user.
3. request is passed to `Index Hash Table` and it is checked if such key is present in there. If so, if it has offset
   in `Hot Files` present, read value from there, otherwise read it from `Data File`.

### Insert

Here is how `insert` operation works in **KVik**: just insert it into `Log Hash Table` and set bits corresponding to
this key in filter to ones.

Interesting part is how merge operation works because obviously, since `Log Hash Table` is in `DRAM` it will be filled
pretty quick. Well, it is also pretty easy actually, keys are sorted in ascending order of hash function that is used
in `Index Hash Table`. After that values are inserted as they would be in any ordinary hash mao. The idea of this
sorting is to have some keys hit same buckets and therefore using fewer operations with disk.

Another interesting merge is `Hot Files` with `Data File`. Here merge is done very straightforward way: just traverse
all records in `Hot Files` and insert them into `Data File` one by one.

### Update

`update` operation is very similar to `insert`. The only important difference is that before actually updating record we
apply filter.
If filter says that record is definitely not in storage we ignore it. If `update` operation is called you must be sure
that record
is already in the storage (however, this behavior can be modified in the future, be careful with updating library to
newer version,
read patch notes carefully beforehand).

### Delete

`delete` operation is logically pretty similar to `get`, it has same steps in it (filter first, log second, index third)
.
The only interesting detail is how we actually delete this key from all of these structures in case it was found.

#### Delete from `Log Hash Table`

Deletion of key from hash table with open addressing is as simple as always. But apart from that, we should also delete
corresponding value from `Log File`. It will be really simple though, we will do it lazily, we won't do anything until
it overflows (maybe just write some reserved value in corresponding place in `Log File` so that it can be distinguished
from actual value we want to store). When it does, we will rebuild it from scratch as well as `Log Hash Table`
(to take new offset into account).

#### Delete from `Index Hash Table`

Deletion of key from index is also going to be lazy. But in this case it won't be a problem for bucket overflow,
since we scan whole bucket during insertion anyway, let's just store `-1` in both offsets, it will mean that this key
is actually deleted. So, during next insertion we will scan the entire bucket and if -1 was found we will replace
it with a new value. If we want to delete corresponding value there are several cases (corresponding value is in
`Hot Files` and in `Data File` or in one of those). Deletion from `Data File` will be done exactly the same way as from
`Log File`. The issue is how to delete from `Data File`. It will be done using lined list. Linked list will store free
spots for values in `Data File`. It won't maintain correct topology of values in `Data File` but before using it,
list can be sorted to have less random disk accesses.