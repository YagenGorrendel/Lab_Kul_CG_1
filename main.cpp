#include <QImage>
#include "filter.h"

float clamp(float value, float max, float min)
{
    if (value > max)
        return max;
    if (value < min)
        return min;
    return value;
}

int main(int argc, char *argv[])
{
    std::string s1;
    QImage img;

    for (int i = 0; i < argc; i++)
    {
        if (!strcmp(argv[i], "-p") && (i + 1 < argc))
        {
            s1 = argv [i+1];
        }
    }

    img.load(QString(s1.c_str()));

    /*InvertFilter invert;
    invert.process(img).save("Images/Invert.png");

    BlurFilter blur(3);
    blur.process(img).save("Images/Blur.png");

    GaussianFilter gauss(3,2);
    gauss.process(img).save("Images/Gauss.png");

    GrayScaleFilter grayscale;
    grayscale.process(img).save("Images/GrayScale.png");

    SepiaFilter sepia;
    sepia.process(img).save("Images/Sepia.png");

    BrightFilter bright;
    bright.process(img).save("Images/Bright.png");

    SobelFilterY sobely(1);
    sobely.process(img).save("Images/SobelY.png");

    SobelFilterX sobelx(1);
    sobelx.process(img).save("Images/SobelX.png");

    SharpnessFilter sharpness(1);
    sharpness.process(img).save("Images/Sharpness.png");

    GrayWorldFilter grayworld;
    grayworld.process(img).save("Images/GrayWorld.png");

    BarGraphNFilter bargraph;
    bargraph.process(img).save("Images/BarGraphFilter.png");

    GlassFilter glass;
    glass.process(img).save("Images/Glass.png");

    MotionFilter motion(4);
    motion.process(img).save("Images/Motion.png");

    DilationFilter dilation(2);
    dilation.process(img).save("Images/Dilation.png");

    ErosionFilter erosion(2);
    erosion.process(img).save("Images/Erosion.png");

    //Closing
    erosion.process(dilation.process(img)).save("Images/Closing.png");

    //Opening
    dilation.process(erosion.process(img)).save("Images/Opening.png");

    GradFilter grad;
    grad.process(dilation.process(img), erosion.process(img)).save("Images/Grad.png");
*/
    MedianFilter median;
    median.process(img).save("Images/Median.png");

    return 0;
}
