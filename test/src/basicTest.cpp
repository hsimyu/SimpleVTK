#include <test_simple_vtk.hpp>

namespace TEST_SIMPLE_VTK {
    TEST_F(Test1, check_header) {
        ASSERT_EQ(gen.getRawString(), "");
    }

    TEST_F(Test1, check_append_data) {
        gen.beginVTK("StructuredGrid");
            gen.beginContent();
            gen.endContent();
            gen.beginAppendData();
            gen.setEncoding("base64");
            gen.endAppendData();
        gen.endVTK();

        const std::string target = 
            header +
            "<VTKFile type=\"StructuredGrid\">\n" +
                indent + "<StructuredGrid>\n" +
                indent + "</StructuredGrid>\n" +
                indent + "<AppendData encoding=\"base64\">\n" +
                indent + "</AppendData>\n" +
            "</VTKFile>\n";
        compareCurrentContentAndTarget(target);
        ASSERT_EQ(gen.getRawString(), target);
    }
}