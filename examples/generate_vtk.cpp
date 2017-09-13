#include <simple_vtk.hpp>

int main() {
    SimpleVTK gen;

    std::vector<float> values{
        0, 1, 2, 3, 4, 5, 6, 7, 8,
        9, 10, 11, 12, 13, 14, 15, 16,
        17, 18, 19, 20, 21, 22, 23
    };
    gen.beginVTK("ImageData");
        gen.beginContent();
        gen.setWholeExtent(0, 2, 0, 2, 0, 2);
        gen.setOrigin(0.0, 0.0, 0.0);
        gen.setSpacing(1.0, 1.0, 1.0);
            gen.beginPiece();
            gen.setExtent(0, 2, 0, 2, 0, 2);
                gen.beginPointData();
                gen.setScalars("potential");
                    gen.beginDataArray("potential", "Float32", "ascii");
                        gen.addVector(values);
                    gen.endDataArray();
                gen.endPointData();
            gen.endPiece();
        gen.endContent();
    gen.endVTK();
    gen.generate("generate_vtk");

    return 0;
}