#include "plot.h"

Plot::Plot(QWidget *parent):
    QwtPlot( parent )
{
    setAutoFillBackground( true );
    setPalette( QPalette( QColor( 165, 193, 228 ) ) );
    updateGradient();

    setTitle( "График аппроксимации" );
    insertLegend( new QwtLegend(), QwtPlot::RightLegend );

    // axes
    setAxisTitle( xBottom, "" );
    setAxisScale( xBottom, 0.0, 60.0 );

    setAxisTitle( yLeft, "Функция принадлежности -->" );
    setAxisScale( yLeft, -0.1, 1.1 );

    // canvas
    QwtPlotCanvas *canvas = new QwtPlotCanvas();
    canvas->setLineWidth( 1 );
    canvas->setFrameStyle( QFrame::Box | QFrame::Plain );
    canvas->setBorderRadius( 15 );

    QPalette canvasPalette( Qt::white );
    canvasPalette.setColor( QPalette::Foreground, QColor( 133, 190, 232 ) );
    canvas->setPalette( canvasPalette );

    setCanvas( canvas );

    // panning with the left mouse button
    //( void ) new QwtPlotPanner( canvas );

    // zoom in/out with the wheel
    ( void ) new QwtPlotMagnifier( canvas );

    zoom = new QwtPlotZoomer(canvas);
    zoom->setRubberBandPen(QPen(Qt::red));

    clear();

    //  ...a horizontal line at y = 0...


//    curvePoints = new QwtPlotCurve();
//    curvePoints->setStyle( QwtPlotCurve::Dots );
//    curvePoints->attach( this );
}


void Plot::setLegend(QString axis){
    setAxisTitle( xBottom, axis );

}

bool bySize(const QVector<struct pattern> &v1, const QVector<struct pattern> &v2){
    return v1.size() > v2.size();
}

double func_normal(double x, double n, double k){
    return 1-(fabs(x-n))/k;
}

double func_gauss(double x, double m){
    return exp(-M_PI *(x-m)*(x-m));
}

class NormData: public QwtSyntheticPointData
{
public:
    NormData(double nn, double kk): k(kk), n(nn), QwtSyntheticPointData(100){}
    virtual double y(double x) const
    {
        return 1-(fabs(x-n))/k;
    }
    double n,k;

};

class GaussData: public QwtSyntheticPointData
{
public:
    GaussData(double mm, double aa): m(mm), a(aa), QwtSyntheticPointData(100){}
    virtual double y(double x) const
    {
        return exp(-(x-m)*(x-m)/(2*a*a));
    }
    double m,a;

};

class ExpData: public QwtSyntheticPointData
{
public:
    ExpData(double mm, double aa): m(mm), a(aa), QwtSyntheticPointData(100){}
    virtual double y(double x) const
    {
        return 1/(1 + exp(-a*(x - m)));
    }
    double m,a;

};
/*
class NewData: public QwtSyntheticPointData
{
public:
    NewData(double mm, double aa): m(mm), a(aa), QwtSyntheticPointData(100){}
    virtual double y(double x) const
    {
        return 1/(1+fabs(pow((x-m)/a,2*a)));
    }
    double m,a;
};
*/

