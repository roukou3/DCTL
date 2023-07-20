#ifndef DCTL_H
#define DCTL_H

#include <cmath>
#include <algorithm>
#include <cstdio>
#include <cstdarg>
#include <cassert>
#include <string>
#include <vector>
#include <cstdint>

#include <QObject>
#include <QLabel>
#include <QCheckBox>
#include <QSlider>

#define __DEVICE__

struct float2
{
    float x, y;
};

struct float3
{
    float x, y, z;
};

struct Texture
{
    Texture() { w = 1; h = 1; p.push_back(0); }
    void resize(int x, int y) { w = x; h = y; p.resize(w * h); }

    int w;
    int h;
    std::vector<float> p;
};

typedef const Texture& __TEXTURE__;

struct DCTL_CheckBox : public QObject
{
    Q_OBJECT
public:
    const char* name;
    bool def;
    bool value;

    operator bool() {return value;}

    QCheckBox* cb = nullptr;

public slots:
    void stateChanged(int state);

    void reset();
};

class DCTL_Slider : public QObject
{
    Q_OBJECT
public:
    const char* name;
    float def;
    float lo;
    float hi;
    float step;
    float value;

    operator float() {return value;}

    QLabel* labelValue = nullptr;

    QSlider* sl = nullptr;

public slots:
    void valueChanged(int value);

    void reset();
};

inline float _fdivide(float x, float y)
{
    return x/y;
}

inline float _cosf(float f)
{
    return std::cos(f);
}

inline float _sinf(float f)
{
    return std::sin(f);
}

inline float _fmaf(float x, float y, float z)
{
    return std::fma(x, y, z);
}

inline float2 make_float2(float x, float y)
{
    float2 ret;
    ret.x = x;
    ret.y = y;
    return ret;
}

inline float3 make_float3(float x, float y, float z)
{
    float3 ret;
    ret.x = x;
    ret.y = y;
    ret.z = z;
    return ret;
}

inline float _fabs(float f)
{
    return std::fabs(f);
}

inline float _floor(float f)
{
    return std::floor(f);
}

inline float _tex2D(const __TEXTURE__ t, float xf, float yf)
{
    int x = std::floor(xf);
    int y = std::floor(yf);

    if (x < 0) x = 0; else if (x >= t.w) x = t.w - 1;
    if (y < 0) y = 0; else if (y >= t.h) y = t.h - 1;

    return t.p[x + y * t.w];

    //return 0;
}

inline float _round(float f)
{
    return std::round(f);
}

inline float _hypotf(float x, float y)
{
    return std::hypot(x, y);
}

inline float _acosf(float f)
{
    return std::acos(f);
}

inline float _clampf(float f, float l, float h)
{
    //return std::clamp(f, l, h);

    if (f < l) f = l;
    if (f > h) f = h;

    return f;

}

inline float _copysignf(float x, float y)
{
    return std::copysign(x, y);
}

extern DCTL_CheckBox CIRCLE_ADJ;
extern DCTL_Slider CIRCLE_CENTX;
extern DCTL_Slider CIRCLE_CENTY;
extern DCTL_Slider CIRCLE_RAD;
extern DCTL_Slider PIX_ASPECT;
extern DCTL_Slider FOCAL_LENGTH;
extern DCTL_CheckBox INTERP;
extern DCTL_CheckBox EQUIRECT;
extern DCTL_CheckBox HIDE_OUTSIDE;
extern DCTL_Slider ANGLE_PAN;
extern DCTL_Slider ANGLE_TILT;
extern DCTL_Slider ANGLE_ROTATE;
extern DCTL_Slider DEFISH_RATE;
extern DCTL_Slider ANGLE_PAN2;
extern DCTL_Slider ANGLE_TILT2;
extern DCTL_Slider ANGLE_ROTATE2;
extern DCTL_Slider HIDE_OUTSIDE_DEGREE;

#define DCTLUI_CHECK_BOX 0
#define DCTLUI_SLIDER_FLOAT 1

inline int define_ui_param(DCTL_CheckBox& v, const char* name, int type, ...)
{
    assert(type == DCTLUI_CHECK_BOX);

    va_list ptr;
    va_start(ptr, type);
    int value = va_arg(ptr, int);
    va_end(ptr);

    v.value = 0;
    v.def = value != 0;
    v.name = name;

    return 0;
}

inline int define_ui_param(DCTL_Slider& v, const char* name, int type, ...)
{
    assert(type == DCTLUI_SLIDER_FLOAT);

    va_list ptr;
    va_start(ptr, type);
    double value0 = va_arg(ptr, double);
    double value1 = va_arg(ptr, double);
    double value2 = va_arg(ptr, double);
    double value3 = va_arg(ptr, double);
    va_end(ptr);

    v.value = 0;
    v.def = value0;
    v.lo = value1;
    v.hi = value2;
    v.step = value3;
    v.name = name;

    return 0;
}

#define DEFINE_UI_PARAMS(v, name, type, ...) static int DUMMY_##v = define_ui_param(v, #name, type, __VA_ARGS__);

inline float _saturatef(float f)
{
    if (f != f) return 0;
    return _clampf(f, 0, 1);
}

inline float _exp2f(float f)
{
    return std::exp2(f);
}

inline float _fminf(float x, float y)
{
    return std::fmin(x, y);
}

inline float _fmaxf(float x, float y)
{
    return std::fmax(x, y);
}

float3 transform(int p_Width, int p_Height, int p_X, int p_Y, __TEXTURE__ p_TexR, __TEXTURE__ p_TexG, __TEXTURE__ p_TexB);

#endif // DCTL_H
