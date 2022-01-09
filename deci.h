#ifndef _DECI_H_
#define _DECI_H_

#include "basis_pms.h"
#include <algorithm>

using namespace std;

class Decimation
{
  private:
    int *hardunsat_stack = nullptr;		   //store the unsat clause number
    int *index_in_hardunsat_stack = nullptr; //which position is a clause in the unsat_stack
    int hardunsat_stack_fill_pointer = 0;

  public:
    Decimation(lit **ls_var_lit, int *ls_var_lit_count, lit **ls_clause_lit, long long *ls_org_clause_weight, long long ls_top_clause_weight);

    void make_space(int max_c, int max_v);
    void free_memory();
    void init(int *ls_local_opt, int *ls_global_opt, lit *ls_unit_clause, int ls_unit_clause_count, const int *ls_clause_lit_count, int ls_solu_feasible);
    void push_unit_clause_to_queue(lit tem_l);
    void assign(int v, int sense);
    void remove_unassigned_var(int v);
    void hunit_propagation();
    void sunit_propagation();
    void hard_random_propagation();
    void unit_prosess();

    vector<int> fix;

    int num_vars;
    int num_clauses;

    long long *h_true_score;
    long long *h_false_score;
    long long *hscore;
    long long *s_true_score;
    long long *s_false_score;
    long long *sscore;

    lit **clause_lit;
    lit **var_lit;
    int *var_lit_count;

    int *local_opt;
    int *global_opt;
    long long *org_clause_weight;
    long long top_clause_weight;

    lit *hunit_clause_queue;
    int *sense_in_hunit_clause_queue;
    int hunit_beg_pointer;
    int hunit_end_pointer;

    lit *sunit_clause_queue;
    int *sense_in_sunit_clause_queue;
    int sunit_beg_pointer;
    int sunit_end_pointer;

    int *unassigned_var;
    int *index_in_unassigned_var;
    int unassigned_var_count;

    int *clause_delete;
    int *clause_lit_count;

    int solu_feasible;

    void random_propagation();
};

Decimation::Decimation(lit **ls_var_lit, int *ls_var_lit_count, lit **ls_clause_lit, long long *ls_org_clause_weight, long long ls_top_clause_weight)
{
    var_lit = ls_var_lit;
    var_lit_count = ls_var_lit_count;
    clause_lit = ls_clause_lit;
    org_clause_weight = ls_org_clause_weight;
    top_clause_weight = ls_top_clause_weight;
}

void Decimation::make_space(int max_c, int max_v)
{
    num_vars = max_v;
    num_clauses = max_c;

    max_c += 10;
    max_v += 10;

    h_true_score = new long long[max_v];
    h_false_score = new long long[max_v];
    hscore = new long long[max_v];
    s_true_score = new long long[max_v];
    s_false_score = new long long[max_v];
    sscore = new long long[max_v];

    fix.resize(max_v);
    fix.reserve(max_v);

    hunit_clause_queue = new lit[max_v];
    sense_in_hunit_clause_queue = new int[max_v];

    sunit_clause_queue = new lit[max_v];
    sense_in_sunit_clause_queue = new int[max_v];

    unassigned_var = new int[max_v];
    index_in_unassigned_var = new int[max_v];

    clause_delete = new int[max_c];
    clause_lit_count = new int[max_c];

    hardunsat_stack = new int[max_c];
    index_in_hardunsat_stack = new int[max_c];

}

void Decimation::free_memory()
{
    fix.clear();
    delete[] h_true_score;
    delete[] h_false_score;
    delete[] hscore;
    delete[] s_true_score;
    delete[] s_false_score;
    delete[] sscore;

    delete[] hunit_clause_queue;
    delete[] sense_in_hunit_clause_queue;

    delete[] sunit_clause_queue;
    delete[] sense_in_sunit_clause_queue;

    delete[] unassigned_var;
    delete[] index_in_unassigned_var;

    delete[] clause_delete;
    delete[] clause_lit_count;

    delete[] hardunsat_stack;
    delete[] index_in_hardunsat_stack;

}

