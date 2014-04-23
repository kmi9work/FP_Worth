#ifdef FPTREE

#include "fptree.h"
#include <iostream>
#include <iomanip>

FPTree::FPTree()
{
    data.number = -1;
    data.support = -1;
    father = NULL;
    str_numbers.clear();
}

bool bySupport(const struct term &p1, const struct term &p2){
    return p1.support > p2.support;
}

int FPTree::addChild(struct term d, FPTree** ret){
    int i;
    for (i = 0; i < children.size(); i++){
        //Find child with the same number and add support
        if (children[i]->data.number == d.number){
            children[i]->data.support += d.support;
            children[i]->str_numbers.append(d.str_num);
            *ret = children[i];

            return 0;
        }
    }
    //If there is no children. Add new child
    *ret = new FPTree();
    (*ret)->data = d;
    (*ret)->str_numbers.append(d.str_num);
    (*ret)->father = this;
    children.append(*ret);
    return 1;
}

void FPTree::clear(){
    int i;
    if (children.empty()){
        return;
    }else{
        for (i = 0; i < children.size(); i++){
            if (children[i]->children.empty()){
                delete children[i];
                children.removeAt(i);
            }else{
                children[i]->clear();
            }
        }
    }
    data.number = -1;
    data.support = -1;
    father = NULL;
}

void FPTree::printTree()
{
    int i;
    std::cout << data.number << " <--" << std::endl;
    for (i = 0; i < children.size(); i++){
        std::cout << children[i]->data.number << "," << children[i]->data.support << " ";
    }
    std::cout << std::endl;
    std::cout << "===" << std::endl;
    for (i = 0; i < children.size(); i++){
        children[i]->printTree();
    }
}

QVector<struct pattern> FPTree::findFP(QVector<struct level> levels, QVector<double> deltas, int rows){
// deltas?
    QVector<struct pattern> ret;
    QVector<struct pattern> patterns;
    QVector<FPTree*> nodes;
    struct pattern buf_pattern;

    FPTree *conTree;
    QVector<struct level> condLevels;
    int i,j;
    if (levels.size() == 1){
        for (i = 0; i < levels[0].nodes.size(); i++){
            buf_pattern.word.clear();
            buf_pattern.str_numbers.clear();
            buf_pattern.word.append(levels[0].nodes[i]->data);
            buf_pattern.count = rows;
            buf_pattern.str_numbers.append(levels[0].nodes[i]->data.str_num);
            buf_pattern.support = levels[0].nodes[i]->data.support;
            ret.append(buf_pattern);
        }
        return ret;
    }
    if (levels.size() < 1){
        std::cout << "SHOULD NOT PRINTS!" << std::endl;
        return ret;
    }

    for (i = levels.size()-1; i >= 0; i--){
        if (!levels[i].nodes.empty()){
            condLevels.clear();
            conTree = makeConTree(levels[i].nodes, &condLevels);

            patterns.clear();
            patterns = conTree->findFP(condLevels, deltas, rows);
            for (j = 0; j < patterns.size(); j++){
                patterns[j].word.prepend(levels[i].nodes[j]->data);
                ret.append(patterns[j]);
            }
        }
    }
    return ret;
}



