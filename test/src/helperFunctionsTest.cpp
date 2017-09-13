#include <test_simple_vtk.hpp>

namespace TEST_SIMPLE_VTK {
    TEST_F(Test1, check_data_array_helper) {
        std::vector<float> values{
            0, 1, 2, 3, 4, 5, 6, 7, 8,
            9, 10, 11, 12, 13, 14, 15, 16,
            17, 18, 19, 20, 21, 22, 23, 24,
            25, 26
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
                        gen.addDataArray("potential", "Float32", "ascii", values);
                    gen.endPointData();
                gen.endPiece();
            gen.endContent();
        gen.endVTK();

        const std::string target = 
            header +
            "<VTKFile type=\"ImageData\">\n" +
                indent + "<ImageData WholeExtent=\"0 2 0 2 0 2\" Origin=\"0 0 0\" Spacing=\"1 1 1\">\n" +
                indent + indent + "<Piece Extent=\"0 2 0 2 0 2\">\n" +
                indent + indent + indent + "<PointData Scalars=\"potential\">\n" +
                indent + indent + indent + indent + "<DataArray Name=\"potential\" type=\"Float32\" format=\"ascii\">\n" +
                indent + indent + indent + indent + indent + "0 1 2 3 4 5 6 7 8 9 \n" +
                indent + indent + indent + indent + indent + "10 11 12 13 14 15 16 17 18 19 \n" +
                indent + indent + indent + indent + indent + "20 21 22 23 24 25 26\n" +
                indent + indent + indent + indent + "</DataArray>\n" +
                indent + indent + indent + "</PointData>\n" +
                indent + indent + "</Piece>\n" +
                indent + "</ImageData>\n"+
            "</VTKFile>\n";
        compareCurrentContentAndTarget(target);
        ASSERT_EQ(gen.getRawString(), target);
    }

    TEST_F(Test1, check_add_point_scalars_helper) {
        std::vector<float> values{
            0, 1, 2, 3, 4, 5, 6, 7, 8,
            9, 10, 11, 12, 13, 14, 15, 16,
            17, 18, 19, 20, 21, 22, 23, 24,
            25, 26
        };

        gen.beginVTK("ImageData");
            gen.beginContent();
            gen.setWholeExtent(0, 2, 0, 2, 0, 2);
            gen.setOrigin(0.0, 0.0, 0.0);
            gen.setSpacing(1.0, 1.0, 1.0);
                gen.beginPiece();
                gen.setExtent(0, 2, 0, 2, 0, 2);
                    gen.addPointScalars("potential", "Float32", "ascii", values);
                gen.endPiece();
            gen.endContent();
        gen.endVTK();

        const std::string target = 
            header +
            "<VTKFile type=\"ImageData\">\n" +
                indent + "<ImageData WholeExtent=\"0 2 0 2 0 2\" Origin=\"0 0 0\" Spacing=\"1 1 1\">\n" +
                indent + indent + "<Piece Extent=\"0 2 0 2 0 2\">\n" +
                indent + indent + indent + "<PointData Scalars=\"potential\">\n" +
                indent + indent + indent + indent + "<DataArray Name=\"potential\" type=\"Float32\" format=\"ascii\">\n" +
                indent + indent + indent + indent + indent + "0 1 2 3 4 5 6 7 8 9 \n" +
                indent + indent + indent + indent + indent + "10 11 12 13 14 15 16 17 18 19 \n" +
                indent + indent + indent + indent + indent + "20 21 22 23 24 25 26\n" +
                indent + indent + indent + indent + "</DataArray>\n" +
                indent + indent + indent + "</PointData>\n" +
                indent + indent + "</Piece>\n" +
                indent + "</ImageData>\n"+
            "</VTKFile>\n";
        compareCurrentContentAndTarget(target);
        ASSERT_EQ(gen.getRawString(), target);
    }

    TEST_F(Test1, check_add_cell_scalars_helper) {
        std::vector<float> values{
            0, 1, 2, 3, 4, 5, 6, 7, 8,
            9, 10, 11, 12, 13, 14, 15, 16,
            17, 18, 19, 20, 21, 22, 23, 24,
            25, 26
        };

        gen.beginVTK("ImageData");
            gen.beginContent();
            gen.setWholeExtent(0, 2, 0, 2, 0, 2);
            gen.setOrigin(0.0, 0.0, 0.0);
            gen.setSpacing(1.0, 1.0, 1.0);
                gen.beginPiece();
                gen.setExtent(0, 2, 0, 2, 0, 2);
                    gen.addCellScalars("potential", "Float32", "ascii", values);
                gen.endPiece();
            gen.endContent();
        gen.endVTK();

        const std::string target = 
            header +
            "<VTKFile type=\"ImageData\">\n" +
                indent + "<ImageData WholeExtent=\"0 2 0 2 0 2\" Origin=\"0 0 0\" Spacing=\"1 1 1\">\n" +
                indent + indent + "<Piece Extent=\"0 2 0 2 0 2\">\n" +
                indent + indent + indent + "<CellData Scalars=\"potential\">\n" +
                indent + indent + indent + indent + "<DataArray Name=\"potential\" type=\"Float32\" format=\"ascii\">\n" +
                indent + indent + indent + indent + indent + "0 1 2 3 4 5 6 7 8 9 \n" +
                indent + indent + indent + indent + indent + "10 11 12 13 14 15 16 17 18 19 \n" +
                indent + indent + indent + indent + indent + "20 21 22 23 24 25 26\n" +
                indent + indent + indent + indent + "</DataArray>\n" +
                indent + indent + indent + "</CellData>\n" +
                indent + indent + "</Piece>\n" +
                indent + "</ImageData>\n"+
            "</VTKFile>\n";
        compareCurrentContentAndTarget(target);
        ASSERT_EQ(gen.getRawString(), target);
    }
}