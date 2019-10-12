# DPLLTsetin

(p → q) ↔ (¬q → ¬p)  
input:  
((1)>(2))~((-2)>(-1))#  
output:  
 1,2 satisfied  
 1,-2 satisfied  
-1,2 satisfied  
-1,-2 satisfied  
This fomula is valid  


(p → (q → r)) ↔ (¬r → (¬q → ¬p))  
input:  
((1)>((2)>(3)))~((-3)>((-2)>(-1)))#  
output:  
Introduce a new variables for the formula:  
4~-23  
Introduce a new variables for the formula:  
5~2-1  
 1, 2, 3, 4,5 satisfied  
 1,-2, 3, 4,-5 satisfied  
-1, 2, 3, 4,5 satisfied  
-1, 2,-3,-4,5 satisfied  
-1,-2, 3, 4,5 satisfied  
-1,-2,-3, 4,5 satisfied  
This fomula is satisfiabled  


¬(¬(p ∧ q) → ¬r)  
input:  
-((-1-2)>(-3))#  
output:  
Introduce a new variable for the subformula:  
4~-1-2  
This fomula is unsatisfiable  
