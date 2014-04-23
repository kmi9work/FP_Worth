#ifdef FPTREE
#ifndef FPTREE_H
#define FPTREE_H

#include <iostream>
#include <QVector>
#include <QPlainTextEdit>
#include <QString>
#include <QLinkedList>
#include <QHash>

#include "fpworth.h"

class FPTree;
struct level;



class FPTree
{
public:
    QVector<FPTree*> children;
    FPTree *father;
    struct term data;
    FPTree();
    QVector<int> str_numbers;
    QVector<struct pattern> makeFP(QVector< QVector<FPTree*> > levels,
                            QVector<double> deltas,
                            int rows);
    int addChild(struct term d, FPTree **ret);
    void printTree();
    void clear();
    void postorder(QPlainTextEdit *edit, int indent);
    QVector<struct pattern> findFP(QVector<struct level> levels, QVector<double> deltas, int rows);
    QVector<struct pattern> finderFP(QVector<struct level> levels, int rows);
private:
    FPTree *makeConTree(QVector<FPTree *> nodes, QVector<struct level> *levels);
};



#endif // FPTREE_H
#endif
