#include <test_simple_vtk.hpp>

namespace TEST_SIMPLE_VTK {

    TEST_F(Test1, check_header) {
        ASSERT_EQ(gen.getRawString(), header);
    }

    TEST_F(Test1, make_vtk_imagedata) {
        gen.beginVTK("ImageData");
        gen.endVTK();

        const std::string target = header + "<VTKFile type=\"ImageData\">\n</VTKFile>\n";
        compareCurrentContentAndTarget(target);
        ASSERT_EQ(gen.getRawString(), target);
    }

    TEST_F(Test1, make_vtk_imagedata_with_attributes) {
        gen.beginVTK("ImageData");
        gen.setByteOrder("LittleEndian");
        gen.setVersion("1.1");
        gen.setCompressor("vtkTestCompressor");
        gen.endVTK();

        const std::string target = header + "<VTKFile type=\"ImageData\" byte_order=\"LittleEndian\" version=\"1.1\" compressor=\"vtkTestCompressor\">\n</VTKFile>\n";
        compareCurrentContentAndTarget(target);
        ASSERT_EQ(gen.getRawString(), target);
    }

    TEST_F(Test1, make_vtk_imagedata_with_some_extent) {
        gen.beginVTK("ImageData");
            gen.beginContent();
            gen.setWholeExtent(0, 2, 0, 2, 0, 2);
            gen.setOrigin(0.0, 0.5, 0.0);
            gen.setSpacing(1.0, 1.1, 1.0);
            gen.endContent();
        gen.endVTK();

        const std::string target = 
            header +
            "<VTKFile type=\"ImageData\">\n" +
                indent + "<ImageData WholeExtent=\"0 2 0 2 0 2\" Origin=\"0 0.5 0\" Spacing=\"1 1.1 1\">\n" + indent + "</ImageData>\n"+
            "</VTKFile>\n";
        compareCurrentContentAndTarget(target);
        ASSERT_EQ(gen.getRawString(), target);
    }

    TEST_F(Test1, make_vtk_imagedata_piece) {
        gen.beginVTK("ImageData");
            gen.beginContent();
            gen.setWholeExtent(0, 2, 0, 2, 0, 2);
            gen.setOrigin(0.0, 0.5, 0.0);
            gen.setSpacing(1.0, 1.1, 1.0);
                gen.beginPiece();
                gen.setExtent(0, 2, 0, 2, 0, 2);
                gen.endPiece();
            gen.endContent();
        gen.endVTK();

        const std::string target = 
            header +
            "<VTKFile type=\"ImageData\">\n" +
                indent + "<ImageData WholeExtent=\"0 2 0 2 0 2\" Origin=\"0 0.5 0\" Spacing=\"1 1.1 1\">\n" +
                indent + indent + "<Piece Extent=\"0 2 0 2 0 2\">\n" +
                indent + indent + "</Piece>\n" +
                indent + "</ImageData>\n"+
            "</VTKFile>\n";
        compareCurrentContentAndTarget(target);
        ASSERT_EQ(gen.getRawString(), target);
    }

    TEST_F(Test1, make_vtk_imagedata_piece_with_pointdata_and_celldata) {
        gen.beginVTK("ImageData");
            gen.beginContent();
            gen.setWholeExtent(0, 2, 0, 2, 0, 2);
            gen.setOrigin(0.0, 0.5, 0.0);
            gen.setSpacing(1.0, 1.1, 1.0);
                gen.beginPiece();
                gen.setNumberOfPoints(3);
                gen.setNumberOfCells("3");
                gen.setExtent(0, 2, 0, 2, 0, 2);
                    gen.beginPointData();
                    gen.setScalars("potential");
                    gen.setVectors("efield");
                    gen.endPointData();

                    gen.beginCellData();
                    gen.setScalars("density");
                    gen.endCellData();
                gen.endPiece();
            gen.endContent();
        gen.endVTK();

        const std::string target = 
            header +
            "<VTKFile type=\"ImageData\">\n" +
                indent + "<ImageData WholeExtent=\"0 2 0 2 0 2\" Origin=\"0 0.5 0\" Spacing=\"1 1.1 1\">\n" +
                indent + indent + "<Piece NumberOfPoints=\"3\" NumberOfCells=\"3\" Extent=\"0 2 0 2 0 2\">\n" +
                indent + indent + indent + "<PointData Scalars=\"potential\" Vectors=\"efield\">\n" +
                indent + indent + indent + "</PointData>\n" +
                indent + indent + indent + "<CellData Scalars=\"density\">\n" +
                indent + indent + indent + "</CellData>\n" +
                indent + indent + "</Piece>\n" +
                indent + "</ImageData>\n"+
            "</VTKFile>\n";
        compareCurrentContentAndTarget(target);
        ASSERT_EQ(gen.getRawString(), target);
    }

