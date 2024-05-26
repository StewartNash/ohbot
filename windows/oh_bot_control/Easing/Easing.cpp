#include "Easing.h"



namespace ht {
namespace ease {

    double ElasticEase::easeIn(double t) const
    {
        if (t == 0) return 0;
        t /= t_scale;
        if (t == 1) return v_scale;

        double p, a, s;

        if (v_period == 0.) p = t_scale * 0.3;
        else               p = v_period;

        a = v_amplitude;
        if (a == 0. || a < fabs(v_scale))
        {
            a = v_scale;
            s = p / 4.;
        }
        else
            s = p / (2. * M_PI) * asin(v_scale / a);

        t -= 1.;
        return -(a * pow(2., 10. * t) * sin((t * t_scale - s) * (2. * M_PI) / p));
    }

    double ElasticEase::easeOut(double t) const
    {
        if (t == 0.) return 0;
        t /= t_scale;
        if (t == 1.) return v_scale;

        double p, a, s;

        if (v_period == 0.) p = t_scale * 0.3;
        else               p = v_period;

        a = v_amplitude;
        if (a == 0 || a < fabs(v_scale))
        {
            a = v_scale;
            s = p / 4.;
        }
        else
            s = p / (2. * M_PI) * asin(v_scale / a);

        return a * pow(2., -10. * t) * sin((t * t_scale - s) * (2. * M_PI) / p) + v_scale;
    }

    double ElasticEase::easeInOut(double t) const
    {
        if (t == 0.) return 0;
        t /= t_scale / 2.;
        if (t == 2.) return v_scale;

        double p, a, s;

        if (v_period == 0.) p = t_scale * (0.3 * 1.5);
        else               p = v_period;

        a = v_amplitude;
        if (a == 0 || a < fabs(v_scale))
        {
            a = v_scale;
            s = p / 4.;
        }
        else
            s = p / (2. * M_PI) * asin(v_scale / a);

        if (t < 1.)
        {
            t -= 1.;
            return -0.5 * (a * pow(2., 10. * t) * sin((t * t_scale - s) * (2. * M_PI) / p));
        }

        t -= 1.;
        return a * pow(2., -10. * t) * sin((t * t_scale - s) * (2. * M_PI) / p) * 0.5 + v_scale;
    }

    void ElasticEase::period(const double p) { v_period = p; }
    void ElasticEase::amplitude(const double a) { v_amplitude = a; }
    double ElasticEase::period() const { return v_period; }
    double ElasticEase::amplitude() const { return v_amplitude; }

} // ease
} // ht
