Reviewer: 1

Comments to the Author
------------------
Summary:
------------------

The paper is very interesting. Integrating a fuzzer into S2E is a rather natural choice, given the potential shown in other symbolic execution engines (e.g., Mayhem and angr). In this paper, the authors present BREACHER, a system that combines S2E and AFL. 

The main contributions are: 

1) lazy concretization of symbolic pointers: whenever a symbolic memory access is evaluated by the symbolic engine, a hard constraint describing the assumption on the memory address is added to the path constraints, while a forked state with the opposite condition set as soft constraint is added to the pending queue. If a branch condition is not satisfied due to a hard constraint, their algorithm can lazily generate a new state where the branch condition is satisfied.

2) symbolic loop bucket: unconstrained loops are unrolled following a strategy similar to the loop bucket mechanism employed in AFL.

3) a seed selection method based on distance metrics: a fuzzer has to select a candidate from a seed queue; the authors propose few simple metrics to pick the next seed sample; the main goal of their selection methods is to maximize the branch coverage. 

--------------------
Evaluation:
--------------------

The authors should revise the paper accordingly to the following comments (each comment is marked with the severity):

1) [minor] Section 2.1, paragraph "Consider a program P [...]": I find the phrase "a program state concentrates the value of each variable" a bit unintuitive if you are not familiar with the concept of symbolic execution. Maybe you can rephrase it.

2) [minor] Section 2.1, paragraph "Dynamic symbolic execution": the "<" and ">" in the state could be formatted in more nice way, (see e.g., [1]).

3) [minor] Section 2.2, paragraph "However, function ": "as line 9" => "at line 9"?

4) [minor] Section 2.2: the statement "function path_explosion prevents symbolic execution from solving the conditional instruction" is not always true, it depends on the exploration strategy (e.g., BFS vs DFS). You should make the statement less "bold" or refine it.

5) [major] Section 3.1: there is confusion between a symbolic address and the (symbolic) content at an address. In your example, for instance: (a) the address buf[i] is constant (assuming the address of buf is known; i is constant given a specific loop iteration), (b) the value buf[i] is symbolic, (c) the address text_chars[buf[i]] is symbolic, (d) chosen a concretization for text_chars[buf[i]], its content is constant.

6) [major] Section 3.1: the discussion of fully symbolic memories is inaccurate. If you take your example and run it under KLEE (see [2]), it will find the triggering input and generates only a linear (and not exponential) number of states. KLEE does not fork different states for each address value within the same object (e.g., an array): if you have a fully unconstrained pointer, then KLEE generates N states (one for each allocated object) + one out-of-bound error state. Within each object, theory of arrays is used to reason on memory accesses. angr with option SYMBOLIC_WRITE_ADDRESSES enabled (i.e., read/write partial memory model) can handle symbolic pointers within a range up to 128 distinct write addresses (and up to 1024 distinct read addresses) by exploiting ITE expressions without state forking. A recent work [3] allows angr to deal with even more unconstrained pointers. Both solutions (KLEE with theory of arrays and angr with ITE expressions) move the burden from the symbolic engine to the constraint solver. Also the paper from Trtik does not fork for each address value (but it crucially depends on type information).
I am not familiar with KLEE inside S2E, but I suppose that since S2E is analyzing binaries and performing lifting from x86 to LLVM IR, the object size could be not recoverable for vector/struct/etc objects and thus the number of objects increases since each memory access must be handled as potentially referring many different scalar objects. This could explain why your discussion is considering the worst case scenario for KLEE (where for each value you have to fork a new state). If my intuition is right, then please clearly state that you analyzing binaries and this makes harder for S2E to deal with memory accesses. Notice that angr (and thus [3]) works on binaries as well and does not exploit type information on objects. If I am wrong, please explain the reasons and why your example runs nicely under KLEE. In any case, the discussion of fully symbolic memories in Section 3.1 must be revised.

7) [major] Section 3.1: the semantic of "condition = EQ(e_addr, M(e_addr))" should be defined more formally.

8) [minor] Section 3.1, algorithm 1, line 3: the condition could be rewritten in a more intuitive / high level way.

