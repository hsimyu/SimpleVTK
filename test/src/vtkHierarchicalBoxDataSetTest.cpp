#include <test_simple_vtk.hpp>

namespace TEST_SIMPLE_VTK {
    TEST_F(Test1, check_hierarchical_box) {
        gen.beginVTK("vtkHierarchicalBoxDataSet");
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
            "<VTKFile type=\"vtkHierarchicalBoxDataSet\">\n" +
                indent + "<vtkHierarchicalBoxDataSet WholeExtent=\"0 2 0 2 0 2\" Origin=\"0 0.5 0\" Spacing=\"1 1.1 1\">\n" +
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
                indent + "</vtkHierarchicalBoxDataSet>\n"+
            "</VTKFile>\n";
        compareCurrentContentAndTarget(target);
        ASSERT_EQ(gen.getRawString(), target);
    }

}