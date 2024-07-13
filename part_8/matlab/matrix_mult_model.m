k = [8, 8, 5, 5;
     8, 5, 8, 5]
wb2 = [1.37, 1.37, -19.88;
       0.77, 0.97,  -0.90;
       1.05, 0.64,  -0.89]
wb3 = [ 7.11, -1.31, 0.08, -2.59;
       -7.10,  1.63, 1.97,  0.20]

k_padded = cat(1, k, ones(1, 4))

z2 = wb2 * k_padded
a2 = 1./(1+exp(-z2))

a2_padded = cat(1, a2, ones(1, 4))

z3 = wb3 * a2_padded	
a3 = 1./(1+exp(-z3))

a3_rounded = round(a3)