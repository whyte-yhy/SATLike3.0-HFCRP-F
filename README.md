# SATLike3.0-HRP-F
Fabricate SATLike3.0 with "hard random propagate" and "feadback" scheme. For learning local search method towards MaxSAT solving.


《基于SATLike3.0局部搜索求解器的算法改进》的代码，主要效果可以由构造初始解时优先传播未满足硬子句中未赋值变量实现，
但目前感觉做初始解并不是好的选择，混合求解器能利用问题结构又具备随机性，也许已经是单纯SLS的性能上限了。


里面的策略可以进行轻量级实现，请见仓库HFCRP-C（将在发表学位论文后公开）
