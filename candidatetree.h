#ifndef CANDIDATETREE_H
#define CANDIDATETREE_H
#include <QVector>
#include "fpworth.h"
#include "structs.h"
#include <iostream>

struct term;

class CandidateTree
{
public:
    CandidateTree(QVector<double> ds, int rs, int lp_count);
    struct term data;
    int support; // Поддержка
    int rows;
    double conf; // Достоверность
    CandidateTree *father;
    QVector<CandidateTree*> children;
    QVector<int> string_numbers;
    int addChild(struct term d, int supp, QVector<int> str_nums);

    int makeTree(QVector<QVector<numCluster> > data, int rows, int cols);
    QVector<pattern> assocRules(int first, int last);
    QVector<double> deltas;
    int level;
    void printTree();
    static QVector< QVector<CandidateTree*> > levels;
    void clear();
private:
    //double
};

#endif // CANDIDATETREE_H