    TEST_F(Test1, make_vtk_imagedata_piece_with_points) {
        gen.beginVTK("ImageData");
            gen.beginContent();
            gen.setWholeExtent(0, 2, 0, 2, 0, 2);
            gen.setOrigin(0.0, 0.5, 0.0);
            gen.setSpacing(1.0, 1.1, 1.0);
                gen.beginPiece();
                gen.setNumberOfPoints(3);
                gen.setNumberOfCells("3");
                gen.setExtent(0, 2, 0, 2, 0, 2);
                    gen.beginPoints();
                    gen.endPoints();
                    gen.addCells();
                    gen.beginPointData();
                    gen.setScalars("potential");
                    gen.setVectors("efield");
                    gen.endPointData();

                    gen.beginCellData();
                    gen.setScalars("density");
                    gen.endCellData();
                gen.endPiece();
            gen.endContent();
        gen.endVTK();

        const std::string target = 
            header +
            "<VTKFile type=\"ImageData\">\n" +
                indent + "<ImageData WholeExtent=\"0 2 0 2 0 2\" Origin=\"0 0.5 0\" Spacing=\"1 1.1 1\">\n" +
                indent + indent + "<Piece NumberOfPoints=\"3\" NumberOfCells=\"3\" Extent=\"0 2 0 2 0 2\">\n" +
                indent + indent + indent + "<Points>\n" +
                indent + indent + indent + "</Points>\n" +
                indent + indent + indent + "<Cells>\n" +
                indent + indent + indent + "</Cells>\n" +
                indent + indent + indent + "<PointData Scalars=\"potential\" Vectors=\"efield\">\n" +
                indent + indent + indent + "</PointData>\n" +
                indent + indent + indent + "<CellData Scalars=\"density\">\n" +
                indent + indent + indent + "</CellData>\n" +
                indent + indent + "</Piece>\n" +
                indent + "</ImageData>\n"+
            "</VTKFile>\n";
        compareCurrentContentAndTarget(target);
        ASSERT_EQ(gen.getRawString(), target);
    }

    TEST_F(Test1, check_dataarray) {
        gen.beginVTK("ImageData");
            gen.beginContent();
            gen.setWholeExtent(0, 2, 0, 2, 0, 2);
            gen.setOrigin(0.0, 0.5, 0.0);
            gen.setSpacing(1.0, 1.1, 1.0);
                gen.beginPiece();
                gen.setNumberOfPoints(3);
                gen.setNumberOfCells("3");
                gen.setExtent(0, 2, 0, 2, 0, 2);
                    gen.beginPoints();
                    gen.endPoints();
                    gen.addCells();
                    gen.beginPointData();
                    gen.setScalars("potential");
                    gen.setVectors("efield");
                        gen.beginDataArray("potential", "Float32", "ascii");
                        gen.endDataArray();
                        gen.beginDataArray("efield", "Float32", "ascii");
                        gen.endDataArray();
                    gen.endPointData();

                    gen.beginCellData();
                    gen.setScalars("density");
                        gen.beginDataArray("density", "Float32", "ascii");
                        gen.endDataArray();
                    gen.endCellData();
                gen.endPiece();
            gen.endContent();
        gen.endVTK();

        const std::string target = 
            header +
            "<VTKFile type=\"ImageData\">\n" +
                indent + "<ImageData WholeExtent=\"0 2 0 2 0 2\" Origin=\"0 0.5 0\" Spacing=\"1 1.1 1\">\n" +
                indent + indent + "<Piece NumberOfPoints=\"3\" NumberOfCells=\"3\" Extent=\"0 2 0 2 0 2\">\n" +
                indent + indent + indent + "<Points>\n" +
                indent + indent + indent + "</Points>\n" +
                indent + indent + indent + "<Cells>\n" +
                indent + indent + indent + "</Cells>\n" +
                indent + indent + indent + "<PointData Scalars=\"potential\" Vectors=\"efield\">\n" +
                indent + indent + indent + indent + "<DataArray Name=\"potential\" type=\"Float32\" format=\"ascii\">\n" +
                indent + indent + indent + indent + "</DataArray>\n" +
                indent + indent + indent + indent + "<DataArray Name=\"efield\" type=\"Float32\" format=\"ascii\">\n" +
                indent + indent + indent + indent + "</DataArray>\n" +
                indent + indent + indent + "</PointData>\n" +
                indent + indent + indent + "<CellData Scalars=\"density\">\n" +
                indent + indent + indent + indent + "<DataArray Name=\"density\" type=\"Float32\" format=\"ascii\">\n" +
                indent + indent + indent + indent + "</DataArray>\n" +
                indent + indent + indent + "</CellData>\n" +
                indent + indent + "</Piece>\n" +
                indent + "</ImageData>\n"+
            "</VTKFile>\n";
        compareCurrentContentAndTarget(target);
        ASSERT_EQ(gen.getRawString(), target);
    }
    TEST_F(Test1, check_dataarray_addVector) {
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
                        gen.beginDataArray("potential", "Float32", "ascii");
                            gen.addVector(values);
                        gen.endDataArray();
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
}