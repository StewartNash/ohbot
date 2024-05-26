/*
 * Easing Functions: Copyright (c) 2010 Andy Brown
 * http://www.andybrown.me.uk
 *
 * This work is licensed under a Creative Commons
 * Attribution-ShareAlike 3.0 Unported License.
 * http://creativecommons.org/licenses/by-sa/3.0/
 */

#pragma once
#ifndef HT_TWEEN_EASING_ELASTICEASE_H
#define HT_TWEEN_EASING_ELASTICEASE_H

#include "EasingBase.h"




namespace ht {
namespace ease {

class ElasticEase : public EasingBase
{
    double v_period {0};
    double v_amplitude {0};

public:

    virtual ~ElasticEase() {}

    virtual double easeIn(double t) const;

    virtual double easeOut(double t) const;

    virtual double easeInOut(double t) const;

    void period(const double p);
    void amplitude(const double a);
    double period() const;
    double amplitude() const;
};

template <>
struct Easing<ElasticIn> : public ElasticEase
{
    double get(const double t) const { return ElasticEase::easeIn(t); }
};

template <>
struct Easing<ElasticOut> : public ElasticEase
{
    double get(const double t) const { return ElasticEase::easeOut(t); }
};

template <>
struct Easing<ElasticInOut> : public ElasticEase
{
    double get(const double t) const { return ElasticEase::easeInOut(t); }
};

} // ease
} // ht

#endif
