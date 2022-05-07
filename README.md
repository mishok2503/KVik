# Key value store

## Notes

This storage doesn't support values with variable length by default, so possible approaches are

 - Use fixed size that is sufficient for all your values (can be really inefficient for situations when most of your values are small)
 - Build modification on top of this implementation to support it (update operations will be a bit more complicated, maybe some other insignificant changes)

By default, expected `key` size is `128` bits and expected `value` size is `2` kilobytes.

## Architecture

By red color we will denote everything that is stored in `DRAM`, blue will stand for `SSD` storage, purple means that component is stored in `HDD`.

Thus, general architecture is as follows

<p align="center">
  <img src="resources/images/architecture.png" alt="storage architecture">
</p>

Now let's see what each element of diagram is responsible for

### Shard controller

In order to distribute requests on independent workers for each key that comes as a part of request (get, insert, update, delete) we use hashing of keys (`XXH32`)
 and choose bucket depending on range where this hash appeared to be (for example, if we have 10 shards, then each of those will theoretically be responsible for approximately `429496729` keys)













# Previous version of README


**Архитектура**

Главные компоненты: `Filter (Bloom's), Log HashTable (open addressing, Robin Hood hashing, on DRAM) + Log file(SSD)`, `Global HashTable (closed addressing, bucket is memory page, on SSD)`,
`HoT file (hot values and keys)`, `Data file (all values)`.

Как примерно всё устроено: при добавлении мы так или иначе записываем значение на Log file (если там хранилище переполняется, то мы копируем это дело в `Data file`). А что происходит с ключами? Ключи сначала попадают на первый уровень, в `Log HashTable`, там при использовании хеширования Робина Гуда получается добиться высокого коэффицента заполненности при сохранении очень быстрого `lookup`. При обновлении мы можем посмотреть в LOG и очень быстро перезаписать значение по уже указанному offset'у, что тоже весьма удобно. При удалении всё тоже весьма нехитро: если значение есть в LOG, то удаление будет выглядеть крайне просто, достаточно пометить соответствующий элемент в `Log file` как удалённый и удалить элемент из `Log HashTable`. Если же нам не повезло, то надо будет пойти в большую таблицу на `SSD` (`Global HashTable`) и поискать ключ там (это делается весьма просто, поскольку достаточно посчитать у ключа хеш и прочитать выделенную заранее под бакет страницу и за линию сравнить с присутствующими там ключами, что эффективно с точки зрения числа чтений с диска, всего одно). Если ключ нашёлся, то смотрим, какой offset у него записан. В теории там может быть два значения, но при наличии величины offset'а в `Hot file` искать будем именно там, поскольку операция чтения с `SSD` намного быстрее, чем с `HDD` и к тому же на `HDD` может храниться неактуальная версия. C получением нового ключа должно быть несколько проще, поскольку у нас есть фильтр, котороый в случае отсутствующего ключа сразу об этом скажет (вернее, если ключа нет, то с какой-то вероятностью он это сделает), дальше вроде бы процесс достаточно очевидный.

**Подсчёт метрик**

`t = |HoT File| / (|Data file| + |Hot file|)`

`M` -- размер лога (число записей в логе)

`Sk` -- размер ключа в байтах

`Sv` -- размер значения в байтах

`Sb` -- количество байт в одном бакете

`N` -- число всех записей

`f` -- offset

`b` -- размер блока памяти

`m` -- размер хеш-таблицы (количество бакетов)

`E` -- количество доступных циклов перезаписи на диск

`F` -- размер `SSD`

`Uw` -- средняя скорость поступления запросов на запись (б/c)

`z` -- допустимый процент запросов к `SSD` (чтобы наш `Hot file` был действительно "горячим").

`w = Sk + 2 * f` 

Давайте теперь посчитаем, что получается со всеми интересными для нас метриками.

`WA = 1 + 1/M * M * [(Sk + Sv)/b] + 1/(N * t) * N * t * [(Sk + Sv) / b] + w/(m * Sb) * (m * Sb)/w = 2 + 2 * [(Sk + Sv)/b]`

`RA = M/N * 1 + (N - M)/N * (Sb/b + [Sv/b]) + w/(m * Sb) * [m * Sb/b] + 1/(N * t)[N * t * (Sk + Sv) / b]`

`SA = (N * t * (Sv + Sk) + N * Sv + m * Sb)/(N*(Sv + Sk)) = t + Sv/(Sv + Sk) + m * Sb/(N * (Sv + Sk)) <= t + 1 + 2 * b / (Sv + Sk)` (при условии, что мы расширяем хеш-таблицу в 2 раза (именно количество бакетов, то есть `m := 2 * m`))

`MO = M * (Sk + f) / N`

`WA * Uw * T <= F * E` тогда `T <= F * E / (WA * Uw)`, поэтому оценца сверху `SSDLifetime = F * E / (WA * Uw)`


Заметки:  
  Наша основная цель -- перфоманс  
  Поэтому нет сжатия, поэтому не очень хорошая отказоустойчивость  
  Для каждого шарда храним свой фильтр(идеальная парралельность).  
  Для лучшего распределения ключей, будем шардить по их хэшам ключей.  
  Размер данных ~ размер страницы => пишем сначала в удалённые  
  фильтр - блум, так как он по перфомансу быстрее + мало памяти  
  балансировка шардов - если максимум отклонений от среднего слишком большой - перестраиваем вообще все шарды(полная перестройка)  
  В SSD - "горячие" файлы
