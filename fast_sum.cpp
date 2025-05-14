#include <stdio.h>
// UNDERSTAND IF MY CPU SUPPORTS THESE YOU CAN DO
// cat /proc/cpuinfo and inspect the flags section
//INCLUDE SSE
#include <xmmintrin.h>
//INCLUDE AVX
#ifdef __clang__
#include <immintrin.h>
#endif

#include <time.h>
#include <stdlib.h>

typedef unsigned int u32;
typedef unsigned long long u64;

#define BUFFER_SIZE 1048576
#define SECONDS_TO_NANOSECONDS_MULT 1000000000

//PERFORMANCE CHECKLIST:
// REMOVE WASTE[ ]
// GET ILP[ ]
//  a) UNROLL LOOP <- this increases this ratio workdoneinloop/loopmaintainance
//  b) BREAK DEPENDENCY CHAIN <- this allows for the cpu to do ILP
// COMBINE THE ABOVE WITH SIMD[ ]

static inline
u64 Nanos()
{
  struct timespec Start;
  clock_gettime(CLOCK_MONOTONIC_RAW,&Start);
  return ((u64)Start.tv_sec*SECONDS_TO_NANOSECONDS_MULT + (u64)Start.tv_nsec);
}

//NORMAL LOOP
u32 SingleScalar(u32 Count, u32 *Input)
{
	u32 Sum = 0;
	for(u32 Index = 0; Index < Count; ++Index)
	{
		Sum += Input[Index];
	}
	return Sum;
}

u32 DualScalar(u32 Count, u32 *Input)
{
	u32 SumA = 0;
	u32 SumB = 0;
	for(u32 Index = 0; Index < Count; Index+=2)
	{
		SumA += Input[Index];
		SumB += Input[Index+1];
	}
  u32 Sum = SumA + SumB;
	return Sum;
}

//NORMAL LOOP SSED (128 wide which means 4 unit32 wide adds in this case)
u32 __attribute__((target("ssse3"))) SingleSSE(u32 Count, u32 *Input)
{
	__m128i Sum = _mm_setzero_si128();
	for(u32 Index = 0; Index < Count; Index += 4)
	{
		Sum = _mm_add_epi32(Sum, _mm_load_si128((__m128i *)&Input[Index]));
	}

	Sum = _mm_hadd_epi32(Sum, Sum);
	Sum = _mm_hadd_epi32(Sum, Sum);

	return _mm_cvtsi128_si32(Sum);
}

//NORMAL LOOP AVXED (256 wide which means 8 unit32 wide adds in this case)
u32 __attribute__((target("avx2"))) SingleAVX(u32 Count, u32 *Input)
{
	__m256i Sum = _mm256_setzero_si256();
	for(u32 Index = 0; Index < Count; Index += 8)
	{
		Sum = _mm256_add_epi32(Sum, _mm256_loadu_si256((__m256i *)&Input[Index]));
	}

	Sum = _mm256_hadd_epi32(Sum, Sum);
	Sum = _mm256_hadd_epi32(Sum, Sum);
	__m256i SumS = _mm256_permute2x128_si256(Sum, Sum, 1 | (1 << 4));
	Sum = _mm256_add_epi32(Sum, SumS);

	return _mm256_cvtsi256_si32(Sum);
}

//UNROLL TWICE AVX
u32 __attribute__((target("avx2"))) DualAVX(u32 Count, u32 *Input)
{
	__m256i SumA = _mm256_setzero_si256();
	__m256i SumB = _mm256_setzero_si256();
	for(u32 Index = 0; Index < Count; Index += 16)
	{
		SumA = _mm256_add_epi32(SumA, _mm256_loadu_si256((__m256i *)&Input[Index]));
		SumB = _mm256_add_epi32(SumB, _mm256_loadu_si256((__m256i *)&Input[Index + 8]));
	}

	__m256i Sum = _mm256_add_epi32(SumA, SumB);

	Sum = _mm256_hadd_epi32(Sum, Sum);
	Sum = _mm256_hadd_epi32(Sum, Sum);
	__m256i SumS = _mm256_permute2x128_si256(Sum, Sum, 1 | (1 << 4));
	Sum = _mm256_add_epi32(Sum, SumS);

	return _mm256_cvtsi256_si32(Sum);
}

//UNROLL QUAD AVX
u32 __attribute__((target("avx2"))) QuadAVX(u32 Count, u32 *Input)
{
	__m256i SumA = _mm256_setzero_si256();
	__m256i SumB = _mm256_setzero_si256();
	__m256i SumC = _mm256_setzero_si256();
	__m256i SumD = _mm256_setzero_si256();
	for(u32 Index = 0; Index < Count; Index += 32)
	{
		SumA = _mm256_add_epi32(SumA, _mm256_loadu_si256((__m256i *)&Input[Index]));
		SumB = _mm256_add_epi32(SumB, _mm256_loadu_si256((__m256i *)&Input[Index + 8]));
		SumC = _mm256_add_epi32(SumC, _mm256_loadu_si256((__m256i *)&Input[Index + 16]));
		SumD = _mm256_add_epi32(SumD, _mm256_loadu_si256((__m256i *)&Input[Index + 24]));
	}

	__m256i SumAB = _mm256_add_epi32(SumA, SumB);
	__m256i SumCD = _mm256_add_epi32(SumC, SumD);
	__m256i Sum = _mm256_add_epi32(SumAB, SumCD);

	Sum = _mm256_hadd_epi32(Sum, Sum);
	Sum = _mm256_hadd_epi32(Sum, Sum);
	__m256i SumS = _mm256_permute2x128_si256(Sum, Sum, 1 | (1 << 4));
	Sum = _mm256_add_epi32(Sum, SumS);

	return _mm256_cvtsi256_si32(Sum);
}

