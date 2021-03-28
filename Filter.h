#ifndef FILTER_H
#define FILTER_H
#include <QImage>
#include <memory>
#include <math.h>

class Filter
{
    protected:
        virtual QColor calcNewPixelColor(const QImage &img, int x, int y) const = 0;
    public:
        virtual ~Filter() = default;
        virtual QImage process(const QImage &img) const;
};

class InvertFilter : public Filter
{
    QColor calcNewPixelColor(const QImage &img, int x, int y) const override;
};

class GrayScaleFilter : public Filter
{
    QColor calcNewPixelColor(const QImage &img, int x, int y) const override;
};

class SepiaFilter : public Filter
{
    QColor calcNewPixelColor(const QImage &img, int x, int y) const override;
};

class BrightFilter : public Filter
{
    QColor calcNewPixelColor(const QImage &img, int x, int y) const override;
};

class Kernel
{
protected:

    std::unique_ptr<float []> data;
    std::size_t radius;
    std::size_t getLen() const {return getSize() * getSize();}

public:

    Kernel(std::size_t radius) : radius(radius)
    {
        data = std::make_unique<float []>(getLen());
    }
    Kernel(const Kernel& other): Kernel(other.radius)
    {
        std::copy(other.data.get(), other.data.get() + getLen(), data.get());
    }

    std::size_t getRadius() const { return radius; }
    std::size_t getSize() const { return 2 * radius + 1; }
    float operator [] (std::size_t id) const { return data[id]; }
    float& operator [] (std::size_t id) { return data[id]; }
};

class MatrixFilter : public Filter
{
protected:
    Kernel mKernel;
    QColor calcNewPixelColor (const QImage &img, int x, int y) const override;
public:
    MatrixFilter(const Kernel &kernel) : mKernel(kernel) {};
    virtual ~MatrixFilter() = default;
};

class BlurKernel: public Kernel
{
public:
using Kernel::Kernel;
BlurKernel(std::size_t radius = 1) : Kernel(radius)
{
for (std::size_t i = 0; i < getLen(); i++)
data[i] = 1.0f / getLen();
}
};

class BlurFilter : public MatrixFilter
{
public:
BlurFilter(std::size_t radius = 1) : MatrixFilter(BlurKernel(radius)) {}
};

class GaussianKernel: public Kernel
{
public:
    using Kernel::Kernel;
    GaussianKernel(std::size_t radius = 2, float sigma = 3.f) : Kernel(radius)
    {
        float norm = 0;
        int signed_radius = static_cast<int>(radius);

        for (int x = -signed_radius; x<= signed_radius; x++)
            for(int y = -signed_radius; y<= signed_radius; y++)
            {
                std::size_t idx = (x+radius)*getSize() + (y+radius);
                data[idx] = std::exp(-(x*x+y*y)/(sigma*sigma));
                norm += data[idx];
            }

        for (std::size_t i = 0; i<getLen(); i++)
        {
            data[i] /= norm;
        }
    }
};

class GaussianFilter : public MatrixFilter
{
public:
GaussianFilter(std::size_t radius = 1, std::size_t sigma = 1.f) : MatrixFilter(GaussianKernel(radius, sigma)) {}
};

class SobelKernelY: public Kernel
{
public:
using Kernel::Kernel;
SobelKernelY(std::size_t radius = 1) : Kernel(radius)
{
data[0] = -1;
data[1] = -2;
data[2] = -1;
data[3] = 0;
data[4] = 0;
data[5] = 0;
data[6] = 1;
data[7] = 2;
data[8] = 1;
}
};

class SobelFilterY: public MatrixFilter
{
public:
SobelFilterY(std::size_t radius = 1) : MatrixFilter(SobelKernelY(radius)) {}
};

class SobelKernelX: public Kernel
{
public:
using Kernel::Kernel;
SobelKernelX(std::size_t radius = 1) : Kernel(radius)
{
data[0] = -1;
data[1] = 0;
data[2] = 1;
data[3] = -2;
data[4] = 0;
data[5] = 2;
data[6] = -1;
data[7] = 0;
data[8] = 1;
}
};

class SobelFilterX: public MatrixFilter
{
public:
SobelFilterX(std::size_t radius = 1) : MatrixFilter(SobelKernelX(radius)) {}
};

class SharpnessKernel: public Kernel
{
public:
using Kernel::Kernel;
SharpnessKernel(std::size_t radius = 1) : Kernel(radius)
{
data[0] = 0;
data[1] = -1;
data[2] = 0;
data[3] = -1;
data[4] = 5;
data[5] = -1;
data[6] = 0;
data[7] = -1;
data[8] = 0;
}
};

class SharpnessFilter: public MatrixFilter
{
public:
SharpnessFilter(std::size_t radius = 1) : MatrixFilter(SharpnessKernel(radius)) {}
};

class GrayFilter
{
    protected:
        virtual QColor calcNewPixelColor(const QImage &img, int x, int y, QColor rescolor, int resnum) const = 0;
    public:
        virtual ~GrayFilter() = default;
        virtual QImage process(const QImage &img) const;
};

class GrayWorldFilter : public GrayFilter
{
    QColor calcNewPixelColor(const QImage &img, int x, int y, QColor rescolor, int resnum) const override;
};

class BarGraphFilter
{
    protected:
        virtual QColor calcNewPixelColor(const QImage &img, int x, int y, int ymin, int ymax) const = 0;
    public:
        virtual ~BarGraphFilter() = default;
        virtual QImage process(const QImage &img) const;
};

class BarGraphNFilter : public BarGraphFilter
{
    QColor calcNewPixelColor(const QImage &img, int x, int y, int ymin, int ymax) const override;
};

class XYFilter
{
    protected:
        virtual QColor calcNewPixelColor(const QImage &img, int x, int y) const = 0;
    public:
        virtual ~XYFilter() = default;
        virtual QImage process(const QImage &img) const;
};

class GlassFilter : public XYFilter
{
    QColor calcNewPixelColor(const QImage &img, int x, int y) const override;
};

class MotionKernel: public Kernel
{
public:
using Kernel::Kernel;
MotionKernel(std::size_t radius = 1) : Kernel(radius)
{
    for (std::size_t i = 0; i < getLen(); i++)
    if (sqrt(i) - trunc(sqrt(i)) == 0) data[i] = 1.f/(2*radius+1);
    else data[i] = 0;
}
};

class MotionFilter: public MatrixFilter
{
public:
MotionFilter(std::size_t radius = 1) : MatrixFilter(MotionKernel(radius)) {}
};

/*class ilationFilter
{
protected:
    virtual QColor calcNewPixelColor(const QImage &img, int x, int y) const = 0;
public:
    virtual QImage process(const QImage &img) const;
    virtual ~ilationFilter() = default;
};*/

class DilationFilter : public Filter
{
protected:
    Kernel mKernel;
    QColor calcNewPixelColor (const QImage &img, int x, int y) const override;
public:
    DilationFilter(const Kernel &kernel) : mKernel(kernel) {};
    virtual ~DilationFilter() = default;
};

class DilationKernel: public Kernel
{
public:
using Kernel::Kernel;
DilationKernel(std::size_t radius = 2) : Kernel(radius)
{
    for (std::size_t i = 0; i < getLen(); i++) data[i] = 1;
    printf("%d\n",data[5]);
}
};

class dilationFilter: public DilationFilter
{
public:
dilationFilter(std::size_t radius = 1) : DilationFilter(DilationKernel(radius)) {}
};

#endif // FILTER_H
