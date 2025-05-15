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
         chained calculations
         pending loads and stores
         LOADS AND STORES INFO:
             CPU CORE INFO:
                In the CPU there is something called the register file

                RegisterFile size: (very small)
                    The register file produces values at maximum speed even faster than L1 chache when doing oppertations
                    _L1_: size (32k)
                    When we are about to load a new value in a loop for eg. Input[some_idx_we_haven_t_hit_before] the CPU first goes into L1 cache and checks if the mem address we're refering to is there already
                        if it is there it returns it back and we don't go into main memory ( happens in 3 or 4 cycles )


 COMBINE THE ABOVE WITH SIMD [ ]

 COMBINE THE ABOVE WITH MULTITHREADING [ ]


```