u32 __attribute__((target("avx2"))) QuadAVXWithMagic(u32 Count, u32 *Input)
{
	__m256i SumA = _mm256_setzero_si256();
	__m256i SumB = _mm256_setzero_si256();
	__m256i SumC = _mm256_setzero_si256();
	__m256i SumD = _mm256_setzero_si256();

	Count /= 32;
	while(Count--)
	{
		SumA = _mm256_add_epi32(SumA, _mm256_loadu_si256((__m256i *)&Input[0]));
		SumB = _mm256_add_epi32(SumB, _mm256_loadu_si256((__m256i *)&Input[8]));
		SumC = _mm256_add_epi32(SumC, _mm256_loadu_si256((__m256i *)&Input[16]));
		SumD = _mm256_add_epi32(SumD, _mm256_loadu_si256((__m256i *)&Input[24]));

		Input += 32;
	}

	__m256i SumAB = _mm256_add_epi32(SumA, SumB);
	__m256i SumCD = _mm256_add_epi32(SumC, SumD);
	__m256i Sum = _mm256_add_epi32(SumAB, SumCD);

	Sum = _mm256_hadd_epi32(Sum, Sum);
	Sum = _mm256_hadd_epi32(Sum, Sum);
	__m256i SumS = _mm256_permute2x128_si256(Sum, Sum, 1 | (1 << 4));
	Sum = _mm256_add_epi32(Sum, SumS);

	return _mm256_cvtsi256_si32(Sum);
}

//FASTEST NON SIMD LOOP Without Magix
u32 FastestNonSIMDLoopNoMagic(u32 Count, u32 *Input)
{
  //BREAK DEPENDENCY chain to get ILP
	u32 SumA = 0;
	u32 SumB = 0;
	u32 SumC = 0;
	u32 SumD = 0;

	for(u32 Index = 0; Index < Count; Index += 4)
	{
    //UNROLL LOOP to increase ratio of workdoneinloop/loopmaintainance
		SumA += Input[Index + 0];
		SumB += Input[Index + 1];
		SumC += Input[Index + 2];
		SumD += Input[Index + 3];
	}

	u32 Sum = SumA + SumB + SumC + SumD;
	return Sum;
}

u32 FastestNonSIMDLoopWithMagic(u32 Count, u32 *Input)
{
	u32 SumA = 0;
	u32 SumB = 0;
	u32 SumC = 0;
	u32 SumD = 0;

	Count /= 4;
	while(Count--)
	{
		SumA += Input[0];
		SumB += Input[1];
		SumC += Input[2];
		SumD += Input[3];
		Input += 4;
	}

	u32 Sum = SumA + SumB + SumC + SumD;
	return Sum;
}

int main(int ArgCount, char **Args)
{
  srand(time(0));
  u32 MyBufferToSum[BUFFER_SIZE] = {0};
  for(u32 Index = 0; Index < BUFFER_SIZE; Index+=4)
  {
    MyBufferToSum[Index]   =  rand();
    MyBufferToSum[Index+1] =  rand();
    MyBufferToSum[Index+2] =  rand();
    MyBufferToSum[Index+3] =  rand();
  }
  u64 Start = Nanos();
  SingleScalar(BUFFER_SIZE, MyBufferToSum);
  u64 End = Nanos();
  printf("nanos elapsed in SingleScalar Loop                  %llu\n",End-Start);

  Start = Nanos();
  SingleScalar(BUFFER_SIZE, MyBufferToSum);
  End = Nanos();
  printf("nanos elapsed in DualScalar Loop                    %llu\n",End-Start);

  Start = Nanos();
  FastestNonSIMDLoopNoMagic(BUFFER_SIZE, MyBufferToSum);
  End = Nanos();
  printf("nanos elapsed in QuadScalar Loop with no magic      %llu\n",End-Start);

  Start = Nanos();
  FastestNonSIMDLoopWithMagic(BUFFER_SIZE, MyBufferToSum);
  End = Nanos();
  printf("nanos elapsed in QuadScalar Loop with magic         %llu\n",End-Start);

  Start = Nanos();
  SingleSSE(BUFFER_SIZE, MyBufferToSum);
  End = Nanos();
  printf("nanos elapsed in SingleSIMD 4 wide                  %llu\n",End-Start);

  Start = Nanos();
  SingleAVX(BUFFER_SIZE, MyBufferToSum);
  End = Nanos();
  printf("nanos elapsed in SingleSIMD 8 wide                  %llu\n",End-Start);

  Start = Nanos();
  DualAVX(BUFFER_SIZE, MyBufferToSum);
  End = Nanos();
  printf("nanos elapsed in Dual unroll SIMD 8 wide            %llu\n",End-Start);

  Start = Nanos();
  QuadAVX(BUFFER_SIZE, MyBufferToSum);
  End = Nanos();
  printf("nanos elapsed in Quad unroll SIMD 8 wide            %llu\n",End-Start);

  Start = Nanos();
  QuadAVXWithMagic(BUFFER_SIZE, MyBufferToSum);
  End = Nanos();
  printf("nanos elapsed in Quad unroll SIMD 8 wide wtih magic %llu\n",End-Start);

  return(0);
}
