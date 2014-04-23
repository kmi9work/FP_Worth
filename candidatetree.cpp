#include "candidatetree.h"
#define min(a,b) ((a)<(b) ? (a) : (b))

QVector< QVector<CandidateTree*> > CandidateTree::levels;

CandidateTree::CandidateTree(QVector<double> ds, int rs)
{
    father = NULL;
    data.lp_number = -1;
    data.term_number = -1;
    data.support = -1;
    level = -1;
    deltas = ds;
    rows = rs;
}

void CandidateTree::addChild(struct term d, int supp, QVector<int> str_nums)
{
    CandidateTree *ct;// String Numbers!!!
    if (level == -1){
        if ((double)d.support / rows > deltas[0]){
            ct = new CandidateTree(deltas, rows);
            ct->father = this;
            ct->data = d;
            ct->level = level + 1;
            ct->support = d.support;
            ct->string_numbers = str_nums;
            children.append(ct);
            if (levels.size() < ct->level + 1){
                levels.resize(ct->level + 1);
            }
            levels[ct->level].append(ct);
        }
    }else{//if ((double)min(support, d.support)/rows > deltas[min(level + 1, deltas.size() - 1)]){
        ct = new CandidateTree(deltas, rows);
        ct->father = this;
        ct->data = d;
        ct->level = level + 1;
        ct->support = supp;
        ct->string_numbers = str_nums;
        children.append(ct);
        if (levels.size() < ct->level + 1){
            levels.resize(ct->level + 1);
        }
        levels[ct->level].append(ct);
    }
}


void CandidateTree::makeTree(QVector< QVector<int> > numbers, int rows)

{
    int i, j, k, l;
    CandidateTree *node;
    QVector<struct term> currentTerms;
    QVector<int> str_nums;
    int currentSupp;
    int fl;

    for (i = 0; i < children.size() - 1; i++){
        for (j = i + 1; j < children.size(); j++){// ???
            currentTerms.clear();
            str_nums.clear();
            currentSupp = 0;
            currentTerms.reserve(children[j]->level + 1);
            str_nums.reserve(children[j]->level + 1);
            currentTerms.append(children[j]->data);
            for (node = children[i]; node->father != NULL; node = node->father){
                currentTerms.append(node->data);
            }
            for (l = 0; l < rows; l++){
                fl = 1;
                for (k = 0; k < currentTerms.size(); k++){
                    if (numbers[l][currentTerms[k].lp_number] != currentTerms[k].term_number){
                        fl = 0;
                        break;
                        // String numbers maybe here
                    }
                }
                if (fl == 1){
                    currentSupp += 1;
                    str_nums.append(l);
                }
            }
            if ((double)currentSupp / rows > deltas[min(currentTerms.size() - 1, deltas.size() - 1)]){
                children[i]->addChild(children[j]->data, currentSupp, str_nums);
            }
        }
        children[i]->makeTree(numbers, rows);
    }
}

QVector<struct pattern> CandidateTree::assocRules(int first, int last, int step)
{
    int i, j;
    CandidateTree *node;
    struct pattern buf_pattern;
    QVector<struct pattern> ret;
    if (last < first) return ret;
    for (i = first - 1; i < min(last,levels.size()); i++){
        for (j = 0; j < levels[i].size(); j++){
            node = levels[i][j];
            buf_pattern.count = rows;
            buf_pattern.support = node->support;
            buf_pattern.word.clear();
            buf_pattern.str_numbers.clear();
            buf_pattern.str_numbers = node->string_numbers;
            buf_pattern.cluster = step;
            for (; node->father != NULL; node = node->father){
                buf_pattern.word.prepend(node->data);
                //buf_pattern.str_numbers.append(node->data.str_num);// ?
            }
            ret.append(buf_pattern);
        }
    }
    return ret;
}

void CandidateTree::clear(){
    int i;
    for (i = 0; i < children.size(); i++){
        children[i]->clear();
    }
    children.clear();
}

void CandidateTree::printTree()
{
    int i;

    std::cout << data.lp_number << " <-- level: " << level + 1 << std::endl;
    for (i = 0; i < children.size(); i++){
        std::cout << children[i]->data.lp_number << "," << children[i]->data.support << " | ";
    }
    std::cout << std::endl;
    std::cout << "===" << std::endl;
    for (i = 0; i < children.size(); i++){
        children[i]->printTree();
    }
}


