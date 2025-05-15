![img place](/imgs/repo_header_img.png)
#### repo of random useuful things that I easily forget
___
```
tar -xvzf community_images.tar.gz
```
___
```
PERFORMANCE CHECKLIST:

 REMOVE WASTE [ ]
    Make sure you're producing the minmal ammount of cpu instructions needed to do the thing you want

 GET ILP [ ]
    a) UNROLL LOOP <- this increases this ratio workdoneinloop/loopmaintainance
    b) BREAK DEPENDENCY CHAIN <- this allows for the cpu to do ILP
       Dependencies to clean:
         * chained calculations
         * pending loads and stores (keep loads and stores as close to L1 cache as possible)

         KEEP SHIT CLOSE TO CACHE [ ]
                            (know the size of L1 cache and try to keep loops to opperate within that mem address count size)
                            EG 12900k has L1 cache size of 80kb for P-Cores (performance core intel parlance)
                            80kb = 640,000bits

         LOADS AND STORES INFO (sizes of different caches vary depending on chip arch):
             CPU PER CORE INFO:
                 if ( MemAddressToLoadOrStore is in RegisterFile ) {
                    aweosome ! fastest load or store ever
                 } else if (MemAddressToLoadOrStore is in L1 ) {
                    still pretty awesome happens in 3 or 4 cycles
                 } else if (MemAddressToLoadOrStore is in L2 ) {
                    less awesome than L1 happens in 14 cycles or more
                    ------------------shared between cpu cores------------------
                 } else if (MemAddressToLoadOrStore is in L3) {
                    even less awesome than L2 happens in 80 cycles or more
                 } else {
                    Fetching from main memory (THIS SUCKS)
                 }

 MUTLITHREADING [ ]


 COMBINE THE ABOVE WITH SIMD [ ]
```
