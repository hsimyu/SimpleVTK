#include <test_simple_vtk.hpp>

namespace TEST_SIMPLE_VTK {
    TEST_F(Test1, check_hierarchical_box) {
        gen.beginVTK("vtkHierarchicalBoxDataSet");
            gen.beginContent();
            gen.setOrigin(0.0, 0.0, 0.0);
            gen.setGridDescription("XYZ");

                gen.beginBlock(0);
                gen.setSpacing(1.0, 1.0, 1.0);
                gen.endBlock();

            gen.endContent();
        gen.endVTK();

        const std::string target = 
            header +
            "<VTKFile type=\"vtkHierarchicalBoxDataSet\" version=\"1.1\">\n" +
                indent + "<vtkHierarchicalBoxDataSet origin=\"0 0 0\" grid_description=\"XYZ\">\n" +
                indent + indent + "<Block level=\"0\" spacing=\"1 1 1\">\n" +
                indent + indent + "</Block>\n" +
                indent + "</vtkHierarchicalBoxDataSet>\n"+
            "</VTKFile>\n";
        compareCurrentContentAndTarget(target);
        ASSERT_EQ(gen.getRawString(), target);
    }

    TEST_F(Test1, check_hierarchical_box_dataset) {
        gen.beginVTK("vtkHierarchicalBoxDataSet");
            gen.beginContent();
            gen.setOrigin(0.0, 0.0, 0.0);
            gen.setGridDescription("XYZ");

                gen.beginBlock(0);
                gen.setSpacing(1.0, 1.0, 1.0);
                    gen.beginDataSet(0);
                    gen.setAMRBox(0, 2, 0, 2, 0, 2);
                    gen.setFile("test.vti");
                    gen.endDataSet();
                gen.endBlock();

            gen.endContent();
        gen.endVTK();

        const std::string target = 
            header +
            "<VTKFile type=\"vtkHierarchicalBoxDataSet\" version=\"1.1\">\n" +
                indent + "<vtkHierarchicalBoxDataSet origin=\"0 0 0\" grid_description=\"XYZ\">\n" +
                indent + indent + "<Block level=\"0\" spacing=\"1 1 1\">\n" +
                indent + indent + indent + "<DataSet index=\"0\" amr_box=\"0 2 0 2 0 2\" file=\"test.vti\">\n" +
                indent + indent + indent + "</DataSet>\n" +
                indent + indent + "</Block>\n" +
                indent + "</vtkHierarchicalBoxDataSet>\n"+
            "</VTKFile>\n";
        compareCurrentContentAndTarget(target);
        ASSERT_EQ(gen.getRawString(), target);
    }

    TEST_F(Test1, check_hierarchical_box_partial_block) {
        gen.enableExtentManagement();
        gen.changeBaseExtent(0, 2, 0, 2, 0, 2);
        gen.changeBaseOrigin(0.0, 0.0, 0.0);
        gen.changeBaseSpacing(1.0, 0.5, 0.25);

        gen.beginVTKPartial("vtkHierarchicalBoxDataSet");
        gen.beginBlock(0);
            gen.beginDataSet(0);
            gen.setAMRBox(0, 2, 0, 2, 0, 2);
            gen.setFile("parent.vti");
            gen.endDataSet();
        gen.endBlock();

        gen.beginBlock(1);
            gen.beginDataSet(0);
            gen.setAMRBoxNodeFromParentIndex(0, 0, 0, 2, 0, 2, 0, 2);
            gen.setFile("child.vti");
            gen.endDataSet();
        gen.endBlock();
        gen.endVTKPartial();

        const std::string target = 
                "<Block level=\"0\" spacing=\"1 0.5 0.25\">\n" +
                indent + "<DataSet index=\"0\" amr_box=\"0 2 0 2 0 2\" file=\"parent.vti\">\n" +
                indent + "</DataSet>\n" +
                "</Block>\n" +
                "<Block level=\"1\" spacing=\"0.5 0.25 0.125\">\n" +
                indent + "<DataSet index=\"0\" amr_box=\"0 3 0 3 0 3\" file=\"child.vti\">\n" +
                indent + "</DataSet>\n" +
                "</Block>\n";
        compareCurrentContentAndTarget(target);
        ASSERT_EQ(gen.getRawString(), target);
    }
}