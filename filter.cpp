#include "filter.h"
#include <QImage>
#include <cstdlib>

template <class T>
T clamp(T value, T max, T min)
{
    if (value > max)
        return max;
    if (value < min)
        return min;
    return value;
}

QImage Filter::process(const QImage & img) const
{
    QImage result(img);

    for (int x=0; x<=img.width()/2; x++)
        for (int y=0; y<img.height(); y++)
        {
            QColor color = calcNewPixelColor(img, x, y);
            result.setPixelColor(x, y, color);
        }

    return result;
}

QColor InvertFilter::calcNewPixelColor(const QImage & img, int x, int y) const
{
    QColor color = img.pixelColor(x, y);
    color.setRgb(255 - color.red(), 255 - color.green(), 255 - color.blue());
    return color;
}

QColor GrayScaleFilter::calcNewPixelColor(const QImage & img, int x, int y) const
{
    QColor color = img.pixelColor(x, y);

    std::size_t Intensity = 0.299 * color.red() +0.587 * color.green()+ 0.114 * color.blue();

    color.setRgb(Intensity, Intensity, Intensity);
    return color;
}

QColor SepiaFilter::calcNewPixelColor(const QImage & img, int x, int y) const
{
    QColor color = img.pixelColor(x, y);

    float Intensity = 0.299 * color.red() +0.587 * color.green()+ 0.114 * color.blue();

    float k = 50;

    color.setRgb(clamp(Intensity + 2*k, 255.f, 0.f), clamp(Intensity + k/2, 255.f, 0.f), clamp(Intensity - k, 255.f, 0.f));
    return color;
}

QColor BrightFilter::calcNewPixelColor(const QImage & img, int x, int y) const
{
    QColor color = img.pixelColor(x, y);
    float k = 50;
    color.setRgb(clamp(k + color.red(), 255.f, 0.f), clamp(k + color.green(), 255.f, 0.f), clamp(k + color.blue(), 255.f, 0.f));
    return color;
}

QColor MatrixFilter::calcNewPixelColor(const QImage &img, int x, int y) const
{
    float returnR = 0;
    float returnG = 0;
    float returnB = 0;

    int size = mKernel.getSize();
    int radius = mKernel.getRadius();

    for (int i = -radius; i <= radius; i++)
        for (int j = -radius; j <= radius; j++)
        {
            int idx = (i + radius) * size + j + radius;

            QColor color = img.pixelColor(clamp(x+j, img.width()-1, 0), clamp(y+i, img.height()-1,0));
            returnR += color.red() *mKernel[idx];
            printf("%d", mKernel[idx]);


            returnG += color.green() *mKernel[idx];
            returnB += color.blue() *mKernel[idx];
        }


    return QColor(clamp(returnR, 255.f, 0.f), clamp(returnG, 255.f, 0.f), clamp(returnB, 255.f, 0.f));
}

QImage GrayFilter::process(const QImage & img) const
{
    QImage result(img);

    float resred = 0, resgreen = 0, resblue = 0;

    for (int x=0; x<=img.width(); x++)
        for (int y=0; y<img.height(); y++)
        {
            QColor color = img.pixelColor(x, y);
            resred += color.red();
            resgreen += color.green();
            resblue += color.blue();
        }

    QColor rescolor;

    rescolor.setRgb(clamp(resred/(img.width()*img.height()), 255.f, 0.f), clamp(resgreen/(img.width()*img.height()), 255.f, 0.f), clamp(resblue/(img.width()*img.height()), 255.f, 0.f));

    int resnum = (rescolor.red() + rescolor.green() + rescolor.blue())/3;

    for (int x=0; x<=img.width()/2; x++)
        for (int y=0; y<img.height(); y++)
        {
            QColor color = calcNewPixelColor(img, x, y, rescolor, resnum);
            result.setPixelColor(x, y, color);
        }

    return result;
}

QColor GrayWorldFilter::calcNewPixelColor(const QImage & img, int x, int y, QColor rescolor, int resnum) const
{
    QColor color = img.pixelColor(x, y);
    color.setRgb(clamp(color.red()*resnum/rescolor.red(), 255, 0), clamp(color.green()*resnum/rescolor.green(), 255, 0), clamp(color.blue()*resnum/rescolor.blue(), 255, 0));
    return color;
}

QImage BarGraphFilter::process(const QImage & img) const
{
    QImage result(img);

    int a[256] = {0};

    int ymin = 0, ymax = 0;

    for (int x=0; x<img.width(); x++)
        for (int y=0; y<img.height(); y++)
        {
            QColor color = img.pixelColor(x, y);
            int res = (color.red() + color.green() + color.blue())/3;
            a[res] ++;
        }

    int i = 0;

    while (a[i] == 0) i ++;

    ymin = i;

    i = 255;

    while (a[i] == 0) i --;

    ymax = i;

    //printf ("%d %d\n",ymin,ymax);

    for (int x=0; x<=img.width()/2; x++)
        for (int y=0; y<img.height(); y++)
        {
            QColor color = calcNewPixelColor(img, x, y, ymin, ymax);
            result.setPixelColor(x, y, color);
        }

    return result;
}

QColor BarGraphNFilter::calcNewPixelColor(const QImage & img, int x, int y, int ymin, int ymax) const
{
    QColor color = img.pixelColor(x, y);
    color.setRgb(clamp(((color.red() - ymin)*255/(ymax-ymin)), 255, 0), clamp(((color.green() - ymin)*255/(ymax-ymin)), 255, 0), clamp(((color.blue() - ymin)*255/(ymax-ymin)), 255, 0));
    return color;
}

QImage XYFilter::process(const QImage & img) const
{
    QImage result(img);

    for (int x=0; x<=img.width()/2; x++)
        for (int y=0; y<img.height(); y++)
        {
            QColor color = calcNewPixelColor(img, x, y);
            result.setPixelColor(x, y, color);
        }

    return result;
}

QColor GlassFilter::calcNewPixelColor(const QImage & img, int x, int y) const
{
    int u = x + (rand() %10 - 5);
    int k = y + (rand() %10 - 5);

    return img.pixelColor(clamp(u, 699, 0), clamp(k, 699, 0));
}

QColor DilationFilter::calcNewPixelColor(const QImage & img, int x, int y) const
{
    int maxr = 0, maxg = 0, maxb = 0;

    int size = mKernel.getSize();
    int radius = mKernel.getRadius();

    printf("%d", mKernel[5]);

    for (int i = -radius; i <= radius; i++)
        for (int j = -radius; j <= radius; j++)
        {
            int idx = (i + radius) * size + j + radius;

            if (mKernel[idx] != 0)
            {
            QColor color = img.pixelColor(clamp(x+j, img.width()-1, 0), clamp(y+i, img.height()-1,0));
            if (color.red() >= maxr) maxr = color.red();
            if (color.green() >= maxg) maxg = color.green();
            if (color.blue() >= maxb) maxb = color.blue();
            }
        }

    QColor rescolor;

    rescolor.setRgb(maxr, maxg, maxb);

    return rescolor;
}