void Decimation::init(int *ls_local_opt, int *ls_global_opt, lit *ls_unit_clause, int ls_unit_clause_count, const int *ls_clause_lit_count, int ls_solu_feasible)
{
    int v;
    int c;
    //parameters used in decimation
    hunit_beg_pointer = 0;
    hunit_end_pointer = 0;

    sunit_beg_pointer = 0;
    sunit_end_pointer = 0;

    unassigned_var_count = num_vars;
    hardunsat_stack_fill_pointer = 0;

    //data structure of the instance
    local_opt = ls_local_opt;
    global_opt = ls_global_opt;

    solu_feasible = ls_solu_feasible;

    for (int i = 0; i < num_vars; ++i)
    {
        v = i + 1;
        unassigned_var[i] = v;
        index_in_unassigned_var[v] = i;

        fix[v] = -1;
        sense_in_hunit_clause_queue[v] = -1; // not in hunit queue
        sense_in_sunit_clause_queue[v] = -1; // not in sunit queue
    }

    for (int i = 0; i < num_clauses; ++i)
    {
        clause_lit_count[i] = ls_clause_lit_count[i];
        clause_delete[i] = 0;
        // 维护 unsat clause info
        if (org_clause_weight[i] == top_clause_weight)
        {
            hardunsat_stack[hardunsat_stack_fill_pointer] = i;
            index_in_hardunsat_stack[i] = hardunsat_stack_fill_pointer;
            hardunsat_stack_fill_pointer++;
        }
    }

    for (int i = 0; i < ls_unit_clause_count; ++i)
    {
        push_unit_clause_to_queue(ls_unit_clause[i]);
    }

    //figout score
    for (v = 1; v <= num_vars; ++v)
    {
        h_false_score[v] = 0;
        h_true_score[v] = 0;
        s_false_score[v] = 0;
        s_true_score[v] = 0;
        for (int i = 0; i < var_lit_count[v]; ++i)
        {
            c = var_lit[v][i].clause_num;
            if (org_clause_weight[c] == top_clause_weight)
            {
                if (var_lit[v][i].sense == 1)
                    ++h_true_score[v];
                else
                    ++h_false_score[v];
            }
            else
            {
                if (var_lit[v][i].sense == 1)
                    s_true_score[v] += (org_clause_weight[c] + basis_clause_weight[c]);
                else
                    s_false_score[v] += (org_clause_weight[c] + basis_clause_weight[c]);
            }
        }
        hscore[v] = max(h_false_score[v], h_true_score[v]);
        sscore[v] = max(s_false_score[v], s_true_score[v]);
    }
}

void Decimation::push_unit_clause_to_queue(lit tem_l)
{
    int v = tem_l.var_num;
    int c = tem_l.clause_num;
    if (org_clause_weight[c] == top_clause_weight)
    {
        if (sense_in_hunit_clause_queue[v] == -1)
        {
            sense_in_hunit_clause_queue[v] = tem_l.sense;
            hunit_clause_queue[hunit_end_pointer++] = tem_l;
        }
        else
        {
            if (sense_in_hunit_clause_queue[v] != tem_l.sense) //conflict var in hard unit queue
            {
                sense_in_hunit_clause_queue[v] = -2; //means this variable is conflict in hard unit queue
            }
        }
    }
    else
    {
        if (sense_in_hunit_clause_queue[v] != -1)
            return; //be defined by hard unit queue

        if (sense_in_sunit_clause_queue[v] == -1)
        {
            sense_in_sunit_clause_queue[v] = tem_l.sense;
            sunit_clause_queue[sunit_end_pointer++] = tem_l;
        }
        else
        {
            if (sense_in_sunit_clause_queue[v] != tem_l.sense) //conflict var in hard unit queue
            {
                sense_in_sunit_clause_queue[v] = -3; //means this variable is conflict in hard unit queue
            }
        }
    }
}

void Decimation::remove_unassigned_var(int v)
{
    int index = index_in_unassigned_var[v];
    int last_var = unassigned_var[--unassigned_var_count];
    unassigned_var[index] = last_var;
    index_in_unassigned_var[last_var] = index;
}

void Decimation::assign(int v, int sense)
{
    int c, l;
    lit tem_lit;
    fix[v] = sense;
    remove_unassigned_var(v);

    for (int i = 0; i < var_lit_count[v]; ++i)
    {
        c = var_lit[v][i].clause_num;
        if (clause_delete[c] == 1)
            continue;

        if (sense == var_lit[v][i].sense)
        {
            clause_delete[c] = 1;
            if (org_clause_weight[c] == top_clause_weight)
            {
                // 维护信息
                if (fix[v] == -1) {
                    int last_unsat_clause = hardunsat_stack[--hardunsat_stack_fill_pointer];  // 队尾的c
                    int index = index_in_hardunsat_stack[c];  // 当前c的index
                    hardunsat_stack[index] = last_unsat_clause;
                    index_in_hardunsat_stack[last_unsat_clause] = index;
                }

                for (int j = 0; j < clause_lit_count[c]; j++)
                {
                    tem_lit = clause_lit[c][j];
                    if (tem_lit.sense == 1)
                    {
                        h_true_score[tem_lit.var_num]--;
                    }
                    else
                        h_false_score[tem_lit.var_num]--;
                    hscore[tem_lit.var_num] = max(h_true_score[tem_lit.var_num], h_false_score[tem_lit.var_num]);
                }
            }
            else
            {
                for (int j = 0; j < clause_lit_count[c]; j++)
                {
                    tem_lit = clause_lit[c][j];
                    if (tem_lit.sense == 1)
                    {
                        s_true_score[tem_lit.var_num] -= org_clause_weight[c];
                    }
                    else
                        s_false_score[tem_lit.var_num] -= org_clause_weight[c];
                    sscore[tem_lit.var_num] = max(s_true_score[tem_lit.var_num], s_false_score[tem_lit.var_num]);
                }
            }
            continue;
        }

        for (int j = 0; j < clause_lit_count[c]; j++)
        {
            if (clause_lit[c][j].var_num == v)
            {
                swap(clause_lit[c][j], clause_lit[c][--clause_lit_count[c]]);
                break;
            }
        }
        if (clause_lit_count[c] == 1)
        {
            push_unit_clause_to_queue(clause_lit[c][0]);
        }
    }
}

