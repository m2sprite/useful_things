![img place](/imgs/repo_header_img.png)
#### repo of random useuful things that I easily forget
___
```
tar -xvzf community_images.tar.gz
```

```



PERFORMANCE CHECKLIST:
 REMOVE WASTE [ ]
    Don't use gay garbage collected languages

 GET ILP [ ]
    a) UNROLL LOOP <- this increases this ratio workdoneinloop/loopmaintainance
    b) BREAK DEPENDENCY CHAIN <- this allows for the cpu to do ILP
       Dependencies to clean:
         * chained calculations
         * pending loads and stores

         LOADS AND STORES INFO (sizes vary depending on chip arch):
             CPU CORE INFO:
             if ( MemAddressToLoadOrStore is in RegisterFile ) {
                aweosome ! fastest load or store ever
             } else if (MemAddressToLoadOrStore is in L1 ) {
                still pretty awesome happens in 3 or 4 cycles
             } else if (MemAddressToLoadOrStore is in L2 ) {
                less awesome than L1 happens in 14 cycles or more
             } else if (MemAddressToLoadOrStore is in L3) {
                even less awesome than L2 happens in 80 cycles or more
             }


 COMBINE THE ABOVE WITH SIMD [ ]

 COMBINE THE ABOVE WITH MULTITHREADING [ ]


```