void Plot::drawDots(QVector< QVector<struct numCluster> > data, double n, double k, int index, int size, int number)
{
    int j, l;
    QPolygonF points;
    QwtPlotCurve *curve;

    QwtSymbol *symbol;

    points.clear();
    curve = new QwtPlotCurve();//QString("y = norm%1(x)").arg(index));
    curve->setItemAttribute(QwtPlotItem::Legend, false);
    curve->setStyle( QwtPlotCurve::Dots );
    for (l = 0; l < data.size(); l++){
        if (data[l][number].cluster == index){
            // ПЕРЕДЕЛАТЬ если возможно!!! Нужно, чтобы он печатал сразу для всех кластеров одной л.п.
            if (index == 0 && data[l][number].number < n){
                points << QPointF(data[l][number].number, 1);
            }else if (index == size - 1 && data[l][number].number > n){
                points << QPointF(data[l][number].number, 1);
            }else{
                points << QPointF(data[l][number].number, func_normal(data[l][number].number,n,k));
            }

            //std::cout << index << "data = " << data[l][i].number << std::endl;
        }
    }
    curve->setSamples(points);
    switch (index % 5){
    case 0:
        symbol = new QwtSymbol( QwtSymbol::Ellipse,
                                QBrush( Qt::yellow ), QPen( Qt::red, 1 ), QSize( 8, 8 ) );
        curve->setSymbol(symbol);
        break;
    case 1:
        symbol = new QwtSymbol( QwtSymbol::Ellipse,
                                QBrush( Qt::green ), QPen( Qt::red, 1 ), QSize( 8, 8 ) );
        curve->setSymbol(symbol);
        break;
    case 2:
        symbol = new QwtSymbol( QwtSymbol::Ellipse,
                                QBrush( Qt::cyan ), QPen( Qt::red, 1 ), QSize( 8, 8 ) );
        curve->setSymbol(symbol);
        break;
    case 3:
        symbol = new QwtSymbol( QwtSymbol::Ellipse,
                                QBrush( Qt::magenta ), QPen( Qt::red, 1 ), QSize( 8, 8 ) );
        curve->setSymbol(symbol);
        break;
    case 4:
        symbol = new QwtSymbol( QwtSymbol::Ellipse,
                                QBrush( Qt::gray ), QPen( Qt::red, 1 ), QSize( 8, 8 ) );
        curve->setSymbol(symbol);
        break;
    default:
        break;
    }
    curve->attach(this);
    this->replot();
}

void Plot::addPoint(double x, double y){
    pointsArr << QPointF(x, y);
}


void Plot::drawLine(QColor color){
    QwtPlotCurve *curvePoints;
    curvePoints = new QwtPlotCurve();
    curvePoints->setPen(color);
    curvePoints->setSamples(pointsArr);
    curvePoints->attach(this);
    this->replot();
    pointsArr.clear();
}


void Plot::drawPoints(){
    QwtPlotCurve *curvePoints;
    curvePoints = new QwtPlotCurve();
    curvePoints->setPen(Qt::green);
    curvePoints->setSamples(pointsArr);
    curvePoints->attach(this);
    this->replot();
}

void Plot::clear()
{
    QwtPlotGrid *grid;
    this->detachItems();
    pointsArr.clear();


    //  ...a horizontal line at y = 1...
    QwtPlotMarker *mY1 = new QwtPlotMarker();
    mY1->setLabel( QString::fromLatin1( "y = 1" ) );
    mY1->setLabelAlignment( Qt::AlignRight | Qt::AlignTop );
    mY1->setLineStyle( QwtPlotMarker::HLine );
    mY1->setYValue( 1.0 );
    mY1->setLinePen(Qt::black, 1, Qt::DashLine);
    mY1->attach( this );


    QwtPlotMarker *mY = new QwtPlotMarker();
    mY->setLabel( QString::fromLatin1( "y = 0" ) );
    mY->setLabelAlignment( Qt::AlignRight | Qt::AlignTop );
    mY->setLineStyle( QwtPlotMarker::HLine );
    mY->setYValue( 0.0 );
    mY->attach( this );

    //  ...a vertical line at x = 0
    QwtPlotMarker *mX = new QwtPlotMarker();
    mX->setLabel( QString::fromLatin1( "x = 0" ) );
    mX->setLabelAlignment( Qt::AlignLeft | Qt::AlignBottom );
    mX->setLabelOrientation( Qt::Vertical );
    mX->setLineStyle( QwtPlotMarker::VLine );
    mX->setLinePen( Qt::black, 0, Qt::DashDotLine );
    mX->setXValue( 0 );
    mX->attach( this );

    grid = new QwtPlotGrid;
    grid->enableXMin(true);
    grid->setMajorPen(QPen(Qt::black,0,Qt::DotLine));
    grid->setMinorPen(QPen(Qt::gray,0,Qt::DotLine));
    grid->attach(this);
}

