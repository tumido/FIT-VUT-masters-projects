// Library = EvoApprox 16x16
// Circuit = mul32x32_061
// Area   (45) = 1150
// Delay  (45) = 1.750
// Power  (45) = 0.69
// MAE = 10647482012080490.0
// MRE = 19.69 %
// WCE LIMIT = 922337203685477580800
// WCE LIMIT % = 0.5 %
// EP = 100.0 %
#include <stdint.h>
inline uint64_t mul32x32_061(uint32_t opA, uint32_t opB) {

  int A[32] = { 0 };
  A[0] = (opA >> 0) & 0x01;
  A[1] = (opA >> 1) & 0x01;
  A[2] = (opA >> 2) & 0x01;
  A[3] = (opA >> 3) & 0x01;
  A[4] = (opA >> 4) & 0x01;
  A[5] = (opA >> 5) & 0x01;
  A[6] = (opA >> 6) & 0x01;
  A[7] = (opA >> 7) & 0x01;
  A[8] = (opA >> 8) & 0x01;
  A[9] = (opA >> 9) & 0x01;
  A[10] = (opA >> 10) & 0x01;
  A[11] = (opA >> 11) & 0x01;
  A[12] = (opA >> 12) & 0x01;
  A[13] = (opA >> 13) & 0x01;
  A[14] = (opA >> 14) & 0x01;
  A[15] = (opA >> 15) & 0x01;
  A[16] = (opA >> 16) & 0x01;
  A[17] = (opA >> 17) & 0x01;
  A[18] = (opA >> 18) & 0x01;
  A[19] = (opA >> 19) & 0x01;
  A[20] = (opA >> 20) & 0x01;
  A[21] = (opA >> 21) & 0x01;
  A[22] = (opA >> 22) & 0x01;
  A[23] = (opA >> 23) & 0x01;
  A[24] = (opA >> 24) & 0x01;
  A[25] = (opA >> 25) & 0x01;
  A[26] = (opA >> 26) & 0x01;
  A[27] = (opA >> 27) & 0x01;
  A[28] = (opA >> 28) & 0x01;
  A[29] = (opA >> 29) & 0x01;
  A[30] = (opA >> 30) & 0x01;
  A[31] = (opA >> 31) & 0x01;
  int B[32] = { 0 };
  B[0] = (opB >> 0) & 0x01;
  B[1] = (opB >> 1) & 0x01;
  B[2] = (opB >> 2) & 0x01;
  B[3] = (opB >> 3) & 0x01;
  B[4] = (opB >> 4) & 0x01;
  B[5] = (opB >> 5) & 0x01;
  B[6] = (opB >> 6) & 0x01;
  B[7] = (opB >> 7) & 0x01;
  B[8] = (opB >> 8) & 0x01;
  B[9] = (opB >> 9) & 0x01;
  B[10] = (opB >> 10) & 0x01;
  B[11] = (opB >> 11) & 0x01;
  B[12] = (opB >> 12) & 0x01;
  B[13] = (opB >> 13) & 0x01;
  B[14] = (opB >> 14) & 0x01;
  B[15] = (opB >> 15) & 0x01;
  B[16] = (opB >> 16) & 0x01;
  B[17] = (opB >> 17) & 0x01;
  B[18] = (opB >> 18) & 0x01;
  B[19] = (opB >> 19) & 0x01;
  B[20] = (opB >> 20) & 0x01;
  B[21] = (opB >> 21) & 0x01;
  B[22] = (opB >> 22) & 0x01;
  B[23] = (opB >> 23) & 0x01;
  B[24] = (opB >> 24) & 0x01;
  B[25] = (opB >> 25) & 0x01;
  B[26] = (opB >> 26) & 0x01;
  B[27] = (opB >> 27) & 0x01;
  B[28] = (opB >> 28) & 0x01;
  B[29] = (opB >> 29) & 0x01;
  B[30] = (opB >> 30) & 0x01;
  B[31] = (opB >> 31) & 0x01;
  int Z[64] = { 0 };


  int sig_65 = A[26] & B[26];
  int sig_83 = A[11];
  int sig_85 = A[25] & B[30];
  int sig_87 = A[25] & B[27];
  int sig_101 = A[28] & B[28];
  int sig_121 = ~ B[26];
  int sig_149 = A[29] & B[25];
  int sig_158 = A[28] & B[31];
  int sig_160 = A[29] & B[27];
  int sig_174 = ~ (A[22] | B[25]);
  int sig_187 = A[27] & B[29];
  int sig_196 = sig_121 | sig_121;
  int sig_197 = A[26] & B[31];
  int sig_199 = A[27] & B[27];
  int sig_231 = A[24] & B[31];
  int sig_241 = A[31] & B[23];
  int sig_255 = A[31] & B[20];
  int sig_258 = A[27] & B[25];
  int sig_267 = A[30] & B[21];
  int sig_268 = A[31] & B[27];
  int sig_275 = A[22] & B[31];
  int sig_277 = ~ sig_267;
  int sig_280 = A[30] & B[30];
  int sig_333 = A[23] & B[29];
  int sig_341 = A[29] & B[22];
  int sig_365 = A[28] & B[30];
  int sig_384 = ~ sig_174;
  int sig_403 = A[30] & B[23];
  int sig_407 = ~ B[27];
  int sig_433 = ~ sig_187;
  int sig_435 = A[29] & B[29];
  int sig_437 = A[28] & B[23];
  int sig_472 = A[22] & B[30];
  int sig_481 = A[29] & B[31];
  int sig_487 = A[28] & B[24];
  int sig_489 = A[24] & B[29];
  int sig_504 = A[31] & B[28];
  int sig_531 = A[30] & B[24];
  int sig_547 = sig_487 ^ sig_258;
  int sig_555 = A[26] & B[30];
  int sig_562 = A[23] & B[30];
  int sig_567 = A[25] & B[28];
  int sig_577 = A[29] & B[26];
  int sig_592 = A[26] & B[27];
  int sig_599 = A[24] & B[30];
  int sig_600 = sig_547 ^ sig_65;
  int sig_606 = A[27] & B[28];
  int sig_608 = sig_531 ^ sig_149;
  int sig_624 = A[27] & B[26];
  int sig_637 = ~ (sig_241 | B[27]);
  int sig_654 = A[29] & B[28];
  int sig_666 = A[25] & B[26];
  int sig_683 = A[30] & B[27];
  int sig_726 = A[30] & B[25];
  int sig_728 = A[28] & B[27];
  int sig_735 = A[27] & B[31];
  int sig_738 = ~ sig_365;
  int sig_782 = A[25] & B[31];
  int sig_810 = A[31] & B[31];
  int sig_815 = A[26] & B[25];
  int sig_824 = A[23] & B[31];
  int sig_827 = A[31] & B[29];
  int sig_828 = A[31] & B[21];
  int sig_856 = A[28] & B[25];
  int sig_869 = A[21] & B[31];
  int sig_892 = ~ B[12];
  int sig_919 = A[30] & B[22];
  int sig_947 = sig_160 ^ sig_101;
  int sig_958 = A[26] & B[29];
  int sig_966 = A[30] & B[28];
  int sig_997 = A[29] & B[30];
  int sig_1012 = A[29] & B[23];
  int sig_1015 = A[31] & B[30];
  int sig_1021 = sig_160 & sig_101;
  int sig_1058 = A[30] & B[29];
  int sig_1083 = A[31] & B[22];
  int sig_1097 = A[31] & B[25];
  int sig_1105 = A[27] & B[24];
  int sig_1143 = sig_267 | sig_341;
  int sig_1162 = A[29] & B[24];
  int sig_1168 = A[25] & B[29];
  int sig_1208 = A[30] & B[26];
  int sig_1219 = A[30] & B[31];
  int sig_1228 = A[31] & B[24];
  int sig_1245 = sig_268 ^ sig_966;
  int sig_1265 = A[31] & B[26];
  int sig_1267 = A[24] & B[28];
  int sig_1268 = sig_267 & sig_341;
  int sig_1280 = A[28] & B[26];
  int sig_1311 = A[27] & B[30];
  int sig_1315 = A[28] & B[29];
  int sig_1329 = ~ sig_624;
  int sig_1335 = A[26] & B[28];
  int sig_1361 = sig_1208 & sig_1097;
  int sig_1395 = sig_268 & sig_966;
  int sig_1405 = sig_504 ^ sig_1058;
  int sig_1408 = sig_65 & sig_547;
  int sig_1435 = sig_1280 & sig_608;
  int sig_1445 = sig_531 & sig_149;
  int sig_1460 = ~ sig_472;
  int sig_1494 = sig_1228 & sig_726;
  int sig_1515 = sig_1228 ^ sig_726;
  int sig_1529 = sig_728 ^ sig_606;
  int sig_1567 = sig_683 ^ sig_654;
  int sig_1569 = sig_1162 & sig_856;
  int sig_1582 = ~ sig_241;
  int sig_1632 = sig_592 ^ sig_567;
  int sig_1641 = sig_683 & sig_654;
  int sig_1646 = sig_487 & sig_258;
  int sig_1654 = sig_1105 | sig_815;
  int sig_1668 = sig_1265 ^ sig_1361;
  int sig_1680 = sig_828 | sig_919;
  int sig_1681 = sig_728 & sig_606;
  int sig_1723 = sig_592 & sig_567;
  int sig_1742 = ~ sig_1435;
  int sig_1760 = sig_1097 ^ sig_1208;
  int sig_1764 = sig_637 & sig_174;
  int sig_1779 = sig_435 & sig_1245;
  int sig_1805 = sig_1105 & sig_815;
  int sig_1807 = sig_608 ^ sig_1280;
  int sig_1812 = sig_437 & sig_1143;
  int sig_1821 = sig_828 & sig_919;
  int sig_1824 = sig_947 ^ sig_187;
  int sig_1886 = sig_1162 ^ sig_856;
  int sig_1891 = sig_1268 | sig_1812;
  int sig_1915 = sig_666 & sig_1654;
  int sig_1924 = sig_403 & sig_1083;
  int sig_1931 = sig_1245 ^ sig_435;
  int sig_1943 = sig_624 & sig_1886;
  int sig_1948 = ~ sig_407;
  int sig_1958 = sig_1805 | sig_1915;
  int sig_1973 = sig_1395 | sig_1779;
  int sig_1976 = sig_577 & sig_1515;
  int sig_2007 = ~ B[19];
  int sig_2009 = sig_1405 ^ sig_1973;
  int sig_2046 = sig_958 & sig_1529;
  int sig_2070 = sig_199 ^ sig_1335;
  int sig_2077 = sig_1315 & sig_1567;
  int sig_2079 = sig_1361;
  int sig_2088 = sig_187 & sig_947;
  int sig_2089 = sig_1058 & sig_504;
  int sig_2097 = sig_1083 ^ sig_403;
  int sig_2119 = sig_255;
  int sig_2146 = sig_1764;
  int sig_2160 = sig_87 ^ sig_1267;
  int sig_2175 = sig_199 & sig_1335;
  int sig_2176 = sig_1641 | sig_2077;
  int sig_2207 = sig_1681 | sig_2046;
  int sig_2208 = sig_87 & sig_1267;
  int sig_2217 = sig_1632 ^ sig_489;
  int sig_2269 = sig_1168 & sig_2070;
  int sig_2275 = sig_1567 ^ sig_1315;
  int sig_2296 = sig_1515 ^ sig_577;
  int sig_2311 = sig_1529 ^ sig_958;
  int sig_2323 = sig_1886 ^ sig_624;
  int sig_2325 = ~ sig_1948;
  int sig_2326 = sig_1012 & sig_1680;
  int sig_2352 = ~ sig_1582;
  int sig_2353 = sig_1821 | sig_2326;
  int sig_2354 = sig_1445 | sig_1435;
  int sig_2384 = sig_1646 | sig_1408;
  int sig_2398 = sig_1891 ^ sig_600;
  int sig_2401 = sig_1821;
  int sig_2421 = sig_1405 & sig_1973;
  int sig_2424 = sig_2009 ^ sig_997;
  int sig_2440 = sig_1931 ^ sig_2176;
  int sig_2456 = sig_489 & sig_1632;
  int sig_2466 = sig_2175 | sig_2269;
  int sig_2477 = sig_1924 ^ sig_1807;
  int sig_2485 = sig_2440 ^ sig_365;
  int sig_2504 = sig_827 & sig_2089;
  int sig_2517 = ~ sig_1012;
  int sig_2545 = sig_2296 ^ sig_2354;
  int sig_2553 = sig_333 & sig_2160;
  int sig_2576 = sig_1824 ^ sig_2207;
  int sig_2604 = sig_1824 & sig_2207;
  int sig_2621 = sig_2160 | sig_333;
  int sig_2629 = sig_2354 & sig_2296;
  int sig_2641 = ~ sig_2208;
  int sig_2643 = sig_997 & sig_2009;
  int sig_2652 = sig_1924 & sig_1807;
  int sig_2683 = sig_1569 | sig_1943;
  int sig_2713 = sig_1958 & sig_2398;
  int sig_2717 = sig_2070 ^ sig_1168;
  int sig_2730 = sig_1891 & sig_600;
  int sig_2732 = sig_1494 | sig_1976;
  int sig_2737 = sig_2683 & sig_2477;
  int sig_2740 = sig_827 ^ sig_2089;
  int sig_2757 = sig_2208 | sig_2553;
  int sig_2759 = ~ sig_2097;
  int sig_2792 = sig_1021 | sig_2088;
  int sig_2802 = sig_2353 ^ sig_2323;
  int sig_2805 = sig_1931 & sig_2176;
  int sig_2817 = sig_1315 & sig_1105;
  int sig_2837 = ~ sig_1958;
  int sig_2849 = sig_1723 | sig_2456;
  int sig_2897 = sig_2217 | sig_2757;
  int sig_2920 = ~ sig_487;
  int sig_2923 = sig_2477 ^ sig_2683;
  int sig_2936 = sig_2421 | sig_2643;
  int sig_2945 = sig_2730 | sig_2713;
  int sig_2952 = sig_1760 ^ sig_2732;
  int sig_2955 = sig_2275 ^ sig_2792;
  int sig_2964 = sig_2952 ^ sig_2629;
  int sig_2978 = sig_2311 ^ sig_2466;
  int sig_2981 = sig_555 & sig_2576;
  int sig_2994 = sig_2398 ^ sig_1958;
  int sig_3000 = sig_2732 & sig_1760;
  int sig_3002 = sig_2275 & sig_2792;
  int sig_3008 = sig_365 & sig_2440;
  int sig_3022 = sig_2652 | sig_2737;
  int sig_3025 = sig_2311 & sig_2466;
  int sig_3043 = sig_2576 ^ sig_555;
  int sig_3054 = sig_2217 & sig_2757;
  int sig_3102 = sig_2384 & sig_2802;
  int sig_3103 = sig_2353 & sig_2323;
  int sig_3136 = sig_2897 ^ sig_562;
  int sig_3140 = sig_1948;
  int sig_3229 = sig_3022 & sig_2545;
  int sig_3252 = sig_3000 & sig_1668;
  int sig_3263 = sig_2802 ^ sig_2384;
  int sig_3264 = sig_2621 ^ sig_2146;
  int sig_3267 = sig_2805 | sig_3008;
  int sig_3272 = sig_2740 ^ sig_280;
  int sig_3284 = sig_2424 ^ sig_3267;
  int sig_3287 = sig_3103 | sig_3102;
  int sig_3296 = sig_562 & sig_2897;
  int sig_3299 = sig_2545 ^ sig_3022;
  int sig_3305 = sig_2945 & sig_3263;
  int sig_3318 = sig_2964 ^ sig_3229;
  int sig_3321 = sig_85 & sig_2978;
  int sig_3325 = sig_1668 ^ sig_3000;
  int sig_3342 = sig_2717 ^ sig_2849;
  int sig_3344 = sig_1311 & sig_2955;
  int sig_3346 = sig_2604 | sig_2981;
  int sig_3352 = sig_2424 & sig_3267;
  int sig_3363 = sig_2717 & sig_2849;
  int sig_3369 = sig_3263 ^ sig_2945;
  int sig_3372 = sig_2955 ^ sig_1311;
  int sig_3383 = sig_280 & sig_2740;
  int sig_3399 = sig_2119;
  int sig_3403 = sig_2978 ^ sig_85;
  int sig_3438 = sig_3272 & sig_2936;
  int sig_3452 = sig_2504 | sig_3383;
  int sig_3475 = sig_1015 ^ sig_3452;
  int sig_3476 = sig_3284 ^ sig_158;
  int sig_3482 = sig_2964 & sig_3229;
  int sig_3544 = sig_3399 & sig_2994;
  int sig_3561 = sig_3287 & sig_2923;
  int sig_3575 = sig_2629 & sig_2952;
  int sig_3580 = sig_2079 & sig_3252;
  int sig_3585 = sig_472 & sig_3264;
  int sig_3590 = sig_2923 ^ sig_3287;
  int sig_3602 = sig_599 & sig_3342;
  int sig_3607 = sig_3299 ^ sig_3561;
  int sig_3613 = sig_158 & sig_3284;
  int sig_3626 = sig_3325 ^ sig_3575;
  int sig_3628 = sig_3025 | sig_3321;
  int sig_3631 = sig_3372 & sig_3346;
  int sig_3645 = sig_3372 ^ sig_3346;
  int sig_3647 = sig_3002 | sig_3344;
  int sig_3652 = B[10];
  int sig_3671 = sig_197 & sig_3645;
  int sig_3687 = sig_3054 | sig_3296;
  int sig_3692 = sig_3342 ^ sig_599;
  int sig_3699 = sig_3043 & sig_3628;
  int sig_3704 = sig_3264 ^ sig_472;
  int sig_3718 = sig_1015 & sig_3452;
  int sig_3725 = sig_3299 & sig_3561;
  int sig_3734 = sig_1219 & sig_3475;
  int sig_3737 = sig_3475 ^ sig_1219;
  int sig_3740 = sig_3325 & sig_3575;
  int sig_3748 = sig_2079 ^ sig_3252;
  int sig_3760 = sig_3272 ^ sig_2936;
  int sig_3764 = sig_3631 | sig_3671;
  int sig_3847 = sig_3692 & sig_3687;
  int sig_3849 = sig_2485 ^ sig_3647;
  int sig_3857 = sig_3590 ^ sig_3305;
  int sig_3862 = sig_735 & sig_3849;
  int sig_3880 = sig_3363 | sig_3602;
  int sig_3898 = sig_3043 ^ sig_3628;
  int sig_3911 = sig_3692 ^ sig_3687;
  int sig_3912 = sig_782 & sig_3898;
  int sig_3929 = ~ sig_3585;
  int sig_3953 = sig_2485 & sig_3647;
  int sig_3956 = sig_3699 | sig_3912;
  int sig_3974 = sig_3760 ^ sig_481;
  int sig_3982 = sig_3590 & sig_3305;
  int sig_3992 = sig_481 & sig_3760;
  int sig_4019 = sig_3953 | sig_3862;
  int sig_4062 = ~ sig_3929;
  int sig_4078 = sig_3403 & sig_3880;
  int sig_4087 = sig_3352 | sig_3613;
  int sig_4107 = sig_3136;
  int sig_4116 = sig_3645 ^ sig_197;
  int sig_4121 = sig_3544;
  int sig_4153 = sig_3403 ^ sig_3880;
  int sig_4158 = ~ sig_2981;
  int sig_4187 = sig_275 & sig_4107;
  int sig_4191 = sig_3718 | sig_3734;
  int sig_4230 = sig_3849 ^ sig_735;
  int sig_4243 = sig_3369 & sig_4121;
  int sig_4260 = ~ sig_4158;
  int sig_4264 = sig_4062 | sig_4187;
  int sig_4292 = sig_4116 & sig_3626;
  int sig_4294 = sig_3704;
  int sig_4321 = ~ sig_3704;
  int sig_4328 = sig_3911 ^ sig_824;
  int sig_4340 = sig_3898 ^ sig_782;
  int sig_4351 = sig_824 & sig_3911;
  int sig_4394 = sig_869 & sig_4321;
  int sig_4402 = sig_3438 | sig_3992;
  int sig_4410 = sig_3847 | sig_4351;
  int sig_4412 = sig_3626 ^ sig_4116;
  int sig_4431 = sig_4294 | sig_4394;
  int sig_4446 = sig_810 ^ sig_4191;
  int sig_4461 = sig_4230 & sig_3748;
  int sig_4473 = sig_4340 & sig_3318;
  int sig_4484 = sig_4107 ^ sig_275;
  int sig_4486 = sig_4328 & sig_3857;
  int sig_4493 = sig_3369 ^ sig_4121;
  int sig_4494 = sig_3748 ^ sig_4230;
  int sig_4497 = sig_4153 ^ sig_231;
  int sig_4503 = ~ sig_384;
  int sig_4511 = sig_3580 | sig_4461;
  int sig_4516 = sig_4191 & sig_810;
  int sig_4518 = ~ sig_4260;
  int sig_4523 = ~ sig_433;
  int sig_4532 = sig_4511 & sig_3476;
  int sig_4533 = sig_231 & sig_4153;
  int sig_4569 = sig_3857 ^ sig_4328;
  int sig_4584 = sig_3318 ^ sig_4340;
  int sig_4622 = sig_4078 | sig_4533;
  int sig_4639 = sig_3740 | sig_4292;
  int sig_4643 = sig_3974 ^ sig_4532;
  int sig_4663 = sig_4497 & sig_3607;
  int sig_4666 = ~ sig_4518;
  int sig_4667 = sig_4321 | sig_869;
  int sig_4671 = sig_4494 ^ sig_4639;
  int sig_4676 = sig_4484 & sig_4493;
  int sig_4696 = sig_4532 & sig_3974;
  int sig_4709 = sig_4639 & sig_4494;
  int sig_4730 = sig_4493 ^ sig_4484;
  int sig_4738 = sig_3737 ^ sig_4696;
  int sig_4741 = sig_3607 ^ sig_4497;
  int sig_4777 = sig_3982 | sig_4486;
  int sig_4778 = sig_3652 & sig_4666;
  int sig_4790 = sig_3482 | sig_4473;
  int sig_4804 = sig_4730;
  int sig_4825 = sig_4790 & sig_4412;
  int sig_4834 = sig_3476 ^ sig_4511;
  int sig_4836 = sig_4777 & sig_4741;
  int sig_4864 = sig_4825 & sig_4671;
  int sig_4880 = sig_4402 & sig_4738;
  int sig_4884 = sig_4741 ^ sig_4777;
  int sig_4888 = sig_4412 ^ sig_4790;
  int sig_4899 = ~ sig_4260;
  int sig_4901 = sig_4243 | sig_4676;
  int sig_4902 = sig_3737 & sig_4696;
  int sig_4905 = sig_4738 ^ sig_4402;
  int sig_4910 = sig_3725 | sig_4663;
  int sig_4915 = sig_4834 ^ sig_4709;
  int sig_4951 = ~ (sig_3652 & sig_4666);
  int sig_4954 = sig_4915 ^ sig_4864;
  int sig_4966 = sig_4902 | sig_4880;
  int sig_4970 = sig_4910 & sig_4584;
  int sig_4978 = sig_4901 & sig_4569;
  int sig_4985 = sig_4954 ^ sig_4019;
  int sig_4998 = sig_4709 & sig_4834;
  int sig_5010 = sig_4584 ^ sig_4910;
  int sig_5011 = sig_5010 ^ sig_4836;
  int sig_5019 = sig_4569 ^ sig_4901;
  int sig_5023 = sig_4970 & sig_4888;
  int sig_5046 = sig_4643 & sig_4998;
  int sig_5057 = sig_4671 ^ sig_4825;
  int sig_5066 = sig_5019 & sig_241;
  int sig_5080 = sig_5057 ^ sig_5023;
  int sig_5091 = sig_4667 & sig_4951;
  int sig_5103 = sig_4019 & sig_4954;
  int sig_5106 = sig_4884 ^ sig_5066;
  int sig_5112 = sig_5080 ^ sig_3764;
  int sig_5130 = sig_5106 ^ sig_4978;
  int sig_5134 = ~ sig_4667;
  int sig_5139 = sig_4888 ^ sig_4970;
  int sig_5142 = sig_4915 & sig_4864;
  int sig_5149 = sig_4643 ^ sig_4998;
  int sig_5153 = sig_4836 & sig_5010;
  int sig_5182 = sig_4446 & sig_4966;
  int sig_5193 = sig_4446 ^ sig_4966;
  int sig_5215 = sig_3764 & sig_5080;
  int sig_5228 = sig_5139 ^ sig_5153;
  int sig_5234 = sig_241 ^ sig_5019;
  int sig_5249 = sig_4087 & sig_5149;
  int sig_5250 = sig_5057 & sig_5023;
  int sig_5253 = sig_4884 & sig_5066;
  int sig_5268 = sig_5149 ^ sig_4087;
  int sig_5277 = sig_4778 | sig_5091;
  int sig_5298 = sig_5277 & sig_4730;
  int sig_5300 = sig_3956 & sig_5228;
  int sig_5303 = sig_5139 & sig_5153;
  int sig_5310 = sig_5142 | sig_5103;
  int sig_5314 = sig_5046 | sig_5249;
  int sig_5317 = sig_4978 & sig_5106;
  int sig_5360 = ~ sig_2325;
  int sig_5374 = sig_5268 ^ sig_5310;
  int sig_5377 = sig_5268 & sig_5310;
  int sig_5386 = sig_5360 & sig_5134;
  int sig_5391 = sig_5250 | sig_5215;
  int sig_5393 = sig_4985 ^ sig_5391;
  int sig_5400 = sig_5228 ^ sig_3956;
  int sig_5426 = sig_5253 | sig_5317;
  int sig_5437 = sig_4985 & sig_5391;
  int sig_5447 = sig_4730 ^ sig_5277;
  int sig_5458 = sig_4905 ^ sig_5314;
  int sig_5460 = sig_4905 & sig_5314;
  int sig_5464 = sig_5303 | sig_5300;
  int sig_5479 = sig_5011 ^ sig_5426;
  int sig_5493 = sig_2354 & sig_2517;
  int sig_5512 = ~ sig_2517;
  int sig_5514 = sig_5112 & sig_5464;
  int sig_5518 = sig_5458 & sig_5377;
  int sig_5534 = sig_4622 & sig_5479;
  int sig_5542 = sig_5011 & sig_5426;
  int sig_5548 = sig_5460 | sig_5518;
  int sig_5554 = sig_5112 ^ sig_5464;
  int sig_5555 = sig_5458 & sig_5374;
  int sig_5567 = sig_5447 & sig_2097;
  int sig_5569 = sig_2097 ^ sig_5447;
  int sig_5583 = sig_5569 & sig_5493;
  int sig_5585 = sig_5569 ^ sig_5493;
  int sig_5599 = sig_5393 & sig_5514;
  int sig_5619 = sig_5386 & sig_5585;
  int sig_5622 = sig_5437 | sig_5599;
  int sig_5623 = sig_5479 ^ sig_4622;
  int sig_5632 = sig_5393 & sig_5554;
  int sig_5641 = sig_5234 & sig_5567;
  int sig_5645 = sig_1742;
  int sig_5649 = sig_5512;
  int sig_5650 = sig_5585 ^ sig_5386;
  int sig_5670 = sig_5542 | sig_5534;
  int sig_5684 = sig_5234 ^ sig_5567;
  int sig_5687 = sig_5400 & sig_5670;
  int sig_5690 = sig_5298 & sig_5684;
  int sig_5702 = ~ sig_5645;
  int sig_5707 = sig_5684 ^ sig_5298;
  int sig_5723 = sig_5583 | sig_5619;
  int sig_5735 = sig_196 & sig_5702;
  int sig_5744 = sig_5400 ^ sig_5670;
  int sig_5756 = sig_5707 ^ sig_5723;
  int sig_5777 = sig_5641 | sig_5690;
  int sig_5805 = sig_4264 & sig_5756;
  int sig_5808 = sig_5707 & sig_5723;
  int sig_5816 = sig_5130 ^ sig_5777;
  int sig_5851 = sig_4804 & sig_5735;
  int sig_5863 = sig_4410 & sig_5816;
  int sig_5865 = sig_5130 & sig_5777;
  int sig_5866 = sig_5756 ^ sig_4264;
  int sig_5899 = sig_5649;
  int sig_5907 = sig_5735 & sig_4497;
  int sig_5909 = sig_5808 | sig_5805;
  int sig_5915 = sig_5816 ^ sig_4410;
  int sig_5921 = sig_5915 & sig_5909;
  int sig_5929 = sig_5650 ^ sig_5899;
  int sig_5944 = sig_5915 ^ sig_5909;
  int sig_5956 = sig_4431 & sig_5929;
  int sig_5960 = sig_5865 | sig_5863;
  int sig_5965 = ~ sig_738;
  int sig_5972 = sig_5650 & sig_5899;
  int sig_5977 = sig_5623 & sig_5960;
  int sig_5983 = sig_5965 & sig_5851;
  int sig_6006 = sig_1329 & sig_5907;
  int sig_6010 = sig_5744 & sig_5977;
  int sig_6013 = sig_5929 ^ sig_4431;
  int sig_6021 = sig_6013 | sig_6006;
  int sig_6022 = sig_5687 | sig_6010;
  int sig_6029 = sig_5623 ^ sig_5960;
  int sig_6034 = sig_6021;
  int sig_6052 = sig_5972 | sig_5956;
  int sig_6053 = sig_5744 & sig_6029;
  int sig_6065 = sig_5866 & sig_6052;
  int sig_6068 = sig_5866 ^ sig_6052;
  int sig_6069 = sig_5944 & sig_6065;
  int sig_6071 = sig_5944 & sig_6068;
  int sig_6085 = ~ sig_782;
  int sig_6098 = sig_5632 & sig_6022;
  int sig_6100 = sig_5632 & sig_6053;
  int sig_6110 = sig_5622 | sig_6098;
  int sig_6113 = sig_2641 | sig_3140;
  int sig_6116 = sig_6071 & sig_6034;
  int sig_6130 = sig_5921 | sig_6069;
  int sig_6138 = sig_5464 & sig_6034;
  int sig_6157 = sig_6116 & sig_6113;
  int sig_6173 = sig_6130;
  int sig_6174 = sig_6173 | sig_6157;
  int sig_6267 = ~ sig_6110;
  int sig_6287 = sig_6113 | sig_2837;
  int sig_6295 = ~ sig_6287;
  int sig_6298 = sig_6034;
  int sig_6306 = sig_6174;
  int sig_6307 = sig_6138 | sig_6298;
  int sig_6309 = sig_6029 ^ sig_6306;
  int sig_6310 = sig_6068 ^ sig_6307;
  int sig_6311 = sig_6029 & sig_6306;
  int sig_6312 = sig_5977 | sig_6311;
  int sig_6313 = sig_5386;
  int sig_6314 = ~ sig_5983;
  int sig_6316 = sig_5744 ^ sig_6312;
  int sig_6317 = sig_6021 ^ sig_6314;
  int sig_6318 = ~ sig_6313;
  int sig_6319 = sig_6068 & sig_6307;
  int sig_6320 = sig_6053 & sig_6306;
  int sig_6321 = sig_6100 & sig_6306;
  int sig_6322 = sig_6022 | sig_6320;
  int sig_6323 = sig_5554 ^ sig_6322;
  int sig_6325 = sig_6065 | sig_6319;
  int sig_6326 = sig_5944 ^ sig_6325;
  int sig_6327 = sig_6110 | sig_6321;
  int sig_6328 = sig_5554 & sig_6322;
  int sig_6329 = sig_5555 & sig_6327;
  int sig_6330 = sig_5514 | sig_6328;
  int sig_6331 = sig_5374 & sig_6327;
  int sig_6332 = sig_5548 | sig_6329;
  int sig_6333 = sig_5374 ^ sig_6327;
  int sig_6334 = sig_5377 | sig_6331;
  int sig_6335 = sig_5458 ^ sig_6334;
  int sig_6336 = sig_5393 ^ sig_6330;
  int sig_6337 = sig_5193 & sig_6332;
  int sig_6338 = sig_5193 ^ sig_6332;
  int sig_6339 = sig_5182 | sig_6337;
  int sig_6340 = sig_4516 ^ sig_6339;

   Z[0] = sig_3953;
   Z[1] = sig_5687;
   Z[2] = sig_4191;
   Z[3] = sig_5965;
   Z[4] = sig_1219;
   Z[5] = sig_2923;
   Z[6] = sig_5915;
   Z[7] = B[19];
   Z[8] = sig_5512;
   Z[9] = sig_3352;
   Z[10] = sig_4523;
   Z[11] = sig_277;
   Z[12] = sig_121;
   Z[13] = sig_1329;
   Z[14] = sig_828;
   Z[15] = sig_2352;
   Z[16] = sig_2401;
   Z[17] = sig_2920;
   Z[18] = sig_5249;
   Z[19] = sig_892;
   Z[20] = sig_2759;
   Z[21] = sig_5447;
   Z[22] = sig_6329;
   Z[23] = sig_2007;
   Z[24] = sig_6320;
   Z[25] = sig_3363;
   Z[26] = sig_2817;
   Z[27] = sig_1582;
   Z[28] = sig_1460;
   Z[29] = sig_3580;
   Z[30] = sig_6267;
   Z[31] = sig_2621;
   Z[32] = sig_5723;
   Z[33] = B[30];
   Z[34] = sig_6085;
   Z[35] = sig_6313;
   Z[36] = sig_1245;
   Z[37] = sig_606;
   Z[38] = sig_6335;
   Z[39] = sig_4899;
   Z[40] = sig_5983;
   Z[41] = sig_5493;
   Z[42] = sig_1162;
   Z[43] = sig_83;
   Z[44] = sig_2119;
   Z[45] = sig_6267;
   Z[46] = sig_4503;
   Z[47] = sig_1723;
   Z[48] = sig_2740;
   Z[49] = sig_4696;
   Z[50] = sig_4230;
   Z[51] = sig_6318;
   Z[52] = sig_6295;
   Z[53] = sig_6317;
   Z[54] = sig_6310;
   Z[55] = sig_6326;
   Z[56] = sig_6309;
   Z[57] = sig_6316;
   Z[58] = sig_6323;
   Z[59] = sig_6336;
   Z[60] = sig_6333;
   Z[61] = sig_6335;
   Z[62] = sig_6338;
   Z[63] = sig_6340;

  return ((Z[0] & 0x01) << 0) | ((Z[1] & 0x01) << 1) | ((Z[2] & 0x01) << 2) | ((Z[3] & 0x01) << 3) | ((Z[4] & 0x01) << 4) | ((Z[5] & 0x01) << 5) | ((Z[6] & 0x01) << 6) | ((Z[7] & 0x01) << 7) | ((Z[8] & 0x01) << 8) | ((Z[9] & 0x01) << 9) | ((Z[10] & 0x01) << 10) | ((Z[11] & 0x01) << 11) | ((Z[12] & 0x01) << 12) | ((Z[13] & 0x01) << 13) | ((Z[14] & 0x01) << 14) | ((Z[15] & 0x01) << 15) | ((Z[16] & 0x01) << 16) | ((Z[17] & 0x01) << 17) | ((Z[18] & 0x01) << 18) | ((Z[19] & 0x01) << 19) | ((Z[20] & 0x01) << 20) | ((Z[21] & 0x01) << 21) | ((Z[22] & 0x01) << 22) | ((Z[23] & 0x01) << 23) | ((Z[24] & 0x01) << 24) | ((Z[25] & 0x01) << 25) | ((Z[26] & 0x01) << 26) | ((Z[27] & 0x01) << 27) | ((Z[28] & 0x01) << 28) | ((Z[29] & 0x01) << 29) | ((Z[30] & 0x01) << 30) | ((Z[31] & 0x01) << 31) | ((Z[32] & 0x01) << 32) | ((Z[33] & 0x01) << 33) | ((Z[34] & 0x01) << 34) | ((Z[35] & 0x01) << 35) | ((Z[36] & 0x01) << 36) | ((Z[37] & 0x01) << 37) | ((Z[38] & 0x01) << 38) | ((Z[39] & 0x01) << 39) | ((Z[40] & 0x01) << 40) | ((Z[41] & 0x01) << 41) | ((Z[42] & 0x01) << 42) | ((Z[43] & 0x01) << 43) | ((Z[44] & 0x01) << 44) | ((Z[45] & 0x01) << 45) | ((Z[46] & 0x01) << 46) | ((Z[47] & 0x01) << 47) | ((Z[48] & 0x01) << 48) | ((Z[49] & 0x01) << 49) | ((Z[50] & 0x01) << 50) | ((Z[51] & 0x01) << 51) | ((Z[52] & 0x01) << 52) | ((Z[53] & 0x01) << 53) | ((Z[54] & 0x01) << 54) | ((Z[55] & 0x01) << 55) | ((Z[56] & 0x01) << 56) | ((Z[57] & 0x01) << 57) | ((Z[58] & 0x01) << 58) | ((Z[59] & 0x01) << 59) | ((Z[60] & 0x01) << 60) | ((Z[61] & 0x01) << 61) | ((Z[62] & 0x01) << 62) | ((Z[63] & 0x01) << 63);
}
