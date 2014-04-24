#ifndef FPWORTH_H
#define FPWORTH_H

#include <iostream>
#include <QMainWindow>
#include <QFile>
#include <QVector>
#include <QVector2D>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QTextStream>
#include <QFileDialog>
#include <QClipboard>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QCompleter>
#include <QMovie>
#include <plot.h>
#include <QtZlib/zlib.h>
//#include "fptree.h"
#include "candidatetree.h"
#include "alglib/dataanalysis.h"
#include "alglib/interpolation.h"
#include "structs.h"


/*class FPTree;

struct level{
    int number;
    QVector<FPTree *> nodes;
};*/


/*
typedef struct{
    double support;
    QVector<int> word;
} pattern;

*/

class Plot;
class CandidateTree;

namespace Ui {
class FPWorth;
}

class FPWorth : public QMainWindow
{
    Q_OBJECT

public:
    explicit FPWorth(QWidget *parent = 0);
    ~FPWorth();

public slots:
    void customMenuRequested(QPoint pos);
private slots:
    void on_openFuzzyButton_clicked();

    //void on_makeRulesButton_clicked();


    void on_makePlotButton_clicked();

    void on_openNormalButton_clicked();

    void ShowContextMenu(const QPoint &pos);
    void on_lvarSettings_entered(const QModelIndex &index);

    void on_makeRulesAprioriButton_clicked();

    void on_okTermButton_clicked();

    void on_rulesListWidget_doubleClicked(const QModelIndex &index);

    void on_clearFuzzyButton_clicked();

    void on_nnpsButton_clicked();

    void on_lvarPlotButton_clicked();

    void on_clearPlotButton_clicked();

    void on_addNormalButton_clicked();

    void on_tableCombo_currentIndexChanged(int index);

    void on_countRulesEdit_returnPressed();


    void on_fire1Box_a_valueChanged(double arg1);

    void on_fire1Box_b_valueChanged(double arg1);

    void on_fire2Box_a_valueChanged(double arg1);

    void on_fire2Box_b_valueChanged(double arg1);

    void on_fire3Box_a_valueChanged(double arg1);

    void on_fire3Box_b_valueChanged(double arg1);

    void on_fire4Box_a_valueChanged(double arg1);

    void on_fire4Box_b_valueChanged(double arg1);

    void on_horizontalSlider_sliderMoved(int position);

    void on_stepSpin_valueChanged(double arg1);

    void on_pushButton_clicked();

   // void on_fuzzyTable_customContextMenuRequested(const QPoint &pos);

    void on_magicButton_clicked();

    void onfinish(QNetworkReply *rep);

    void onfinish2(QNetworkReply *rep);
    void on_cityDataOpen_clicked();

    void on_cityRp5_clicked();

    void on_coordRp5_clicked();


private:
    void printData();
    void splitColumns();
    void makeWords(int **numbers, int rows, int k);
    void makeTree(int rows);
    void makeTerms(int **numbers, int rows, int k);
    void makeRules(int rows, int f);

    int cols; // number of lvars without Fires
    int rows;

    //QVector< QVector< QVector<int> > > numbers; // Обыкновенные числа
    //QVector< QVector<int> > string_numbers;

    int global_i;
    int word_count;
    int maxWordSize;
    int *fires;
    QStringList names; // Имена термов
    int first_level, last_level; // уровни, которые нужно вывести
    int countRules; // Количество правил для вывода


    Ui::FPWorth *ui;
    //FPTree *rootFPTree;
    CandidateTree *rootCTree;
    //QVector<struct level> levels;
    QVector<double> deltas; //minsupps for levels
    QVector<struct pattern> frequentPatterns;
    QVector<struct pattern> fpList;
    QVector< QVector<struct numCluster> > data; // num and cluster all terms(with fire) [max_rows][cols+1]
    QVector< QVector<int> > count_cluster; //Число элементов в кластере.[cols+1][lvar_size]
    QVector< QVector<double> > means; // ядра кластеров.
    QString sep; // separator
    int rulesListIndex_prev; // Подсветка правила
    QVector<int> fuzzyTableIndexes_prev; // Подсветка строк в нечётком множестве.
    QVector< QVector<double> > base_set;
    QVector<int> term_counts; // Количество термов в каждой л. п.
    int fire_count;

    Plot *plot, *nnpsPlot;
    QHBoxLayout *layout, *layout_nnps;
    QVector< QVector<struct membershipFunction> > fs;
    double epsf; // Параметры аппроксимации.
    double epsx;
    alglib::ae_int_t maxits;
    double diffstep;

    //parser
    QVector<struct city> cities;
    QMovie *movieRp5;

    QVector<QVector<membershipFunction> > approxGauss(QVector< QVector<struct numCluster> > data);
    QStringList readFileToStringList(QString fileName);
    void makeCTree(int rows);
    void findRules(int first, int last);
    void printRules(QVector<pattern> fpList);
    void writeFromTableToDeltas();
    void writeFromDeltasToTable();

    //void nnpsCalc(QVector<double> xs, double *fire1, double *fire2, double *prod);
    void drawTerm(int i, int j);

    QNetworkAccessManager * mgr;
    void getRp5Data(QDate dFrom, QDate dTo, QString id);

};

#endif // FPWORTH_H




