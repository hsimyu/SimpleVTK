#include <simple_vtk.hpp>

int main() {

    // automatically insert extent information
    {
        SimpleVTK gen;
        gen.enableExtentManagement();
        gen.changeBaseExtent(0, 2, 0, 2, 0, 2);
        gen.changeBaseOrigin(0.0, 0.0, 0.0);
        gen.changeBaseSpacing(1.0, 1.0, 0.5);

        std::vector<float> values{
            0, 1, 2, 3, 4, 5, 6, 7, 8,
            9, 10, 11, 12, 13, 14, 15, 16,
            17, 18, 19, 20, 21, 22, 23, 24,
            25, 26
        };

        gen.beginVTK("ImageData");
            gen.beginContent();
                gen.beginPiece();
                    gen.addPointScalars("potential", "Float32", "ascii", values);
                gen.endPiece();
            gen.endContent();
        gen.endVTK();

        gen.generate("auto_extent_management1");
    }

    // or, insert Content and Piece at the same time
    {
        SimpleVTK gen;
        gen.enableExtentManagement();
        gen.changeBaseExtent(0, 2, 0, 2, 0, 2);
        gen.changeBaseOrigin(0.0, 0.0, 0.0);
        gen.changeBaseSpacing(1.0, 1.0, 0.5);

        std::vector<float> values{
            0, 1, 2, 3, 4, 5, 6, 7, 8,
            9, 10, 11, 12, 13, 14, 15, 16,
            17, 18, 19, 20, 21, 22, 23, 24,
            25, 26
        };

        gen.beginVTK("ImageData");
            gen.beginContentWithPiece();
                gen.addPointScalars("potential", "Float32", "ascii", values);
            gen.endContentWithPiece();
        gen.endVTK();

        gen.generate("auto_extent_management2");
    }

    return 0;
}