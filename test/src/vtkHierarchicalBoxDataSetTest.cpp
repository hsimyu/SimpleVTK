#include <test_simple_vtk.hpp>

namespace TEST_SIMPLE_VTK {
    TEST_F(Test1, check_hierarchical_box) {
        gen.beginVTK("vtkHierarchicalBoxDataSet");
            gen.beginContent();
            gen.setOrigin(0.0, 0.0, 0.0);
            gen.setGridDescription("XYZ");

                gen.beginBlock();
                gen.setLevel(0);
                gen.setSpacing(1.0, 1.0, 1.0);
                gen.endBlock();

            gen.endContent();
        gen.endVTK();

        const std::string target = 
            header +
            "<VTKFile type=\"vtkHierarchicalBoxDataSet\">\n" +
                indent + "<vtkHierarchicalBoxDataSet Origin=\"0 0 0\" grid_description=\"XYZ\">\n" +
                indent + indent + "<Block level=\"0\" Spacing=\"1 1 1\">\n" +
                indent + indent + "</Block>\n" +
                indent + "</vtkHierarchicalBoxDataSet>\n"+
            "</VTKFile>\n";
        compareCurrentContentAndTarget(target);
        ASSERT_EQ(gen.getRawString(), target);
    }

}