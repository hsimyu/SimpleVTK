#include <simple_vtk.hpp>

int main() {
    // parent
    {
        std::vector<float> values{
            0, 1, 2, 3, 4, 5, 6, 7, 8,
            9, 10, 11, 12, 13, 14, 15, 16,
            17, 18, 19, 20, 21, 22, 23, 24, 25, 26
        };

        SimpleVTK gen;
        gen.enableExtentManagement();
        gen.changeBaseExtent(0, 2, 0, 2, 0, 2);
        gen.changeBaseOrigin(0.0, 0.0, 0.0);
        gen.changeBaseSpacing(1.0, 1.0, 0.5);

        gen.beginVTK("ImageData");
            gen.beginContentWithPiece();
                gen.addPointScalars("test_point_data", "Float32", "ascii", values);
            gen.endContentWithPiece();
        gen.endVTK();
        gen.generate("parent");
    }

    // child
    {
        std::vector<float> values{
            0, 1, 2, 3, 4, 5, 6, 7, 8,
            9, 10, 11, 12, 13, 14, 15, 16,
            17, 18, 19, 20, 21, 22, 23, 24, 25, 26
        };

        SimpleVTK gen;
        gen.enableExtentManagement();
        gen.changeBaseExtent(0, 2, 0, 2, 0, 2);
        gen.changeBaseOrigin(0.0, 1.0, 0.0);
        gen.changeBaseSpacing(0.5, 0.5, 0.25);

        gen.beginVTK("ImageData");
            gen.beginContentWithPiece();
                gen.addPointScalars("test_point_data", "Float32", "ascii", values);
            gen.endContentWithPiece();
        gen.endVTK();
        gen.generate("child");
    }

    // grandchild
    {
        std::vector<float> values{
            0, 1, 2, 3, 4, 5, 6, 7, 8,
            9, 10, 11, 12, 13, 14, 15, 16,
            17, 18, 19, 20, 21, 22, 23, 24, 25, 26
        };

        SimpleVTK gen;
        gen.enableExtentManagement();
        gen.changeBaseExtent(0, 2, 0, 2, 0, 2);
        gen.changeBaseOrigin(0.0, 1.0, 0.0);
        gen.changeBaseSpacing(0.25, 0.25, 0.125);

        gen.beginVTK("ImageData");
            gen.beginContentWithPiece();
                gen.addPointScalars("test_point_data", "Float32", "ascii", values);
            gen.endContentWithPiece();
        gen.endVTK();
        gen.generate("grandchild");
    }

    {
        SimpleVTK gen;
        gen.enableExtentManagement();
        gen.changeBaseExtent(0, 2, 0, 2, 0, 2);
        gen.changeBaseOrigin(0.0, 0.0, 0.0);
        gen.changeBaseSpacing(1.0, 1.0, 0.5);
        gen.changeRefinementRatio(2.0);

        gen.beginVTK("vtkHierarchicalBoxDataSet");
            gen.beginContent();
            gen.setGridDescription("XYZ");
                gen.beginBlock();
                    gen.beginDataSet(0);
                    gen.setAMRBoxNode(0, 2, 0, 2, 0, 2);
                    gen.setFile("parent.vti");
                    gen.endDataSet();
                gen.endBlock();
                gen.beginBlock();
                    gen.beginDataSet(0);
                    gen.setAMRBoxNodeFromParentIndex(0, 0, 1, 1, 2, 0, 1);
                    gen.setFile("child.vti");
                    gen.endDataSet();
                gen.endBlock();
                gen.beginBlock();
                    gen.beginDataSet(0);
                    gen.setAMRBoxNodeFromParentIndex(0, 0, 1, 0, 1, 0, 1);
                    gen.setFile("grandchild.vti");
                    gen.endDataSet();
                gen.endBlock();
            gen.endContent();
        gen.endVTK();

        gen.generate("test_vthb");
    }

    return 0;
}