void Plot::drawGauss(double m, double a, int type, int i)
{
    // Insert new curves
    int j;
    QwtPlotCurve *curve = new QwtPlotCurve(QString("m: %1; a: %2").arg(m).arg(a));
    if (type == 0){
        curve->setData(new GaussData(m, a));
    }else if (type == 1){
        curve->setData(new ExpData(m, a));
    }else{
        curve->setData(new ExpData(m, -a));
    }


    switch (i % 5) {
    case 0:
        curve->setPen(Qt::red);
        break;
    case 1:
        curve->setPen(Qt::green);
        break;
    case 2:
        curve->setPen(Qt::blue);
        break;
    case 3:
        curve->setPen(Qt::magenta);
        break;
    case 4:
        curve->setPen(Qt::cyan);
        break;
    default:
        break;
    }
    curve->attach(this);

    this->replot();
}

void Plot::updateGradient()
{
    QPalette pal = palette();

    const QColor buttonColor = pal.color( QPalette::Button );

    QLinearGradient gradient( rect().topLeft(), rect().bottomLeft() );
    gradient.setColorAt( 0.0, Qt::white );
    gradient.setColorAt( 0.7, buttonColor );
    gradient.setColorAt( 1.0, buttonColor );

    pal.setBrush( QPalette::Window, gradient );
    setPalette( pal );
}

void Plot::setBaseSet(QVector<double> bs){
    QwtPlotMarker *mX = new QwtPlotMarker();
    mX->setLabel( QString("x = %1").arg(bs[0]) );
    mX->setLabelAlignment( Qt::AlignLeft | Qt::AlignBottom );
    mX->setLabelOrientation( Qt::Vertical );
    mX->setLineStyle( QwtPlotMarker::VLine );
    mX->setLinePen( Qt::blue, 0, Qt::DashLine );
    mX->setXValue( bs[0] );
    mX->attach( this );

    QwtPlotMarker *mX1 = new QwtPlotMarker();
    mX1->setLabel( QString("x = %1").arg(bs[1]) );
    mX1->setLabelAlignment( Qt::AlignLeft | Qt::AlignBottom );
    mX1->setLabelOrientation( Qt::Vertical );
    mX1->setLineStyle( QwtPlotMarker::VLine );
    mX1->setLinePen( Qt::blue, 0, Qt::DashLine );
    mX1->setXValue( bs[1] );
    mX1->attach( this );

    setAxisScale( xBottom, bs[0] - 0.5, bs[1] + 0.5);
    QRectF rect(QPointF(bs[0] - 0.5,1.1),QPointF(bs[1] + 0.5,0));
    zoom->setZoomBase(rect);
    this->replot();
}

void Plot::drawVLine(double x){
    QwtPlotMarker *mX = new QwtPlotMarker();
    mX->setLabel( QString("x = %1").arg(x));
    mX->setLabelAlignment( Qt::AlignLeft | Qt::AlignBottom );
    mX->setLabelOrientation( Qt::Vertical );
    mX->setLineStyle( QwtPlotMarker::VLine );
    mX->setLinePen( Qt::red, 0, Qt::DashLine );
    mX->setXValue( x );
    mX->attach( this );

    QPolygonF points;
    QwtPlotCurve *curve;

    curve = new QwtPlotCurve();
    curve->setItemAttribute(QwtPlotItem::Legend, false);
    curve->setStyle( QwtPlotCurve::Dots );
    points << QPointF(x, 0);
    curve->setSamples(points);
    curve->attach(this);
    this->replot();
}

/*
void Plot::resizeEvent( QResizeEvent *event )
{
    QwtPlot::resizeEvent( event );
    // Qt 4.7.1: QGradient::StretchToDeviceMode is buggy on X11
    updateGradient();
}
*/
