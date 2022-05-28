# KVik, key value store

Memory efficient, fast and yet simple key value store
that is able to contain huge amount of records

## Notes

This storage supports values of fixed length by default, possible approaches to introduce variable length:

- Use fixed size that is sufficient for all your values (can be really inefficient for situations when most of your
  values are small)
- Build modification on top of this implementation to support it (update operations will be a bit more complicated,
  maybe some other insignificant changes)

By default, expected `key` size is `128 bit` and expected `value` size is `2 KB` .

## Architecture

By <span style="color:indianred">red</span> color we will denote everything that is stored in `DRAM`
, <span style="color:darkturquoise">blue</span> will stand for `SSD` storage, <span style="color:mediumpurple">
purple</span> means that component is stored in `HDD`.

Thus, general architecture is as follows

<p align="center">
  <img src="resources/images/architecture.svg" alt="storage architecture" width="90%">
</p>

Now let's see what each element of diagram is responsible for

### Shard controller

In order to distribute requests on independent workers for each key that comes as a part of request (`get`, `insert`
, `update`, `delete`) we calculate its hash (`XXH32`)
and choose bucket depending on range where this hash appeared to be. By doing this, we decrease sizes of
files on each shard linearly (with respect to the number of shards in assumption that hash values will be distributed
uniformly).
Apart from that, this idea helps to increase throughput of the storage (requests can be handled in parallel on different
threads).

### Filter

We try to filter out requests with keys that aren't in the store yet for `delete`, `update` and `get` requests.

There are several ways to do so but in our case it was decided to use Bloom filter since it only requires `O(n)` bits
of memory and is pretty fast.

Our filter was designed to have small error probability (less than `9%`), to achieve that we
use `4` [4-independent](https://en.wikipedia.org/wiki/K-independent_hashing) hash functions and `5n` bits in filter.

### Log Hash Table

When new request is received it is firstly added to a `Log Hash Table`. `Log Hash Table` is an open addressing hash
table that
uses [Robin Hood hashing](https://programming.guide/robin-hood-hashing.html). This technique helps `Log Hash Table` to
be more memory efficient and maintain fill factor around `90%` (for detailed explanation check
out [original paper](https://cs.uwaterloo.ca/research/tr/1986/CS-86-14.pdf)).

`Log Hash Table` has a fixed size, so when it is filled it is merged into `Index Hash Table` as well as
corresponding `Log File` is merged into `Hot Files`

`Log Hash Table` stores key + offset of corresponding value in `Log File`.

### Log File

`Log File` stores `(key, value)` pairs corresponding to keys that are stored in `Log Hash Table`, it is pretty simple
really. Keys are stored to increase fall tolerance because in other case when electricity wil be cut off values will
remain (since they are stored on `SSD` which in energy independent) whereas keys that are stored in `Log Hash Table`
will be lost, we want to avoid situations like that.

### Index Hash Table

`Index Hash Table` is responsible for all keys that are not in the `Log Hash Table`. Its function is very simple, it
maintains set of keys and maps them to two offsets, offset in `Hot Files` and offset in `Data File` (how exactly they
are stored is implementation detail really).

This hash table has the following property: each bucket is just one memory page (`4 KB`). When page is filled hash table
is rebuilt. To avoid problems with bucket overflow during rehashing, if it happens one more memory page will be
allocated for this bucket and will be linked to existing pages in this bucket (like in hash table with closed
addressing). But after add operation to this bucket it will be detected and rehashed again (we just hope that after
rehashing it all be distributed more uniformly in expanded hash table).

Experiments showed the following (number of entries in hash table was up to `2 * 10^8`, 128-bit words were generated
by `rand` and `std::mt19937`, hashing that was used is `XXH32` with random seed):

- If one memory page is being used fill factor is more than 60% (experiments showed numbers between 62 and 70 depending
  on number of entries and varied between different attempts)
- If ten memory page is being used fill factor is more than 85% (experiments showed numbers between 85 and 90 depending
  on number of entries and varied between different attempts) but you should be careful with that because if number of
  pages gets bigger so does `Read Amplification Factor` and `Write Amplification Factor` (But
  yes, `Space Amplification Factor` gets smaller)

### Hot Files

Conceptually, it is cache for values of keys that are used most often. This strategy will most likely be implemented
in the future but for now `Hot Files` will be some kind of intermediate value storage that is a lot faster than storage
on `HDD` (although [this document](https://gist.github.com/jboner/2841832) is 2012 version, difference between `HDD` and
`SSD` access latency is still huge).

`Hot Files` should also store keys in addition to values. It is done for more efficient merge operation to `Data File`
(update entry in `Index Hash Table` and check if some information corresponding to this key is already in `Data File`
so we can just replace this value with a new one)

### Data File

`Data File` is one huge file that is stored on `HDD`. It also maintains linked list of deleted entries so that it is easy
to put new values on place of deleted ones.

`Data File` doesn't store any additional data, only values.

## API

This storage supports basic operations that any associative container supports: `get`, `insert`, `update`, `delete`.
It is really unlikely that it can be used as a database for your application. But, of course, since it doesn't introduce
additional complexity for supporting custom user requests written in somewhat like SQL, it should be faster.

So, if you actually don't need anything more complex than these operations, **KVik** is a viable option for you.

For detailed explanation on how these operations work in **KVik** check [this file](./API_DESCRIPTION.md).

## Metrics

- `Read Amplification Factor =~ 2`
- `Write Amplification Factor =~ 5` (worst case if none of the heuristics are working)
- `Space Amplification Factor =~ 1.23`
- `Memory Overhead =~ 0.865 bytes/key`
- `Disk Lifetime = ?`

As you can see, this storage will be good solution for you in case read requests are more often than modification
requests like `update` and `insert` (which is realistic scenario for some systems).

For detailed explanation about metrics read [this pdf file](./metrics.pdf).