void Decimation::hunit_propagation()
{
    int v, c, sense;

    v = hunit_clause_queue[hunit_beg_pointer].var_num;
    sense = hunit_clause_queue[hunit_beg_pointer].sense;
    hunit_beg_pointer++;

    if (sense_in_hunit_clause_queue[v] == -2)
    {
        if (sscore[v] > 0)
        {
            if (sscore[v] == s_true_score[v])
                sense = 1;
            else
                sense = 0;
        }
//        else {
//            sense = solu_feasible==0 ? rand() % 2 : global_opt[v];
//        }
//        else if (solu_feasible != 0) {
//            sense = global_opt[v];
//        }
        else {
            sense = global_opt[v];
        }
    }

    assign(v, sense);
}

void Decimation::sunit_propagation()
{
    int v, c, sense, rd;

    int ht;
    ht = 15;

    int best_v = sunit_clause_queue[sunit_beg_pointer].var_num;
    c = sunit_clause_queue[sunit_beg_pointer].clause_num;
    int best_score = sscore[best_v];
    int index = sunit_beg_pointer;
    int count = sunit_end_pointer - sunit_beg_pointer;
    if (count > ht)
    {
        for (int i = 0; i < ht; ++i)
        {
            rd = rand() % count;

            v = sunit_clause_queue[sunit_beg_pointer + rd].var_num;
            if (sscore[v] > best_score)
            {
                best_v = v;
                index = sunit_beg_pointer + rd;
            }
        }
    }
    else
    {
        for (int i = sunit_beg_pointer; i < sunit_end_pointer; ++i)
        {
            v = sunit_clause_queue[i].var_num;
            if (sscore[v] > best_score)
            {
                best_v = v;
                index = i;
            }
        }
    }
    swap(sunit_clause_queue[sunit_beg_pointer], sunit_clause_queue[index]);
    v = sunit_clause_queue[sunit_beg_pointer].var_num;
    sense = sunit_clause_queue[sunit_beg_pointer].sense;
    sunit_beg_pointer++;

    if (fix[v] != -1)
        return;

    if (sense_in_sunit_clause_queue[v] == -3)
    {
        sense = rand() % 2;
    }
    assign(v, sense);
}

void Decimation::hard_random_propagation()  // 不贪心hscore，选出falsified hard clause从头找未赋值的弄真 3.1.7，之前的是找出所有var中最大hscore的，现在是找到第一个unassigned var
{
    int c = hardunsat_stack[rand() % hardunsat_stack_fill_pointer];  // 初始解随机性来源
    int v = clause_lit[c][0].var_num;
    //int sense = hscore[v]==h_true_score[v] ? 1 : 0;
    int sense = clause_lit[c][v].sense;
    int temp_var;

    if (fix[v] != -1) {
        for (int i = 1; i < clause_lit_count[c]; ++i) {
            temp_var = clause_lit[c][i].var_num;
            if (fix[temp_var] == -1) {
                v = temp_var;
                //sense = hscore[v]==h_true_score[v] ? 1 : 0;
                sense = clause_lit[c][v].sense;
                break;
            }
        }
    }

    // 全都赋值了
    if (fix[v] != -1) {
        // 维护信息
        int last_unsat_clause = hardunsat_stack[--hardunsat_stack_fill_pointer];  // 队尾的c
        int index = index_in_hardunsat_stack[c];  // 当前c的index
        hardunsat_stack[index] = last_unsat_clause;
        index_in_hardunsat_stack[last_unsat_clause] = index;
        return;
    }

    assign(v, sense);
}


void Decimation::random_propagation() {
    int v, sense;
    v = unassigned_var[rand() % unassigned_var_count];
//    if (hscore[v] > 0) {
//        sense = hscore[v] == h_true_score[v] ? 1 : 0;      // for_fun.2使用的方式
//    }
    //else sense = solu_feasible==0 ? rand() % 2 : global_opt[v];
    sense = rand() % 2;      // hrp-f使用的方式 forfun  forfun.1
    assign(v, sense);
}

void Decimation::unit_prosess()
{
    while (unassigned_var_count > 0)
    {
        if (hunit_beg_pointer != hunit_end_pointer)
        {
            hunit_propagation();
        }
        else if (sunit_beg_pointer != sunit_end_pointer)
        {
            sunit_propagation();
        }
        else if (hardunsat_stack_fill_pointer > 0)
        {
            hard_random_propagation();
        }
        else
        {
            random_propagation();
        }
    }
}

#endif
