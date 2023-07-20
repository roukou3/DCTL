#include "dctl.h"
#include "Meike_6.5mm_f2.0.dctl"

DCTL_CheckBox CIRCLE_ADJ;
DCTL_Slider CIRCLE_CENTX;
DCTL_Slider CIRCLE_CENTY;
DCTL_Slider CIRCLE_RAD;
DCTL_Slider PIX_ASPECT;
DCTL_Slider FOCAL_LENGTH;
DCTL_CheckBox INTERP;
DCTL_CheckBox EQUIRECT;
DCTL_CheckBox HIDE_OUTSIDE;
DCTL_Slider ANGLE_PAN;
DCTL_Slider ANGLE_TILT;
DCTL_Slider ANGLE_ROTATE;
DCTL_Slider DEFISH_RATE;
DCTL_Slider ANGLE_PAN2;
DCTL_Slider ANGLE_TILT2;
DCTL_Slider ANGLE_ROTATE2;
DCTL_Slider HIDE_OUTSIDE_DEGREE;

void DCTL_CheckBox::stateChanged(int state)
{
    value = state == Qt::Checked;
}

void DCTL_CheckBox::reset()
{
    cb->setCheckState(def ? Qt::Checked : Qt::Unchecked);
}

void DCTL_Slider::valueChanged(int val)
{
    value = lo + val * step;
    if (labelValue)
    {
        labelValue->setText(QString::number(value, 'g', 4));
    }
}

void DCTL_Slider::reset()
{
    sl->setValue(std::floor((def - lo)/step));
}
