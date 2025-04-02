/*
 * Visualize some graphs as an example use of morphologica
 */
#include <iostream>
#include <memory>

#include <morph/Visual.h>         // morph::Visual - the scene class
#include <morph/GraphVisual.h>    // morph::GraphVisual - the 2D graph class
#include <morph/DatasetStyle.h>   // morph::DatasetStyle - setting style attributes for graphs
#include <morph/colour.h>         // access to morph::colour namespace
#include <morph/unicode.h>        // morph::unicode - Unicode text handling
#include <morph/vec.h>            // morph::vec - a static-sized vector (like std::array) with maths
#include <morph/vvec.h>           // morph::vvec - a dynamic vector (like std::vector) with maths

int main()
{
    int rtn = 0;

    // Create a morph::Visual. This is linked to a window on your desktop when the program runs
    morph::Visual v(1536, 1536, "A variety of graph formats");
    // Set scene translation to position the graphs in the centre of the window (See Ctrl-z output to stdout)
    v.setSceneTrans (morph::vec<float,3>({-1.21382f, 0.199316f, -5.9f}));

    // Some positioning values used to place each of the GraphVisuals:
    constexpr float step = 1.4f;
    constexpr float row2 = 1.2f;

    try {

        // Some example data in morphologica's 'mathematical vector' class, vvec.
        morph::vvec<float> absc =  {-.5, -.4, -.3, -.2, -.1, 0, .1, .2, .3, .4, .5, .6, .7, .8};
        morph::vvec<float> data = absc.pow(3);

        // Graph 1
        auto gv = std::make_unique<morph::GraphVisual<float>>(morph::vec<float>{0,0,0});
        v.bindmodel (gv); // Necessary boilerplate line (hooks up some callbacks in gv)

        // GraphVisuals have a 'data set style' object
        morph::DatasetStyle ds;
        ds.linecolour = morph::colour::purple; // See morph/colour.h for all the colours or
                                               // http://www.cloford.com/resources/colours/500col.htm
        ds.linewidth = 0.015f;
        ds.markerstyle = morph::markerstyle::diamond;
        ds.markercolour = morph::colour::deepskyblue2;
        gv->setdata (absc, data, ds);

        gv->axisstyle = morph::axisstyle::L;

        // Set labels to include greek characters:
        namespace uc = morph::unicode;
        gv->xlabel = "Include unicode symbols like this: " + uc::toUtf8 (uc::alpha);
        gv->ylabel = "Unicode for Greek gamma is 0x03b3: " + uc::toUtf8 (uc::gamma);

        gv->setthickness (0.001f);
        gv->finalize();
        v.addVisualModel (gv);

        // Graph 2
        gv = std::make_unique<morph::GraphVisual<float>> (morph::vec<float>({step,0,0}));
        v.bindmodel (gv);
        morph::vvec<float> data2 = absc.pow(2);
        ds.linecolour = morph::colour::royalblue;
        ds.markerstyle = morph::markerstyle::hexagon;
        ds.markercolour = morph::colour::black;
        gv->setdata (absc, data2, ds);
        gv->axisstyle = morph::axisstyle::box;
        gv->ylabel = "mm";
        gv->xlabel = "Abscissa (notice that mm is not rotated)";
        gv->setthickness (0.005f);
        gv->finalize();
        v.addVisualModel (gv);

        gv = std::make_unique<morph::GraphVisual<float>> (morph::vec<float>({0,-row2,0}));
        v.bindmodel (gv);
        morph::vvec<float> data3 = absc.pow(4);
        gv->setsize (1,0.8);
        ds.linecolour = morph::colour::cobaltgreen;
        ds.markerstyle = morph::markerstyle::circle;
        ds.markercolour = morph::colour::white;
        ds.markersize = 0.02f;
        ds.markergap = 0.0f;
        gv->setdata (absc, data3, ds);
        gv->axisstyle = morph::axisstyle::boxfullticks;
        gv->tickstyle = morph::tickstyle::ticksin;
        gv->ylabel = "mmi";
        gv->xlabel = "mmi is just long enough to be rotated";
        gv->setthickness (0.001f);
        gv->finalize();
        v.addVisualModel (gv);

        gv = std::make_unique<morph::GraphVisual<float>> (morph::vec<float>({step,-row2,0}));
        v.bindmodel (gv);
        absc.resize(1000, 0.0f);
        for (int i = 0; i < 1000; ++i) {
            absc[i] = static_cast<float>(i-500) * 0.01f;
        }
        gv->setsize (1,0.8);
        ds.linecolour = morph::colour::crimson;
        ds.markerstyle = morph::markerstyle::none;
        ds.markergap = 0.0f;
        ds.linewidth = 0.005f;
        gv->setdata (absc, absc.pow(5), ds);
        gv->axisstyle = morph::axisstyle::cross;
        gv->setthickness (0.002f);
        gv->finalize();
        v.addVisualModel (gv);

        // Display until user closes window
        v.keepOpen();

    } catch (const std::exception& e) {
        std::cerr << "Caught exception: " << e.what() << std::endl;
        rtn = -1;
    }

    return rtn;
}
