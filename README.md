# comparison of hash functions

compared functions: [MurMurHash](https://github.com/aappleby/smhasher) vs [xxHash](https://github.com/Cyan4973/xxHash). Reference point for comparing: `memcpy`


## 1-100 (step = 1)

<table>
  <tr>
    <td align="center">Mishok</td>
    <td align="center">Ilbro</td>
  </tr>
  <tr>
    <td><img src="artifacts/mishok/images/res3_small_plot.png" width="420" /></td>
    <td><img src="artifacts/mishok/images/res2_small_plot.png" width="420" /></td> 
  </tr>
</table>


## 100-1000 (step = 50)

<table>
  <tr>
    <td align="center">Mishok</td>
    <td align="center">Ilbro</td>
  </tr>
  <tr>
    <td><img src="artifacts/mishok/images/res3_big_plot.png" width="420" /></td>
    <td><img src="artifacts/mishok/images/res2_big_plot.png" width="420" /></td> 
  </tr>
</table>
