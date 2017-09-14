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

    TEST_F(Test1, check_base_extent_setting) {
        gen.changeBaseExtent(0, 2, 0, 2, 0, 2);
        gen.changeBaseOrigin(0.0, 0.0, 0.0);
        gen.changeBaseSpacing(1.0, 0.5, 0.25);

        auto extent = gen.getBaseExtent();
        ASSERT_EQ(extent.xmin, 0);
        ASSERT_EQ(extent.xmax, 2);
        ASSERT_EQ(extent.ymin, 0);
        ASSERT_EQ(extent.ymax, 2);
        ASSERT_EQ(extent.zmin, 0);
        ASSERT_EQ(extent.zmax, 2);

        ASSERT_EQ(extent.x0, 0.0);
        ASSERT_EQ(extent.y0, 0.0);
        ASSERT_EQ(extent.z0, 0.0);

        ASSERT_EQ(extent.dx, 1.0);
        ASSERT_EQ(extent.dy, 0.5);
        ASSERT_EQ(extent.dz, 0.25);
    }

    TEST_F(Test1, check_extent_management_status) {
        ASSERT_EQ(gen.isExtentManagementEnable(), false);

        gen.enableExtentManagement();
        ASSERT_EQ(gen.isExtentManagementEnable(), false);

        gen.changeBaseExtent(0, 2, 0, 2, 0, 2);
        gen.changeBaseOrigin(0.0, 0.0, 0.0);
        gen.changeBaseSpacing(1.0, 0.5, 0.25);
        ASSERT_EQ(gen.isExtentManagementEnable(), true);
    }

    TEST_F(Test1, check_extent_management_auto_setting_attributes) {
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

        const std::string target = 
            header +
            "<VTKFile type=\"ImageData\">\n" +
                indent + "<ImageData WholeExtent=\"0 2 0 2 0 2\" Origin=\"0 0 0\" Spacing=\"1 1 0.5\">\n" +
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

    TEST_F(Test1, check_begin_content_with_piece) {
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

        const std::string target = 
            header +
            "<VTKFile type=\"ImageData\">\n" +
                indent + "<ImageData WholeExtent=\"0 2 0 2 0 2\" Origin=\"0 0 0\" Spacing=\"1 1 0.5\">\n" +
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

    TEST_F(Test1, check_begin_content_when_hierarchicalBoxDataSet) {
        gen.enableExtentManagement();
        gen.changeBaseExtent(0, 2, 0, 2, 0, 2);
        gen.changeBaseOrigin(0.0, 0.0, 0.0);
        gen.changeBaseSpacing(1.0, 1.0, 0.5);
        gen.changeRefinementRatio(2.0);

        gen.beginVTK("vtkHierarchicalBoxDataSet");
            gen.beginContent();
            gen.setGridDescription("XYZ");
                gen.beginBlock(0);
                gen.endBlock();
                gen.beginBlock("1");
                gen.endBlock();
                gen.beginBlock(2);
                gen.endBlock();
            gen.endContent();
        gen.endVTK();

        const std::string target = 
            header +
            "<VTKFile type=\"vtkHierarchicalBoxDataSet\">\n" +
                indent + "<vtkHierarchicalBoxDataSet Origin=\"0 0 0\" grid_description=\"XYZ\">\n" +
                indent + indent + "<Block level=\"0\" Spacing=\"1 1 0.5\">\n" +
                indent + indent + "</Block>\n" +
                indent + indent + "<Block level=\"1\" Spacing=\"0.5 0.5 0.25\">\n" +
                indent + indent + "</Block>\n" +
                indent + indent + "<Block level=\"2\" Spacing=\"0.25 0.25 0.125\">\n" +
                indent + indent + "</Block>\n" +
                indent + "</vtkHierarchicalBoxDataSet>\n"+
            "</VTKFile>\n";
        compareCurrentContentAndTarget(target);
        ASSERT_EQ(gen.getRawString(), target);
    }

    TEST_F(Test1, check_hierarchicalBoxDataSet_amr_box_support_cell) {
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
                    gen.setAMRBox(0, 1, 0, 1, 0, 1);
                    gen.setFile("test.vti");
                    gen.endDataSet();
                gen.endBlock();
                gen.beginBlock();
                    gen.beginDataSet(0);
                    gen.setAMRBoxFromParentIndex(0, 0, 0, 0, 0, 0, 0);
                    gen.setFile("test.vti");
                    gen.endDataSet();
                    gen.beginDataSet(1);
                    gen.setAMRBoxFromParentIndex(0, 1, 1, 1, 1, 1, 1);
                    gen.setFile("test.vti");
                    gen.endDataSet();
                gen.endBlock();
                gen.beginBlock();
                    gen.beginDataSet(0);
                    gen.setAMRBoxFromParentIndex(1, 0, 0, 1, 1, 0, 1);
                    gen.setFile("test.vti");
                    gen.endDataSet();
                gen.endBlock();
            gen.endContent();
        gen.endVTK();

        const std::string target = 
            header +
            "<VTKFile type=\"vtkHierarchicalBoxDataSet\">\n" +
                indent + "<vtkHierarchicalBoxDataSet Origin=\"0 0 0\" grid_description=\"XYZ\">\n" +
                indent + indent + "<Block level=\"0\" Spacing=\"1 1 0.5\">\n" +
                indent + indent + indent + "<DataSet index=\"0\" amr_box=\"0 1 0 1 0 1\" file=\"test.vti\">\n" +
                indent + indent + indent + "</DataSet>\n" +
                indent + indent + "</Block>\n" +
                indent + indent + "<Block level=\"1\" Spacing=\"0.5 0.5 0.25\">\n" +
                indent + indent + indent + "<DataSet index=\"0\" amr_box=\"0 1 0 1 0 1\" file=\"test.vti\">\n" +
                indent + indent + indent + "</DataSet>\n" +
                indent + indent + indent + "<DataSet index=\"1\" amr_box=\"2 3 2 3 2 3\" file=\"test.vti\">\n" +
                indent + indent + indent + "</DataSet>\n" +
                indent + indent + "</Block>\n" +
                indent + indent + "<Block level=\"2\" Spacing=\"0.25 0.25 0.125\">\n" +
                indent + indent + indent + "<DataSet index=\"0\" amr_box=\"4 5 6 7 4 7\" file=\"test.vti\">\n" +
                indent + indent + indent + "</DataSet>\n" +
                indent + indent + "</Block>\n" +
                indent + "</vtkHierarchicalBoxDataSet>\n"+
            "</VTKFile>\n";
        compareCurrentContentAndTarget(target);
        ASSERT_EQ(gen.getRawString(), target);
    }

    TEST_F(Test1, check_hierarchicalBoxDataSet_amr_box_support_node) {
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
                    gen.setFile("test.vti");
                    gen.endDataSet();
                gen.endBlock();
                gen.beginBlock();
                    gen.beginDataSet(0);
                    gen.setAMRBoxNodeFromParentIndex(0, 0, 1, 0, 1, 0, 1);
                    gen.setFile("test.vti");
                    gen.endDataSet();
                    gen.beginDataSet(1);
                    gen.setAMRBoxNodeFromParentIndex(0, 1, 2, 1, 2, 1, 2);
                    gen.setFile("test.vti");
                    gen.endDataSet();
                gen.endBlock();
                gen.beginBlock();
                    gen.beginDataSet(0);
                    gen.setAMRBoxNodeFromParentIndex(1, 0, 1, 1, 2, 0, 2);
                    gen.setFile("test.vti");
                    gen.endDataSet();
                gen.endBlock();
            gen.endContent();
        gen.endVTK();

        const std::string target = 
            header +
            "<VTKFile type=\"vtkHierarchicalBoxDataSet\">\n" +
                indent + "<vtkHierarchicalBoxDataSet Origin=\"0 0 0\" grid_description=\"XYZ\">\n" +
                indent + indent + "<Block level=\"0\" Spacing=\"1 1 0.5\">\n" +
                indent + indent + indent + "<DataSet index=\"0\" amr_box=\"0 1 0 1 0 1\" file=\"test.vti\">\n" +
                indent + indent + indent + "</DataSet>\n" +
                indent + indent + "</Block>\n" +
                indent + indent + "<Block level=\"1\" Spacing=\"0.5 0.5 0.25\">\n" +
                indent + indent + indent + "<DataSet index=\"0\" amr_box=\"0 1 0 1 0 1\" file=\"test.vti\">\n" +
                indent + indent + indent + "</DataSet>\n" +
                indent + indent + indent + "<DataSet index=\"1\" amr_box=\"2 3 2 3 2 3\" file=\"test.vti\">\n" +
                indent + indent + indent + "</DataSet>\n" +
                indent + indent + "</Block>\n" +
                indent + indent + "<Block level=\"2\" Spacing=\"0.25 0.25 0.125\">\n" +
                indent + indent + indent + "<DataSet index=\"0\" amr_box=\"4 5 6 7 4 7\" file=\"test.vti\">\n" +
                indent + indent + indent + "</DataSet>\n" +
                indent + indent + "</Block>\n" +
                indent + "</vtkHierarchicalBoxDataSet>\n"+
            "</VTKFile>\n";
        compareCurrentContentAndTarget(target);
        ASSERT_EQ(gen.getRawString(), target);
    }
}