FPTree* FPTree::makeConTree(QVector<FPTree*> nodes, QVector<struct level> *levels){
    QLinkedList<struct term> word;
    QLinkedList<struct term>::iterator iterator;
    QVector< QLinkedList<struct term> > words;
    FPTree *node, *node1;
    FPTree *conTree;
    QHash<int, int> terms; // <number, support>
    struct level buf_level;
    int check, fl;
    int minsupp = 0;
    words.reserve(nodes.size());
    int i,l;
    for (i = 0; i < nodes.size(); i++){
        if (nodes[i]->father != NULL && nodes[i]->data.support > 0){
            node = nodes[i]->father;
        }else{
            continue;
        }
        for (; node->father != NULL; node = node->father){
            if (terms.contains(node->data.number)){
                terms[node->data.number] += node->data.support;
            }else{
                terms[node->data.number] = node->data.support;
            }
        }
    }
    for (i = 0; i < nodes.size(); i++){
        if (nodes[i]->father != NULL && nodes[i]->data.support > 0){
            node1 = nodes[i]->father;
            minsupp = node->data.support;
        }else{
            continue;
        }
        while (node1->father != NULL){
            node = node1;
            if (word.empty()){
                word.append(node->data);
            }else{
                for(iterator = word.begin(); iterator != word.end(); iterator++){
                    if (terms[iterator->number] < terms[node->data.number]){
                        word.insert(iterator, node->data);
                        break;
                    }
                }
                if (iterator == word.end()){
                    word.append(node->data);
                }
            }
            node1 = node->father;
            node->data.support -= minsupp;
        }
        words.append(word);
        word.clear();
    }
    conTree = new FPTree();
    for (i = 0; i < words.size(); i++){
        node = conTree;
        for(iterator = words[i].begin(); iterator != words[i].end(); iterator++){
            check = node->addChild(*iterator, &node);
            if (check == 1){
                fl = 0;
                for (l = 0; l < levels->size(); l++){
                    if ((*levels)[l].number == node->data.number){
                        (*levels)[l].nodes.append(node);
                        fl = 1;
                        break;
                    }
                }
                if (!fl){
                    buf_level.number = node->data.number;
                    buf_level.nodes.append(node);
                    levels->append(buf_level);
                    buf_level.nodes.clear();
                }
            }
        }
    }
    return conTree;
}

/*
 * QVector<struct pattern> FPTree::finderFP(QVector<struct level> levels, int rows){
    QVector<struct level> newLevels;
    QVector<struct pattern> ret;
    QVector<struct pattern> buf_pattern_vec;
    struct pattern buf_pattern;
    FPTree *tree;
    FPTree *node;
    QVector<FPTree*> term_nodes_new;
    if (term_nodes.size() == 1){
        buf_pattern.count = rows;
        buf_pattern.support = term_nodes[0]->data.support;
        for (node = term_nodes[0]; node->father != NULL; node = node->father){
            buf_pattern.word.append(node->data);
            buf_pattern.str_numbers.append(node->data.str_num);
        }
        ret.append(buf_pattern);
        return ret;
    }else{

        tree = makeConTree(term_nodes, newLevels);
        buf_pattern_vec = tree->finderFP(newLevels, rows);
        for (i = 0; i < buf_pattern_vec; i ++){
            ret.append();
        }
    }
}

QVector<struct pattern> FPTree::makeFP(QVector< QVector<FPTree*> > levels,
                                QVector<double> deltas,
                                int rows)
{
    int i,j;
    QVector<struct pattern> ret;
    struct pattern buf_pattern;
    FPTree *node, *node1;
    double delta_i;
    for (i = levels.size() - 1; i >= 0; i--){
        if (i >= deltas.size() - 1){
            delta_i = deltas.size() - 1;
        }else{
            delta_i = i;
        }
        for (j = 0; j < levels[i].children.size(); j++){
            buf_pattern.word.clear();

            node = levels[i].children[j];
            buf_pattern.support = node->data.support;
            buf_pattern.count = rows;
            buf_pattern.str_numbers = node->str_numbers;
            buf_pattern.word.clear();
            if ((double)buf_pattern.support/buf_pattern.count > deltas[delta_i] && buf_pattern.support > 1){
                buf_pattern.word.append(node->data);
                node1 = node->father;

                while (node1->father != NULL){
                    buf_pattern.word.prepend(node1->data); // SLOOOOOOOWWW  prepend !!!
                    node1 = node1->father;
                }
                ret.append(buf_pattern);
            }
        }
    }
    return ret;
    /*QVector<CandidateTree *> bufVect;
    if (!children.empty()){
        for (i = 0; i < children.size(); i++){
            bufVect.append(children[i]);
            children[i]->assocRules(candidate + bufVect, candidates);
            bufVect.clear();
        }
    }else{
        candidates += candidate;
    }
}
*/
void FPTree::postorder(QPlainTextEdit *edit, int indent = 0){
    int i;
    if(!children.empty()) {
        for (i = 0; i < children.size(); i++){
            children[i]->postorder(edit, indent + children.size());
        }
        if (indent) {
            edit->insertPlainText(QString("%1, %2\n").arg(data.number, -indent).arg(data.support, -indent));
        }else{
            edit->insertPlainText(QString("%1, %2\n").arg(data.number).arg(data.support));
        }
    }
}



#endif
