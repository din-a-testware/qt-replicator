#ifndef LCARS_SLIDERS_H
#define LCARS_SLIDERS_H
#include <QWidget>
//#include <QtUiPlugin/QDesignerExportWidget>

#include <QSlider>
#include <QWidget>

class /*QDESIGNER_WIDGET_EXPORT*/ LCARS_Sliders : public QSlider
{
    Q_OBJECT

public:
    explicit LCARS_Sliders(QWidget *parent=0);

    //! NORMAL COLORS
    QColor zero_barBackgroundTop;
    QColor zero_barBackgroundBottom;
    QColor all_handleBackground;
    QColor all_handleBackgroundPressed;
    QColor all_handleBorderColor;
    QColor all_bgGradient_Start;
    QColor all_bgGradient_End;
    QColor all_frontGradient_Start;
    QColor all_frontGradient_End;
    QColor all_dividerColor;
    QColor two_barBackground;

    //! NORMAL COLORS
    QColor initial_zero_barBackgroundTop;
    QColor initial_zero_barBackgroundBottom;
    QColor initial_all_handleBackground;
    QColor initial_all_handleBackgroundPressed;
    QColor initial_all_handleBorderColor;
    QColor initial_all_bgGradient_Start;
    QColor initial_all_bgGradient_End;
    QColor initial_all_frontGradient_Start;
    QColor initial_all_frontGradient_End;
    QColor initial_all_dividerColor;
    QColor initial_two_barBackground;

    //! RED ALERT COLORS INACTIVE
    QColor red_zero_barBackgroundTop;
    QColor red_zero_barBackgroundBottom;
    QColor red_all_handleBackground;
    QColor red_all_handleBackgroundPressed;
    QColor red_all_handleBorderColor;
    QColor red_all_bgGradient_Start;
    QColor red_all_bgGradient_End;
    QColor red_all_frontGradient_Start;
    QColor red_all_frontGradient_End;
    QColor red_all_dividerColor;
    QColor red_two_barBackground;

    //! RED ALERT COLORS ACTIVE
    QColor red_active_zero_barBackgroundTop;
    QColor red_active_zero_barBackgroundBottom;
    QColor red_active_all_handleBackground;
    QColor red_active_all_handleBackgroundPressed;
    QColor red_active_all_handleBorderColor;
    QColor red_active_all_bgGradient_Start;
    QColor red_active_all_bgGradient_End;
    QColor red_active_all_frontGradient_Start;
    QColor red_active_all_frontGradient_End;
    QColor red_active_all_dividerColor;
    QColor red_active_two_barBackground;

    //! POINTS
    QPoint p;
    QPoint cursorPoint;


    //! SIZES
    QSize all_handleSize;
    QSize all_handleBorderSize;
    QSize all_roundedEdge;
    QSize zero_barTopLine;
    QSize zero_barBottomLine;
    QSize two_size_roundedHandle_pie;
    QSize two_size_roundedHandle_rect;
    QSize two_size_bar;

    //! BOOLS
    /*bool one_customRounding = false;
    bool one_onlyOne = false;
    bool one_roundBorder = false;*/


    //! NUMBERS
    int resultValue;
    int all_widthDash;
    int all_distanceDash;
    int one_pieHeight;
    int two_roundHandle_Bar_width;
    int two_border_roundedHandleWidth;
    int slider_Type;
    int redAlertState;
    int roundBorder; //0 = no; 1=one; 2=both; (First, I wanted to implement top/bottom border, but I think it's not that difficult to change the orientation/colors)
    int custom_distance;
    int extra_distance_top;
    int extra_distance_bottom;
signals:


public slots:


    /*! [ --- SLIDER TYPE ZERO --- ] */

    //! THE BAR BACKGROUNDS

        //! HORIZONTAL

            //! NORMAL APPEARANCE
            void Type_0_horizontalBar_Normal(QPainter *painter);

        //! VERTICAL

            //! NORMAL APPEARANCE
            void Type_0_verticalBar_Normal(QPainter *painter);

    //! THE HANDLES

        //! HORIZONTAL

            //! NORMAL APPEARANCE
            void Type_0_horizontalHandle_Normal(QPainter *painter);

        //! VERTICAL

            //! NORMAL APPEARANCE
            void Type_0_verticalHandle_Normal(QPainter *painter);


    /*! [ ------------------------- ] */


            /*! [ --- SLIDER TYPE ONE --- ] */

            //! THE BAR BACKGROUNDS

                //! HORIZONTAL

                    //! NORMAL APPEARANCE
                    void Type_1_horizontalBar_Normal(QPainter *painter);

                //! VERTICAL

                    //! NORMAL APPEARANCE
                    void Type_1_verticalBar_Normal(QPainter *painter);

            //! THE HANDLES

                //! HORIZONTAL

                    //! NORMAL APPEARANCE
                    void Type_1_horizontalHandle_Normal(QPainter *painter);

                //! VERTICAL

                    //! NORMAL APPEARANCE
                    void Type_1_verticalHandle_Normal(QPainter *painter);

            /*! [ ------------------------- ] */


                    /*! [ --- SLIDER TYPE TWO --- ] */

                    //! THE BAR BACKGROUNDS

                        //! HORIZONTAL

                            //! NORMAL APPEARANCE
                            void Type_2_horizontalBar_Normal(QPainter *painter);

                        //! VERTICAL

                            //! NORMAL APPEARANCE
                            void Type_2_verticalBar_Normal(QPainter *painter);

                    //! THE HANDLES

                        //! HORIZONTAL

                            //! NORMAL APPEARANCE
                            void Type_2_horizontalHandle_Normal(QPainter *painter);

                        //! VERTICAL

                            //! NORMAL APPEARANCE
                            void Type_2_verticalHandle_Normal(QPainter *painter);


                    /*! [ ------------------------- ] */



                            /*! [ --- SLIDER TYPE THREE --- ] */

                            //! THE BAR BACKGROUNDS

                                //! HORIZONTAL

                                    //! NORMAL APPEARANCE
                                    void Type_3_horizontalBar_Normal(QPainter *painter);

                                //! VERTICAL

                                    //! NORMAL APPEARANCE
                                    void Type_3_verticalBar_Normal(QPainter *painter);

                            //! THE HANDLES

                                //! HORIZONTAL

                                    //! NORMAL APPEARANCE
                                    void Type_3_horizontalHandle_Normal(QPainter *painter);

                                //! VERTICAL

                                    //! NORMAL APPEARANCE
                                    void Type_3_verticalHandle_Normal(QPainter *painter);


                            /*! [ ------------------------- ] */


protected:
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent * event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent * event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
};

#endif // LCARS_SLIDERS_H