9) [major] Section 3.1, algorithm 1: some relevant details are not explained. In particular:

 - how do you track hard constraints and soft constraints in the state? refer to the definition of the symbolic state
 - can algorithm 1 handle more than one hard constraint? If C_H can be a set, we should still use it at line 6? Or should we consider only the specific hard constraints that is related to C_F?
 - consider this snippet of code:

    // i is symbolic and can have solutions {0, 1}
    int a[2] = {0};
    int b = 0;
    int r = a[i];
    if (r == 5) b = 1;
    if (b == 1) error(); // target

   how do you track the relation between b, r and i? Details are needed if someone wants to implement your idea. I am not asking to include this example, but only to improve your discussion to cover my doubts on this example.

 - consider this snippet of code:
    
    // i is symbolic and can have solutions {0, 1}
    int a[2] = {6, 9};
    int a = 0; 
    int k = a[i]; // in the concrete execution: i is 0, hence k is 6
    if (k % 3 == 0) a = 1; // assume that the solver can handle modulo operations
    if (k % 2 == 0) b = 1;
    if (a == 1 && k == 9) error(); // target

    how does your algorithm behave on this code? I am not asking to include this example, but only to improve your discussion to cover my doubts on this example.

 - if the implementation of generateTestCase() is something more complex than just asking a solver to generate a solution given the path constraints in s_tmp, then please add these details to the paper.

10) [minor] Table 2: it's more intuitive if you use 'A' instead of 0x41

11) [minor] Section 4.2: "suppose the corresponding contents a re" => "are"?

12) [major] Section 5: the setup experiment is not discussed in detail. In particular, the authors should indicate which releases have been considered for S2E, KLEE, AFL, Driller/angr, QEMU, LibFuzzer, Vuzzer, LAVA. Please add any detail (e.g., tool setup) that can help the research community to compare with or build on top of your work.  I hope authors plan to release their implementation.
 
13) [major] Section 5: the CommonMB benchmark is not discussed in details. Is it public available? Can you release it for the research community? It's good that you have considered a simple synthetic benchmark to test different tools since this makes it easier to analyze the results. However, if it is not available, then it's hard to understand how relevant this benchmark is.

14) [minor] Table 3: it would be interesting to see the memory peak usage of these tools.

15) [major] Table 4 reports some experiments taken from another paper. Have you executed your experiments using the same setup? Are they comparable?

16) [major] The experimental section is interesting but very chaotic. Many tools are listed (S2E, KLEE, AFL, libFuzzer, VUzzer, Driller. SES, FUZZER) but not all of them are discussed in all the experiments. Can you better motivate your choice? What is the difference between these tools: e.g., libFuzzer vs AFL vs VUzzer? Can you present them in a more organized way?

17) [minor] Table 6 and Table 7 cover different programs. Why?

18) [major] Section 7: add [3] and fix the discussion on symbolic pointers

19) [major] References: some citations are incomplete: e.g., there is no conference name in [40]

20) [minor] Section 3.2: a citation to [4] could be added due to Loop Exhaustion strategy.

21) [minor] Consider to add a citation/discussion to [5]


[1] https://tex.stackexchange.com/questions/47078/difference-between-langle-and
[2] https://pastebin.com/dckiuuUe
[3] Emilio Coppa, Daniele Cono D’Elia, and Camil Demetrescu. 2017. Rethinking pointer reasoning in symbolic execution. In Proceedings of the 32nd IEEE/ACM International Conference on Automated Software Engineering (ASE 2017).
[4] AEG: Automatic Exploit Generation. Thanassis Avgerinos, Sang Kil Cha, Brent Lim Tze Hao and David Brumley.
[5] On Benchmarking the Capability of Symbolic Execution Tools with Logic Bombs. https://arxiv.org/pdf/1712.01674.pdf

Reviewer: 2

Comments to the Author
I enjoyed this paper but I think some more elicitation is required;
1. the name of the tool is named in the Abstract but not introduced in the paper.
2. More on the seeding process- how files are chosen is needed.
3. I presume only Euclidean distance is used but later in the experimental section the there different ones are used so please remove confusion of metrics in section 5. 
4. you note that Breacher does better than other tools, but explain how this is - because of the seeding, the distance metrics used, the files used etc etc. I don't think there is reproducibility here which is a concern, unless there is more detail given.
5. what do you mean by testing expensive code in Section 6.2.1 - is this critical code or do you mean highly used, developed etc etc.
